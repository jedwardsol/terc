#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <iostream>

#include <exception>
#include <system_error>
#include <stdexcept>


#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/Arithmetic_std.h"


#include "Architecture/MemoryBlock.h"
#include "Architecture/sixTritCPU.h"



class Assembler
{
public:

    Assembler(const std::string_view &filename) : source{filename}
    {
        if(!source)
        {
            throw std::system_error{errno,std::generic_category(),"Error opening source file"};
        }
    }

    void assemble()
    {

    }


private:

    std::ifstream   source;

    int             lineNumber        {1};
    int             currentInstruction{0};
    int             currentDataTryte  {0};

    Architecture::MemoryBlock     code {0,                                  Architecture::sixTrit::recCodeSize,  ".code" };        
    Architecture::MemoryBlock     data {Architecture::sixTrit::recStackSize,Architecture::sixTrit::recDataSize,  ".data" };        
};


int main(int argc, const char *argv[])
try
{
    std::vector<std::string_view>   args(argv+1, argv+argc);

    if(args.empty())
    {
        throw std::runtime_error{"Missing input FileName"};
    }


    Assembler assembler{args[0]};
    
    assembler.assemble();



}
catch(const std::exception &e)
{
    std::cout << "caught " << e.what() << '\n';
}