#pragma once

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
    int                                     currentLineNumber{};

    int                                     currentCodePosition{};      
    int                                     maxCodePosition{};      
    int                                     minCodePosition{};

    int                                     currentDataPosition{};
    int                                     maxDataPosition{};
    int                                     stackSize{};

    enum class Mode
    {
        none, code, data
    }                                       mode{ Mode::none};


    struct Symbol
    {
        tryte   address;
        int     lineWhereDefined;
    };

    struct Dependency
    {
        std::string symbol;
        int         lineWhereDefined;
    };


    std::map<std::string,Symbol>  dataSymbols;
    std::map<std::string,Symbol>  codeSymbols;
    std::map<tryte,Dependency>    dataDependencies;
    std::map<tryte,Dependency>    codeDependencies;

    Architecture::MemoryBlock     code{ maxValue(6), maxValue(6)};
    Architecture::MemoryBlock     data{ 0,           maxValue(6)};

    void addDataSymbol(std::string_view symbol);
    void addCodeSymbol(std::string_view symbol);



    void parseDirective(const SourceLine &source);
    void parseCode     (const SourceLine &source) {}
    void parseData     (const SourceLine &source) ;

    std::string formatMessage(const std::string &type, const std::string &message) const;

    void error          (const std::string &error) const;
    void warning        (const std::string &warning) const;


private:


};
