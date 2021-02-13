#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <exception>
#include <system_error>
#include <stdexcept>
#include <map>

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/Arithmetic_std.h"

#include "Architecture/MemoryBlock.h"
#include "Architecture/sixTritCPU.h"

#include "sourceLine/sourceLine.h"


#include "assembler.h"

/*

Comments

    ; everything after a ; is a comment


Number formats

    42                      : decimal                                -364 to    364
    0+---0                  : 6 balanced ternary trits   range is  ------ to ++++++

Directives:

    .stack  N               :   set stack size
    .code                   :   assemble some instructions
    .data                   :   add some data
    .address N              :   change the current code or data position

Labels

    :label                  :   label the next address


Data

    N                       :   add a tryte to the data section
    "string"                :   add a nul terminated ascii string

*/




void Assembler::parseDirective(const SourceLine &source)
{
    auto directive = source.asString(0).value();

    if(directive == ".code")
    {
        mode = Mode::code;
    }
    else if(directive == ".data")
    {
        mode = Mode::data;
    }
    else if(directive == ".stack")
    {
        auto newStackSize = source.asTryte(1);

        if(!newStackSize)
        {
            error(".stack directive missing a valid size");
        }

        if(!stackSize)
        {
            stackSize = newStackSize.value().operator int();
        }
        else
        {
            if(newStackSize.value().operator int() > stackSize)
            {
                stackSize = newStackSize.value().operator int();
                warning("additional .stack directive.  size increased");
            }
            else
            {
                warning("additional .stack directive.  size ignored");
            }
        }

        mode = Mode::data;
    }
}




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

    assembler.makeMap();

    assembler.assemble();



}
catch(const std::exception &e)
{
    std::cout << e.what() << '\n';
}