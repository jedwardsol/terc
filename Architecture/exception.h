#include "Arithmetic/Arithmetic_std.h"

#pragma once

namespace Architecture
{

enum  Exception         // min to max
{
// positive exceptions are fatal

    InvalidData = std::numeric_limits<tryte>::max(),
    InvalidPort,

    Okay        =  0,

// positive exceptions are fatal

    Halted,
    BadPC,                      // negative, odd,  
    InvalidOpCode,
    InvalidRegister,
    AccessViolation,            // bad address
    DoubleFault = std::numeric_limits<tryte>::max()     // execution after a fatal fault
};

}
