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
    MemoryBlock   code {sixTritArchitecture::instructionSectionSize, ".code" };        
    MemoryBlock   data {sixTritArchitecture::dataSectionSize,        ".data" };        
    MemoryBlock   stack{sixTritArchitecture::stackSectionSize,       ".stack"};        

}
catch(const std::exception &e)
{
    std::cout << "caught " << e.what() << '\n';
}