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

    .stack  N               :   mandatory somewhere
    .code                   :   assemble some instructions
    .data                   :   add some data

*/



class Assembler
{
public:

    Assembler(const std::string_view &filename) : sourceFile{filename}
    {
        if(!sourceFile)
        {
            throw std::system_error{errno,std::generic_category(),"Error opening source file"};
        }
    }

    void parseFile()
    {

        while(   std::getline(sourceFile,currentLine))
        {
            currentLineNumber++;

            const SourceLine  source{currentLine};

            if(source.tokens().empty())
            {
                continue;
            }

            const auto &token = source.asString(0).value();

            if(token[0] == '.')
            {
                parseDirective(source);
            }
            else
            {
                switch(mode)
                {
                case Mode::none:
                    error("Non-directive seen outside .code or .data");
                    break;

                case Mode::code:
                    parseCode(source);
                    break;

                case Mode::data:
                    parseData(source);
                    break;
                }
            }
        }
    }

    void assemble()
    {
    }

private:

    std::ifstream                           sourceFile;
    std::string                             currentLine;
   int                                      currentLineNumber{};

    enum class Mode
    {
        none, code, data
    }                                       mode{ Mode::none};


    void parseDirective(const SourceLine &source);
    void parseCode     (const SourceLine &source) {}
    void parseData     (const SourceLine &source) {}


    auto format(const std::string &type, const std::string &message)
    {
        std::ostringstream str;

        str << type 
            << " at line "s 
            << std::setw(5) << std::to_string(currentLineNumber) 
            << " : <"s 
            << message
            << "> : "s 
            << currentLine;

        return str.str();
    }

    void error         (const std::string &error)
    {
        throw std::runtime_error(  format("Error",error));
    }

    void warning        (const std::string &warning)
    {
        std::cout << format("Warning", warning) << "\n";
    }


private:

    int                                     stackSize{};

};

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

    assembler.assemble();


}
catch(const std::exception &e)
{
    std::cout << e.what() << '\n';
}