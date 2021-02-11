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


struct DummyIOPorts : IOPorts
{
    ~DummyIOPorts() override = default;

    Exception  out(const tryte    port,  const tryte    data) override
    {
        return Exception::Okay;
    }

    Exception  in (const tryte    port,        tryte   &result)
    {
        return Exception::Okay;
    }
};


enum class KnownIOPorts
{
    ExceptionOut = std::numeric_limits<tryte>::min(),

    ASCIIOut     = 0,
    tryteOut,
};


}