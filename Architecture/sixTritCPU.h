#pragma once

#include <vector>
#include <array>

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/tryte.h"
#include "Arithmetic/trit.h"


#include "MemoryBlock.h"




namespace  sixTritArchitecture
{

constexpr int     instructionBusSize        {6};
constexpr int     dataBusSize               {6};


constexpr int     codeSize                  {maxValue (instructionBusSize)};
constexpr int     dataSize                  {maxValue (dataBusSize)};
constexpr int     stackSize                 {maxValue (dataBusSize)};
constexpr int     numRegisters              {numValues(3)};


enum class Register
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


enum  Exception
{
    Okay = 0,
    Halted,
    RanOffEnd,
};

enum  OpCode
{
    Halt,
};



//constexpr   static tryte    ExceptionOkay     {0};
//constexpr   static tryte    ExceptionHalted   {1};
//constexpr   static tryte    ExceptionRanOffEnd{2};



class CPU
{
public:

    CPU(ROMemoryBlock   &code, RWMemoryBlock   &data, RWMemoryBlock   &stack) : code{code}, data{data}, stack{stack}
    {
        reg(Register::RSP)=tryte{stackSize};
    }

    tryte &reg(Register  r)
    {
        static std::array<tryte,numRegisters>       registers;

        constexpr int     registerOffset    {maxValue(3)};

        return registers[ static_cast<int>(r) + registerOffset];
    }


    void    execute();


private:

    ROMemoryBlock                       &code;
    RWMemoryBlock                       &data;
    RWMemoryBlock                       &stack;





};


}


