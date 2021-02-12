
#include "googletest/gtest.h"

#include <iostream>
#include <string>
#include <sstream>
using namespace std::literals;

#include "CPUTest.h"




TEST_F(CPUTest, LoadImmediate)
{
    assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R1,  11);
    assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R2, -12);
    assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R3,  13);
    assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R4, -14);
    assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R5,  15);

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
    assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::RPC,  8);

    assemble(Architecture::sixTrit::OpCode::Out,            Architecture::sixTrit::Register::R0, static_cast<int>(Architecture::KnownIOPorts::ASCIIOut));
    assemble(Architecture::sixTrit::OpCode::Out,            Architecture::sixTrit::Register::R0, static_cast<int>(Architecture::KnownIOPorts::ASCIIOut));
    assemble(Architecture::sixTrit::OpCode::Out,            Architecture::sixTrit::Register::R0, static_cast<int>(Architecture::KnownIOPorts::ASCIIOut));

    assemble(Architecture::CpuControl::Halt);


    cpu.execute();
    cpu.execute();

    EXPECT_EQ(cpu.reg(Register::RPC),   8);
    EXPECT_EQ(cpu.reg(Register::REXC),  Architecture::Exception::Halted);
    ASSERT_EQ(outs.size(), 1);
    EXPECT_EQ(outs[0].first,  static_cast<int>(Architecture::KnownIOPorts::ExceptionOut));
    EXPECT_EQ(outs[0].second, Architecture::Exception::Halted);
}





TEST_F(CPUTest, MovIrBad)
{
    assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::REXC, Architecture::Exception::InvalidOpCode);

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC),  tryte{Architecture::Exception::Okay});

    cpu.execute();
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC),  tryte{Architecture::Exception::InvalidRegister});
}


TEST_F(CPUTest, Copy)
{
    assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R1, 111);
    assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R2, 112);
    assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R3, 113);
    assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R4, 114);
    assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R5, 115);

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


    assemble(Architecture::sixTrit::OpCode::Copy, Architecture::sixTrit::Register::R6,  static_cast<int>(Architecture::sixTrit::Register::R5));
    assemble(Architecture::sixTrit::OpCode::Copy, Architecture::sixTrit::Register::R7,  static_cast<int>(Architecture::sixTrit::Register::R4));
    assemble(Architecture::sixTrit::OpCode::Copy, Architecture::sixTrit::Register::R8,  static_cast<int>(Architecture::sixTrit::Register::R3));
    assemble(Architecture::sixTrit::OpCode::Copy, Architecture::sixTrit::Register::R9,  static_cast<int>(Architecture::sixTrit::Register::R2));
    assemble(Architecture::sixTrit::OpCode::Copy, Architecture::sixTrit::Register::R10, static_cast<int>(Architecture::sixTrit::Register::R1));

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
    assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::Rn1, -42);

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

    EXPECT_EQ( outs[0].first,  tryte{1} );
    EXPECT_EQ( outs[0].second, tryte{-42} );

    EXPECT_EQ( outs[1].first,  tryte{2} );
    EXPECT_EQ( outs[1].second, tryte{-42} );

    ASSERT_EQ( ins.size(), 2 );
    EXPECT_EQ( ins[0],  tryte{3} );
    EXPECT_EQ( ins[1],  tryte{4} );

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::Rn2), tryte{1});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::Rn3), tryte{2});

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC), tryte{Architecture::Exception::Okay});
}



TEST_F(CPUTest, DataLoadAndStore)
{
    auto addressReg = trybble{static_cast<int>(Architecture::sixTrit::Register::R13)};
    int  value      = 201;
    int  address    = 281;

    assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::R13, address);

    assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::Rn1, value-1);
    assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::R0,  value);
    assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::R1,  value+1);

    assemble(Architecture::sixTrit::OpCode::Store,          Architecture::sixTrit::Register::Rn1, addressReg, trybble{-1});      // write 201 to data[280]
    assemble(Architecture::sixTrit::OpCode::Store,          Architecture::sixTrit::Register::R0,  addressReg, trybble{ 0});      // write 202 to data[281]
    assemble(Architecture::sixTrit::OpCode::Store,          Architecture::sixTrit::Register::R1,  addressReg, trybble{+1});      // write 203 to data[282]

    assemble(Architecture::sixTrit::OpCode::Load,           Architecture::sixTrit::Register::R4,  addressReg, trybble{-1});      // load from data[280]
//  assemble(Architecture::CpuControl::Breakpoint);
    assemble(Architecture::sixTrit::OpCode::Load,           Architecture::sixTrit::Register::R5,  addressReg, trybble{ 0});      // load from data[281]
    assemble(Architecture::sixTrit::OpCode::Load,           Architecture::sixTrit::Register::R6,  addressReg, trybble{+1});      // load from data[282]


    ASSERT_EQ( data[address-1],0);
    ASSERT_EQ( data[address],  0);
    ASSERT_EQ( data[address+1],0);

    ASSERT_EQ( cpu.reg(Architecture::sixTrit::Register::R4),0);
    ASSERT_EQ( cpu.reg(Architecture::sixTrit::Register::R5),0);
    ASSERT_EQ( cpu.reg(Architecture::sixTrit::Register::R6),0);

    do
    {
        cpu.execute();
    } while(cpu.reg(Architecture::sixTrit::Register::REXC) <=0 );


    EXPECT_EQ( data[address-1],value-1);
    EXPECT_EQ( data[address],  value);
    EXPECT_EQ( data[address+1],value+1);

    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::R4),value-1);
    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::R5),value);
    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::R6),value+1);
}




TEST_F(CPUTest, StackLoadAndStore)
{
    auto addressReg = trybble{static_cast<int>(Architecture::sixTrit::Register::RSP)};
    int  value      = 123;
    int  address    = -30;

    assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::RSP, address);

    assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::Rn1, value-1);
    assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::R0,  value);
    assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::R1,  value+1);

    assemble(Architecture::sixTrit::OpCode::Store,      Architecture::sixTrit::Register::Rn1, addressReg, trybble{-1});      // write 201 to data[280]
    assemble(Architecture::sixTrit::OpCode::Store,      Architecture::sixTrit::Register::R0,  addressReg, trybble{ 0});      // write 202 to data[281]
    assemble(Architecture::sixTrit::OpCode::Store,      Architecture::sixTrit::Register::R1,  addressReg, trybble{+1});      // write 203 to data[282]

    assemble(Architecture::sixTrit::OpCode::Load,       Architecture::sixTrit::Register::R4, addressReg, trybble{-1});      // load from data[280]
    assemble(Architecture::sixTrit::OpCode::Load,       Architecture::sixTrit::Register::R5, addressReg, trybble{ 0});      // load from data[281]
    assemble(Architecture::sixTrit::OpCode::Load,       Architecture::sixTrit::Register::R6, addressReg, trybble{+1});      // load from data[282]


    ASSERT_EQ( data[address-1],0);
    ASSERT_EQ( data[address],  0);
    ASSERT_EQ( data[address+1],0);

    ASSERT_EQ( cpu.reg(Architecture::sixTrit::Register::R4),0);
    ASSERT_EQ( cpu.reg(Architecture::sixTrit::Register::R5),0);
    ASSERT_EQ( cpu.reg(Architecture::sixTrit::Register::R6),0);

    do
    {
        cpu.execute();
    } while(cpu.reg(Architecture::sixTrit::Register::REXC) <=0 );


    EXPECT_EQ( data[address-1],value-1);
    EXPECT_EQ( data[address],  value);
    EXPECT_EQ( data[address+1],value+1);

    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::R4),value-1);
    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::R5),value);
    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::R6),value+1);
}

TEST_F(CPUTest, AccessViolation)
{
    auto addressReg = trybble{static_cast<int>(Architecture::sixTrit::Register::RSP)};
    int  address    = -100;  // stack is 81

    assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::RSP, address);
    assemble(Architecture::sixTrit::OpCode::Load,           Architecture::sixTrit::Register::R5, addressReg, trybble{ 0});      // read from stack[-100]

    cpu.execute();
    cpu.execute();


    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::REXC),Architecture::Exception::AccessViolation);
}


TEST_F(CPUTest, PushPop)
{
    auto rsp = Architecture::sixTrit::Register::RSP;

    ASSERT_EQ( cpu.reg(rsp), 0 );
    ASSERT_EQ( data[-1] , 0 );
    ASSERT_EQ( data[-2] , 0 );
    ASSERT_EQ( data[-3] , 0 );

    assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::R0, 42);
    cpu.execute();


    assemble(Architecture::sixTrit::OpCode::Push,           Architecture::sixTrit::Register::R0, 0 );
    cpu.execute();
    EXPECT_EQ( cpu.reg(rsp), -1 );
    EXPECT_EQ( data[ -1] , 42 );

    assemble(Architecture::sixTrit::OpCode::Push,           Architecture::sixTrit::Register::R0, 0 );
    cpu.execute();
    EXPECT_EQ( cpu.reg(rsp), -2 );
    EXPECT_EQ( data[ -2] , 42 );

    assemble(Architecture::sixTrit::OpCode::Push,           Architecture::sixTrit::Register::R0, 0 );
    cpu.execute();
    EXPECT_EQ( cpu.reg(rsp), -3 );
    EXPECT_EQ( data[ -3] , 42 );


//--

    assemble(Architecture::sixTrit::OpCode::Pop,            Architecture::sixTrit::Register::R1, 0 );
    cpu.execute();
    EXPECT_EQ( cpu.reg(rsp), -2 );
    EXPECT_EQ( cpu.reg(Register::R1), 42 );

    assemble(Architecture::sixTrit::OpCode::Pop,            Architecture::sixTrit::Register::R2, 0 );
    cpu.execute();
    EXPECT_EQ( cpu.reg(rsp), -1 );
    EXPECT_EQ( cpu.reg(Register::R2), 42 );

    assemble(Architecture::sixTrit::OpCode::Pop,            Architecture::sixTrit::Register::R3, 0 );
    cpu.execute();
    EXPECT_EQ( cpu.reg(rsp), 0 );
    EXPECT_EQ( cpu.reg(Register::R3), 42 );


    EXPECT_NE( cpu.reg(Register::REXC), Architecture::Exception::AccessViolation );
}



TEST_F(CPUTest, StackOverflow)
{
    auto rsp = Architecture::sixTrit::Register::RSP;

    ASSERT_EQ( cpu.reg(rsp), 0 );

    assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::R0, 42);

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
    } while(cpu.reg(Architecture::sixTrit::Register::REXC) <=0 );



    EXPECT_EQ( cpu.reg(Register::REXC), Architecture::Exception::AccessViolation );
    EXPECT_EQ( cpu.reg(rsp), data.negativeSize()-1);
    EXPECT_EQ( data[ -1 ] , 42 );

}




TEST_F(CPUTest, StackUnderflow)
{
    auto rsp = Architecture::sixTrit::Register::RSP;

    ASSERT_EQ( cpu.reg(rsp), 0 );

    assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::R0, 42);

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
    } while(cpu.reg(Architecture::sixTrit::Register::REXC) <=0 );



    EXPECT_EQ( cpu.reg(Register::REXC), Architecture::Exception::AccessViolation );
    EXPECT_EQ( cpu.reg(Register::R0),   42 );
    EXPECT_EQ( cpu.reg(rsp), 0 );
}



TEST_F(CPUTest, Flags)
{
    ASSERT_EQ( cpu.reg(Register::RFlags), 0);

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


    assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::Rn1,  -9);
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{-1}) << "Load -";

    assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R0,   0);
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{0})  << "Load 0";;

    assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R1,   123);
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{1})  << "Load +";;


// 

    assemble(Architecture::sixTrit::OpCode::Copy,   Architecture::sixTrit::Register::R2,  static_cast<int>(Architecture::sixTrit::Register::Rn1));
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{-1})  << "Copy -";;

    assemble(Architecture::sixTrit::OpCode::Copy,   Architecture::sixTrit::Register::R2,  static_cast<int>(Architecture::sixTrit::Register::R0));
    cpu.execute();
    EXPECT_EQ( cpu.getFlag(Architecture::Flag::Sign),    trit{0})  << "Copy 0";

    assemble(Architecture::sixTrit::OpCode::Copy,   Architecture::sixTrit::Register::R2,  static_cast<int>(Architecture::sixTrit::Register::R1));
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


    assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R1, -42);
    assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R2,  99);

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


    assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R1, -42);
    assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R2,  99);

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







