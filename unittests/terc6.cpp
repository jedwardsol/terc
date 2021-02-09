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

    Architecture::RWMemoryBlock               code {Architecture::sixTrit::codeSize};    
    Architecture::RWMemoryBlock               data {Architecture::sixTrit::dataSize};    
    Architecture::RWMemoryBlock               stack{Architecture::sixTrit::stackSize};    
    Architecture::DummyIOPorts                ioPorts;

    Architecture::sixTrit::CPU    cpu{code,data,stack,ioPorts};


    void Assemble(Architecture::sixTrit::OpCode   opcode,
                  Architecture::sixTrit::Register reg,
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

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R0),   tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::RPC),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::RSP),  tryte{Architecture::sixTrit::stackSize});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC), tryte{Architecture::Exception::Okay});
}

TEST_F(CPUTest, DefaultInstructionIsHalt)
{
    cpu.execute();

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::RPC),  tryte{2});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC), tryte{Architecture::Exception::Halted});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXA), tryte{0});
}


TEST_F(CPUTest, RanOffEnd)
{
    cpu.reg(Architecture::sixTrit::Register::RPC) = tryte{Architecture::sixTrit::codeSize - 1};

    cpu.execute();

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::RPC),  tryte{Architecture::sixTrit::codeSize - 1});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC), tryte{Architecture::Exception::RanOffEnd});
}


TEST_F(CPUTest, Invalid)
{
    Assemble(Architecture::sixTrit::OpCode::Invalid,
             Architecture::sixTrit::Register::R0,
             0);


    cpu.execute();

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::RPC),  tryte{2});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC), tryte{Architecture::Exception::InvalidOpCode});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXA), tryte{0});
}




TEST_F(CPUTest, DoubleFault)
{
    Assemble(Architecture::sixTrit::OpCode::Invalid,
             Architecture::sixTrit::Register::R0,
             0);

    Assemble(Architecture::sixTrit::OpCode::Invalid,
             Architecture::sixTrit::Register::R0,
             0);

    cpu.execute();

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::RPC),  tryte{2});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC), tryte{Architecture::Exception::InvalidOpCode});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXA), tryte{0});


    cpu.execute();

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::RPC),  tryte{2});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC), tryte{Architecture::Exception::DoubleFault});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXA), tryte{0});

}




TEST_F(CPUTest, Nop)
{
    Assemble(Architecture::sixTrit::OpCode::Nop,
             Architecture::sixTrit::Register::R0,
             0);


    cpu.execute();

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::RPC),  tryte{2});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC), tryte{Architecture::Exception::Okay});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXA), tryte{0});
}

TEST_F(CPUTest, NopNop)
{
    Assemble(Architecture::sixTrit::OpCode::Nop, Architecture::sixTrit::Register::R0, 0);
    Assemble(Architecture::sixTrit::OpCode::Nop, Architecture::sixTrit::Register::R0, 0);

    cpu.execute();
    cpu.execute();

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::RPC),  tryte{4});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC), tryte{Architecture::Exception::Okay});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXA), tryte{0});
}


TEST_F(CPUTest, MovIr)
{
    Assemble(Architecture::sixTrit::OpCode::MovIR, Architecture::sixTrit::Register::R1,  11);
    Assemble(Architecture::sixTrit::OpCode::MovIR, Architecture::sixTrit::Register::R2, -12);
    Assemble(Architecture::sixTrit::OpCode::MovIR, Architecture::sixTrit::Register::R3,  13);
    Assemble(Architecture::sixTrit::OpCode::MovIR, Architecture::sixTrit::Register::R4, -14);
    Assemble(Architecture::sixTrit::OpCode::MovIR, Architecture::sixTrit::Register::R5,  15);

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R1),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R2),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R3),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R4),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R5),  tryte{0});


    cpu.execute();
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R1),  tryte{11});

    cpu.execute();
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R2),  tryte{-12});

    cpu.execute();
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R3),  tryte{13});

    cpu.execute();
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R4),  tryte{-14});

    cpu.execute();
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R5),  tryte{15});
}


TEST_F(CPUTest, MovIrBad)
{
    Assemble(Architecture::sixTrit::OpCode::MovIR, Architecture::sixTrit::Register::REXC, Architecture::Exception::InvalidOpCode);

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC),  tryte{Architecture::Exception::Okay});

    cpu.execute();
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC),  tryte{Architecture::Exception::InvalidRegister});
}


TEST_F(CPUTest, MovRr)
{
    Assemble(Architecture::sixTrit::OpCode::MovIR, Architecture::sixTrit::Register::R1, 111);
    Assemble(Architecture::sixTrit::OpCode::MovIR, Architecture::sixTrit::Register::R2, 112);
    Assemble(Architecture::sixTrit::OpCode::MovIR, Architecture::sixTrit::Register::R3, 113);
    Assemble(Architecture::sixTrit::OpCode::MovIR, Architecture::sixTrit::Register::R4, 114);
    Assemble(Architecture::sixTrit::OpCode::MovIR, Architecture::sixTrit::Register::R5, 115);

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R1),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R2),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R3),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R4),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R5),  tryte{0});

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R6),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R7),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R8),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R9),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R10), tryte{0});

    cpu.execute();
    cpu.execute();
    cpu.execute();
    cpu.execute();
    cpu.execute();

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R1),  tryte{111});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R2),  tryte{112});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R3),  tryte{113});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R4),  tryte{114});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R5),  tryte{115});

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R6),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R7),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R8),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R9),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R10), tryte{0});


    Assemble(Architecture::sixTrit::OpCode::MovRR, Architecture::sixTrit::Register::R6,  static_cast<int>(Architecture::sixTrit::Register::R5));
    Assemble(Architecture::sixTrit::OpCode::MovRR, Architecture::sixTrit::Register::R7,  static_cast<int>(Architecture::sixTrit::Register::R4));
    Assemble(Architecture::sixTrit::OpCode::MovRR, Architecture::sixTrit::Register::R8,  static_cast<int>(Architecture::sixTrit::Register::R3));
    Assemble(Architecture::sixTrit::OpCode::MovRR, Architecture::sixTrit::Register::R9,  static_cast<int>(Architecture::sixTrit::Register::R2));
    Assemble(Architecture::sixTrit::OpCode::MovRR, Architecture::sixTrit::Register::R10, static_cast<int>(Architecture::sixTrit::Register::R1));

    cpu.execute();
    cpu.execute();
    cpu.execute();
    cpu.execute();
    cpu.execute();

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R1),  tryte{111});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R2),  tryte{112});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R3),  tryte{113});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R4),  tryte{114});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R5),  tryte{115});

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R6),  tryte{115});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R7),  tryte{114});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R8),  tryte{113});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R9),  tryte{112});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R10), tryte{111});
}



TEST_F(CPUTest, MovRrBadOperand)
{
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R10), tryte{0});

    Assemble(Architecture::sixTrit::OpCode::MovRR, Architecture::sixTrit::Register::R10, 28);

    cpu.execute();

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R10),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC), tryte{Architecture::Exception::InvalidRegister});
}
