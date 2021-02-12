#include "googletest/gtest.h"

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

