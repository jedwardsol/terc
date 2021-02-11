#include "Arithmetic/Arithmetic_std.h"

#pragma once

namespace Architecture
{

enum  CpuControl  
{
    Nop         = -1,
    Halt        =  0,
    Breakpoint  =  1,
    Invalid     =  2
};



enum  Exception         // min to max
{
// negative exceptions are not-fatal

    InvalidData = std::numeric_limits<tryte>::max(),
    InvalidPort,

    Okay        =  0,

// Halted is normal shutdown

    Halted,

// positive exceptions are fatal

    BadPC,                      // negative, odd,  
    InvalidOpCode,
    InvalidRegister,
    AccessViolation,            // bad address
    DoubleFault = std::numeric_limits<tryte>::max()     // execution after a fatal fault
};

}
