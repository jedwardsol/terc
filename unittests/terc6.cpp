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


    void Assemble(int                           PC,
                  sixTritArchitecture::OpCode   opcode,
                  sixTritArchitecture::Register reg,
                  int                           argument)
    {
        tryte   first  { trybble{static_cast<int>(opcode)}, trybble{static_cast<int>(reg)}};
        tryte   second { argument};

        code[PC]   = first;
        code[PC+1] = second;
    }
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


TEST_F(CPUTest, Invalid)
{
    Assemble(0,  
             sixTritArchitecture::OpCode::Invalid,
             sixTritArchitecture::Register::R0,
             0);


    cpu.execute();

    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::RPC),  tryte{2});
    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::REXC), tryte{sixTritArchitecture::Exception::InvalidOpCode});
    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::REXA), tryte{0});
}




TEST_F(CPUTest, DoubleFault)
{
    Assemble(0,  
             sixTritArchitecture::OpCode::Invalid,
             sixTritArchitecture::Register::R0,
             0);

    Assemble(2,  
             sixTritArchitecture::OpCode::Invalid,
             sixTritArchitecture::Register::R0,
             0);

    cpu.execute();

    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::RPC),  tryte{2});
    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::REXC), tryte{sixTritArchitecture::Exception::InvalidOpCode});
    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::REXA), tryte{0});


    cpu.execute();

    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::RPC),  tryte{2});
    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::REXC), tryte{sixTritArchitecture::Exception::DoubleFault});
    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::REXA), tryte{0});

}




TEST_F(CPUTest, Nop)
{
    Assemble(0,  
             sixTritArchitecture::OpCode::Nop,
             sixTritArchitecture::Register::R0,
             0);


    cpu.execute();

    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::RPC),  tryte{2});
    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::REXC), tryte{sixTritArchitecture::Exception::Okay});
    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::REXA), tryte{0});
}

TEST_F(CPUTest, NopNop)
{
    Assemble(0,  
             sixTritArchitecture::OpCode::Nop,
             sixTritArchitecture::Register::R0,
             0);

    Assemble(2,  
             sixTritArchitecture::OpCode::Nop,
             sixTritArchitecture::Register::R0,
             0);

    cpu.execute();
    cpu.execute();

    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::RPC),  tryte{4});
    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::REXC), tryte{sixTritArchitecture::Exception::Okay});
    ASSERT_EQ(cpu.reg(sixTritArchitecture::Register::REXA), tryte{0});
}

