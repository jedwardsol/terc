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

