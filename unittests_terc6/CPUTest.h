#define NOMINMAX
#include <Windows.h>
#include "googletest/gtest.h"


#include <iostream>
#include <vector>
#include <tuple>

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/Arithmetic_std.h"
#include "Arithmetic/tryte.h"

#include "Architecture/MemoryBlock.h"
#include "Architecture/sixTritCPU.h"
#include "Architecture/exception.h"
#include "Architecture/IOPorts.h"



using namespace Architecture::sixTrit;

class CPUTest : public ::testing::Test , public Architecture::IOPorts
{
protected:

    virtual void SetUp()
    {      
    }

    virtual void TearDown()
    {
    }

    virtual Architecture::Exception  out(const tryte    ioPort,  const tryte    data)
    {
        auto port = static_cast<Architecture::KnownIOPorts>(static_cast<int>(ioPort)) ;

        if(port == Architecture::KnownIOPorts::ExceptionOut)
        {
            auto exception = static_cast<Architecture::Exception>(static_cast<int>(data));

            switch(exception)
            {
            case Architecture::Exception::Trace:
                traceCounter++;
                break;

            case Architecture::Exception::Breakpoint:
                if(IsDebuggerPresent()) {__debugbreak();}
                break;
            }


            if(data > 1)
            {
                std::cout << "                 Exception " << data << " raised at address " << cpu.reg(Architecture::sixTrit::Register::REXA) << "\n";
            }
        }

        outs.emplace_back(ioPort,data);
        return Architecture::Exception::Okay;
    }

    virtual Architecture::Exception  in (const tryte    port,        tryte   &result)
    {
        ins.emplace_back(port);
        result=tryte{ static_cast<int>(ins.size()) };
        return Architecture::Exception::Okay;
    }

    Architecture::MemoryBlock           code {0,Architecture::sixTrit::recCodeSize};    
    Architecture::MemoryBlock           data {Architecture::sixTrit::recStackSize,
                                              Architecture::sixTrit::recDataSize};    

    Architecture::sixTrit::CPU          cpu  {code,data,*this};

    void Assemble(Architecture::sixTrit::OpCode   opcode,
                  Architecture::sixTrit::Register reg,
                  int                             operand)
    {
        tryte   first  { trybble{static_cast<int>(opcode)}, trybble{static_cast<int>(reg)}};
        tryte   second { operand };

        code[PC++]   = first;
        code[PC++] = second;
    }

    void Assemble(Architecture::sixTrit::OpCode   opcode,
                  Architecture::CpuControl        control)
    {
        tryte   first  { trybble{static_cast<int>(opcode)} , trybble{0}};
        tryte   second { trybble{static_cast<int>(control)}, trybble{0}};

        code[PC++]   = first;
        code[PC++] = second;
    }



    void Assemble(Architecture::sixTrit::OpCode   opcode,
                  Architecture::sixTrit::Register reg,
                  trybble                         operandLow,
                  trybble                         operandHigh)
    {
        tryte   first  { trybble{static_cast<int>(opcode)}, trybble{static_cast<int>(reg)}};
        tryte   second { operandLow, operandHigh};

        code[PC++]   = first;
        code[PC++] = second;
    }



    int                                 PC{0};
    std::vector<std::pair<tryte,tryte>> outs;
    std::vector<tryte>                  ins;
    int                                 traceCounter{};
};
