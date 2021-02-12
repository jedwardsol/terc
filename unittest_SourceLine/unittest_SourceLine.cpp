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


class EmptySourceLineTest :public ::testing::TestWithParam<std::string_view> 
{
};


TEST_P(EmptySourceLineTest, Empty) 
{
    auto line = GetParam();
    const SourceLine source{line};
    EXPECT_EQ(source.tokens().size(), 0);
}

INSTANTIATE_TEST_SUITE_P
(
        SourceLineTests,
        EmptySourceLineTest,
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


class Basic1SourceLineTest :public ::testing::TestWithParam<std::string_view> 
{
};


TEST_P(Basic1SourceLineTest, Basic1) 
{
    const SourceLine source{GetParam()};

    EXPECT_EQ(source.tokens().size(), 1);
    EXPECT_EQ(source.tokens().at(0),  ".Begin");
}

INSTANTIATE_TEST_SUITE_P
(
    SourceLineTests,
    Basic1SourceLineTest,
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




class Basic2SourceLineTest :public ::testing::TestWithParam<std::string_view> 
{
};


TEST_P(Basic2SourceLineTest, Basic2) 
{
    const SourceLine source{GetParam()};

    EXPECT_EQ(source.tokens().size(), 2);
    EXPECT_EQ(source.tokens().at(0),  ".Stack");
    EXPECT_EQ(source.tokens().at(1),  "100");
}



INSTANTIATE_TEST_SUITE_P
(
    SourceLineTests,
    Basic2SourceLineTest,
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


