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


TEST(TrintTest, TrybbleConstruct) 
{
    trybble small{"---"};
    trybble big  {"+++"};

    EXPECT_EQ((tryte{small, small}), tryte{"------"});
    EXPECT_EQ((tryte{small, big}),   tryte{"+++---"});
    EXPECT_EQ((tryte{big,   small}), tryte{"---+++"});
    EXPECT_EQ((tryte{big,   big}),   tryte{"++++++"});
}


TEST(TrintTest, CopyConstruct) 
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

TEST(TrintTest, CopyAssign) 
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



TEST(TrintTest, ToString) 
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
        { -4,   "0000--","0000--(-4)"},
        { -3,   "0000-0","0000-0(-3)"},
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
        { 28,   "00+00+","00+00+(28)"},


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


TEST(TrintTest, equality) 
{
    tryte    l1{-200};
    tryte    l2{ 300};
    tryte    l3{   0};

    tryte    r1{-200};
    tryte    r2{ 300};
    tryte    r3{   0};

    EXPECT_EQ(l1,r1);
    EXPECT_NE(l1,r2);
    EXPECT_NE(l1,r3);

    EXPECT_NE(l2,r1);
    EXPECT_EQ(l2,r2);
    EXPECT_NE(l2,r3);

    EXPECT_NE(l3,r1);
    EXPECT_NE(l3,r2);
    EXPECT_EQ(l3,r3);
}



TEST(TrintTest, lessthan) 
{
    tryte    ln{-201};
    tryte    lz{   0};
    tryte    lp{  99};

    tryte    rn{-201};
    tryte    rz{   0};
    tryte    rp{  99};

    EXPECT_LE(ln,rn);
    EXPECT_LE(ln,rz);
    EXPECT_LT(ln,rz);
    EXPECT_LE(ln,rp);
    EXPECT_LT(ln,rp);

    EXPECT_LE(lz,rz);
    EXPECT_LE(lz,rp);
    EXPECT_LT(lz,rp);

    EXPECT_LE(lp,rp);
}

TEST(TrintTest, greaterthan) 
{
    tryte    ln{-201};
    tryte    lz{   0};
    tryte    lp{  99};

    tryte    rn{-201};
    tryte    rz{   0};
    tryte    rp{  99};


    EXPECT_GE(ln,rn);

    EXPECT_GE(lz,rz);
    EXPECT_GE(lz,rn);
    EXPECT_GT(lz,rn);


    EXPECT_GE(lp,rn);
    EXPECT_GT(lp,rn);
    EXPECT_GE(lp,rz);
    EXPECT_GT(lp,rz);
    EXPECT_GE(lp,rp);
}


TEST(TrintTest, allCompare) 
{
    for(int i =std::numeric_limits<tryte>::min();
            i<=std::numeric_limits<tryte>::max();
            i++)
    {
        for(int j =std::numeric_limits<tryte>::min();
                j<=std::numeric_limits<tryte>::max();
                j++)
        {
            tryte l{i};
            tryte r{j};

            if( i < j)
            {
                EXPECT_LT(l,r);
                EXPECT_LE(l,r);
                EXPECT_NE(l,r);
            }
            else if( i == j)
            {
                EXPECT_LE(l,r);
                EXPECT_EQ(l,r);
                EXPECT_GE(l,r);
            }
            else 
            {
                EXPECT_NE(l,r);
                EXPECT_GE(l,r);
                EXPECT_GT(l,r);
            }
        }
    }
}


TEST(TrintTest, halfAdder) 
{
    for(int i =std::numeric_limits<tryte>::min();
            i<=std::numeric_limits<tryte>::max();
            i++)
    {
        for(int j =std::numeric_limits<tryte>::min();
                j<=std::numeric_limits<tryte>::max();
                j++)
        {
            tryte   l{i};
            tryte   r{j};

            trit    carry;

            tryte   result{halfAdder(l,r,carry)};

            if( i + j < std::numeric_limits<tryte>::min())
            {
                EXPECT_EQ( result, tryte{(i+j) + 2 * std::numeric_limits<tryte>::max() + 1 } );
                EXPECT_EQ( carry,  trit {-1}   );
            }
            else if( i + j > std::numeric_limits<tryte>::max())
            {
                EXPECT_EQ( result, tryte{(i+j) - 2 * std::numeric_limits<tryte>::max() - 1 } );
                EXPECT_EQ( carry,  trit {1}   );
            }
            else
            {
                EXPECT_EQ( result, tryte{i+j} );
                EXPECT_EQ( carry,  trit {0}   );
            }
        }
    }
}


