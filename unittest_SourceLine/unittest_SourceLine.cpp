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





