#include <vector>
#include <array>
#include <cassert>
#include <stdexcept>

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/trit.h"
#include "Arithmetic/trint.h"

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

    case Condition::Overflow:
    case Condition::OverflowPositive:
    case Condition::OverflowNegative:
    case Condition::NotOverflow:

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
    auto O = reg(Register::RFlags).getTrit(static_cast<int>(Flag::Overflow));

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

    case Condition::Overflow:                   return O != trit {0};
    case Condition::NotOverflow:                return O == trit {0};
    case Condition::OverflowPositive:           return O == trit {1};
    case Condition::OverflowNegative:           return O == trit {-1};
    }

    return true;
}



void CPU::executeConditionalInstructions(tryte  operation, tryte operand)
{
    auto opcode         = static_cast<OpCode>   (static_cast<int>(operation.halves().first));
    auto opcondition    = static_cast<Condition>(static_cast<int>(operation.halves().second));

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
        saveReturnAddress();
        jumpImmediate(operand);
        break;

    case OpCode::CallR:
        saveReturnAddress();
        jumpRegister(operand);
        break;

    case OpCode::JmpI:
        jumpImmediate(operand);
        break;
    case OpCode::JmpR:
        jumpRegister(operand);
        break;

    default:
        throw std::logic_error("Unhandled instruction");

    }

    setFlag(Flag::ExecutedConditional, trit{1});        // instruction did execute
}



void CPU::jumpImmediate(tryte operand)
{
    setReg(Register::RPC,operand,CPU::ByPassRegisterChecks::no);
}

void CPU::jumpRegister (tryte operand)
{
    auto jumpReg  = static_cast<Register>(static_cast<int>(operand.halves().first));
    auto jumpDest = reg(jumpReg);
    jumpImmediate(jumpDest);
}



}