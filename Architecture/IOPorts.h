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
        return static_cast<Exception>(0);
    }

    Exception  in (const tryte    port,        tryte   &result)
    {
        return static_cast<Exception>(0);
    }
};


enum KnownIOPorts
{
    ASCIIOut,
    tryteOut,
};


}