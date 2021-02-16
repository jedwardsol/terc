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
    case OpCode::Assign:

        setReg(opreg, operand, ByPassRegisterChecks::no);
        updateSignFlag(opreg) ;
        break;

    case OpCode::Neg:

        {
            auto t = reg(opreg);
            t=-t;

            setReg(opreg, t, ByPassRegisterChecks::no);
            updateSignFlag(opreg) ;
        }
        break;

    case OpCode::Shift:

        {
            auto shift = static_cast<int>(operand.trybbles().first);

            auto t = reg(opreg);

            if(shift < 0)
            {
                t >>= -shift;
            }
            else
            {
                t <<= shift;
            }

            setReg(opreg, t, ByPassRegisterChecks::no);
            updateSignFlag(opreg) ;
        }
        break;


    case OpCode::Copy:
        {
            auto srcreg  = static_cast<Register>(static_cast<int>(operand.trybbles().first));
            setReg(opreg, reg(srcreg), ByPassRegisterChecks::no);
            updateSignFlag(opreg) ;
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
                updateSignFlag(opreg) ;
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

    case OpCode::CmpI:
        {
            auto X  =   reg(opreg);
            auto Y  =   operand;

            if(X < Y)
            {
                setFlag(Flag::Comparison, trit{-1});
            }
            else if(X==Y)
            {
                setFlag(Flag::Comparison, trit{ 0});
            }
            else
            {
                setFlag(Flag::Comparison, trit{ 1});
            }
        }
        break;

        case OpCode::CmpR:
        {
            auto X  =   reg(opreg);
            auto rY = static_cast<Register>(static_cast<int>(operand.trybbles().first));
            auto Y  =   reg(rY);

            if(X < Y)
            {
                setFlag(Flag::Comparison, trit{-1});
            }
            else if(X==Y)
            {
                setFlag(Flag::Comparison, trit{ 0});
            }
            else
            {
                setFlag(Flag::Comparison, trit{ 1});
            }
   
        }
        break;


    case OpCode::AddI:
        {
            auto t  =   reg(opreg);
            auto Y  =   operand;

            trit carry;
            t = halfAdder(t,Y,carry);

            setReg(opreg,t);
            setFlag(Flag::Overflow, carry);
            updateSignFlag(opreg) ;
        }
        break;


        case OpCode::AddR:
        {
            auto t    =  reg(opreg);
            auto rY   =  static_cast<Register>(static_cast<int>(operand.trybbles().first));
            auto Y    =  reg(rY);
            auto sign =  operand.trybbles().second;


            trit carry;
            
            if(sign == tryte{0})
            {
                Y=tryte{0};
            }
            else if(sign < tryte{0})
            {
                Y=-Y;                
            }

            t = halfAdder(t,Y,carry);

            setReg(opreg,t);
            setFlag(Flag::Overflow, carry);
            updateSignFlag(opreg) ;
        }
        break;


    default:
        throw std::logic_error("Unhandled instruction");

    }

    setFlag(Flag::ExecutedConditional, trit{0});        // instruction wasn't conditional
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
            updateSignFlag(destReg) ;
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

    if(carry != trit{0})
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
                updateSignFlag(destReg) ;
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