#include <vector>
#include <string>
#include <iostream>
#include <exception>


#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/Arithmetic_std.h"

#include "Architecture/MemoryBlock.h"
#include "Architecture/sixTritCPU.h"

#pragma comment(lib,"Architecture")


struct DummyIOPorts : Architecture::IOPorts
{
    ~DummyIOPorts() override = default;

    Architecture::Exception  out(const tryte    port,  const tryte    data) override
    {
        return Architecture::Exception::Okay;
    }

    Architecture::Exception  in (const tryte    port,        tryte   &result)
    {
        return Architecture::Exception::Okay;
    }
};


int main()
try
{
    const Architecture::MemoryBlock     code {".code" };        
          Architecture::MemoryBlock     data {".data" , ".data.final" };        
    DummyIOPorts                        ioPorts;

    Architecture::sixTrit::CPU    cpu{code,data,ioPorts};

    do
    {
        cpu.execute();
    } while(cpu.reg(Architecture::sixTrit::Register::REXC) <= 0);
}
catch(const std::exception &e)
{
    std::cout << "caught " << e.what() << '\n';
}