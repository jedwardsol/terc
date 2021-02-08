#include <gtest/gtest.h>
#include <string>
using namespace std::literals;
#include <sstream>


#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/Arithmetic_std.h"
#include "Arithmetic/trit.h"



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

TEST(TritTest, BadConstruct) 
{
    auto construct = [](int i)
    {
        trit t{i};
    };

    EXPECT_THROW(construct( 2),std::out_of_range);
    EXPECT_THROW(construct(-2),std::out_of_range);
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

