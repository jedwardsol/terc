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

void CPU::execute()
{
    if(!checkPC())
    {
        return;
    }


    auto  operation      = code[reg(Register::RPC)];
    auto  operand        = code[reg(Register::RPC)+1];

    auto opcode = static_cast<OpCode>  (static_cast<int>(operation.trybbles().first));


    switch(opcode)
    {
    case OpCode::Assign:
    case OpCode::Copy:
    case OpCode::Out:
    case OpCode::In:
    case OpCode::Load:
    case OpCode::Store:
    case OpCode::Push:
    case OpCode::Pop:
    case OpCode::CmpI:
    case OpCode::CmpR:
    case OpCode::Neg:
    case OpCode::Shift:
    case OpCode::AddI:
    case OpCode::AddR:
        executeRegisterInstructions(operation, operand);
        break;

    case OpCode::CallI:
    case OpCode::CallR:
    case OpCode::JmpI:
    case OpCode::JmpR:
    case OpCode::CpuControl:

        executeConditionalInstructions(operation, operand);
        break;
 
    default:
        raiseException(Exception::InvalidOpCode, reg(Register::RPC));
        break;
    }

    updatePC();
}




void CPU::raiseException(Exception code, tryte PC)
{
    static auto exceptionPort = tryte{ static_cast<int>( KnownIOPorts::ExceptionOut)};
           auto exceptionCode = tryte{ static_cast<int>( code)};

    setReg(Register::REXC, exceptionCode, ByPassRegisterChecks::yes);
    setReg(Register::REXA, PC,            ByPassRegisterChecks::yes);

    ioPorts.out( exceptionPort  ,exceptionCode);
}




bool    CPU::checkPC()
{
    if( reg(Register::REXC) ==  tryte{ Exception::DoubleFault})
    {
        throw std::runtime_error{"Continue after double fault"};
    }

    if( reg(Register::REXC) >  tryte{ Exception::Okay})
    {
        raiseException(Exception::DoubleFault, reg(Register::REXA));
        return false;
    }

// let instruction fetch fault instead
    if( reg(Register::RPC) < tryte{ 0 })            // TODO : allow negative code.  good place for standard library?
    {
        raiseException(Exception::BadPC, reg(Register::RPC));
        return false;
    }

    if( reg(Register::RPC) >= code.positiveSize()-1)
    {
        raiseException(Exception::BadPC, reg(Register::RPC));
        return false;
    }

    if( reg(Register::RPC) % 2 == 1)
    {
        raiseException(Exception::BadPC, reg(Register::RPC));
        return false;
    }
    


    instructionChangedRPC = false;

    return true;
}

bool    CPU::updatePC()
{
    if(!instructionChangedRPC)
    {
        trit carry{};

        auto newPC =  halfAdder(reg(Register::RPC),tryte{2},carry);
    
        if(carry)
        {
            raiseException(Exception::BadPC, reg(Register::RPC));
            return false;
        }
    
        setReg(Register::RPC, newPC, ByPassRegisterChecks::yes);
    }

    return true;
}



void CPU::CpuControl(tryte  operand)
{
    auto cpuControl = static_cast<Architecture::CpuControl>(static_cast<int>(operand.trybbles().first));

    switch(cpuControl)
    {
    case Architecture::CpuControl::Halt:
        raiseException(Exception::Halted, reg(Register::RPC));
        setReg(Register::RPC, reg(Register::RPC), ByPassRegisterChecks::no);
        break;

    case Architecture::CpuControl::Nop:
        break;

    case Architecture::CpuControl::Breakpoint:
        raiseException(Exception::Breakpoint, reg(Register::RPC));
        break;

    case Architecture::CpuControl::Trace:
        raiseException(Exception::Trace, reg(Register::RPC));
        break;

    default:
        raiseException(Exception::InvalidOpCode, reg(Register::RPC));
        break;
    }
}





}

