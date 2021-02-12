#include <vector>
#include <array>
#include <cassert>

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/trit.h"
#include "Arithmetic/tryte.h"

#include "Architecture.h"
#include "MemoryBlock.h"
#include "sixTritCPU.h"


namespace Architecture::sixTrit
{




bool CPU::validCondition(Condition condition)
{
    switch(condition)
    {
    case Condition::AlwaysFalse:
    case Condition::ConditionalExecuted:
    case Condition::ConditionalNotExecuted:
    case Condition::AlwaysTrue:
    case Condition::Positive:
    case Condition::Zero:
    case Condition::Negative:
    case Condition::NotPositive:
    case Condition::NotZero:
    case Condition::NotNegative:
    case Condition::GreaterThan:
    case Condition::Equal:
    case Condition::LessThan:
    case Condition::LessThanOrEqual:
    case Condition::NotEqual:
    case Condition::GreaterOrEqual:
        return true;

    default:    
        return false;
    }
}



void CPU::executeConditionalInstructions(tryte  operation, tryte operand)
{
    auto opcode         = static_cast<OpCode>   (static_cast<int>(operation.trybbles().first));
    auto opcondition    = static_cast<Condition>(static_cast<int>(operation.trybbles().second));

    if(!validCondition(opcondition))
    {
        raiseException(Exception::InvalidCondition,reg(Register::RPC));
        return;
    }



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