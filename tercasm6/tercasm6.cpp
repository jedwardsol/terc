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