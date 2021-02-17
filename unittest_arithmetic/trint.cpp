#include "googletest/gtest.h"

#include <string>
using namespace std::literals;
#include <sstream>

#include "Arithmetic/Arithmetic.h"

#include "Arithmetic/trint.h"
#include "Arithmetic/trint_std.h"

TEST(TrintTest, DefaultConstruct)
{
    trybble trybble;
    tryte   tryte;
    trord   trord;

    ASSERT_EQ( trybble.numTrits, 3);
    ASSERT_EQ( tryte.numTrits,   6);
    ASSERT_EQ( trord.numTrits,  12);

    for(int i=0;i<3;i++)
    {
        ASSERT_EQ( trybble.getTrit(i), trit{0} );
    }

    for(int i=0;i<6;i++)
    {
        ASSERT_EQ( tryte.getTrit(i), trit{0} );
    }

    for(int i=0;i<12;i++)
    {
        ASSERT_EQ( trord.getTrit(i), trit{0} );
    }
}



TEST(TrintTest, ZeroConstruct)
{
    trybble trybble{0};
    tryte   tryte  {0};
    trord   trord  {0};

    ASSERT_EQ( trybble.numTrits, 3);
    ASSERT_EQ( tryte.numTrits,   6);
    ASSERT_EQ( trord.numTrits,  12);

    for(int i=0;i<3;i++)
    {
        ASSERT_EQ( trybble.getTrit(i), trit{0} );
    }

    for(int i=0;i<6;i++)
    {
        ASSERT_EQ( tryte.getTrit(i), trit{0} );
    }

    for(int i=0;i<12;i++)
    {
        ASSERT_EQ( trord.getTrit(i), trit{0} );
    }
}



TEST(TrintTest, BadIntConstruct) 
{
    auto construct = [](int i)
    {
        tryte t{i};
    };

    EXPECT_THROW(construct( std::numeric_limits<tryte>::max() + 1),std::out_of_range);
    EXPECT_THROW(construct( std::numeric_limits<tryte>::min() - 1),std::out_of_range);
}


TEST(TrintTest, BadStringConstruct) 
{
    auto construct = [](const std::string &s)
    {
        tryte t{s};
    };

    EXPECT_THROW(construct( "+++"     ),std::out_of_range); // too short
    EXPECT_THROW(construct( "+++++++" ),std::out_of_range); // too long
    EXPECT_THROW(construct( "00X000"  ),std::out_of_range); // bad char
}




TEST(TrintTest, IntConstruct) 
{
    struct  constructTest
    {
        int i;
        int t5;
        int t4;
        int t3;
        int t2;
        int t1;
        int t0;
    };

    constexpr auto max = std::numeric_limits<tryte>::max();

    constructTest   tests[]=
    {
        {  2,   0, 0, 0, 0, 1,-1},
        { -2,   0, 0, 0, 0,-1, 1},
        {  1,   0, 0, 0, 0, 0, 1},
        { -1,   0, 0, 0, 0, 0,-1},

//            243 81 27  9  3  1

        { -4,   0, 0, 0, 0,-1,-1},
        { -3,   0, 0, 0, 0,-1, 0},
        { -2,   0, 0, 0, 0,-1, 1},

        { -1,   0, 0, 0, 0, 0,-1},
        {  0,   0, 0, 0, 0, 0, 0},
        {  1,   0, 0, 0, 0, 0, 1},

        {  2,   0, 0, 0, 0, 1,-1},
        {  3,   0, 0, 0, 0, 1, 0},
        {  4,   0, 0, 0, 0, 1, 1},

        {  5,   0, 0, 0, 1,-1,-1},
        {  6,   0, 0, 0, 1,-1, 0},
        {  7,   0, 0, 0, 1,-1, 1},

        {  8,   0, 0, 0, 1, 0,-1},
        {  9,   0, 0, 0, 1, 0, 0},
        { 10,   0, 0, 0, 1, 0, 1},

        { 11,   0, 0, 0, 1, 1,-1},
        { 12,   0, 0, 0, 1, 1, 0},
        { 13,   0, 0, 0, 1, 1, 1},

        { 14,   0, 0, 1,-1,-1,-1},
        { 15,   0, 0, 1,-1,-1, 0},
        { 16,   0, 0, 1,-1,-1, 1},

        { 17,   0, 0, 1,-1, 0,-1},
        { 18,   0, 0, 1,-1, 0, 0},
        { 19,   0, 0, 1,-1, 0, 1},

        { 20,   0, 0, 1,-1, 1,-1},
        { 21,   0, 0, 1,-1, 1, 0},
        { 22,   0, 0, 1,-1, 1, 1},

        { 23,   0, 0, 1, 0,-1,-1},
        { 24,   0, 0, 1, 0,-1, 0},
        { 25,   0, 0, 1, 0,-1, 1},

        { 26,   0, 0, 1, 0, 0,-1},
        { 27,   0, 0, 1, 0, 0, 0},
        { 28,   0, 0, 1, 0, 0, 1},

        { 42,   0, 1,-1,-1,-1, 0},
        {-42,   0,-1, 1, 1, 1, 0},

//            243 81 27  9  3  1

        { 81,   0, 1, 0, 0, 0, 0},
        {100,   0, 1, 1,-1, 0, 1},      // 81 + 19 =   81 + 27 - 9 + 1
        {243,   1, 0, 0, 0, 0, 0},
        {max,   1, 1, 1, 1, 1, 1},
    };

    for(const auto &test : tests)
    {
        tryte t{ test.i};
        tryte n{-test.i};

        EXPECT_EQ   (t.getTrit(0),  trit{test.t0})  << "t0 in "s + std::to_string( test.i);
        EXPECT_EQ   (t.getTrit(1),  trit{test.t1})  << "t1 in "s + std::to_string( test.i);
        EXPECT_EQ   (t.getTrit(2),  trit{test.t2})  << "t2 in "s + std::to_string( test.i);
        EXPECT_EQ   (t.getTrit(3),  trit{test.t3})  << "t3 in "s + std::to_string( test.i);
        EXPECT_EQ   (t.getTrit(4),  trit{test.t4})  << "t4 in "s + std::to_string( test.i);
        EXPECT_EQ   (t.getTrit(5),  trit{test.t5})  << "t5 in "s + std::to_string( test.i);

        EXPECT_EQ   (n.getTrit(0), trit{-test.t0})  << "t0 in "s + std::to_string(-test.i);
        EXPECT_EQ   (n.getTrit(1), trit{-test.t1})  << "t1 in "s + std::to_string(-test.i);
        EXPECT_EQ   (n.getTrit(2), trit{-test.t2})  << "t2 in "s + std::to_string(-test.i);
        EXPECT_EQ   (n.getTrit(3), trit{-test.t3})  << "t3 in "s + std::to_string(-test.i);
        EXPECT_EQ   (n.getTrit(4), trit{-test.t4})  << "t4 in "s + std::to_string(-test.i);
        EXPECT_EQ   (n.getTrit(5), trit{-test.t5})  << "t5 in "s + std::to_string(-test.i);

        EXPECT_EQ   (static_cast<int>(t),  test.i)  << "cast t in "s + std::to_string(-test.i);
        EXPECT_EQ   (static_cast<int>(n), -test.i)  << "cast n in "s + std::to_string(-test.i);
    }
}

TEST(TrintTest, StringConstruct) 
{
    tryte  n{"------"};
    tryte  z{"000000"};
    tryte  p{"++++++"};

    EXPECT_EQ(n, tryte{std::numeric_limits<tryte>::min()});
    EXPECT_EQ(z, tryte{0});
    EXPECT_EQ(p, tryte{std::numeric_limits<tryte>::max()});

    for(int i =std::numeric_limits<tryte>::min();
            i<=std::numeric_limits<tryte>::max();
            i++)
    {
        tryte t{i};

        auto string = to_string(t);

        ASSERT_EQ( string.size(), t.numTrits);

        tryte s{ string };

        EXPECT_EQ(t,s);
        EXPECT_EQ(s.operator int(), i);
    }
}
