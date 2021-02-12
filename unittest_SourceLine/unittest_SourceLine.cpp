#include "googletest/gtest.h"

#include <optional>
#include <tuple>

#include "sourceLine/sourceLine.h"


int main(int argc, char *argv[]) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

namespace std
{
inline void PrintTo(const std::string_view &string, ::std::ostream *os)
{
    *os << string;
}
}

inline void PrintTo(const tryte &t, ::std::ostream *os)
{
    *os << t;
}




class SplitEmptyP :public ::testing::TestWithParam<std::string_view> 
{
};


TEST_P(SplitEmptyP, SplitEmpty) 
{
    auto line = GetParam();
    const SourceLine source{line};
    EXPECT_EQ(source.tokens().size(), 0);
}

INSTANTIATE_TEST_SUITE_P
(
    SourceLineTests,
    SplitEmptyP,
    ::testing::Values
    (
        "",
        " ",
        "\t",
        "  ",
        ", ,",
        ";",
        "; comment",
        " ; comment",
        ", ; comment ;"
        ", ; comment ; another comment"
    )
);


class SplitBasic1P : public ::testing::TestWithParam<std::string_view> 
{
};


TEST_P(SplitBasic1P, SplitBasic1) 
{
    const SourceLine source{GetParam()};

    EXPECT_EQ(source.tokens().size(), 1);
    EXPECT_EQ(source.tokens().at(0),  ".Begin");
    EXPECT_EQ(source.asString(0),  ".Begin");
}

INSTANTIATE_TEST_SUITE_P
(
    SourceLineTests,
    SplitBasic1P,
    ::testing::Values
    (
        ".Begin",
        "    .Begin",
        ".Begin    ",
        "\t.Begin  ; start  ",
        "\t.Begin,  ; start  ",
        ".Begin,",
        "    .Begin,",
        ".Begin ; begin ; oh   "
    )
);




class SplitBasic2P :public ::testing::TestWithParam<std::string_view> 
{
};


TEST_P(SplitBasic2P, SplitBasic2) 
{
    const SourceLine source{GetParam()};

    EXPECT_EQ(source.tokens().size(), 2);
    EXPECT_EQ(source.tokens().at(0),  ".Stack");
    EXPECT_EQ(source.tokens().at(1),  "100");

    EXPECT_EQ(source.asString(0),  ".Stack");
    EXPECT_EQ(source.asString(1),  "100");


}

INSTANTIATE_TEST_SUITE_P
(
    SourceLineTests,
    SplitBasic2P,
    ::testing::Values
    (
        ".Stack 100",
        " .Stack  100    ; stack size ",
        " ,,.Stack  100    ; stack size ",
        " .Stack  100,,    ; stack size",
        " ,,.Stack  100,,    ; stack size",
        " ,,.Stack , 100,,    ; stack size",
        " ,,.Stack ,, 100,,    ; stack size"
    )
);




class AsTryteP :public ::testing::TestWithParam< std::pair<std::string_view, std::optional<tryte>>>
{
};


TEST_P(AsTryteP, AsTryte) 
{
    const auto &string = GetParam().first;

    const SourceLine source{string};

    EXPECT_EQ(source.tokens().size(), 1);
    EXPECT_EQ(source.asTryte(0),  GetParam().second);
}

INSTANTIATE_TEST_SUITE_P
(
    SourceLineTests,
    AsTryteP,
    ::testing::Values
    (
        std::make_pair("100",       tryte{100}),
        std::make_pair("-100",      tryte{-100}),
        std::make_pair("0",         tryte{0}),
        std::make_pair("000000",    tryte{0}),
        std::make_pair("000002",    tryte{2}),
        std::make_pair("00000+",    tryte{1}),
        std::make_pair("00000+",    tryte{"00000+"}),
        std::make_pair("00000-",    tryte{-1}),
        std::make_pair("00000-",    tryte{"00000-"}) ,
        std::make_pair("364",       tryte{ 364}),
        std::make_pair("++++++",    tryte{ 364}),
        std::make_pair("-364",      tryte{-364}),
        std::make_pair("------",    tryte{-364}),

        std::make_pair("0x10",      std::nullopt),
        std::make_pair("-0x10",     std::nullopt),
        std::make_pair("+100",      std::nullopt),  // leading + 
        std::make_pair("a",         std::nullopt),
        std::make_pair("1a",        std::nullopt),
        std::make_pair("1.2",       std::nullopt),
        std::make_pair("-----1",    std::nullopt),
        std::make_pair("+++++1",    std::nullopt),
        std::make_pair("+++++",     std::nullopt),  // too short 
        std::make_pair("+++++++",   std::nullopt),  // too long

        std::make_pair("365",       std::nullopt),  // too big
        std::make_pair("-365",      std::nullopt)   // too small

    )
);



class AsRegP :public ::testing::TestWithParam< std::pair<std::string_view, std::optional<Architecture::sixTrit::Register>>>
{
};


TEST_P(AsRegP, AsReg) 
{
    const auto &string = GetParam().first;

    const SourceLine source{string};

    EXPECT_EQ(source.tokens().size(), 1);
    EXPECT_EQ(source.asRegister(0),  GetParam().second);
}

INSTANTIATE_TEST_SUITE_P
(
    SourceLineTests,
    AsRegP,
    ::testing::Values
    (
        std::make_pair("RPC",   Architecture::sixTrit::Register::RPC),
        std::make_pair("RSP",   Architecture::sixTrit::Register::RSP),   
        std::make_pair("RFlags",Architecture::sixTrit::Register::RFlags),
        std::make_pair("RRA",   Architecture::sixTrit::Register::RRA),   
        std::make_pair("REXC",  Architecture::sixTrit::Register::REXC),  
        std::make_pair("REXA",  Architecture::sixTrit::Register::REXA),  
        std::make_pair("Rn7",   Architecture::sixTrit::Register::Rn7),
        std::make_pair("Rn6",   Architecture::sixTrit::Register::Rn6),
        std::make_pair("Rn5",   Architecture::sixTrit::Register::Rn5),
        std::make_pair("Rn4",   Architecture::sixTrit::Register::Rn4),
        std::make_pair("Rn3",   Architecture::sixTrit::Register::Rn3),
        std::make_pair("Rn2",   Architecture::sixTrit::Register::Rn2),
        std::make_pair("Rn1",   Architecture::sixTrit::Register::Rn1),
        std::make_pair("R0",    Architecture::sixTrit::Register::R0),
        std::make_pair("R1",    Architecture::sixTrit::Register::R1),
        std::make_pair("R2",    Architecture::sixTrit::Register::R2),
        std::make_pair("R3",    Architecture::sixTrit::Register::R3),
        std::make_pair("R4",    Architecture::sixTrit::Register::R4),
        std::make_pair("R5",    Architecture::sixTrit::Register::R5),
        std::make_pair("R6",    Architecture::sixTrit::Register::R6),
        std::make_pair("R7",    Architecture::sixTrit::Register::R7),
        std::make_pair("R8",    Architecture::sixTrit::Register::R8),
        std::make_pair("R9",    Architecture::sixTrit::Register::R9),
        std::make_pair("R10",   Architecture::sixTrit::Register::R10),
        std::make_pair("R11",   Architecture::sixTrit::Register::R11),
        std::make_pair("R12",   Architecture::sixTrit::Register::R12),
        std::make_pair("R13",   Architecture::sixTrit::Register::R13),

        std::make_pair("R14",   std::nullopt),
        std::make_pair("0",     std::nullopt),
        std::make_pair("R",     std::nullopt),
        std::make_pair("RP",    std::nullopt),
        std::make_pair("RPCC",  std::nullopt)
    )
);



class AsConditionP :public ::testing::TestWithParam< std::pair<std::string_view, std::optional<Architecture::Condition>>>
{
};


TEST_P(AsConditionP, AsCondition) 
{
    const auto &string = GetParam().first;

    const SourceLine source{string};

    EXPECT_EQ(source.tokens().size(), 1);
    EXPECT_EQ(source.asCondition(0),  GetParam().second);
}

INSTANTIATE_TEST_SUITE_P
(
    SourceLineTests,
    AsConditionP,
    ::testing::Values
    (
        std::make_pair("False",   Architecture::Condition::AlwaysFalse),
        std::make_pair("True",    Architecture::Condition::AlwaysTrue),

        std::make_pair("CE",      Architecture::Condition::ConditionalExecuted),   
        std::make_pair("NCE",     Architecture::Condition::ConditionalNotExecuted),

        std::make_pair("P",       Architecture::Condition::Positive),      
        std::make_pair("Z",       Architecture::Condition::Zero),          
        std::make_pair("N",       Architecture::Condition::Negative),      
        std::make_pair("NP",      Architecture::Condition::NotPositive),  
        std::make_pair("NZ",      Architecture::Condition::NotZero),      
        std::make_pair("NN",      Architecture::Condition::NotNegative),  

        std::make_pair("GT",      Architecture::Condition::GreaterThan),        
        std::make_pair("E",       Architecture::Condition::Equal),              
        std::make_pair("LT",      Architecture::Condition::LessThan),           
        std::make_pair("LE",      Architecture::Condition::LessThanOrEqual),    
        std::make_pair("NE",      Architecture::Condition::NotEqual),           
        std::make_pair("GE",      Architecture::Condition::GreaterOrEqual),     

        std::make_pair(">",       Architecture::Condition::GreaterThan),        
        std::make_pair("==",      Architecture::Condition::Equal),              
        std::make_pair("<",       Architecture::Condition::LessThan),           
        std::make_pair("<=",      Architecture::Condition::LessThanOrEqual),    
        std::make_pair("!=",      Architecture::Condition::NotEqual),           
        std::make_pair(">=",      Architecture::Condition::GreaterOrEqual),     

        std::make_pair("=!",      std::nullopt),
        std::make_pair("JLE",     std::nullopt),
        std::make_pair("true",    std::nullopt),
        std::make_pair("R0",      std::nullopt)
    )
);




class AsCpuControlP :public ::testing::TestWithParam< std::pair<std::string_view, std::optional<Architecture::CpuControl>>>
{
};


TEST_P(AsCpuControlP, AsCpuControl) 
{
    const auto &string = GetParam().first;

    const SourceLine source{string};

    EXPECT_EQ(source.tokens().size(), 1);
    EXPECT_EQ(source.asCpuControl(0),  GetParam().second);
}

INSTANTIATE_TEST_SUITE_P
(
    SourceLineTests,
    AsCpuControlP,
    ::testing::Values
    (
        std::make_pair("Nop",       Architecture::CpuControl::Nop),        
        std::make_pair("Halt",      Architecture::CpuControl::Halt),              
        std::make_pair("Breakpoint",Architecture::CpuControl::Breakpoint),           
        std::make_pair("Trace",     Architecture::CpuControl::Trace),    
        std::make_pair("Invalid",   Architecture::CpuControl::Invalid),           

        std::make_pair("=!",      std::nullopt),
        std::make_pair("JLE",     std::nullopt),
        std::make_pair("true",    std::nullopt),
        std::make_pair("R0",      std::nullopt)
    )
);


