#include <vector>
#include <array>
#include <cassert>

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/trit.h"
#include "Arithmetic/tryte.h"

#include "MemoryBlock.h"
#include "sixTritCPU.h"


namespace Architecture::sixTrit
{


void CPU::executeConditionalInstructions(tryte  operation, tryte operand)
{
    auto opcode = static_cast<OpCode>  (static_cast<int>(operation.trybbles().first));
//    auto opreg  = static_cast<Register>(static_cast<int>(operation.trybbles().second));

    switch(opcode)
    {
    case OpCode::CpuControl:
        CpuControl(operand);
        break;


    case OpCode::CallI:
    case OpCode::CallR:
    case OpCode::JmpI:
    case OpCode::JmpR:
    default:
        assert(false);

    }
}

}