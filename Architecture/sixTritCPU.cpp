#define NOMINMAX
#include <Windows.h>

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


    if( reg(Register::RPC) < tryte{ 0 })
    {
        raiseException(Exception::BadPC, reg(Register::RPC));
        return false;
    }

    if( reg(Register::RPC) >= code.positiveSize()-1)
    {
        raiseException(Exception::BadPC, reg(Register::RPC));
        return false;
    }

    // TODO : check for odd


    instructionChangedRPC = false;

    return true;
}

bool    CPU::updatePC()
{
    if(!instructionChangedRPC)
    {
        trit carry{};

        auto newPC =  halfAdder(reg(Register::RPC),tryte{2},carry);
    
        if(carry != 0)
        {
            raiseException(Exception::BadPC, reg(Register::RPC));
            return false;
        }
    
        setReg(Register::RPC, newPC, ByPassRegisterChecks::yes);
    }

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

        setReg(opreg, operand, ByPassRegisterChecks::no);
        break;


    case OpCode::Copy:

        {
            auto srcreg  = static_cast<Register>(static_cast<int>(operand.trybbles().first));
            setReg(opreg, reg(srcreg), ByPassRegisterChecks::no);
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
            tryte value;

            auto exception = ioPorts.in(operand.trybbles().first, value);

            if(exception != Exception::Okay)
            {
                raiseException(exception, reg(Register::RPC));
            }
            else
            {
                setReg(opreg,value);
            }

        }
        break;


    case OpCode::Load:
        load(opreg,
             static_cast<sixTrit::Register>(operand.trybbles().first.operator int()),
             operand.trybbles().second);

        break;

    case OpCode::Store:
        store(opreg,
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
        setReg(Register::RPC, reg(Register::RPC), ByPassRegisterChecks::no);
        break;

    case OpCode::Nop:

        if(operand != 0)
        {
            if(::IsDebuggerPresent()) __debugbreak();
        }

        break;

    case OpCode::Invalid:
    default:
        raiseException(Exception::InvalidOpCode, reg(Register::RPC));
        break;
    }


    updatePC();

}



void CPU::load (sixTrit::Register   destReg,
                sixTrit::Register   addressReg, 
                trybble             offset)
{
    auto address = calculateAddress(addressReg, offset);

    if(address)
    {
        try
        {
            setReg(destReg,  data[address.value()]);
        }
        catch(const std::out_of_range &)
        {
            raiseException(Exception::AccessViolation, reg(Register::RPC));
        }
    }
}

void CPU::store(sixTrit::Register   sourceReg,
                sixTrit::Register   addressReg, 
                trybble             offset)
{
    auto address = calculateAddress(addressReg, offset);

    if(address)
    {
        try
        {
            data[address.value()] = reg(sourceReg);
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
        if(address.value() >= 0)
        {
            raiseException(Exception::AccessViolation, reg(Register::RPC));
        }
        else
        {
            try
            {
                setReg(Register::RSP, address.value());
                data[address.value()] = reg(sourceReg);
            }
            catch(const std::out_of_range &)
            {
                raiseException(Exception::AccessViolation, reg(Register::RPC));
            }
        }
    }
}

void CPU::pop  (Architecture::sixTrit::Register   destReg)
{
    auto currentAddress = reg(Register::RSP);
    auto newAddress     = calculateAddress(Register::RSP, trybble{+1});
    
    if(newAddress)
    {
        if(newAddress.value() > 0)
        {
            raiseException(Exception::AccessViolation, reg(Register::RPC));
        }
        else
        {
            try
            {
                setReg(destReg,        data[currentAddress]);
                setReg(Register::RSP,  newAddress.value());
            }
            catch(const std::out_of_range &)
            {
                raiseException(Exception::AccessViolation, reg(Register::RPC));
            }
        }
    }
}


}
