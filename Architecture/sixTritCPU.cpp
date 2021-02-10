
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


    if( reg(Register::RPC) < tryte{ 0 })
    {
        raiseException(Exception::BadPC, reg(Register::RPC));
        return false;
    }

    if( reg(Register::RPC) >= code.size()-1)
    {
        raiseException(Exception::BadPC, reg(Register::RPC));
        return false;
    }

    // TODO : check for odd

    return true;
}

bool    CPU::updatePC()
{
    trit carry{};

    auto newPC =  halfAdder(reg(Register::RPC),tryte{2},carry);
    
    if(carry != 0)
    {
        raiseException(Exception::BadPC, reg(Register::RPC));
        return false;
    }
    
    reg(Register::RPC) = newPC;

    return true;
}


void CPU::execute()
{
    if(!checkPC())
    {
        return;
    }


    auto  operation      = code[reg(Register::RPC)];
    auto  operand        = code[reg(Register::RPC)+1];

    auto opcode = static_cast<OpCode>  (static_cast<int>(operation.trybbles().first));
    auto opreg  = static_cast<Register>(static_cast<int>(operation.trybbles().second));


    switch(opcode)
    {

    case OpCode::LoadImmediate:

        if(   opreg == Register::REXC
           || opreg == Register::REXA)
        {
            raiseException(Exception::InvalidRegister, reg(Register::RPC));
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
            raiseException(Exception::InvalidRegister, reg(Register::RPC));
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
                raiseException(exception, reg(Register::RPC));
            }
        }
        break;

    case OpCode::In:

        {
            auto exception = ioPorts.in(operand.trybbles().first, reg(opreg));

            if(exception != Exception::Okay)
            {
                raiseException(exception, reg(Register::RPC));
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


    case OpCode::Push:
        push(opreg);
        break;

    case OpCode::Pop:
        pop(opreg);
        break;



    case OpCode::Halt:
        raiseException(Exception::Halted, reg(Register::RPC));
        break;

    case OpCode::Nop:
        break;

    case OpCode::Invalid:
    default:
        raiseException(Exception::InvalidOpCode, reg(Register::RPC));
        break;
    }


    updatePC();

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
            raiseException(Exception::AccessViolation, reg(Register::RPC));
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
            raiseException(Exception::AccessViolation, reg(Register::RPC));
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
        raiseException(Exception::AccessViolation, reg(Register::RPC));
        return std::nullopt;
    }

    return address;
}

void CPU::push (Architecture::sixTrit::Register   sourceReg)
{
    auto address = calculateAddress(Register::RSP, trybble{-1});
    
    if(address)
    {
        try
        {
            reg(Register::RSP) = address.value();
            stack[address.value()] = reg(sourceReg);
        }
        catch(const std::out_of_range &)
        {
            raiseException(Exception::AccessViolation, reg(Register::RPC));
        }
    }
}

void CPU::pop  (Architecture::sixTrit::Register   destReg)
{
    auto currentAddress = reg(Register::RSP);
    auto newAddress     = calculateAddress(Register::RSP, trybble{+1});
    
    if(newAddress)
    {
        try
        {
            reg(destReg)       = stack[currentAddress] ;
            reg(Register::RSP) = newAddress.value();
        }
        catch(const std::out_of_range &)
        {
            raiseException(Exception::AccessViolation, reg(Register::RPC));
        }
    }
}


}
