#pragma once

#include "exception.h"
#include "IOPorts.h"
#include "MemoryBlock.h"


namespace Architecture
{

enum  class CpuControl      // trybble
{
    Nop         = -1,         // do nothing
    Halt        =  0,         // do halt      (0 so default instruction is halt)
    Breakpoint,               // send breakpoint exception to exception port
    Trace,                    // send Trace exception to exception port
    Invalid
};



enum class Flag             // trybble
{
//  Flag                    // -1                           0                       +1

    Sign,                   // dest register                dest register           dest register
                            // is negative                  is zero                 is positive

    Overflow,               // result is too                no overflow             result is too
                            // negative                                             positive

    Comparison,             // X is smaller than            X is equal to           X is larger than
                            // Y or immediate               Y or immediate          Y or immediate


    ExecutedConditional     // conditional instruction      instruction wasn't      conditional instruction 
                            // wasn't executed              conditional             was executed
};




enum class Condition      // // trybble
{
    AlwaysFalse=-13,


    ConditionalExecuted,            // EC        +
    ConditionalNotExecuted,         // EC      -

    // TODO : test
    // TODO : assember
    Overflow,                       // O    - +
    OverflowPositive,               // O      +
    OverflowNegative,               // O    -
    NotOverflow,                    // O     0


    AlwaysTrue = 0,                 // 0 so empty memory  {"000000"}{"000000"}  decodes to unconditional halt instruction

    Positive,                       // sign      +
    Zero,                           // sign     0
    Negative,                       // sign    -
    NotPositive,                    // sign    -0
    NotZero,                        // sign    - +
    NotNegative,                    // sign     0+

    GreaterThan,                    // Compare   +
    Equal,                          // Compare  0
    LessThan,                       // Compare -
    LessThanOrEqual,                // Compare -0
    NotEqual,                       // Compare - +
    GreaterOrEqual,                 // Compare  0+
};

}


