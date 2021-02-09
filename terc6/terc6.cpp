#include <vector>
#include <string>
#include <iostream>
#include <exception>


#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/Arithmetic_std.h"

#include "Architecture/MemoryBlock.h"
#include "Architecture/sixTritCPU.h"

#pragma comment(lib,"Architecture")


int main()
try
{
    ROMemoryBlock   code {sixTritArchitecture::codeSize,  ".code" };        
    RWMemoryBlock   data {sixTritArchitecture::dataSize,  ".data" };        
    RWMemoryBlock   stack{sixTritArchitecture::stackSize, ".stack"};        

    sixTritArchitecture::CPU        cpu{code,data,stack};

    do
    {
        cpu.execute();
    } while(cpu.reg(sixTritArchitecture::Register::REXC) == sixTritArchitecture::ExceptionHalted);
}
catch(const std::exception &e)
{
    std::cout << "caught " << e.what() << '\n';
}