#pragma once

#include "Arithmetic/tryte.h"


namespace Architecture
{

struct IOPorts
{
    virtual ~IOPorts() = default;

    virtual Exception  out(const trybble    port,  const tryte    data)  =0;
    virtual Exception  in (const trybble    port,        tryte   &result)=0;
};






}