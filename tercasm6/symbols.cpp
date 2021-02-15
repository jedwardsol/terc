#include <fstream>
#include <iomanip>
#include <string>
#include <cassert>
#include "assembler.h"

#pragma comment(lib,"Arithmetic")

using namespace std::literals;


Assembler::Symbol *Assembler::findSymbol(const std::string &symbol)
{
    if(symbol[0] == '&')
    {
        auto found = dataSymbols.find(symbol);

        if(found == dataSymbols.end())
        {
            return nullptr;
        }
        else
        {
            return &found->second;
        }
    }
    else 
    {
        auto found = codeSymbols.find(symbol);

        if(found == codeSymbols.end())
        {
            return nullptr;
        }
        else
        {
            return &found->second;
        }
    }
}


void Assembler::resolveDependencies()
{
    for(const auto &[address, dependency] : codeDependencies)
    {
        auto symbol = findSymbol(dependency.symbol);

        if(!symbol)
        {
            throw std::runtime_error(  "Error : Unresolved symbol "s + dependency.symbol + " from line "s + std::to_string(dependency.lineWhereDefined));
        }

        symbol->numReferences++;

        code[address + 1] = symbol->address;
    }


    for(const auto &[address, dependency] : dataDependencies)
    {
        auto symbol = findSymbol(dependency.symbol);

        if(!symbol)
        {
            throw std::runtime_error(  "Error : Unresolved symbol "s + dependency.symbol + " from line "s + std::to_string(dependency.lineWhereDefined));
        }

        symbol->numReferences++;
        data[address] = symbol->address;
    }



}



void Assembler::writeMap()
{
    auto mapName{sourceFileName};

    mapName.replace_extension(".map");

    std::ofstream  map{mapName};

    if(!map)
    {
        std::cout << "Can't open map file\n";
        return;
    }

    map   <<  "Code Section :   " << tryte{minCodePosition} << " - " << tryte{maxCodePosition} << '\n';
    map   <<  "Data Section :   " << tryte{0}               << " - " << tryte{maxDataPosition} << '\n';
    map   <<  "Stack Size   :   " << tryte{stackSize} << '\n';


    map << "\nSymbols in code section\n";
    for(const auto &[symbol, location] : codeSymbols)
    {
        map << "    " << std::left 
                      << std::setw(16) << symbol << " : " 
                      << std::setw(16) << location.address 
                      << "    Referenced " << std::setw(2) << location.numReferences << " times \n";
    }

    map << "\nDependencies in code section\n";
    for(const auto &[address,dependency] : codeDependencies)
    {
        map << "    " << std::left << std::setw(16) << address << " : " << dependency.symbol << "\n";
    }

    map << "\nSymbols in data section\n";
    for(const auto &[symbol,location] : dataSymbols)
    {
        map << "    " << std::left 
                      << std::setw(16) << symbol  << " : "  
                      << std::setw(16) << location.address 
                      << "    Referenced " << std::setw(2) << location.numReferences << " times \n";
    }

    map << "\nDependencies in data section\n";
    for(const auto &[address,dependency] : dataDependencies)
    {
        map << "    " << std::left << std::setw(16) << address << " : " << dependency.symbol << "\n";
    }
}



void Assembler::addDataSymbol(std::string_view label)
{
    assert(label[0]==':');

    label.remove_prefix(1);
    
    if(label.size()==0)
    {
        error("Empty data label");
    }

    auto symbol = "&"s + std::string{label};

    if(auto found = dataSymbols.find(symbol); found != dataSymbols.end())
    {
        error("Duplicate data label. First defined at line "s + std::to_string(found->second.lineWhereDefined));
    }

    dataSymbols[symbol] = {tryte{currentDataPosition},currentLineNumber,0};
}


void Assembler::addDataDependency(std::string_view symbol)
{
    assert(symbol[0]=='$' || symbol[0]=='&');

    if(symbol.size()==0)
    {
        error("Empty data symbol");
    }

    dataDependencies[tryte{currentDataPosition}] = { std::string{symbol}, currentLineNumber};
}




void Assembler::addCodeSymbol(std::string_view label)
{
    assert(label[0]==':');

    label.remove_prefix(1);

    if(label.size()==0)
    {
        error("Empty code label");
    }

    auto symbol = "$"s + std::string{label};

    if(auto found = codeSymbols.find(symbol); found != codeSymbols.end())
    {
        error("Duplicate code label. First defined at line "s + std::to_string(found->second.lineWhereDefined));
    }

    codeSymbols[symbol] = {tryte{currentCodePosition},currentLineNumber,0};
}


void Assembler::addCodeDependency(std::string_view symbol)
{
    assert(symbol[0]=='$' || symbol[0]=='&');

    if(symbol.size()==0)
    {
        error("Empty code symbol");
    }

    codeDependencies[tryte{currentCodePosition}] = { std::string{symbol}, currentLineNumber};
}


tryte Assembler::parseImmediate (const SourceLine &source, int index)
{
    auto string = source.asString(index).value();

    tryte t;

    if(    string[0]=='&'
       ||  string[0]=='$')
    {
        t= tryte{0};

        if(mode == Mode::data)
        {
            addDataDependency(string);
        }
        else
        {
            addCodeDependency(string);
        }
    }
    else
    {
        auto value = source.asTryte(index);

        if(!value)
        {
            error("Unparsable tryte at "s + std::to_string(index));
        }

        t=value.value();
    }

    return t;
}