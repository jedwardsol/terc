#pragma once

#include "Arithmetic/tryte.h"
#include "exception.h"

namespace Architecture
{



struct IOPorts
{
    virtual ~IOPorts() = default;

    virtual Exception  out(const trybble    port,  const tryte    data)  =0;
    virtual Exception  in (const trybble    port,        tryte   &result)=0;
};




enum class KnownIOPorts     // trybble
{
    ExceptionOut = -maxValue(3),

    ASCIIOut     = 0,
    tryteOut,
};


}