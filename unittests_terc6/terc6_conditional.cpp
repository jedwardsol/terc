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
