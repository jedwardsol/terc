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

#include <filesystem>
namespace fs=std::filesystem;

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/trit.h"
#include "Arithmetic/trit_std.h"
#include "Arithmetic/tryte.h"
#include "Arithmetic/tryte_std.h"

#include "Architecture/MemoryBlock.h"
#include "Architecture/sixTritCPU.h"

#include "sourceLine/sourceLine.h"


class Assembler
{
public:

    Assembler(fs::path filename) : sourceFileName{std::move(filename)}, sourceFile{sourceFileName}
    {
        if(!sourceFile)
        {
            throw std::system_error{errno,std::generic_category(),"Error opening source file"};
        }
    }

    void parseFile()
    {
        while(std::getline(sourceFile,currentLine))
        {
            currentLineNumber++;

            const auto  source = [&]
            {
                try
                {
                    return SourceLine{currentLine};
                }
                catch(const std::exception &e)
                {
                    error("Exception <"s + e.what() + "> parsing line");
                    throw;
                }
            }();

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

    void resolveDependencies();
    void writeMap();
    void writeSections();


private:

    fs::path                                sourceFileName;
    std::ifstream                           sourceFile;
    std::string                             currentLine;
    int                                     currentLineNumber{};

    int                                     currentCodePosition{};      
    int                                     maxCodePosition{};      
    int                                     minCodePosition{};

    int                                     currentDataPosition{};
    int                                     maxDataPosition{};
    int                                     stackSize{};



    void setDataPosition(int i)
    {
        if(i > std::numeric_limits<tryte>::max())
        {
            error("Data position overflow");
        }

        if(i < 0)
        {
            error("Data position underflow");
        }


        currentDataPosition=i;
        maxDataPosition = std::max(maxDataPosition,currentDataPosition);
    }

    void incrementDataPosition()
    {
        setDataPosition(currentDataPosition+1);
    }
    
    void setCodePosition(int i)
    {
        if(i > std::numeric_limits<tryte>::max())
        {
            error("Code position overflow");
        }

        if(i < std::numeric_limits<tryte>::min())
        {
            error("Code position underflow");
        }

        if(i % 2 == 1)
        {
            error("RPC can't be odd");
        }

        currentCodePosition=i;
        maxCodePosition = std::max(maxDataPosition,currentCodePosition);
        minCodePosition = std::min(minCodePosition,currentCodePosition);
    }

    void incrementCodePosition()
    {
        setCodePosition(currentCodePosition+2);
    }


    enum class Mode
    {
        none, code, data
    }                                       mode{ Mode::none};


    void parseDirective                         (const SourceLine &source);
    void parseData                              (const SourceLine &source);


    void parseCode                              (const SourceLine &source);
    void parseCode_OpCodeConditionCpuControl    (const SourceLine &source);
    void parseCode_OpCodeConditionImmediate     (const SourceLine &source);
    void parseCode_OpCodeConditionRegister      (const SourceLine &source);



    enum class RegisterIs
    {
        Source, Destination
    };
    void checkReadOnlyRegister(Architecture::sixTrit::Register reg);

    void parseCode_OpCodeRegister               (const SourceLine &source,RegisterIs registerIs);
    void parseCode_OpCodeRegisterImmediate      (const SourceLine &source,RegisterIs registerIs);
    void parseCode_OpCodeRegisterRegister       (const SourceLine &source,RegisterIs registerIs);
    void parseCode_OpCodeRegisterTrybble        (const SourceLine &source,RegisterIs registerIs);
    void parseCode_OpCodeRegisterRegisterTrybble(const SourceLine &source,RegisterIs registerIs);
    
    tryte parseImmediate                        (const SourceLine &source, int index);

    void addInstruction                         (tryte first, tryte second);



    struct Symbol
    {
        tryte   address;
        int     lineWhereDefined;
        int     numReferences;
    };

    struct Dependency
    {
        std::string symbol;
        int         lineWhereDefined;
    };


    Architecture::MemoryBlock     code{ BalancedTernary::maxValue(6), BalancedTernary::maxValue(6)};
    Architecture::MemoryBlock     data{ 0,                            BalancedTernary::maxValue(6)};


    std::map<std::string,Symbol>  dataSymbols;
    std::map<std::string,Symbol>  codeSymbols;
    std::map<tryte,Dependency>    dataDependencies;
    std::map<tryte,Dependency>    codeDependencies;


    void addDataSymbol    (std::string_view symbol);
    void addCodeSymbol    (std::string_view symbol);
    void addDataDependency(std::string_view symbol);
    void addCodeDependency(std::string_view symbol);


    Symbol  *findSymbol(const std::string &symbol);


    std::string formatMessage(const std::string &type, const std::string &message) const;

    void error          (const std::string &error) const;
    void warning        (const std::string &warning) const;


private:


};
