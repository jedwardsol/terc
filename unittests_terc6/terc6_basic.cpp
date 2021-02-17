#include "googletest/gtest.h"

#include <iostream>
#include <string>
#include <sstream>
using namespace std::literals;

#include "CPUTest.h"


TEST_F(CPUTest, InitialState)
{
    EXPECT_EQ(code[0],  tryte{0});
    EXPECT_EQ(data[0],  tryte{0});
    EXPECT_EQ(data[-1], tryte{0});

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R0),   tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::RPC),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::RSP),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC), tryte{Architecture::Exception::Okay});
}

TEST_F(CPUTest, DefaultInstructionIsHalt)
{
    cpu.execute();

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::RPC),  tryte{0});    // halt doesn't update RPC 
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC), tryte{Architecture::Exception::Halted});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXA), tryte{0});
}


TEST_F(CPUTest, RanOffEnd)
{
    cpu.setReg(Architecture::sixTrit::Register::RPC,  tryte{code.positiveSize() - 1} );

    cpu.execute();

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::RPC),  tryte{code.positiveSize() - 1});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC), tryte{Architecture::Exception::BadPC});
}


TEST_F(CPUTest, Invalid)
{
    assembleCpuControl(Architecture::CpuControl::Invalid);


    cpu.execute();

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::RPC),  tryte{2});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC), tryte{Architecture::Exception::InvalidOpCode});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXA), tryte{0});
}




TEST_F(CPUTest, DoubleFault)
{
    assembleCpuControl(Architecture::CpuControl::Invalid);
    assembleCpuControl(Architecture::CpuControl::Invalid);


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
    assembleCpuControl(Architecture::Condition::AlwaysFalse, Architecture::CpuControl::Trace);

    cpu.execute();

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::RPC),  tryte{2});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC), tryte{Architecture::Exception::Okay});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXA), tryte{0});
}

TEST_F(CPUTest, NopNop)
{
    assembleCpuControl(Architecture::Condition::AlwaysFalse, Architecture::CpuControl::Trace);
    assembleCpuControl(Architecture::Condition::AlwaysFalse, Architecture::CpuControl::Trace);

    cpu.execute();
    cpu.execute();

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::RPC),  tryte{4});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC), tryte{Architecture::Exception::Okay});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXA), tryte{0});
}

