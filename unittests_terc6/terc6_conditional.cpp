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
