#include "googletest/gtest.h"

#include <string>
using namespace std::literals;
#include <sstream>

#include "Arithmetic/Arithmetic.h"

#include "Arithmetic/trit.h"
#include "Arithmetic/trit_std.h"


TEST(TritTest, Construct) 
{
    trit    td;
    trit    t0{ 0};
    trit    t1{ 1};
    trit    tn{-1};

    EXPECT_EQ(td.t, 0);
    EXPECT_EQ(t0.t, 0);
    EXPECT_EQ(t1.t, 1);
    EXPECT_EQ(tn.t,-1);
}


TEST(TritTest, ConstructType) 
{
    trit    ti  { int{1}     };
    trit    ti8 { int8_t{1}  };
    trit    ti16{ int16_t{1} };

    EXPECT_EQ(ti.t,   1);
    EXPECT_EQ(ti8.t,  1);
    EXPECT_EQ(ti16.t, 1);

    trit    tn { '-'};
    trit    t0 { '0' };
    trit    tp { '+' };

    EXPECT_EQ(tn.t,      -1);
    EXPECT_EQ(t0.t,    0);
    EXPECT_EQ(tp.t,    1);

}




TEST(TritTest, BadConstruct) 
{
    auto constructi = [](int i)
    {
        trit t{i};
    };

    EXPECT_THROW(constructi( 2),std::out_of_range);
    EXPECT_THROW(constructi(-2),std::out_of_range);


    auto constructc = [](char c)
    {
        trit t{c};
    };

    EXPECT_THROW(constructc( '1'),std::out_of_range);
    EXPECT_THROW(constructc( ' '),std::out_of_range);

}

TEST(TritTest, CopyConstruct) 
{
    trit    t0{ 0};
    trit    t1{ 1};
    trit    tn{-1};

    trit    c0{t0};
    trit    c1{t1};
    trit    cn{tn};

    EXPECT_EQ(c0.t, 0);
    EXPECT_EQ(c1.t, 1);
    EXPECT_EQ(cn.t,-1);
}

TEST(TritTest, CopyAssign) 
{
    trit    t0{ 0};
    trit    t1{ 1};
    trit    tn{-1};

    trit    a0;
    trit    a1;
    trit    an;

    a0=t0;
    a1=t1;
    an=tn;

    EXPECT_EQ(a0.t, 0);
    EXPECT_EQ(a1.t, 1);
    EXPECT_EQ(an.t,-1);
}


TEST(TritTest, int) 
{
    trit    t0{ 0};
    trit    t1{ 1};
    trit    tn{-1};

    EXPECT_EQ(static_cast<int>(t0), 0);
    EXPECT_EQ(static_cast<int>(t1), 1);
    EXPECT_EQ(static_cast<int>(tn),-1);
}


TEST(TritTest, int16_t) 
{
    trit    t0{ 0};
    trit    t1{ 1};
    trit    tn{-1};

    EXPECT_EQ(static_cast<int16_t>(t0), 0);
    EXPECT_EQ(static_cast<int16_t>(t1), 1);
    EXPECT_EQ(static_cast<int16_t>(tn),-1);
}

TEST(TritTest, bool) 
{
    trit    tn{-1};
    trit    t0{ 0};
    trit    t1{ 1};

    EXPECT_TRUE (tn);
    EXPECT_FALSE(t0);
    EXPECT_TRUE (t1);
}



TEST(TritTest, to_string) 
{
    trit    t0{ 0};
    trit    t1{ 1};
    trit    tn{-1};

    EXPECT_EQ(to_string(t0),"0"s);
    EXPECT_EQ(to_string(t1),"+"s);
    EXPECT_EQ(to_string(tn),"-"s);
}

TEST(TritTest, stream) 
{
    std::ostringstream str;

    trit    t0{ 0};
    trit    t1{ 1};
    trit    tn{-1};


    str << tn << t0 << t1;

    EXPECT_EQ(str.str(),"-0+"s);
}



TEST(TritTest, equality) 
{
    trit    l0{ 0};
    trit    l1{ 1};
    trit    ln{-1};

    trit    r0{ 0};
    trit    r1{ 1};
    trit    rn{-1};

    EXPECT_EQ(l0,r0);
    EXPECT_NE(l0,r1);
    EXPECT_NE(l0,rn);

    EXPECT_NE(l1,r0);
    EXPECT_EQ(l1,r1);
    EXPECT_NE(l1,rn);

    EXPECT_NE(ln,r0);
    EXPECT_NE(ln,r1);
    EXPECT_EQ(ln,rn);
}

TEST(TritTest, lessthan) 
{
    trit    l0{ 0};
    trit    l1{ 1};
    trit    ln{-1};

    trit    r0{ 0};
    trit    r1{ 1};
    trit    rn{-1};

    EXPECT_LE(ln,rn);
    EXPECT_LE(ln,r0);
    EXPECT_LT(ln,r0);
    EXPECT_LE(ln,r1);
    EXPECT_LT(ln,r1);

    EXPECT_LE(l0,r0);
    EXPECT_LE(l0,r1);
    EXPECT_LT(l0,r1);

    EXPECT_LE(l1,r1);
}

TEST(TritTest, greaterthan) 
{
    trit    l0{ 0};
    trit    l1{ 1};
    trit    ln{-1};

    trit    r0{ 0};
    trit    r1{ 1};
    trit    rn{-1};

    EXPECT_GE(ln,rn);

    EXPECT_GE(l0,rn);
    EXPECT_GT(l0,rn);
    EXPECT_GE(l0,r0);

    EXPECT_GE(l1,rn);
    EXPECT_GT(l1,rn);
    EXPECT_GE(l1,r0);
    EXPECT_GT(l1,r0);
    EXPECT_GE(l1,r1);
}

TEST(TritTest, minus) 
{
    trit    t0{ 0};
    trit    t1{ 1};
    trit    tn{-1};

    EXPECT_EQ(t0, -t0);
    EXPECT_EQ(t1, -tn);
    EXPECT_EQ(tn, -t1);
}


TEST(TritTest, plusequal) 
{
    auto plusequal = [](int initial,int add)
    {
        trit    t{initial};

        t+=trit{add};
        
        return t;
    };
    
    EXPECT_THROW( plusequal(-1,-1),  std::overflow_error );
    EXPECT_EQ   ( plusequal(-1, 0),  trit{-1});
    EXPECT_EQ   ( plusequal(-1, 1),  trit{ 0});

    EXPECT_EQ   ( plusequal( 0,-1),  trit{-1});
    EXPECT_EQ   ( plusequal( 0, 0),  trit{ 0});
    EXPECT_EQ   ( plusequal( 0, 1),  trit{ 1});

    EXPECT_EQ   ( plusequal( 1,-1),  trit{ 0});
    EXPECT_EQ   ( plusequal( 1, 0),  trit{ 1});
    EXPECT_THROW( plusequal( 1, 1),  std::overflow_error );
}

TEST(TritTest, minusequal) 
{
    auto minusequal = [](int initial,int add)
    {
        trit    t{initial};

        t-=trit{add};
        
        return t;
    };
    
    EXPECT_EQ   ( minusequal(-1,-1),  trit{ 0});
    EXPECT_EQ   ( minusequal(-1, 0),  trit{-1});
    EXPECT_THROW( minusequal(-1, 1),  std::overflow_error );

    EXPECT_EQ   ( minusequal( 0,-1),  trit{ 1});
    EXPECT_EQ   ( minusequal( 0, 0),  trit{ 0});
    EXPECT_EQ   ( minusequal( 0, 1),  trit{-1});

    EXPECT_THROW( minusequal( 1,-1),  std::overflow_error);
    EXPECT_EQ   ( minusequal( 1, 0),  trit{ 1});
    EXPECT_EQ   ( minusequal( 1, 1),  trit{ 0});
}


TEST(TritTest, preincrement) 
{
    EXPECT_EQ   ( ++trit{-1}, trit{0});
    EXPECT_EQ   ( ++trit{ 0}, trit{1});
    EXPECT_THROW( ++trit{ 1}, std::overflow_error);
}


TEST(TritTest, postincrement) 
{
    trit    tn{-1};
    trit    t0{ 0};
    trit    t1{ 1};

    auto tni = tn++;
    auto t0i = t0++;

    EXPECT_EQ   ( tni, trit{-1});
    EXPECT_EQ   ( tn , trit{ 0});

    EXPECT_EQ   ( t0i, trit{0});
    EXPECT_EQ   ( t0 , trit{1});

    EXPECT_THROW( t1++, std::overflow_error);
}


TEST(TritTest, predecrement) 
{
    EXPECT_THROW( --trit{-1}, std::overflow_error);
    EXPECT_EQ   ( --trit{ 0}, trit{-1});
    EXPECT_EQ   ( --trit{ 1}, trit{ 0});
}

TEST(TritTest, postdecrement) 
{
    trit    tn{-1};
    trit    t0{ 0};
    trit    t1{ 1};

    auto t0i = t0--;
    auto t1i = t1--;

    EXPECT_THROW( tn--, std::overflow_error);

    EXPECT_EQ   ( t0i, trit{ 0});
    EXPECT_EQ   ( t0 , trit{-1});

    EXPECT_EQ   ( t1i, trit{ 1});
    EXPECT_EQ   ( t1 , trit{ 0});
}


//https://homepage.divms.uiowa.edu/~jones/ternary/arith.shtml#halfbalanced
TEST(TritTest, halfaddertn) 
{
    trit  result;
    trit  carry;

    result=halfAdder(trit{-1}, trit{-1}, carry);
    EXPECT_EQ   ( result, trit{1});
    EXPECT_EQ   ( carry,  trit{-1});

    result=halfAdder(trit{-1}, trit{0}, carry);
    EXPECT_EQ   ( result, trit{-1});
    EXPECT_EQ   ( carry,  trit{ 0});

    result=halfAdder(trit{-1}, trit{1}, carry);
    EXPECT_EQ   ( result, trit{0});
    EXPECT_EQ   ( carry,  trit{0});
}

TEST(TritTest, halfaddert0) 
{
    trit  result;
    trit  carry;

    result=halfAdder(trit{0}, trit{-1}, carry);
    EXPECT_EQ   ( result, trit{-1});
    EXPECT_EQ   ( carry,  trit{ 0});

    result=halfAdder(trit{0}, trit{0}, carry);
    EXPECT_EQ   ( result, trit{ 0});
    EXPECT_EQ   ( carry,  trit{ 0});

    result=halfAdder(trit{0}, trit{1}, carry);
    EXPECT_EQ   ( result, trit{1});
    EXPECT_EQ   ( carry,  trit{0});
}

TEST(TritTest, halfaddert1) 
{
    trit  result;
    trit  carry;

    result=halfAdder(trit{1}, trit{-1}, carry);
    EXPECT_EQ   ( result, trit{0});
    EXPECT_EQ   ( carry,  trit{0});

    result=halfAdder(trit{1}, trit{0}, carry);
    EXPECT_EQ   ( result, trit{1});
    EXPECT_EQ   ( carry,  trit{0});

    result=halfAdder(trit{1}, trit{1}, carry);
    EXPECT_EQ   ( result, trit{-1});
    EXPECT_EQ   ( carry,  trit{ 1});
}



TEST(TritTest, fulladder) 
{
    struct  fulladderTest
    {
        trit    ain;
        trit    bin;
        trit    cin;

        trit    result;
        trit    carry;
    };

    // https://homepage.divms.uiowa.edu/~jones/ternary/arith.shtml#fullbalanced
    fulladderTest    tests[]
    {
        { trit{-1}, trit{-1}, trit{-1},  trit{ 0}, trit{-1} },
        { trit{-1}, trit{-1}, trit{ 0},  trit{ 1}, trit{-1} },
        { trit{-1}, trit{-1}, trit{ 1},  trit{-1}, trit{ 0} },

        { trit{-1}, trit{ 0}, trit{-1},  trit{ 1}, trit{-1} },
        { trit{-1}, trit{ 0}, trit{ 0},  trit{-1}, trit{ 0} },
        { trit{-1}, trit{ 0}, trit{ 1},  trit{ 0}, trit{ 0} },

        { trit{-1}, trit{ 1}, trit{-1},  trit{-1}, trit{ 0} },
        { trit{-1}, trit{ 1}, trit{ 0},  trit{ 0}, trit{ 0} },
        { trit{-1}, trit{ 1}, trit{ 1},  trit{ 1}, trit{ 0} },

        //-----

        { trit{ 0}, trit{-1}, trit{-1},  trit{ 1}, trit{-1} },
        { trit{ 0}, trit{-1}, trit{ 0},  trit{-1}, trit{ 0} },
        { trit{ 0}, trit{-1}, trit{ 1},  trit{ 0}, trit{ 0} },

        { trit{ 0}, trit{ 0}, trit{-1},  trit{-1}, trit{ 0} },
        { trit{ 0}, trit{ 0}, trit{ 0},  trit{ 0}, trit{ 0} },
        { trit{ 0}, trit{ 0}, trit{ 1},  trit{ 1}, trit{ 0} },

        { trit{ 0}, trit{ 1}, trit{-1},  trit{ 0}, trit{ 0} },
        { trit{ 0}, trit{ 1}, trit{ 0},  trit{ 1}, trit{ 0} },
        { trit{ 0}, trit{ 1}, trit{ 1},  trit{-1}, trit{ 1} },

        //-----


        { trit{ 1}, trit{-1}, trit{-1},  trit{-1}, trit{ 0} },
        { trit{ 1}, trit{-1}, trit{ 0},  trit{ 0}, trit{ 0} },
        { trit{ 1}, trit{-1}, trit{ 1},  trit{ 1}, trit{ 0} },

        { trit{ 1}, trit{ 0}, trit{-1},  trit{ 0}, trit{ 0} },
        { trit{ 1}, trit{ 0}, trit{ 0},  trit{ 1}, trit{ 0} },
        { trit{ 1}, trit{ 0}, trit{ 1},  trit{-1}, trit{ 1} },

        { trit{ 1}, trit{ 1}, trit{-1},  trit{ 1}, trit{ 0} },
        { trit{ 1}, trit{ 1}, trit{ 0},  trit{-1}, trit{ 1} },
        { trit{ 1}, trit{ 1}, trit{ 1},  trit{ 0}, trit{ 1} },
    };


    for(auto test : tests)
    {
        trit carry{test.cin};

        auto result = fullAdder(test.ain, test.bin, carry);

        EXPECT_EQ   (result, test.result)  << "Result of "s + to_char(test.ain) + to_char(test.bin) + to_char(test.cin);
        EXPECT_EQ   (carry,  test.carry)   << "Carry  of "s + to_char(test.ain) + to_char(test.bin) + to_char(test.cin);
    }
}

