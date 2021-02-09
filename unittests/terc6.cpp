#include <gtest/gtest.h>
#include <string>
using namespace std::literals;
#include <sstream>

#include <iostream>

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/Arithmetic_std.h"
#include "Arithmetic/tryte.h"

#include "Architecture/MemoryBlock.h"
#include "Architecture/sixTritCPU.h"

#pragma comment(lib,"Architecture")


class CPUTest : public ::testing::Test
{
protected:

    virtual void SetUp()
    {      
    }

    virtual void TearDown()
    {
    }

    RWMemoryBlock               code {sixTritArchitecture::codeSize};    
    RWMemoryBlock               data {sixTritArchitecture::dataSize};    
    RWMemoryBlock               stack{sixTritArchitecture::stackSize};    

    sixTritArchitecture::CPU    cpu{code,data,stack};
};



TEST_F(CPUTest, InitialState)
{
    ASSERT_EQ(code[0],  tryte{0});
    ASSERT_EQ(data[0],  tryte{0});
    ASSERT_EQ(stack[0], tryte{0});

    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::R0),   tryte{0});
    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::RPC),  tryte{0});
    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::RSP),  tryte{sixTritArchitecture::stackSize});
    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::REXC), tryte{sixTritArchitecture::Exception::Okay});
}

TEST_F(CPUTest, DefaultInstructionIsHalt)
{
    cpu.execute();

    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::RPC),  tryte{2});
    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::REXC), tryte{sixTritArchitecture::Exception::Halted});
    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::REXA), tryte{0});
}


TEST_F(CPUTest, RanOffEnd)
{
    cpu.reg(sixTritArchitecture::Register::RPC) = tryte{sixTritArchitecture::codeSize - 1};

    cpu.execute();

    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::RPC),  tryte{sixTritArchitecture::codeSize - 1});
    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::REXC), tryte{sixTritArchitecture::Exception::RanOffEnd});
}
