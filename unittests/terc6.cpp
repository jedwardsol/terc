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


    void Assemble(sixTritArchitecture::OpCode   opcode,
                  sixTritArchitecture::Register reg,
                  int                           argument)
    {
        tryte   first  { trybble{static_cast<int>(opcode)}, trybble{static_cast<int>(reg)}};
        tryte   second { argument};

        code[PC++]   = first;
        code[PC++] = second;
    }

    int PC{0};

};



TEST_F(CPUTest, InitialState)
{
    EXPECT_EQ(code[0],  tryte{0});
    EXPECT_EQ(data[0],  tryte{0});
    EXPECT_EQ(stack[0], tryte{0});

    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R0),   tryte{0});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::RPC),  tryte{0});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::RSP),  tryte{sixTritArchitecture::stackSize});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::REXC), tryte{sixTritArchitecture::Exception::Okay});
}

TEST_F(CPUTest, DefaultInstructionIsHalt)
{
    cpu.execute();

    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::RPC),  tryte{2});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::REXC), tryte{sixTritArchitecture::Exception::Halted});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::REXA), tryte{0});
}


TEST_F(CPUTest, RanOffEnd)
{
    cpu.reg(sixTritArchitecture::Register::RPC) = tryte{sixTritArchitecture::codeSize - 1};

    cpu.execute();

    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::RPC),  tryte{sixTritArchitecture::codeSize - 1});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::REXC), tryte{sixTritArchitecture::Exception::RanOffEnd});
}


TEST_F(CPUTest, Invalid)
{
    Assemble(sixTritArchitecture::OpCode::Invalid,
             sixTritArchitecture::Register::R0,
             0);


    cpu.execute();

    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::RPC),  tryte{2});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::REXC), tryte{sixTritArchitecture::Exception::InvalidOpCode});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::REXA), tryte{0});
}




TEST_F(CPUTest, DoubleFault)
{
    Assemble(sixTritArchitecture::OpCode::Invalid,
             sixTritArchitecture::Register::R0,
             0);

    Assemble(sixTritArchitecture::OpCode::Invalid,
             sixTritArchitecture::Register::R0,
             0);

    cpu.execute();

    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::RPC),  tryte{2});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::REXC), tryte{sixTritArchitecture::Exception::InvalidOpCode});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::REXA), tryte{0});


    cpu.execute();

    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::RPC),  tryte{2});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::REXC), tryte{sixTritArchitecture::Exception::DoubleFault});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::REXA), tryte{0});

}




TEST_F(CPUTest, Nop)
{
    Assemble(sixTritArchitecture::OpCode::Nop,
             sixTritArchitecture::Register::R0,
             0);


    cpu.execute();

    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::RPC),  tryte{2});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::REXC), tryte{sixTritArchitecture::Exception::Okay});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::REXA), tryte{0});
}

TEST_F(CPUTest, NopNop)
{
    Assemble(sixTritArchitecture::OpCode::Nop, sixTritArchitecture::Register::R0, 0);
    Assemble(sixTritArchitecture::OpCode::Nop, sixTritArchitecture::Register::R0, 0);

    cpu.execute();
    cpu.execute();

    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::RPC),  tryte{4});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::REXC), tryte{sixTritArchitecture::Exception::Okay});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::REXA), tryte{0});
}


TEST_F(CPUTest, MovIr)
{
    Assemble(sixTritArchitecture::OpCode::MovIR, sixTritArchitecture::Register::R1,  11);
    Assemble(sixTritArchitecture::OpCode::MovIR, sixTritArchitecture::Register::R2, -12);
    Assemble(sixTritArchitecture::OpCode::MovIR, sixTritArchitecture::Register::R3,  13);
    Assemble(sixTritArchitecture::OpCode::MovIR, sixTritArchitecture::Register::R4, -14);
    Assemble(sixTritArchitecture::OpCode::MovIR, sixTritArchitecture::Register::R5,  15);

    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R1),  tryte{0});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R2),  tryte{0});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R3),  tryte{0});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R4),  tryte{0});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R5),  tryte{0});


    cpu.execute();
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R1),  tryte{11});

    cpu.execute();
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R2),  tryte{-12});

    cpu.execute();
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R3),  tryte{13});

    cpu.execute();
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R4),  tryte{-14});

    cpu.execute();
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R5),  tryte{15});
}


TEST_F(CPUTest, MovIrBad)
{
    Assemble(sixTritArchitecture::OpCode::MovIR, sixTritArchitecture::Register::REXC, sixTritArchitecture::Exception::InvalidOpCode);

    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::REXC),  tryte{sixTritArchitecture::Exception::Okay});

    cpu.execute();
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::REXC),  tryte{sixTritArchitecture::Exception::InvalidRegister});
}


TEST_F(CPUTest, MovRr)
{
    Assemble(sixTritArchitecture::OpCode::MovIR, sixTritArchitecture::Register::R1, 111);
    Assemble(sixTritArchitecture::OpCode::MovIR, sixTritArchitecture::Register::R2, 112);
    Assemble(sixTritArchitecture::OpCode::MovIR, sixTritArchitecture::Register::R3, 113);
    Assemble(sixTritArchitecture::OpCode::MovIR, sixTritArchitecture::Register::R4, 114);
    Assemble(sixTritArchitecture::OpCode::MovIR, sixTritArchitecture::Register::R5, 115);

    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R1),  tryte{0});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R2),  tryte{0});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R3),  tryte{0});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R4),  tryte{0});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R5),  tryte{0});

    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R6),  tryte{0});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R7),  tryte{0});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R8),  tryte{0});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R9),  tryte{0});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R10), tryte{0});

    cpu.execute();
    cpu.execute();
    cpu.execute();
    cpu.execute();
    cpu.execute();

    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R1),  tryte{111});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R2),  tryte{112});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R3),  tryte{113});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R4),  tryte{114});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R5),  tryte{115});

    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R6),  tryte{0});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R7),  tryte{0});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R8),  tryte{0});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R9),  tryte{0});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R10), tryte{0});


    Assemble(sixTritArchitecture::OpCode::MovRR, sixTritArchitecture::Register::R6,  static_cast<int>(sixTritArchitecture::Register::R5));
    Assemble(sixTritArchitecture::OpCode::MovRR, sixTritArchitecture::Register::R7,  static_cast<int>(sixTritArchitecture::Register::R4));
    Assemble(sixTritArchitecture::OpCode::MovRR, sixTritArchitecture::Register::R8,  static_cast<int>(sixTritArchitecture::Register::R3));
    Assemble(sixTritArchitecture::OpCode::MovRR, sixTritArchitecture::Register::R9,  static_cast<int>(sixTritArchitecture::Register::R2));
    Assemble(sixTritArchitecture::OpCode::MovRR, sixTritArchitecture::Register::R10, static_cast<int>(sixTritArchitecture::Register::R1));

    cpu.execute();
    cpu.execute();
    cpu.execute();
    cpu.execute();
    cpu.execute();

    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R1),  tryte{111});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R2),  tryte{112});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R3),  tryte{113});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R4),  tryte{114});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R5),  tryte{115});

    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R6),  tryte{115});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R7),  tryte{114});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R8),  tryte{113});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R9),  tryte{112});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R10), tryte{111});


}



TEST_F(CPUTest, MovRrBadOperand)
{
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R10), tryte{0});

    Assemble(sixTritArchitecture::OpCode::MovRR, sixTritArchitecture::Register::R10, 28);

    cpu.execute();

    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::R10),  tryte{0});
    EXPECT_EQ(cpu.reg(sixTritArchitecture::Register::REXC), tryte{sixTritArchitecture::Exception::InvalidRegister});
}
