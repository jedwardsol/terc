#pragma once

#include "Arithmetic/tryte.h"


namespace Architecture
{

enum  Exception         // min to max
{
    DoubleFault = -1,               // execute called after an exception
    Okay        =  0,
    Halted,
    RanOffEnd,                      // should be some overflow pointing at RPC
    InvalidOpCode,
    InvalidRegister,
};


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