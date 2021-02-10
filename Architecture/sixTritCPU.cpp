
#include <vector>
#include <array>

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/trit.h"
#include "Arithmetic/tryte.h"

#include "MemoryBlock.h"
#include "sixTritCPU.h"


namespace Architecture::sixTrit
{

void CPU::raiseException(Exception code, tryte PC)
{
    static auto exceptionPort = tryte{ static_cast<int>( KnownIOPorts::ExceptionOut)};
           auto exceptionCode = tryte{ static_cast<int>( code)};

    reg(Register::REXC) = exceptionCode;
    reg(Register::REXA) = PC;


    ioPorts.out( exceptionPort  ,exceptionCode);
}

bool    CPU::updatePC()
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


    if( reg(Register::RPC) < tryte{ 0 })
    {
        raiseException(Exception::BadPC, reg(Register::RPC));
        return false;
    }

    if( reg(Register::RPC) >= code.size())
    {
        raiseException(Exception::BadPC, reg(Register::RPC));
        return false;
    }


    trit carry{};

    auto newPC =  halfAdder(reg(Register::RPC),tryte{2},carry);
    
    if(carry != 0)
    {
        raiseException(Exception::BadPC, currentPC);
        return false;
    }
    
    currentPC = reg(Register::RPC);
    reg(Register::RPC) = newPC;

    return true;
}


void CPU::execute()
{
    if(!updatePC())
    {
        return;
    }


    auto  first          = code[currentPC];
    auto  operand        = code[currentPC+1];

    auto opcode = static_cast<OpCode>  (static_cast<int>(first.trybbles().first));
    auto opreg  = static_cast<Register>(static_cast<int>(first.trybbles().second));


    switch(opcode)
    {
    case OpCode::Halt:

        raiseException(Exception::Halted, currentPC);
        break;

    case OpCode::Nop:
        break;

    case OpCode::LoadImmediate:

        if(   opreg == Register::REXC
           || opreg == Register::REXA)
        {
            raiseException(Exception::InvalidRegister, currentPC);
        }
        else
        {
            reg(opreg) = operand;
        }
        break;


    case OpCode::Copy:

        if(   opreg == Register::REXC
           || opreg == Register::REXA)
        {
            raiseException(Exception::InvalidRegister, currentPC);
        }
        else
        {
            auto srcreg  = static_cast<Register>(static_cast<int>(operand.trybbles().first));

            reg(opreg) = reg(srcreg);
        }
        break;



    case OpCode::Out:

        {
            auto exception = ioPorts.out(operand.trybbles().first, reg(opreg));

            if(exception != Exception::Okay)
            {
                raiseException(exception, currentPC);
            }
        }
        break;

    case OpCode::In:

        {
            auto exception = ioPorts.in(operand.trybbles().first, reg(opreg));

            if(exception != Exception::Okay)
            {
                raiseException(exception, currentPC);
            }
        }
        break;


    case OpCode::LoadData:
        load(data,
             opreg,
             static_cast<sixTrit::Register>(operand.trybbles().first.operator int()),
             operand.trybbles().second);

        break;

    case OpCode::LoadStack:
        load(stack,
             opreg,
             static_cast<sixTrit::Register>(operand.trybbles().first.operator int()),
             operand.trybbles().second);
        break;

    case OpCode::StoreData:
        store(data,
              opreg,
              static_cast<sixTrit::Register>(operand.trybbles().first.operator int()),
              operand.trybbles().second);
        break;

    case OpCode::StoreStack:
        store(stack,
              opreg,
              static_cast<sixTrit::Register>(operand.trybbles().first.operator int()),
              operand.trybbles().second);
        break;


    case OpCode::Invalid:
    default:
        raiseException(Exception::InvalidOpCode, currentPC);
        break;
    }
}



void CPU::load (RWMemoryBlock       &memory, 
                sixTrit::Register   destReg,
                sixTrit::Register   addressReg, 
                trybble             offset)
{
    auto address = calculateAddress(addressReg, offset);

    if(address)
    {
        try
        {
            reg(destReg) = memory[address.value()];
        }
        catch(const std::out_of_range &)
        {
            raiseException(Exception::AccessViolation, currentPC);
        }
    }
}

void CPU::store(RWMemoryBlock       &memory, 
                sixTrit::Register   sourceReg,
                sixTrit::Register   addressReg, 
                trybble             offset)
{
    auto address = calculateAddress(addressReg, offset);

    if(address)
    {
        try
        {
            memory[address.value()] = reg(sourceReg);
        }
        catch(const std::out_of_range &)
        {
            raiseException(Exception::AccessViolation, currentPC);
        }
    }
}


std::optional<tryte> CPU::calculateAddress(Architecture::sixTrit::Register   addressReg, 
                                           trybble                           offset)
{
    auto address = reg(addressReg);

    trit    carry;
    address = halfAdder(address,offset,carry);

    if(carry != 0)
    {
        raiseException(Exception::AccessViolation, currentPC);
        return std::nullopt;
    }

    return address;
}



}
