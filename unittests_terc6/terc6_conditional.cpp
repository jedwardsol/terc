#include "googletest/gtest.h"

#include <iostream>
#include <string>
#include <sstream>
using namespace std::literals;

#include "CPUTest.h"


TEST_F(CPUTest, UnConditionalInstructionFlag)
{
    assembleLoadImm(Register::R0,tryte{0});

    ASSERT_EQ(traceCounter,0);
    cpu.execute();
    EXPECT_EQ(traceCounter,0);
    EXPECT_EQ(cpu.reg(Register::RFlags).getTrit(static_cast<int>(Architecture::Flag::ExecutedConditional)),trit{0});
}


TEST_F(CPUTest, ConditionalInstructionExecutes)
{
    assembleCpuControl(Architecture::Condition::AlwaysTrue,Architecture::CpuControl::Trace);

    ASSERT_EQ(traceCounter,0);
    cpu.execute();
    EXPECT_EQ(traceCounter,1);
    EXPECT_EQ(cpu.reg(Register::RFlags).getTrit(static_cast<int>(Architecture::Flag::ExecutedConditional)),trit{1});


    assembleLoadImm(Register::R0,tryte{0});

    cpu.execute();
    EXPECT_EQ(cpu.reg(Register::RFlags).getTrit(static_cast<int>(Architecture::Flag::ExecutedConditional)),trit{0});    // reset to 0
}

TEST_F(CPUTest, ConditionalInstructionDoesntExecute)
{
    assembleCpuControl(Architecture::Condition::AlwaysFalse,Architecture::CpuControl::Trace);

    ASSERT_EQ(traceCounter,0);
    cpu.execute();
    EXPECT_EQ(traceCounter,0);
    EXPECT_EQ(cpu.reg(Register::RFlags).getTrit(static_cast<int>(Architecture::Flag::ExecutedConditional)),trit{-1});
}


TEST_F(CPUTest, ConditionalOnConditional)
{
    assembleCpuControl(Architecture::Condition::AlwaysFalse,Architecture::CpuControl::Trace);

    ASSERT_EQ(traceCounter,0);
    cpu.execute();
    EXPECT_EQ(traceCounter,0);
    EXPECT_EQ(cpu.reg(Register::RFlags).getTrit(static_cast<int>(Architecture::Flag::ExecutedConditional)),trit{-1});

    // this instruction is conditional on the previous one not executing,  which it didn't
    assembleCpuControl(Architecture::Condition::ConditionalNotExecuted,Architecture::CpuControl::Trace);                // not follows didn't

    cpu.execute();
    EXPECT_EQ(traceCounter,1);
    EXPECT_EQ(cpu.reg(Register::RFlags).getTrit(static_cast<int>(Architecture::Flag::ExecutedConditional)),trit{1});


    // this instruction is conditional on the previous one not executing,  but it did
    assembleCpuControl(Architecture::Condition::ConditionalNotExecuted,Architecture::CpuControl::Trace);                // not follows did

    cpu.execute();
    EXPECT_EQ(traceCounter,1);
    EXPECT_EQ(cpu.reg(Register::RFlags).getTrit(static_cast<int>(Architecture::Flag::ExecutedConditional)),trit{-1});



    // this instruction is conditional on the previous one executing,  but it didn't
    assembleCpuControl(Architecture::Condition::ConditionalExecuted,Architecture::CpuControl::Trace);                   // did follows didn't

    cpu.execute();
    EXPECT_EQ(traceCounter,1);
    EXPECT_EQ(cpu.reg(Register::RFlags).getTrit(static_cast<int>(Architecture::Flag::ExecutedConditional)),trit{-1});


    assembleCpuControl(Architecture::Condition::AlwaysTrue,Architecture::CpuControl::Trace);
    cpu.execute();
    EXPECT_EQ(traceCounter,2);
    EXPECT_EQ(cpu.reg(Register::RFlags).getTrit(static_cast<int>(Architecture::Flag::ExecutedConditional)),trit{1});


    // this instruction is conditional on the previous one executing,  which it did
    assembleCpuControl(Architecture::Condition::ConditionalExecuted,Architecture::CpuControl::Trace);                   // did follows did

    cpu.execute();
    EXPECT_EQ(traceCounter,3);
    EXPECT_EQ(cpu.reg(Register::RFlags).getTrit(static_cast<int>(Architecture::Flag::ExecutedConditional)),trit{1});
}



TEST_F(CPUTest, ConditionalOnSign)
{
    ASSERT_EQ(traceCounter,0);

    assembleLoadImm(Register::R0, tryte{0});
    cpu.execute();
    EXPECT_EQ(cpu.reg(Register::RFlags).getTrit(static_cast<int>(Architecture::Flag::Sign)),trit{0});


    assembleCpuControl(Architecture::Condition::Zero,   Architecture::CpuControl::Trace);
    cpu.execute();
    EXPECT_EQ(traceCounter,1);  // executed

    assembleCpuControl(Architecture::Condition::NotZero,Architecture::CpuControl::Trace);
    cpu.execute();
    EXPECT_EQ(traceCounter,1);  // didn't execute



    ASSERT_EQ(traceCounter,1);

    assembleLoadImm(Register::R0, tryte{88});
    cpu.execute();
    EXPECT_EQ(cpu.reg(Register::RFlags).getTrit(static_cast<int>(Architecture::Flag::Sign)),trit{1});


    assembleCpuControl(Architecture::Condition::Positive,   Architecture::CpuControl::Trace);
    cpu.execute();
    EXPECT_EQ(traceCounter,2);  // executed

    assembleCpuControl(Architecture::Condition::Negative,   Architecture::CpuControl::Trace);
    cpu.execute();
    EXPECT_EQ(traceCounter,2);  // didn't execute

    // TODO : table driven tests?

}



TEST_F(CPUTest, ConditionalOnComparison)
{
    ASSERT_EQ(traceCounter,0);

    assembleLoadImm(Register::R0, tryte{-3});
    cpu.execute();
    EXPECT_EQ(cpu.reg(Register::RFlags).getTrit(static_cast<int>(Architecture::Flag::Sign)),trit{-1});

    assemble(Architecture::sixTrit::OpCode::CmpI, Register::R0, -3);                                             
    cpu.execute();
    EXPECT_EQ(cpu.reg(Register::RFlags).getTrit(static_cast<int>(Architecture::Flag::Comparison)),trit{0});    // equal


    assembleCpuControl(Architecture::Condition::Equal,   Architecture::CpuControl::Trace);
    cpu.execute();
    EXPECT_EQ(traceCounter,1);  // executed

    assembleCpuControl(Architecture::Condition::GreaterThan,Architecture::CpuControl::Trace);
    cpu.execute();
    EXPECT_EQ(traceCounter,1);  // didn't execute


    // TODO : table driven tests?

}




TEST_F(CPUTest, JumpImmediate)
{
    assemble(OpCode::JmpI, Architecture::Condition::AlwaysTrue, tryte{10});                     // 0
    assembleCpuControl(Architecture::Condition::AlwaysTrue, Architecture::CpuControl::Halt);    // 2
    assembleCpuControl(Architecture::Condition::AlwaysTrue, Architecture::CpuControl::Halt);    // 4
    assembleCpuControl(Architecture::Condition::AlwaysTrue, Architecture::CpuControl::Halt);    // 6
    assembleCpuControl(Architecture::Condition::AlwaysTrue, Architecture::CpuControl::Halt);    // 8

    assembleCpuControl(Architecture::Condition::AlwaysTrue, Architecture::CpuControl::Trace);   // 10

    ASSERT_EQ(traceCounter,0);
    cpu.execute();
    EXPECT_EQ(cpu.reg(Register::RPC),tryte{10});
    cpu.execute();
    EXPECT_EQ(traceCounter,1);
}


TEST_F(CPUTest, JumpRegister)
{
    assembleLoadImm(Register::R10, tryte{12});
    cpu.execute();
    assemble(OpCode::JmpR, Architecture::Condition::AlwaysTrue, Register::R10);                 // 2
    assembleCpuControl(Architecture::Condition::AlwaysTrue, Architecture::CpuControl::Halt);    // 4
    assembleCpuControl(Architecture::Condition::AlwaysTrue, Architecture::CpuControl::Halt);    // 6
    assembleCpuControl(Architecture::Condition::AlwaysTrue, Architecture::CpuControl::Halt);    // 8
    assembleCpuControl(Architecture::Condition::AlwaysTrue, Architecture::CpuControl::Halt);    // 10

    assembleCpuControl(Architecture::Condition::AlwaysTrue, Architecture::CpuControl::Trace);   // 12

    ASSERT_EQ(traceCounter,0);
    cpu.execute();
    EXPECT_EQ(cpu.reg(Register::RPC),tryte{12});
    EXPECT_EQ(cpu.reg(Register::RRA),tryte{ 0});    // unchanged
    cpu.execute();
    EXPECT_EQ(traceCounter,1);
}