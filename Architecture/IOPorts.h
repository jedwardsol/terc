#pragma once

#include "Arithmetic/tryte.h"
#include "exception.h"

namespace Architecture
{



struct IOPorts
{
    virtual ~IOPorts() = default;

    virtual Exception  out(const tryte    port,  const tryte    data)  =0;
    virtual Exception  in (const tryte    port,        tryte   &result)=0;
};




enum class KnownIOPorts     // tryte
{
    ExceptionOut = std::numeric_limits<tryte>::min(),

    ASCIIOut     = 0,
    tryteOut,
};


}