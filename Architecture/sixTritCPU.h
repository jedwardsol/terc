#pragma once

#include <vector>
#include <array>
#include <optional>

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/tryte.h"
#include "Arithmetic/trit.h"


#include "MemoryBlock.h"
#include "IOPorts.h"




namespace Architecture::sixTrit
{

constexpr int     instructionBusSize        {6};
constexpr int     dataBusSize               {6};


constexpr int     codeSize                  {maxValue (instructionBusSize)};
constexpr int     dataSize                  {maxValue (dataBusSize)};
constexpr int     stackSize                 {maxValue (dataBusSize)};
constexpr int     numRegisters              {numValues(3)};


enum class Register     // -13 to 13
{
    RPC  = -13,   // program counter        
    RSP  = -12,   // stack pointer
    RFL  = -11,   // flags
    RRA  = -10,   // return address
    REXC = -9,    // exception code
    REXA = -8,    // exception addess

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
        high trybble    :   register
    second tryte        :   arbitrary argument

 
 
*/



enum  OpCode        // -13 to 13
{
 // Opcode                  // register         operand                                                     exceptions

    Invalid = -13,          // unused           unused                                                      InvalidOpCode
    Nop,                    // unused           unused                                                      InvalidOpCode
    In11,                   // unused           unused                                                      InvalidOpCode
    In10,                   // unused           unused                                                      InvalidOpCode
    In9,                    // unused           unused                                                      InvalidOpCode
    In8,                    // unused           unused                                                      InvalidOpCode
    In7,                    // unused           unused                                                      InvalidOpCode
    In6,                    // unused           unused                                                      InvalidOpCode
    In5,                    // unused           unused                                                      InvalidOpCode
    In4,                    // unused           unused                                                      InvalidOpCode
    In3,                    // unused           unused                                                      InvalidOpCode
    In2,                    // unused           unused                                                      InvalidOpCode
    In1,                    // unused           unused                                                      InvalidOpCode

    Halt    =   0,          // unused           unused                                                      Halted

    LoadImmediate,          // destination      immediate                   dest = immediate                InvalidRegister if destination = REXC, REXA
    Copy,                   // destination      low:source                  destination = source            InvalidRegister if destination = REXC, REXA     
    Out,                    // source           low:port                    write source to port            InvalidPort,  InvalidData
    In,                     // destination      low:port                    read port to destination        InvalidPort,  InvalidData

    LoadData,               // destination      low:source high:offset      dest = [source+offset]          InvalidRegister if destination = REXC, REXA     
    StoreData,              // source           low:dest   high:offset      [dest+offset] = source          
    LoadStack,              // destination      low:source high:offset      dest = [source+offset]          InvalidRegister if destination = REXC, REXA     
    StoreStack,             // sourcee          low:dest   high:offset      [dest+offset] = source          

    I9,                     // unused           unused                                                      InvalidOpCode
    I10,                    // unused           unused                                                      InvalidOpCode
    I11,                    // unused           unused                                                      InvalidOpCode
    I12,                    // unused           unused                                                      InvalidOpCode
    I13,                    // unused           unused                                                      InvalidOpCode

};





class CPU
{
public:

    CPU(Architecture::ROMemoryBlock   &code, 
        Architecture::RWMemoryBlock   &data, 
        Architecture::RWMemoryBlock   &stack,
        Architecture::IOPorts         &ioPorts) : code{code}, data{data}, stack{stack},  ioPorts{ioPorts}
    {
        reg(Register::RSP)=tryte{stackSize};
    }

    tryte &reg(Register  r)
    {
        constexpr int     registerOffset    {maxValue(3)};

        return registers.at(static_cast<int>(r) + registerOffset);
    }


    void    execute();
    void    raiseException(Architecture::Exception code, tryte PC);

    void    load (Architecture::RWMemoryBlock       &memory, 
                  Architecture::sixTrit::Register   destReg,
                  Architecture::sixTrit::Register   addressReg, 
                  trybble                           offset);

    void    store(Architecture::RWMemoryBlock       &memory, 
                  Architecture::sixTrit::Register   sourceReg,
                  Architecture::sixTrit::Register   addressReg, 
                  trybble                           offset);

    std::optional<tryte> calculateAddress(Architecture::sixTrit::Register   addressReg, 
                                          trybble                           offset);
   

private:

    Architecture::ROMemoryBlock                       &code;
    Architecture::RWMemoryBlock                       &data;
    Architecture::RWMemoryBlock                       &stack;
    Architecture::IOPorts                             &ioPorts;

    std::array<tryte,numRegisters>       registers{};


};


}


