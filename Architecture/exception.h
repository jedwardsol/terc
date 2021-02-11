#include "Arithmetic/Arithmetic_std.h"

#pragma once

namespace Architecture
{

enum  Exception         // min to max
{
// negative exceptions are not-fatal

    InvalidData = std::numeric_limits<tryte>::min(),
    InvalidPort,
    Breakpoint,
    Trace,

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
