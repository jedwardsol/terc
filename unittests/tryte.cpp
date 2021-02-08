#include <gtest/gtest.h>
#include <string>
using namespace std::literals;
#include <sstream>

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/Arithmetic_std.h"
#include "Arithmetic/tryte.h"


TEST(TryteTest, DefaultConstruct) 
{
    tryte    td;

    EXPECT_EQ(td.t0, 0);
    EXPECT_EQ(td.t1, 0);
    EXPECT_EQ(td.t2, 0);
    EXPECT_EQ(td.t3, 0);
    EXPECT_EQ(td.t4, 0);
    EXPECT_EQ(td.t5, 0);
    EXPECT_EQ(td.unused, 0);
}


TEST(TryteTest, ZeroConstruct) 
{
    tryte    tz{0};

    EXPECT_EQ(tz.t0, 0);
    EXPECT_EQ(tz.t1, 0);
    EXPECT_EQ(tz.t2, 0);
    EXPECT_EQ(tz.t3, 0);
    EXPECT_EQ(tz.t4, 0);
    EXPECT_EQ(tz.t5, 0);
    EXPECT_EQ(tz.unused, 0);
}



TEST(TryteTest, BadConstruct) 
{
    auto construct = [](int i)
    {
        tryte t{i};
    };

    EXPECT_THROW(construct( std::numeric_limits<tryte>::max() + 1),std::out_of_range);
    EXPECT_THROW(construct( std::numeric_limits<tryte>::min() - 1),std::out_of_range);
}



TEST(TryteTest, Construct) 
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
        { 81,   0, 1, 0, 0, 0, 0},

        {100,   0, 1, 1,-1, 0, 1},      // 81 + 19 =   81 + 27 - 9 + 1

        {243,   1, 0, 0, 0, 0, 0},
        {max,   1, 1, 1, 1, 1, 1},
    };

    for(auto test : tests)
    {
        tryte t{ test.i};
        tryte n{-test.i};

        EXPECT_EQ   (t.t0,  test.t0)  << "t0 in "s + std::to_string( test.i);
        EXPECT_EQ   (t.t1,  test.t1)  << "t1 in "s + std::to_string( test.i);
        EXPECT_EQ   (t.t2,  test.t2)  << "t2 in "s + std::to_string( test.i);
        EXPECT_EQ   (t.t3,  test.t3)  << "t3 in "s + std::to_string( test.i);
        EXPECT_EQ   (t.t4,  test.t4)  << "t4 in "s + std::to_string( test.i);
        EXPECT_EQ   (t.t5,  test.t5)  << "t5 in "s + std::to_string( test.i);

        EXPECT_EQ   (n.t0, -test.t0)  << "t0 in "s + std::to_string(-test.i);
        EXPECT_EQ   (n.t1, -test.t1)  << "t1 in "s + std::to_string(-test.i);
        EXPECT_EQ   (n.t2, -test.t2)  << "t2 in "s + std::to_string(-test.i);
        EXPECT_EQ   (n.t3, -test.t3)  << "t3 in "s + std::to_string(-test.i);
        EXPECT_EQ   (n.t4, -test.t4)  << "t4 in "s + std::to_string(-test.i);
        EXPECT_EQ   (n.t5, -test.t5)  << "t5 in "s + std::to_string(-test.i);

        EXPECT_EQ   (static_cast<int>(t),  test.i)  << "cast t in "s + std::to_string(-test.i);
        EXPECT_EQ   (static_cast<int>(n), -test.i)  << "cast n in "s + std::to_string(-test.i);
    }
}



TEST(TryteTest, CopyConstruct) 
{
    for(int i =std::numeric_limits<tryte>::min();
            i<=std::numeric_limits<tryte>::max();
            i++)
    {
        tryte t{i};
        tryte c{t};

        EXPECT_EQ   (static_cast<int>(t), static_cast<int>(c))  << "copyConstruct "s + std::to_string(i);
    }
}

TEST(TryteTest, CopyAssign) 
{
    for(int i =std::numeric_limits<tryte>::min();
            i<=std::numeric_limits<tryte>::max();
            i++)
    {
        tryte t{i};
        tryte c;

        c=t;

        EXPECT_EQ   (static_cast<int>(t), static_cast<int>(c))  << "copyConstruct "s + std::to_string(i);
    }
}

TEST(TryteTest, ToString) 
{
    struct  toStringTest
    {
        int i;
        std::string s1;
        std::string s2;
    };

    constexpr auto max = std::numeric_limits<tryte>::max();

    toStringTest   tests[]=
    {
        { -2,   "0000-+","0000-+(-2)"},
        { -1,   "00000-","00000-(-1)"},
        {  0,   "000000","000000(0)"},
        {  1,   "00000+","00000+(1)"},
        {  2,   "0000+-","0000+-(2)"},
        {  3,   "0000+0","0000+0(3)"},
        {  4,   "0000++","0000++(4)"},
        {  5,   "000+--","000+--(5)"},
        {  6,   "000+-0","000+-0(6)"},
        {  7,   "000+-+","000+-+(7)"},
        {  8,   "000+0-","000+0-(8)"},
        {  9,   "000+00","000+00(9)"},
        { 10,   "000+0+","000+0+(10)"},
        { 11,   "000++-","000++-(11)"},
        { 12,   "000++0","000++0(12)"},
        { 13,   "000+++","000+++(13)"},
        { 14,   "00+---","00+---(14)"},
        { 15,   "00+--0","00+--0(15)"},
        { 16,   "00+--+","00+--+(16)"},
        { 17,   "00+-0-","00+-0-(17)"},
        { 18,   "00+-00","00+-00(18)"},
        { 19,   "00+-0+","00+-0+(19)"},
        { 20,   "00+-+-","00+-+-(20)"},
        { 21,   "00+-+0","00+-+0(21)"},
        { 22,   "00+-++","00+-++(22)"},
        { 23,   "00+0--","00+0--(23)"},
        { 24,   "00+0-0","00+0-0(24)"},
        { 25,   "00+0-+","00+0-+(25)"},
        { 26,   "00+00-","00+00-(26)"},
        { 27,   "00+000","00+000(27)"},
        { 81,   "0+0000","0+0000(81)"},
        {100,   "0++-0+","0++-0+(100)"},
        {243,   "+00000","+00000(243)"},
        {max,   "++++++","++++++(364)"},
        {-max,  "------","------(-364)"},

    };

    for(auto test : tests)
    {
        tryte t{ test.i};

        EXPECT_EQ   (to_string(t),  test.s1)  << "string "s + std::to_string( test.i);


        std::ostringstream str;

        str << t ;

        EXPECT_EQ   (str.str(),  test.s2 )  << "stream "s + std::to_string( test.i);


    }
}

