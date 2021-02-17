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
#include <filesystem>
namespace fs=std::filesystem;


#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/trint.h"
#include "Arithmetic/trint_std.h"

#include "Architecture/MemoryBlock.h"
#include "terc6cpu/sixTritCPU.h"

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


    fs::path  sourceName{args[0]};

    if(!sourceName.has_extension())
    {
        sourceName.replace_extension(".terc6");
    }

    Assembler assembler{sourceName};
    
    assembler.parseFile();
    assembler.resolveDependencies();
    assembler.writeMap();
    assembler.writeSections();

    std::cout << "Success\n";

}
catch(const std::exception &e)
{
    std::cout << e.what() << '\n';
}



void Assembler::writeSections()
try
{
    auto    codeFileName{sourceFileName};
    auto    dataFileName{sourceFileName};

    codeFileName.replace_extension(".code");
    dataFileName.replace_extension(".data");

    Architecture::MemoryBlock   codeFile{ -minCodePosition, maxCodePosition, codeFileName.string()};
    Architecture::MemoryBlock   dataFile{  stackSize,       maxDataPosition, dataFileName.string()};

    memcpy(&codeFile[minCodePosition],  &code[minCodePosition],  (maxCodePosition-minCodePosition)*sizeof(tryte));

    if(maxDataPosition)
    {
        memcpy(&dataFile[0],                &data[0],                (maxDataPosition)*sizeof(tryte));
    }

}
catch(const std::exception &e)
{
    std::cout << "writeSections : " << e.what() << '\n';
}