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


void CPU::executeRegisterInstructions(tryte  operation, tryte operand)
{
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

    default:
        assert(false);
    }
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