
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
    auto  operand        = code[PC+1];

    auto opcode = static_cast<OpCode>  (static_cast<int>(first.trybbles().first));
    auto opreg  = static_cast<Register>(static_cast<int>(first.trybbles().second));


    switch(opcode)
    {
    case OpCode::Halt:
        reg(Register::REXC) = tryte{Exception::Halted};
        reg(Register::REXA) = PC;
        break;

    case OpCode::Nop:
        break;

    case OpCode::MovIR:

        if(   opreg == Register::REXC
           || opreg == Register::REXA)
        {
            reg(Register::REXC) = tryte{Exception::InvalidRegister};
            reg(Register::REXA) = PC;
        }
        else
        {
            reg(opreg) = operand;
        }
        break;


    case OpCode::MovRR:

        if(   opreg == Register::REXC
           || opreg == Register::REXA)
        {
            reg(Register::REXC) = tryte{Exception::InvalidRegister};
            reg(Register::REXA) = PC;
        }
        else if(operand.trybbles().second != trybble{0})
        {
            reg(Register::REXC) = tryte{Exception::InvalidRegister};
            reg(Register::REXA) = PC;
        }
        else
        {
            auto srcreg  = static_cast<Register>(static_cast<int>(operand.trybbles().first));

            reg(opreg) = reg(srcreg);
        }
        break;


    case OpCode::Invalid:
    default:
        reg(Register::REXC) = tryte{Exception::InvalidOpCode};
        reg(Register::REXA) = PC;
        break;


    }





}



}
