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
    O_Exception     = -maxValue(3),

    IO_ASCII        = 0,
    IO_tryte,
};


}