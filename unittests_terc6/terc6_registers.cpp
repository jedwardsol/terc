
#include "googletest/gtest.h"

#include <iostream>
#include <string>
#include <sstream>
using namespace std::literals;

#include "CPUTest.h"

                            
TEST_F(CPUTest, LoadImmediate)
{
    assembleAssign(Architecture::sixTrit::Register::R1, tryte{ 11});
    assembleAssign(Architecture::sixTrit::Register::R2, tryte{-12});
    assembleAssign(Architecture::sixTrit::Register::R3, tryte{ 13});
    assembleAssign(Architecture::sixTrit::Register::R4, tryte{-14});
    assembleAssign(Architecture::sixTrit::Register::R5, tryte{ 15});

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


TEST_F(CPUTest, AbsoluteJump)
{
    assembleAssign(Architecture::sixTrit::Register::RPC,  tryte{8});

    assemble(Architecture::sixTrit::OpCode::Out,            Architecture::sixTrit::Register::R0, static_cast<int>(Architecture::KnownIOPorts::IO_ASCII));
    assemble(Architecture::sixTrit::OpCode::Out,            Architecture::sixTrit::Register::R0, static_cast<int>(Architecture::KnownIOPorts::IO_ASCII));
    assemble(Architecture::sixTrit::OpCode::Out,            Architecture::sixTrit::Register::R0, static_cast<int>(Architecture::KnownIOPorts::IO_ASCII));

    assembleCpuControl(Architecture::CpuControl::Halt);


    cpu.execute();
    cpu.execute();

    EXPECT_EQ(cpu.reg(Register::RPC),   tryte{8});
    EXPECT_EQ(cpu.reg(Register::REXC),  tryte{Architecture::Exception::Halted});
    ASSERT_EQ(outs.size(), 1);
    EXPECT_EQ(outs[0].first,  trybble{static_cast<int>(Architecture::KnownIOPorts::O_Exception)});
    EXPECT_EQ(outs[0].second, tryte{Architecture::Exception::Halted});
}


TEST_F(CPUTest, MovIrBad)
{
    assembleAssign(Architecture::sixTrit::Register::REXC, tryte{Architecture::Exception::InvalidOpCode});

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC),  tryte{Architecture::Exception::Okay});

    cpu.execute();
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC),  tryte{Architecture::Exception::InvalidRegister});
}


TEST_F(CPUTest, Copy)
{
    assembleAssign(Architecture::sixTrit::Register::R1, tryte{111});
    assembleAssign(Architecture::sixTrit::Register::R2, tryte{112});
    assembleAssign(Architecture::sixTrit::Register::R3, tryte{113});
    assembleAssign(Architecture::sixTrit::Register::R4, tryte{114});
    assembleAssign(Architecture::sixTrit::Register::R5, tryte{115});

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


    assembleCopy(Architecture::sixTrit::Register::R6,  Architecture::sixTrit::Register::R5);
    assembleCopy(Architecture::sixTrit::Register::R7,  Architecture::sixTrit::Register::R4);
    assembleCopy(Architecture::sixTrit::Register::R8,  Architecture::sixTrit::Register::R3);
    assembleCopy(Architecture::sixTrit::Register::R9,  Architecture::sixTrit::Register::R2);
    assembleCopy(Architecture::sixTrit::Register::R10, Architecture::sixTrit::Register::R1);

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





TEST_F(CPUTest, InOut)
{
    assembleAssign(Architecture::sixTrit::Register::Rn1, tryte{-42});

    assemble(Architecture::sixTrit::OpCode::Out,            Architecture::sixTrit::Register::Rn1, 1);
    assemble(Architecture::sixTrit::OpCode::Out,            Architecture::sixTrit::Register::Rn1, 2);

    assemble(Architecture::sixTrit::OpCode::In,             Architecture::sixTrit::Register::Rn2, 3);
    assemble(Architecture::sixTrit::OpCode::In,             Architecture::sixTrit::Register::Rn3, 4);

    cpu.execute();
    cpu.execute();
    cpu.execute();
    cpu.execute();
    cpu.execute();

    ASSERT_EQ( outs.size(), 2 );

    EXPECT_EQ( outs[0].first,  trybble{1} );
    EXPECT_EQ( outs[0].second, tryte{-42} );

    EXPECT_EQ( outs[1].first,  trybble{2} );
    EXPECT_EQ( outs[1].second, tryte{-42} );

    ASSERT_EQ( ins.size(), 2 );
    EXPECT_EQ( ins[0],  trybble{3} );
    EXPECT_EQ( ins[1],  trybble{4} );

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::Rn2), tryte{1});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::Rn3), tryte{2});

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC), tryte{Architecture::Exception::Okay});
}



TEST_F(CPUTest, DataLoadAndStore)
{
    auto addressReg = trybble{static_cast<int>(Architecture::sixTrit::Register::R13)};
    int  value      = 201;
    int  address    = 281;

    assembleAssign(Architecture::sixTrit::Register::R13, tryte{address});

    assembleAssign(Architecture::sixTrit::Register::Rn1, tryte{value-1});
    assembleAssign(Architecture::sixTrit::Register::R0,  tryte{value});
    assembleAssign(Architecture::sixTrit::Register::R1,  tryte{value+1});

    assemble(Architecture::sixTrit::OpCode::Store,          Architecture::sixTrit::Register::Rn1, addressReg, trybble{-1});      // write 201 to data[280]
    assemble(Architecture::sixTrit::OpCode::Store,          Architecture::sixTrit::Register::R0,  addressReg, trybble{ 0});      // write 202 to data[281]
    assemble(Architecture::sixTrit::OpCode::Store,          Architecture::sixTrit::Register::R1,  addressReg, trybble{+1});      // write 203 to data[282]

    assemble(Architecture::sixTrit::OpCode::Load,           Architecture::sixTrit::Register::R4,  addressReg, trybble{-1});      // load from data[280]
    assemble(Architecture::sixTrit::OpCode::Load,           Architecture::sixTrit::Register::R5,  addressReg, trybble{ 0});      // load from data[281]
    assemble(Architecture::sixTrit::OpCode::Load,           Architecture::sixTrit::Register::R6,  addressReg, trybble{+1});      // load from data[282]


    ASSERT_EQ( data[address-1],tryte{0});
    ASSERT_EQ( data[address],  tryte{0});
    ASSERT_EQ( data[address+1],tryte{0});

    ASSERT_EQ( cpu.reg(Architecture::sixTrit::Register::R4),tryte{0});
    ASSERT_EQ( cpu.reg(Architecture::sixTrit::Register::R5),tryte{0});
    ASSERT_EQ( cpu.reg(Architecture::sixTrit::Register::R6),tryte{0});

    do
    {
        cpu.execute();
    } while(cpu.reg(Architecture::sixTrit::Register::REXC) <= tryte{0} );


    EXPECT_EQ( data[address-1],tryte{value-1});
    EXPECT_EQ( data[address],  tryte{value});
    EXPECT_EQ( data[address+1],tryte{value+1});

    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::R4),tryte{value-1});
    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::R5),tryte{value});
    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::R6),tryte{value+1});
}




TEST_F(CPUTest, StackLoadAndStore)
{
    auto addressReg = trybble{static_cast<int>(Architecture::sixTrit::Register::RSP)};
    int  value      = 123;
    int  address    = -30;

    assembleAssign( Architecture::sixTrit::Register::RSP, tryte{address});

    assembleAssign( Architecture::sixTrit::Register::Rn1, tryte{value-1});
    assembleAssign( Architecture::sixTrit::Register::R0,  tryte{value});
    assembleAssign( Architecture::sixTrit::Register::R1,  tryte{value+1});

    assemble(Architecture::sixTrit::OpCode::Store,      Architecture::sixTrit::Register::Rn1, addressReg, trybble{-1});      // write 201 to data[280]
    assemble(Architecture::sixTrit::OpCode::Store,      Architecture::sixTrit::Register::R0,  addressReg, trybble{ 0});      // write 202 to data[281]
    assemble(Architecture::sixTrit::OpCode::Store,      Architecture::sixTrit::Register::R1,  addressReg, trybble{+1});      // write 203 to data[282]

    assemble(Architecture::sixTrit::OpCode::Load,       Architecture::sixTrit::Register::R4, addressReg, trybble{-1});      // load from data[280]
    assemble(Architecture::sixTrit::OpCode::Load,       Architecture::sixTrit::Register::R5, addressReg, trybble{ 0});      // load from data[281]
    assemble(Architecture::sixTrit::OpCode::Load,       Architecture::sixTrit::Register::R6, addressReg, trybble{+1});      // load from data[282]


    ASSERT_EQ( data[address-1],tryte{0});
    ASSERT_EQ( data[address],  tryte{0});
    ASSERT_EQ( data[address+1],tryte{0});

    ASSERT_EQ( cpu.reg(Architecture::sixTrit::Register::R4),tryte{0});
    ASSERT_EQ( cpu.reg(Architecture::sixTrit::Register::R5),tryte{0});
    ASSERT_EQ( cpu.reg(Architecture::sixTrit::Register::R6),tryte{0});

    do
    {
        cpu.execute();
    } while(cpu.reg(Architecture::sixTrit::Register::REXC) <= tryte{0} );


    EXPECT_EQ( data[address-1],tryte{value-1});
    EXPECT_EQ( data[address],  tryte{value});
    EXPECT_EQ( data[address+1],tryte{value+1});

    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::R4),tryte{value-1});
    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::R5),tryte{value});
    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::R6),tryte{value+1});
}

TEST_F(CPUTest, AccessViolation)
{
    auto addressReg = trybble{static_cast<int>(Architecture::sixTrit::Register::RSP)};
    int  address    = -100;  // stack is 81

    assembleAssign(Architecture::sixTrit::Register::RSP, tryte{address});
    assemble(Architecture::sixTrit::OpCode::Load,           Architecture::sixTrit::Register::R5, addressReg, trybble{ 0});      // read from stack[-100]

    cpu.execute();
    cpu.execute();


    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::REXC),tryte{Architecture::Exception::AccessViolation});
}


TEST_F(CPUTest, PushPop)
{
    auto rsp = Architecture::sixTrit::Register::RSP;

    ASSERT_EQ( cpu.reg(rsp), tryte{0} );
    ASSERT_EQ( data[-1] ,    tryte{0} );
    ASSERT_EQ( data[-2] ,    tryte{0} );
    ASSERT_EQ( data[-3] ,    tryte{0} );

    assembleAssign(Architecture::sixTrit::Register::R0, tryte{42});
    cpu.execute();


    assemble(Architecture::sixTrit::OpCode::Push,           Architecture::sixTrit::Register::R0, 0 );
    cpu.execute();
    EXPECT_EQ( cpu.reg(rsp), tryte{-1} );
    EXPECT_EQ( data[ -1] ,   tryte{42} );

    assemble(Architecture::sixTrit::OpCode::Push,           Architecture::sixTrit::Register::R0, 0 );
    cpu.execute();
    EXPECT_EQ( cpu.reg(rsp), tryte{-2} );
    EXPECT_EQ( data[ -2] ,   tryte{42} );

    assemble(Architecture::sixTrit::OpCode::Push,           Architecture::sixTrit::Register::R0, 0 );
    cpu.execute();
    EXPECT_EQ( cpu.reg(rsp), tryte{-3} );
    EXPECT_EQ( data[ -3] ,   tryte{42} );


//--

    assemble(Architecture::sixTrit::OpCode::Pop,            Architecture::sixTrit::Register::R1, 0 );
    cpu.execute();
    EXPECT_EQ( cpu.reg(rsp),          tryte{-2} );
    EXPECT_EQ( cpu.reg(Register::R1), tryte{42} );

    assemble(Architecture::sixTrit::OpCode::Pop,            Architecture::sixTrit::Register::R2, 0 );
    cpu.execute();
    EXPECT_EQ( cpu.reg(rsp),            tryte{-1} );
    EXPECT_EQ( cpu.reg(Register::R2),   tryte{42} );

    assemble(Architecture::sixTrit::OpCode::Pop,            Architecture::sixTrit::Register::R3, 0 );
    cpu.execute();
    EXPECT_EQ( cpu.reg(rsp),            tryte{0} );
    EXPECT_EQ( cpu.reg(Register::R3), tryte{42} );


    EXPECT_NE( cpu.reg(Register::REXC), tryte{Architecture::Exception::AccessViolation });
}



TEST_F(CPUTest, StackOverflow)
{
    auto rsp = Architecture::sixTrit::Register::RSP;

    ASSERT_EQ( cpu.reg(rsp), tryte{0} );

    assembleAssign(Architecture::sixTrit::Register::R0, tryte{42});

    auto stackSize = -data.negativeSize();

    for(int i=0;i < stackSize;i++)
    {
        assemble(Architecture::sixTrit::OpCode::Push,           Architecture::sixTrit::Register::R0, 0 );
    }

//  assemble(Architecture::CpuControl::Breakpoint);
    assemble(Architecture::sixTrit::OpCode::Push,           Architecture::sixTrit::Register::R0, 0 );


    do
    {
        cpu.execute();
    } while(cpu.reg(Architecture::sixTrit::Register::REXC) <= tryte{0} );



    EXPECT_EQ( cpu.reg(Register::REXC), tryte{Architecture::Exception::AccessViolation} );
    EXPECT_EQ( cpu.reg(rsp), tryte{data.negativeSize()-1});
    EXPECT_EQ( data[ -1 ] , tryte{42} );

}




TEST_F(CPUTest, StackUnderflow)
{
    auto rsp = Architecture::sixTrit::Register::RSP;

    ASSERT_EQ( cpu.reg(rsp), tryte{0} );

    assembleAssign(Architecture::sixTrit::Register::R0, tryte{42});

    for(int i=0;i<3;i++)
    {
        assemble(Architecture::sixTrit::OpCode::Push,       Architecture::sixTrit::Register::R0, i+42 );
    }

    for(int i=0;i<3;i++)
    {
        assemble(Architecture::sixTrit::OpCode::Pop,        Architecture::sixTrit::Register::R0, 0 );
    }

//  assemble(Architecture::CpuControl::Breakpoint);
    assemble(Architecture::sixTrit::OpCode::Pop,           Architecture::sixTrit::Register::R0, 0 );

    do
    {
        cpu.execute();
    } while(cpu.reg(Architecture::sixTrit::Register::REXC) <= tryte{0} );


    EXPECT_EQ( cpu.reg(Register::REXC), tryte{Architecture::Exception::AccessViolation} );
    EXPECT_EQ( cpu.reg(Register::R0),   tryte{42} );
    EXPECT_EQ( cpu.reg(rsp),            tryte{0} );
}



TEST_F(CPUTest, Flags)
{
    ASSERT_EQ( cpu.reg(Register::RFlags), tryte{0});

    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),        trit{0});
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::ExecutedConditional), trit{0});

    cpu.setFlag(Architecture::Flag::Sign, trit{1});
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),        trit{1});
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::ExecutedConditional), trit{0});


    cpu.setFlag(Architecture::Flag::Sign, trit{0});
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),        trit{0});
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::ExecutedConditional), trit{0});

    cpu.setFlag(Architecture::Flag::ExecutedConditional, trit{-1});
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),        trit{0});
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::ExecutedConditional), trit{-1});

    cpu.setFlag(Architecture::Flag::ExecutedConditional, trit{0});
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),        trit{0});
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::ExecutedConditional), trit{0});
}



TEST_F(CPUTest, SignFlag)
{
// UpdateSignFlag :  LoadImmediate, Copy, In, load, pop
    ASSERT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{0});


    assembleAssign(Architecture::sixTrit::Register::Rn1,  tryte{-9});
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{-1}) << "Load -";

    assembleAssign(Architecture::sixTrit::Register::R0,   tryte{0});
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{0})  << "Load 0";;

    assembleAssign(Architecture::sixTrit::Register::R1,   tryte{123});
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{1})  << "Load +";;


// 

    assembleCopy(Architecture::sixTrit::Register::R2,  Architecture::sixTrit::Register::Rn1);
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{-1})  << "Copy -";;

    assembleCopy(Architecture::sixTrit::Register::R2,  Architecture::sixTrit::Register::R0);
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{0})  << "Copy 0";

    assembleCopy(Architecture::sixTrit::Register::R2,  Architecture::sixTrit::Register::R1);
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{1})  << "Copy +";



//  pushes - flag doesn't change

    assemble(Architecture::sixTrit::OpCode::Push,   Architecture::sixTrit::Register::Rn1 ,0);
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{1}) << "Push -";

    assemble(Architecture::sixTrit::OpCode::Push,   Architecture::sixTrit::Register::R0 ,0);
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{1}) << "Push 0";

    assemble(Architecture::sixTrit::OpCode::Push,   Architecture::sixTrit::Register::R1,0);    
    cpu.execute();                                                                         
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{1}) << "Push +";


//  pops


    assemble(Architecture::sixTrit::OpCode::Pop,   Architecture::sixTrit::Register::R2 ,0);
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{1}) << "Pop +";

    assemble(Architecture::sixTrit::OpCode::Pop,   Architecture::sixTrit::Register::R2 ,0);
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{0}) << "Pop 0";

    assemble(Architecture::sixTrit::OpCode::Pop,   Architecture::sixTrit::Register::R1,0);    
    cpu.execute();                                                                         
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{-1}) << "Push -";
}





TEST_F(CPUTest, CompareImmediate)
{

    ASSERT_EQ( cpu.getFlag(Architecture::Flag::Comparison),    trit{0});


    assembleAssign(Architecture::sixTrit::Register::R1, tryte{-42});
    assembleAssign(Architecture::sixTrit::Register::R2, tryte{ 99});

    cpu.execute();
    cpu.execute();



    assemble(Architecture::sixTrit::OpCode::CmpI, Architecture::sixTrit::Register::R1,   -100);
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Comparison),    trit{1})  << "-42 > -100";;

    assemble(Architecture::sixTrit::OpCode::CmpI, Architecture::sixTrit::Register::R1,   -1);
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Comparison),    trit{-1})  << "-42 < -1";;

    assemble(Architecture::sixTrit::OpCode::CmpI, Architecture::sixTrit::Register::R1,   11);
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Comparison),    trit{-1})  << "-42 < 11";;

    assemble(Architecture::sixTrit::OpCode::CmpI, Architecture::sixTrit::Register::R1,   -42);
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Comparison),    trit{0})  << "-42 == 42";;



    assemble(Architecture::sixTrit::OpCode::CmpI, Architecture::sixTrit::Register::R2,   -50);
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Comparison),    trit{1})  << "99  > -50";

    assemble(Architecture::sixTrit::OpCode::CmpI, Architecture::sixTrit::Register::R2,   98);
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Comparison),    trit{1})  << "99 > 98";

    assemble(Architecture::sixTrit::OpCode::CmpI, Architecture::sixTrit::Register::R2,   110);
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Comparison),    trit{-1})  << "99 < 110";;

    assemble(Architecture::sixTrit::OpCode::CmpI, Architecture::sixTrit::Register::R2,   99);
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Comparison),    trit{0})  << "99 == 99";;
}


TEST_F(CPUTest, CompareRegister)
{

    ASSERT_EQ( cpu.getFlag(Architecture::Flag::Comparison),    trit{0});


    assembleAssign(Architecture::sixTrit::Register::R1, tryte{-42});
    assembleAssign(Architecture::sixTrit::Register::R2, tryte{ 99});

    cpu.execute();
    cpu.execute();


    assemble(Architecture::sixTrit::OpCode::CmpR, Architecture::sixTrit::Register::R1,   static_cast<int>(Register::R2));
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Comparison),    trit{-1})  << "R1 < R2";

    assemble(Architecture::sixTrit::OpCode::CmpR, Architecture::sixTrit::Register::R2,   static_cast<int>(Register::R1));
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Comparison),    trit{ 1})  << "R2 > R1";

    assemble(Architecture::sixTrit::OpCode::CmpR, Architecture::sixTrit::Register::R1,   static_cast<int>(Register::R1));
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Comparison),    trit{ 0})  << "R1 == R1";

    assemble(Architecture::sixTrit::OpCode::CmpR, Architecture::sixTrit::Register::R2,   static_cast<int>(Register::R2));
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Comparison),    trit{ 0})  << "R2 == R2";

}



TEST_F(CPUTest, Neg)
{
    assembleAssign(Architecture::sixTrit::Register::R0, tryte{ 0});
    assembleAssign(Architecture::sixTrit::Register::R1, tryte{"+-+-+-"});
    assembleAssign(Architecture::sixTrit::Register::R2, tryte{-42});

    assemble(Architecture::sixTrit::Neg, Architecture::sixTrit::Register::R0, tryte{0});
    assemble(Architecture::sixTrit::Neg, Architecture::sixTrit::Register::R1, tryte{0});
    assemble(Architecture::sixTrit::Neg, Architecture::sixTrit::Register::R2, tryte{0});

    cpu.execute();
    cpu.execute();
    cpu.execute();

    ASSERT_EQ( cpu.reg(Register::R0),    tryte{0})  ;
    ASSERT_EQ( cpu.reg(Register::R1),    tryte{"+-+-+-"})  ;
    ASSERT_EQ( cpu.reg(Register::R2),    tryte{-42})  ;


///


    cpu.execute();
    EXPECT_EQ( cpu.reg(Register::R0),    tryte{0}) ;
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{0})  ;


    cpu.execute();
    EXPECT_EQ( cpu.reg(Register::R1),    tryte{"-+-+-+"}) ;
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{-1})  ;

    cpu.execute();
    EXPECT_EQ( cpu.reg(Register::R2),    tryte{42}) ;
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{1})  ;
}



TEST_F(CPUTest, Shift0)
{
    assembleAssign(Architecture::sixTrit::Register::Rn1, tryte{"0000-0"});
    assembleAssign(Architecture::sixTrit::Register::R0,  tryte{"000000"});
    assembleAssign(Architecture::sixTrit::Register::R1,  tryte{"000+00"});

    cpu.execute();
    cpu.execute();
    cpu.execute();

    ASSERT_EQ( cpu.reg(Register::Rn1),   tryte{"0000-0"});
    ASSERT_EQ( cpu.reg(Register::R0),    tryte{"000000"});
    ASSERT_EQ( cpu.reg(Register::R1),    tryte{"000+00"});


    assemble(Architecture::sixTrit::Shift, Architecture::sixTrit::Register::Rn1, trybble{0}, trybble{0});
    assemble(Architecture::sixTrit::Shift, Architecture::sixTrit::Register::R0,  trybble{0}, trybble{0});
    assemble(Architecture::sixTrit::Shift, Architecture::sixTrit::Register::R1,  trybble{0}, trybble{0});

///
    cpu.execute();
    EXPECT_EQ( cpu.reg(Register::Rn1),    tryte{"0000-0"}) ;
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{-1})  ;

    cpu.execute();
    EXPECT_EQ( cpu.reg(Register::R0),    tryte{"000000"}) ;
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{0})  ;

    cpu.execute();
    EXPECT_EQ( cpu.reg(Register::R1),    tryte{"000+00"}) ;
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{1})  ;
}


TEST_F(CPUTest, Shift2)
{
    assembleAssign(Architecture::sixTrit::Register::Rn1, tryte{"0000-0"});
    assembleAssign(Architecture::sixTrit::Register::R0,  tryte{"000000"});
    assembleAssign(Architecture::sixTrit::Register::R1,  tryte{"000+00"});

    cpu.execute();
    cpu.execute();
    cpu.execute();

    ASSERT_EQ( cpu.reg(Register::Rn1),   tryte{"0000-0"});
    ASSERT_EQ( cpu.reg(Register::R0),    tryte{"000000"});
    ASSERT_EQ( cpu.reg(Register::R1),    tryte{"000+00"});


    assemble(Architecture::sixTrit::Shift, Architecture::sixTrit::Register::Rn1, trybble{2}, trybble{0});
    assemble(Architecture::sixTrit::Shift, Architecture::sixTrit::Register::R0,  trybble{2}, trybble{0});
    assemble(Architecture::sixTrit::Shift, Architecture::sixTrit::Register::R1,  trybble{2}, trybble{0});

///
    cpu.execute();
    EXPECT_EQ( cpu.reg(Register::Rn1),    tryte{"00-000"}) ;
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{-1})  ;

    cpu.execute();
    EXPECT_EQ( cpu.reg(Register::R0),    tryte{"000000"}) ;
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{0})  ;

    cpu.execute();
    EXPECT_EQ( cpu.reg(Register::R1),    tryte{"0+0000"}) ;
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{1})  ;
}


TEST_F(CPUTest, Shiftn2)
{
    assembleAssign(Architecture::sixTrit::Register::Rn1, tryte{"000-00"});
    assembleAssign(Architecture::sixTrit::Register::R0,  tryte{"000000"});
    assembleAssign(Architecture::sixTrit::Register::R1,  tryte{"00+000"});

    cpu.execute();
    cpu.execute();
    cpu.execute();

    ASSERT_EQ( cpu.reg(Register::Rn1),   tryte{"000-00"});
    ASSERT_EQ( cpu.reg(Register::R0),    tryte{"000000"});
    ASSERT_EQ( cpu.reg(Register::R1),    tryte{"00+000"});


    assemble(Architecture::sixTrit::Shift, Architecture::sixTrit::Register::Rn1, trybble{-2}, trybble{0});
    assemble(Architecture::sixTrit::Shift, Architecture::sixTrit::Register::R0,  trybble{-2}, trybble{0});
    assemble(Architecture::sixTrit::Shift, Architecture::sixTrit::Register::R1,  trybble{-2}, trybble{0});

///
    cpu.execute();
    EXPECT_EQ( cpu.reg(Register::Rn1),    tryte{"00000-"}) ;
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{-1})  ;

    cpu.execute();
    EXPECT_EQ( cpu.reg(Register::R0),    tryte{"000000"}) ;
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{0})  ;

    cpu.execute();
    EXPECT_EQ( cpu.reg(Register::R1),    tryte{"0000+0"}) ;
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{1})  ;
}





TEST_F(CPUTest, AddI)
{
    std::vector tests
    {
        std::make_tuple( tryte{ 0},        tryte{ 0},  tryte{ 0},        trit{ 0}),
        std::make_tuple( tryte{ 1},        tryte{ 0},  tryte{ 1},        trit{ 0}),
        std::make_tuple( tryte{ 0},        tryte{-1},  tryte{-1},        trit{ 0}),
        std::make_tuple( tryte{-1},        tryte{ 0},  tryte{-1},        trit{ 0}),
        std::make_tuple( tryte{ 0},        tryte{ 1},  tryte{ 1},        trit{ 0}),
        std::make_tuple( tryte{ 1},        tryte{ 2},  tryte{ 3},        trit{ 0}),
        std::make_tuple( tryte{ 2},        tryte{ 1},  tryte{ 3},        trit{ 0}),
        std::make_tuple( tryte{ 1},        tryte{-1},  tryte{ 0},        trit{ 0}),
        std::make_tuple( tryte{-1},        tryte{ 1},  tryte{ 0},        trit{ 0}),

        std::make_tuple( tryte{"++++++"},  tryte{-1},  tryte{"+++++0"},  trit{ 0}),
        std::make_tuple( tryte{"------"},  tryte{ 1},  tryte{"-----0"},  trit{ 0}),

        std::make_tuple( tryte{"++++++"},  tryte{ 1},  tryte{"------"},  trit{ 1}),
        std::make_tuple( tryte{"------"},  tryte{-1},  tryte{"++++++"},  trit{-1}),

        std::make_tuple( tryte{"+++++0"},  tryte{ 2},  tryte{"------"},  trit{ 1}),
        std::make_tuple( tryte{"-----0"},  tryte{-2},  tryte{"++++++"},  trit{-1}),
    };

    for(const auto test : tests)
    {
        const auto x                = std::get<0>(test);
        const auto y                = std::get<1>(test);
        const auto expectedResult   = std::get<2>(test);
        const auto expectedOverflow = std::get<3>(test);

        assembleAssign(Architecture::sixTrit::Register::R0,  x);
        assemble      (Architecture::sixTrit::AddI, Architecture::sixTrit::Register::R0,  y);

        cpu.execute();
        ASSERT_EQ( cpu.reg(Register::R0),    x);

        cpu.execute();
        EXPECT_EQ( cpu.reg(Register::R0),    expectedResult) ;
        EXPECT_EQ( cpu.getFlag(Architecture::Flag::Overflow),expectedOverflow)  ;
    }
}



TEST_F(CPUTest, AddRAdd)
{
    std::vector tests
    {
//                       X                 Y           sign         result            overflow
        std::make_tuple( tryte{ 0},        tryte{ 0},  trybble{1},  tryte{ 0},        trit{ 0}),
        std::make_tuple( tryte{ 1},        tryte{ 0},  trybble{1},  tryte{ 1},        trit{ 0}),
        std::make_tuple( tryte{ 0},        tryte{-1},  trybble{1},  tryte{-1},        trit{ 0}),
        std::make_tuple( tryte{-1},        tryte{ 0},  trybble{1},  tryte{-1},        trit{ 0}),
        std::make_tuple( tryte{ 0},        tryte{ 1},  trybble{1},  tryte{ 1},        trit{ 0}),
        std::make_tuple( tryte{ 1},        tryte{ 2},  trybble{1},  tryte{ 3},        trit{ 0}),
        std::make_tuple( tryte{ 2},        tryte{ 1},  trybble{1},  tryte{ 3},        trit{ 0}),
        std::make_tuple( tryte{ 1},        tryte{-1},  trybble{1},  tryte{ 0},        trit{ 0}),
        std::make_tuple( tryte{-1},        tryte{ 1},  trybble{1},  tryte{ 0},        trit{ 0}),

        std::make_tuple( tryte{"++++++"},  tryte{-1},  trybble{1},  tryte{"+++++0"},  trit{ 0}),
        std::make_tuple( tryte{"------"},  tryte{ 1},  trybble{1},  tryte{"-----0"},  trit{ 0}),

        std::make_tuple( tryte{"++++++"},  tryte{ 1},  trybble{1},  tryte{"------"},  trit{ 1}),
        std::make_tuple( tryte{"------"},  tryte{-1},  trybble{1},  tryte{"++++++"},  trit{-1}),

        std::make_tuple( tryte{"+++++0"},  tryte{ 2},  trybble{1},  tryte{"------"},  trit{ 1}),
        std::make_tuple( tryte{"-----0"},  tryte{-2},  trybble{1},  tryte{"++++++"},  trit{-1}),

    };

    for(const auto test : tests)
    {
        const auto x                = std::get<0>(test);
        const auto y                = std::get<1>(test);
        const auto sign             = std::get<2>(test);

        const auto expectedResult   = std::get<3>(test);
        const auto expectedOverflow = std::get<4>(test);

        assembleAssign(Architecture::sixTrit::Register::R0,  x);
        assembleAssign(Architecture::sixTrit::Register::R1,  y);
        assemble      (Architecture::sixTrit::AddR, Architecture::sixTrit::Register::R0,
                                                    trybble{static_cast<int>(Architecture::sixTrit::Register::R1)}, sign);

        cpu.execute();
        cpu.execute();
        ASSERT_EQ( cpu.reg(Register::R0),    x);
        ASSERT_EQ( cpu.reg(Register::R1),    y);

        cpu.execute();
        EXPECT_EQ( cpu.reg(Register::R0),    expectedResult) ;
        EXPECT_EQ( cpu.getFlag(Architecture::Flag::Overflow),expectedOverflow)  ;
    }
}




TEST_F(CPUTest, AddRSub)
{
    std::vector tests
    {
//                       X                 Y           sign          result            overflow
        std::make_tuple( tryte{ 0},        tryte{ 0},  trybble{-1},  tryte{ 0},        trit{ 0}),
        std::make_tuple( tryte{ 1},        tryte{ 0},  trybble{-1},  tryte{ 1},        trit{ 0}),
        std::make_tuple( tryte{ 2},        tryte{ 0},  trybble{-1},  tryte{ 2},        trit{ 0}),
        std::make_tuple( tryte{-1},        tryte{ 0},  trybble{-1},  tryte{-1},        trit{ 0}),
        std::make_tuple( tryte{-2},        tryte{ 0},  trybble{-1},  tryte{-2},        trit{ 0}),

        std::make_tuple( tryte{ 0},        tryte{ 1},  trybble{-1},  tryte{-1},        trit{ 0}),
        std::make_tuple( tryte{ 0},        tryte{ 2},  trybble{-1},  tryte{-2},        trit{ 0}),
        std::make_tuple( tryte{ 0},        tryte{-1},  trybble{-1},  tryte{ 1},        trit{ 0}),
        std::make_tuple( tryte{ 0},        tryte{-2},  trybble{-1},  tryte{ 2},        trit{ 0}),

        std::make_tuple( tryte{"++++++"},  tryte{-1},  trybble{-1},  tryte{"------"},  trit{ 1}),
        std::make_tuple( tryte{"------"},  tryte{ 1},  trybble{-1},  tryte{"++++++"},  trit{-1}),
    };

    for(const auto test : tests)
    {
        const auto x                = std::get<0>(test);
        const auto y                = std::get<1>(test);
        const auto sign             = std::get<2>(test);

        const auto expectedResult   = std::get<3>(test);
        const auto expectedOverflow = std::get<4>(test);

        assembleAssign(Architecture::sixTrit::Register::R0,  x);
        assembleAssign(Architecture::sixTrit::Register::R1,  y);
        assemble      (Architecture::sixTrit::AddR, Architecture::sixTrit::Register::R0,
                                                    trybble{static_cast<int>(Architecture::sixTrit::Register::R1)}, sign);

        cpu.execute();
        cpu.execute();
        ASSERT_EQ( cpu.reg(Register::R0),    x);
        ASSERT_EQ( cpu.reg(Register::R1),    y);

        cpu.execute();
        EXPECT_EQ( cpu.reg(Register::R0),    expectedResult) ;
        EXPECT_EQ( cpu.getFlag(Architecture::Flag::Overflow),expectedOverflow)  ;
    }
}
