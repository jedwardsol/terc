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
#include "Architecture/exception.h"
#include "Architecture/IOPorts.h"


#pragma comment(lib,"Architecture")


using namespace Architecture::sixTrit;

class CPUTest : public ::testing::Test , public Architecture::IOPorts
{
protected:

    virtual void SetUp()
    {      
    }

    virtual void TearDown()
    {
    }

    virtual Architecture::Exception  out(const tryte    port,  const tryte    data)
    {
        if( static_cast<Architecture::KnownIOPorts>(static_cast<int>(port)) == Architecture::KnownIOPorts::ExceptionOut)
        {
            if(data > 1)
            {
                std::cout << "                 Exception " << data << " raised at address " << cpu.reg(Architecture::sixTrit::Register::REXA) << "\n";
            }
        }

        outs.emplace_back(port,data);
        return Architecture::Exception::Okay;
    }

    virtual Architecture::Exception  in (const tryte    port,        tryte   &result)
    {
        ins.emplace_back(port);
        result=tryte{ static_cast<int>(ins.size()) };
        return Architecture::Exception::Okay;
    }

    Architecture::RWMemoryBlock         code {Architecture::sixTrit::recCodeSize};    
    Architecture::RWMemoryBlock         data {Architecture::sixTrit::recDataSize};    
    Architecture::RWMemoryBlock         stack{Architecture::sixTrit::recStackSize};    
    Architecture::sixTrit::CPU          cpu  {code,data,stack,*this};

    void Assemble(Architecture::sixTrit::OpCode   opcode,
                  Architecture::sixTrit::Register reg,
                  int                             operand)
    {
        tryte   first  { trybble{static_cast<int>(opcode)}, trybble{static_cast<int>(reg)}};
        tryte   second { operand };

        code[PC++]   = first;
        code[PC++] = second;
    }


    void Assemble(Architecture::sixTrit::OpCode   opcode,
                  Architecture::sixTrit::Register reg,
                  trybble                         operandLow,
                  trybble                         operandHigh)
    {
        tryte   first  { trybble{static_cast<int>(opcode)}, trybble{static_cast<int>(reg)}};
        tryte   second { operandLow, operandHigh};

        code[PC++]   = first;
        code[PC++] = second;
    }



    int PC{0};

    std::vector<std::pair<tryte,tryte>> outs;
    std::vector<tryte>                  ins;

};



TEST_F(CPUTest, InitialState)
{
    EXPECT_EQ(code[0],  tryte{0});
    EXPECT_EQ(data[0],  tryte{0});
    EXPECT_EQ(stack[0], tryte{0});

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::R0),   tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::RPC),  tryte{0});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::RSP),  tryte{stack.size()});
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
    cpu.reg(Architecture::sixTrit::Register::RPC) = tryte{code.size() - 1};

    cpu.execute();

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::RPC),  tryte{code.size() - 1});
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC), tryte{Architecture::Exception::BadPC});
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


TEST_F(CPUTest, LoadImmediate)
{
    Assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R1,  11);
    Assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R2, -12);
    Assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R3,  13);
    Assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R4, -14);
    Assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R5,  15);

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
    Assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::REXC, Architecture::Exception::InvalidOpCode);

    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC),  tryte{Architecture::Exception::Okay});

    cpu.execute();
    EXPECT_EQ(cpu.reg(Architecture::sixTrit::Register::REXC),  tryte{Architecture::Exception::InvalidRegister});
}


TEST_F(CPUTest, Copy)
{
    Assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R1, 111);
    Assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R2, 112);
    Assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R3, 113);
    Assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R4, 114);
    Assemble(Architecture::sixTrit::OpCode::LoadImmediate, Architecture::sixTrit::Register::R5, 115);

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


    Assemble(Architecture::sixTrit::OpCode::Copy, Architecture::sixTrit::Register::R6,  static_cast<int>(Architecture::sixTrit::Register::R5));
    Assemble(Architecture::sixTrit::OpCode::Copy, Architecture::sixTrit::Register::R7,  static_cast<int>(Architecture::sixTrit::Register::R4));
    Assemble(Architecture::sixTrit::OpCode::Copy, Architecture::sixTrit::Register::R8,  static_cast<int>(Architecture::sixTrit::Register::R3));
    Assemble(Architecture::sixTrit::OpCode::Copy, Architecture::sixTrit::Register::R9,  static_cast<int>(Architecture::sixTrit::Register::R2));
    Assemble(Architecture::sixTrit::OpCode::Copy, Architecture::sixTrit::Register::R10, static_cast<int>(Architecture::sixTrit::Register::R1));

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
    Assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::Rn1, -42);

    Assemble(Architecture::sixTrit::OpCode::Out,            Architecture::sixTrit::Register::Rn1, 1);
    Assemble(Architecture::sixTrit::OpCode::Out,            Architecture::sixTrit::Register::Rn1, 2);

    Assemble(Architecture::sixTrit::OpCode::In,             Architecture::sixTrit::Register::Rn2, 3);
    Assemble(Architecture::sixTrit::OpCode::In,             Architecture::sixTrit::Register::Rn3, 4);

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
    auto addressReg = trybble{static_cast<int>(Architecture::sixTrit::Register::R0)};
    int  address    = 281;

    Assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::R0, address);

    Assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::R1, 201);
    Assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::R2, 202);
    Assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::R3, 203);


    Assemble(Architecture::sixTrit::OpCode::StoreData,      Architecture::sixTrit::Register::R1, addressReg, trybble{-1});      // write 201 to data[280]
    Assemble(Architecture::sixTrit::OpCode::StoreData,      Architecture::sixTrit::Register::R2, addressReg, trybble{ 0});      // write 202 to data[281]
    Assemble(Architecture::sixTrit::OpCode::StoreData,      Architecture::sixTrit::Register::R3, addressReg, trybble{+1});      // write 203 to data[282]


    Assemble(Architecture::sixTrit::OpCode::LoadData,       Architecture::sixTrit::Register::R4, addressReg, trybble{-1});      // load from data[280]
    Assemble(Architecture::sixTrit::OpCode::LoadData,       Architecture::sixTrit::Register::R5, addressReg, trybble{ 0});      // load from data[281]
    Assemble(Architecture::sixTrit::OpCode::LoadData,       Architecture::sixTrit::Register::R6, addressReg, trybble{+1});      // load from data[282]


    ASSERT_EQ( data[address-1],0);
    ASSERT_EQ( data[address],0);
    ASSERT_EQ( data[address+1],0);

    ASSERT_EQ( cpu.reg(Architecture::sixTrit::Register::R4),0);
    ASSERT_EQ( cpu.reg(Architecture::sixTrit::Register::R5),0);
    ASSERT_EQ( cpu.reg(Architecture::sixTrit::Register::R6),0);

    do
    {
        cpu.execute();
    } while(cpu.reg(Architecture::sixTrit::Register::REXC) <=0 );


    EXPECT_EQ( data[address-1],201);
    EXPECT_EQ( data[address],202);
    EXPECT_EQ( data[address+1],203);

    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::R4),201);
    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::R5),202);
    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::R6),203);
}




TEST_F(CPUTest, StackLoadAndStore)
{
    auto addressReg = trybble{static_cast<int>(Architecture::sixTrit::Register::RSP)};
    int  address    = 30;

    Assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::RSP, address);

    Assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::R1, -301);
    Assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::R2, -302);
    Assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::R3, -303);

    Assemble(Architecture::sixTrit::OpCode::StoreStack,     Architecture::sixTrit::Register::R1, addressReg, trybble{-1});      // write -301 to stack[30]
    Assemble(Architecture::sixTrit::OpCode::StoreStack,     Architecture::sixTrit::Register::R2, addressReg, trybble{ 0});      // write -302 to stack[31]
    Assemble(Architecture::sixTrit::OpCode::StoreStack,     Architecture::sixTrit::Register::R3, addressReg, trybble{+1});      // write -303 to stack[32]

    Assemble(Architecture::sixTrit::OpCode::LoadStack,      Architecture::sixTrit::Register::R4, addressReg, trybble{-1});      // read from stack[30]
    Assemble(Architecture::sixTrit::OpCode::LoadStack,      Architecture::sixTrit::Register::R5, addressReg, trybble{ 0});      // read from stack[31]
    Assemble(Architecture::sixTrit::OpCode::LoadStack,      Architecture::sixTrit::Register::R6, addressReg, trybble{+1});      // read from stack[32]

    ASSERT_EQ( stack[address-1],0);
    ASSERT_EQ( stack[address],0);
    ASSERT_EQ( stack[address+1],0);

    ASSERT_EQ( cpu.reg(Architecture::sixTrit::Register::R4),0);
    ASSERT_EQ( cpu.reg(Architecture::sixTrit::Register::R5),0);
    ASSERT_EQ( cpu.reg(Architecture::sixTrit::Register::R6),0);

    do
    {
        cpu.execute();
    } while(cpu.reg(Architecture::sixTrit::Register::REXC) <=0 );


    EXPECT_EQ( stack[address-1],-301);
    EXPECT_EQ( stack[address],-302);
    EXPECT_EQ( stack[address+1],-303);

    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::R4),-301);
    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::R5),-302);
    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::R6),-303);
}

TEST_F(CPUTest, AccessViolation)
{
    auto addressReg = trybble{static_cast<int>(Architecture::sixTrit::Register::RSP)};
    int  address    = 100;  // stack is 81

    Assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::RSP, address);
    Assemble(Architecture::sixTrit::OpCode::LoadStack,      Architecture::sixTrit::Register::R5, addressReg, trybble{ 0});      // read from stack[100]

    cpu.execute();
    cpu.execute();


    EXPECT_EQ( cpu.reg(Architecture::sixTrit::Register::REXC),Architecture::Exception::AccessViolation);
}


TEST_F(CPUTest, PushPop)
{
    auto rsp = Architecture::sixTrit::Register::RSP;

    ASSERT_EQ( cpu.reg(rsp), stack.size() );
    ASSERT_EQ( stack[ stack.size() - 1] , 0 );
    ASSERT_EQ( stack[ stack.size() - 2] , 0 );
    ASSERT_EQ( stack[ stack.size() - 3] , 0 );

    Assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::R0, 42);
    cpu.execute();


    Assemble(Architecture::sixTrit::OpCode::Push,           Architecture::sixTrit::Register::R0, 0 );
    cpu.execute();
    EXPECT_EQ( cpu.reg(rsp), stack.size()-1 );
    EXPECT_EQ( stack[ stack.size() - 1] , 42 );

    Assemble(Architecture::sixTrit::OpCode::Push,           Architecture::sixTrit::Register::R0, 0 );
    cpu.execute();
    EXPECT_EQ( cpu.reg(rsp), stack.size()-2 );
    EXPECT_EQ( stack[ stack.size() - 2] , 42 );

    Assemble(Architecture::sixTrit::OpCode::Push,           Architecture::sixTrit::Register::R0, 0 );
    cpu.execute();
    EXPECT_EQ( cpu.reg(rsp), stack.size()-3 );
    EXPECT_EQ( stack[ stack.size() - 3] , 42 );


//--

    Assemble(Architecture::sixTrit::OpCode::Pop,            Architecture::sixTrit::Register::R1, 0 );
    cpu.execute();
    EXPECT_EQ( cpu.reg(rsp), stack.size()-2 );
    EXPECT_EQ( cpu.reg(Register::R1), 42 );

    Assemble(Architecture::sixTrit::OpCode::Pop,            Architecture::sixTrit::Register::R2, 0 );
    cpu.execute();
    EXPECT_EQ( cpu.reg(rsp), stack.size()-1 );
    EXPECT_EQ( cpu.reg(Register::R2), 42 );

    Assemble(Architecture::sixTrit::OpCode::Pop,            Architecture::sixTrit::Register::R3, 0 );
    cpu.execute();
    EXPECT_EQ( cpu.reg(rsp), stack.size() );
    EXPECT_EQ( cpu.reg(Register::R3), 42 );


    EXPECT_NE( cpu.reg(Register::REXC), Architecture::Exception::AccessViolation );

}



TEST_F(CPUTest, StackOverflow)
{
    auto rsp = Architecture::sixTrit::Register::RSP;

    ASSERT_EQ( cpu.reg(rsp), stack.size() );

    Assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::R0, 42);

    for(int i=0;i<=stack.size();i++)
    {
        Assemble(Architecture::sixTrit::OpCode::Push,           Architecture::sixTrit::Register::R0, 0 );
    }


    do
    {
        cpu.execute();
    } while(cpu.reg(Architecture::sixTrit::Register::REXC) <=0 );



    EXPECT_EQ( cpu.reg(Register::REXC), Architecture::Exception::AccessViolation );
    EXPECT_EQ( cpu.reg(rsp), -1 );
    EXPECT_EQ( stack[ 0 ] , 42 );

}




TEST_F(CPUTest, StackUnderflow)
{
    auto rsp = Architecture::sixTrit::Register::RSP;

    ASSERT_EQ( cpu.reg(rsp), stack.size() );

    Assemble(Architecture::sixTrit::OpCode::LoadImmediate,  Architecture::sixTrit::Register::R0, 42);

    for(int i=0;i<3;i++)
    {
        Assemble(Architecture::sixTrit::OpCode::Push,           Architecture::sixTrit::Register::R0, i+42 );
    }

    for(int i=0;i<4;i++)
    {
        Assemble(Architecture::sixTrit::OpCode::Pop,           Architecture::sixTrit::Register::R0, 0 );
    }


    do
    {
        cpu.execute();
    } while(cpu.reg(Architecture::sixTrit::Register::REXC) <=0 );



    EXPECT_EQ( cpu.reg(Register::REXC), Architecture::Exception::AccessViolation );
    EXPECT_EQ( cpu.reg(Register::R0),   42 );
    EXPECT_EQ( cpu.reg(rsp), stack.size() );

}
