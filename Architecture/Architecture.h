#pragma once


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






}


