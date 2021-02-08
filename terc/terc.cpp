#include <vector>
#include <string>
#include <iostream>
#include <exception>

#include "Architecture/Architecture.h"
#include "Architecture/MemoryBlock.h"

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/Arithmetic_std.h"



int main()
try
{

    ROMemoryBlock   code {sixTritArchitecture::instructionSectionSize, ".code" };        
    RWMemoryBlock   data {sixTritArchitecture::instructionSectionSize, ".data" };        
    RWMemoryBlock   stack{sixTritArchitecture::instructionSectionSize, ".stack"};        


    stack[0]=tryte{1};

}
catch(const std::exception &e)
{
    std::cout << "caught " << e.what() << '\n';
}