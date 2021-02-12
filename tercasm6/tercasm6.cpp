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

#include "sourceLine/sourceLine.h"


/*

Comments

    ; everything after a ; is a comment


Number formats

    42                      : decimal                                -364 to    364
    0+---0                  : 6 balanced ternary trits   range is  ------ to ++++++

Directives:

    .StackSize  N           :   mandatory before .Begin

    .Begin                  :   mandatory


    .End                    :   mandatory

*/



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

    void parseFile()
    {
        std::string  line;

        while(std::getline(source,line))
        {
            const SourceLine  source{line};

            if(source.tokens().empty())
            {
                continue;
            }
        }
    }

    void assemble()
    {
    }

private:

    std::ifstream                           source;
    int                                     lineNumber        {1};

private:

    Architecture::MemoryBlock     code {0,                                  Architecture::sixTrit::recCodeSize,  ".code" };        
//    Architecture::MemoryBlock     data {Architecture::sixTrit::recStackSize,Architecture::sixTrit::recDataSize,  ".data" };        
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
    
    assembler.parseFile();

    assembler.assemble();


}
catch(const std::exception &e)
{
    std::cout << "caught " << e.what() << '\n';
}