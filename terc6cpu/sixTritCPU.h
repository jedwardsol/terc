#pragma once

#include <vector>
#include <array>
#include <optional>
#include <cassert>
#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/trint.h"
#include "Arithmetic/trit.h"

#include <Architecture/Architecture.h>

#include "Architecture/MemoryBlock.h"
#include "Architecture/IOPorts.h"



namespace Architecture::sixTrit
{

constexpr int     instructionBusSize        {6};
constexpr int     dataBusSize               {6};


constexpr int     maxCodeSize               {BalancedTernary::maxValue (instructionBusSize)};
constexpr int     maxDataSize               {BalancedTernary::maxValue (dataBusSize)};
constexpr int     maxStackSize              {BalancedTernary::maxValue (dataBusSize)};

constexpr int     recCodeSize               {maxCodeSize};
constexpr int     recDataSize               {maxDataSize};
constexpr int     recStackSize              {81};


constexpr int     numRegisters              {BalancedTernary::numValues(3)};


enum class Register     // // trybble   -13 to 13
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
        high trybble    :   register or condition
    second tryte        :   arbitrary argument
 
*/


enum  OpCode        // // trybble  -13 to 13
{
 // Opcode                  // condition        operand                                                             flags           exceptions
                                                                                                                         
    In13=-13,               // unused           unused                                                                              InvalidOpCode
    Out,                    // source           low:port                            write source to port            E              InvalidPort,  InvalidData
    In,                     // destination      low:port                            read port to destination        E S            InvalidPort,  InvalidData
    In10,                   // unused           unused                                                                              InvalidOpCode
    In9,                    // unused           unused                                                                              InvalidOpCode
    In8,                    // unused           unused                                                                              InvalidOpCode
    In7,                    // unused           unused                                                                              InvalidOpCode
    In6,                    // unused           unused                                                                              InvalidOpCode
    In5,                    // unused           unused                                                                              InvalidOpCode
                                                                                                                         
 // Opcode                  // condition        operand                                                                             exceptions
                                                                                                                         
    CallI,                  // condition        immediate destination                                               E                                           Ret is LoadImmediate RPC, RRA,  or pop RPC  if RRA was pushed
    CallR,                  // condition        low: register destination                                           E                                           Ret is LoadImmediate RPC, RRA,  or pop RPC  if RRA was pushed
    JmpI,                   // condition        immediate destination                                               E                          
    JmpR,                   // condition        low: register destination                                           E                          
                                                                                                                         
    CpuControl =   0,       // condition        CpuControl                                                          E              Halted, InvalidOpCode, Breakpoint Trace
                                                                                                                         
 // Opcode                  // register         operand                                                                             exceptions
                                                                                                                         
    Assign,                 // destination      immediate                           dest = immediate                E S            InvalidRegister if destination is readonly
    Copy,                   // destination      low:source                          destination = source            E S            InvalidRegister if destination is readonly
    Load,                   // destination      low:regsource      high:offset      dest = [source+offset]          E S            AccessViolation is address is out of range.  InvalidRegister if destination is readonly
    Store,                  // source           low:regdest        high:offset      [dest+offset] = source          E              AccessViolation is address is out of range.  
    Push,                   // source           unused                              SP-- stack[SP]=src              E              StackOverflow if stack is full
    Pop,                    // destination      unused                              dest=stack[SP] SP++             E S            StackOverflow if stack is empty      InvalidRegister if destination is readonly
    CmpI,                   // Reg X            immediate                                                           E C         
    CmpR,                   // Reg X            low:: Reg Y                                                         E C         
    Neg,                    // reg              unused                              reg = -reg                      E S                         
    Shift,                  // reg              low:N                               reg<<=N or reg >>=N             E S                
    AddI,                   // reg              immediate                           reg += I                        E S O            
    AddR,                   // reg              low:reg2   high: direction          reg += reg2 * sign(dir)         E S O           
    I13,                    // unused           unused                                                                              InvalidOpCode
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
        constexpr int     registerOffset    {BalancedTernary::maxValue(3)};

        return registers.at(static_cast<int>(r) + registerOffset);
    }

    
//private:    
    enum class ByPassRegisterChecks
    {
        no, yes
    };

    void setReg(Register  r,   tryte  value,  ByPassRegisterChecks byPassRegisterChecks= ByPassRegisterChecks::no) 
    {
        constexpr int     registerOffset    {BalancedTernary::maxValue(3)};

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
                if(value > tryte{0})
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

public:
    // flag isn't under the control of any instruction
    trit getFlag(Flag flag) const
    {
        return reg(Register::RFlags).getTrit(static_cast<int>(flag));
    }


//private:
    void setFlag(Flag flag,   trit value) 
    {
        assert(    flag == Architecture::Flag::Comparison
               ||  flag == Architecture::Flag::ExecutedConditional
               ||  flag == Architecture::Flag::Overflow
               ||  flag == Architecture::Flag::Sign);

        auto flags = reg(Register::RFlags);
        
        flags.setTrit(static_cast<int>(flag), value);

        setReg(Register::RFlags, flags,  ByPassRegisterChecks::yes);
    }


private:
    std::optional<std::pair<tryte, tryte>> fetch();

public:
    void    execute();

private:
    void    executeRegisterInstructions(tryte  operation, tryte operand);
    void    executeConditionalInstructions(tryte  operation, tryte operand);

    bool    validCondition (Condition condition);
    bool    isConditionTrue(Condition condition);

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
   

    void    jumpImmediate(tryte operand);
    void    jumpRegister (tryte operand);

    void    saveReturnAddress()
    {
        trit carry{};

        auto returnAddress = halfAdder(reg(Register::RPC),tryte{2}, carry);

        if(carry)
        {
            raiseException(Exception::BadPC, reg(Register::RPC));
        }
        else
        {
            setReg(Register::RRA, returnAddress, CPU::ByPassRegisterChecks::no);
        }
    }


private:

    Architecture::MemoryBlock const                &code;
    Architecture::MemoryBlock                      &data;
    Architecture::IOPorts                          &ioPorts;

    std::array<tryte,numRegisters>                  registers{};    // only to be touched in setReg and getReg.  Here for debugability

    bool                                            instructionChangedRPC{};
};


}



#pragma comment(lib,"terc6cpu")
