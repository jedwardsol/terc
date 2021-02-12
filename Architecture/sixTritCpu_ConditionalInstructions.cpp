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

bool  CPU::isConditionTrue(Condition condition)
{
    auto S = reg(Register::RFlags).getTrit(static_cast<int>(Flag::Sign));
    auto C = reg(Register::RFlags).getTrit(static_cast<int>(Flag::Comparison));
    auto E = reg(Register::RFlags).getTrit(static_cast<int>(Flag::ExecutedConditional));

    switch(condition)
    {
    case Condition::AlwaysFalse:                return false;
    case Condition::AlwaysTrue:                 return true;

    case Condition::ConditionalExecuted:        return E == trit{1};                        // E        +
    case Condition::ConditionalNotExecuted:     return E == trit{-1};                       // E      -

    case Condition::Positive:                   return S == trit{1};
    case Condition::Zero:                       return S == trit{0};
    case Condition::Negative:                   return S == trit{-1};
    case Condition::NotPositive:                return S != trit{1};
    case Condition::NotZero:                    return S != trit{0};
    case Condition::NotNegative:                return S != trit{-1};

    case Condition::GreaterThan:                return C == trit{1};
    case Condition::Equal:                      return C == trit{0};
    case Condition::LessThan:                   return C == trit{-1};
    case Condition::LessThanOrEqual:            return C != trit{1};
    case Condition::NotEqual:                   return C != trit{0};
    case Condition::GreaterOrEqual:             return C != trit{-1};
    }

    return true;
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

    if(!isConditionTrue(opcondition))
    {
        setFlag(Flag::ExecutedConditional, trit{-1});        // instruction didn't execute
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

    setFlag(Flag::ExecutedConditional, trit{1});        // instruction did execute
}

}