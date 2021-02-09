#include <vector>
#include <string>
#include <iostream>
#include <exception>




#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/Arithmetic_std.h"


#include "Architecture/MemoryBlock.h"
#include "Architecture/sixTritCPU.h"


int main()
try
{
    MemoryBlock   code {sixTritArchitecture::codeSize,  ".code" };        
    MemoryBlock   data {sixTritArchitecture::dataSize,  ".data" };        
    MemoryBlock   stack{sixTritArchitecture::stackSize, ".stack"};        

}
catch(const std::exception &e)
{
    std::cout << "caught " << e.what() << '\n';
}