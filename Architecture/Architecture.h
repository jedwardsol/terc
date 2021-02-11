#pragma once

#include "exception.h"
#include "IOPorts.h"
#include "MemoryBlock.h"


namespace Architecture
{

enum  class CpuControl  
{
    Nop         = -1,         // do nothing
    Halt        =  0,         // do halt
    Breakpoint,               // send breakpoint exception to exception port
    Trace,                    // send Trace exception to exception port
    Invalid
};



enum class Flag
{
//  Flag                    // -1                           0                       +1

    Sign = 0,               // dest register                dest register           dest register
                            // is negative                  is zero                 is positive

    Conditional = 1         // conditional instruction      instruction wasn't      conditional instruction 
                            // wasn't executed              conditional             was executed
};


}

