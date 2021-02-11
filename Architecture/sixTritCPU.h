#pragma once

#include <vector>
#include <array>
#include <optional>

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/tryte.h"
#include "Arithmetic/trit.h"

#include <Architecture/Architecture.h>
#include "Architecture/exception.h"
#include "MemoryBlock.h"
#include "IOPorts.h"



namespace Architecture::sixTrit
{

constexpr int     instructionBusSize        {6};
constexpr int     dataBusSize               {6};


constexpr int     maxCodeSize               {maxValue (instructionBusSize)};
constexpr int     maxDataSize               {maxValue (dataBusSize)};
constexpr int     maxStackSize              {maxValue (dataBusSize)};

constexpr int     recCodeSize               {maxCodeSize};
constexpr int     recDataSize               {maxDataSize};
constexpr int     recStackSize              {81};


constexpr int     numRegisters              {numValues(3)};


enum class Register     // -13 to 13
{
    RPC     = -13,   // program counter        
    RSP     = -12,   // stack pointer
    RFlags  = -11,   // flags
    RRA     = -10,   // return address
    REXC    = -9,    // exception code
    REXA    = -8,    // exception address

    // General purpose registers

    Rn7,
    Rn6,
    Rn5,
    Rn4, 
    Rn3, 
    Rn2, 
    Rn1, 
    R0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
};



/*
 
Each instruction is 2 trytes

    first tryte
        low trybble     :   opcode
        high trybble    :   register, condition, misc
    second tryte        :   arbitrary argument
 
*/



enum  OpCode        // -13 to 13
{
 // Opcode                  // condition        operand                                                             exceptions
                                                                   
    In13=-13,               // unused           unused                                                              InvalidOpCode
    In12,                   // unused           break                                                               InvalidOpCode
    In11,                   // unused           unused                                                              InvalidOpCode
    In10,                   // unused           unused                                                              InvalidOpCode
    In9,                    // unused           unused                                                              InvalidOpCode
    In8,                    // unused           unused                                                              InvalidOpCode
    In7,                    // unused           unused                                                              InvalidOpCode
    In6,                    // unused           unused                                                              InvalidOpCode
    In5,                    // unused           unused                                                              InvalidOpCode

 // Opcode                  // condition        operand                                                             exceptions

    CallI,                  // condition        immediate destination                                                              
    CallR,                  // condition        low: register destination 
    JmpI,                   // condition        immediate destination                                                               
    JmpR,                   // condition        low: register destination                                                           
                                                                   


    CpuControl =   0,       // condition        CpuControl                                                          Halted, InvalidOpCode, Breakpoint (TODO)


 // Opcode                  // register         operand                                                             exceptions

                                                                   
    LoadImmediate,          // destination      immediate                           dest = immediate                InvalidRegister if destination is readonly
    Copy,                   // destination      low:source                          destination = source            InvalidRegister if destination is readonly
    Out,                    // source           low:port                            write source to port            InvalidPort,  InvalidData
    In,                     // destination      low:port                            read port to destination        InvalidPort,  InvalidData
                                                                   
    Load,                   // destination      low:regsource      high:offset      dest = [source+offset]          AccessViolation is address is out of range.  InvalidRegister if destination is readonly
    Store,                  // source           low:regdest        high:offset      [dest+offset] = source          AccessViolation is address is out of range.  
                                                                   
    Push,                   // source           unused                              SP-- stack[SP]=src              StackOverflow if stack is full
    Pop,                    // destination      unused                              dest=stack[SP] SP++             StackOverflow if stack is empty      InvalidRegister if destination is readonly

    I9,                     // unused           unused                                                              InvalidOpCode
    I10,                    // unused           unused                                                              InvalidOpCode
    I11,                    // unused           unused                                                              InvalidOpCode
    I12,                    // unused           unused                                                              InvalidOpCode
    I13,                    // unused           unused                                                              InvalidOpCode

};





class CPU
{
public:

    CPU(Architecture::MemoryBlock   const &code, 
        Architecture::MemoryBlock         &data, 
        Architecture::IOPorts             &ioPorts) : code   {code}, 
                                                      data   {data},  
                                                      ioPorts{ioPorts}
    {}

#pragma warning(push)
#pragma warning(disable: 26451)    // overflow in indexing operation

    const tryte &reg(Register  r) const
    {
        constexpr int     registerOffset    {maxValue(3)};

        return registers.at(static_cast<int>(r) + registerOffset);
    }

    enum class ByPassRegisterChecks
    {
        no, yes
    };

    void setReg(Register  r,   tryte  value,  ByPassRegisterChecks byPassRegisterChecks= ByPassRegisterChecks::no) 
    {
        constexpr int     registerOffset    {maxValue(3)};

        if(byPassRegisterChecks == ByPassRegisterChecks::no)
        {
            if(r == Register::RPC)
            {
                instructionChangedRPC = true;
            }
            else if(   r == Register::REXA
                    || r == Register::REXC
                    || r == Register::RFlags)
            {
                // write to read-only register!
                raiseException(Exception::InvalidRegister, reg(Register::RPC));
                return;
            }
            else if(   r == Register::RSP)
            {
                if(value > 0)
                {
                    // RSP can't be in data zone
                    raiseException(Exception::AccessViolation, reg(Register::RPC));
                    return;
                }
            }
        }

        registers.at(static_cast<int>(r) + registerOffset) = value;
    }



    void updateSignFlag(Register  r) 
    {
        const auto value = reg(r);

        if(value < tryte{0})
        {
            setFlag(Flag::Sign, trit{-1});    
        }
        else if(value == tryte{0})
        {
            setFlag(Flag::Sign, trit{0});    
        }
        else 
        {
            setFlag(Flag::Sign, trit{1});    
        }
    }




#pragma warning(pop)


    trit getFlag(Flag flag) const
    {
        return reg(Register::RFlags).getTrit(static_cast<int>(flag));
    }

    void setFlag(Flag flag,   trit value) 
    {
        auto flags = reg(Register::RFlags);
        
        flags.setTrit(static_cast<int>(flag), value);

        setReg(Register::RFlags, flags,  ByPassRegisterChecks::yes);
    }




    void    execute();
    void    executeRegisterInstructions(tryte  operation, tryte operand);
    void    executeConditionalInstructions(tryte  operation, tryte operand);

private:

    bool    checkPC();
    bool    updatePC();

    void    raiseException(Architecture::Exception code, tryte PC);

    void    load (Architecture::sixTrit::Register   destReg,
                  Architecture::sixTrit::Register   addressReg, 
                  trybble                           offset);

    void    store(Architecture::sixTrit::Register   sourceReg,
                  Architecture::sixTrit::Register   addressReg, 
                  trybble                           offset);


    void    push (Architecture::sixTrit::Register   sourceReg);
    void    pop  (Architecture::sixTrit::Register   destReg);


    std::optional<tryte> calculateAddress(Architecture::sixTrit::Register   addressReg, 
                                          trybble                           offset);


    void    CpuControl(tryte  operand);
   

private:

    Architecture::MemoryBlock const                &code;
    Architecture::MemoryBlock                      &data;
    Architecture::IOPorts                          &ioPorts;

    std::array<tryte,numRegisters>                  registers{};    // only to be touched in setReg and getReg.  Here for debugability

    bool                                            instructionChangedRPC{};      // TODO : move to flag
};


}


