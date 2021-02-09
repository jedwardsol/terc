
#include <vector>
#include <array>

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/trit.h"
#include "Arithmetic/tryte.h"

#include "MemoryBlock.h"
#include "sixTritCPU.h"


namespace sixTritArchitecture
{

void CPU::execute()
{
    if( reg(Register::REXC) !=  tryte{ Exception::Okay})
    {
        reg(Register::REXC) = tryte{Exception::DoubleFault};
        return;
    }

    if( reg(Register::RPC) > tryte{ codeSize-2})
    {
        reg(Register::REXC) = tryte{Exception::RanOffEnd};
        reg(Register::REXA) = reg(Register::RPC);
        return;
    }

    auto PC = reg(Register::RPC);
    reg(Register::RPC) =  tryte { static_cast<int>(reg(Register::RPC))+2 };     // TODO do tryte arithmetic
                                                                                // TODO can overflow here.  new excption


    auto  first          = code[PC];
    auto  second         = code[PC+1];
    auto [opcode, opreg] = first.trybbles();


    switch(opcode)
    {
    case OpCode::Halt:
        reg(Register::REXC) = tryte{Exception::Halted};
        reg(Register::REXA) = PC;
        break;

    case OpCode::Nop:
        break;


    case OpCode::Invalid:
    default:
        reg(Register::REXC) = tryte{Exception::InvalidOpCode};
        reg(Register::REXA) = PC;
        break;


    }





}



}
