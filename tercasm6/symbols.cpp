#include <fstream>
#include <iomanip>
#include <string>
#include <cassert>
#include "assembler.h"

#pragma comment(lib,"Arithmetic")

using namespace std::literals;


void Assembler::makeMap()
{
    std::ofstream  map{"map"};

    map << "Symbols in data section\n";

    for(auto symbol : dataSymbols)
    {
        map << "    " << std::left << std::setw(10) << symbol.first << " : " << symbol.second.address << "\n";
    }

    map << "\nSymbols in code section\n";

    for(auto symbol : codeSymbols)
    {
        map << "    " << std::left << std::setw(10) << symbol.first << " : " << symbol.second.address << "\n";
    }


    map << "\nDependencies in data section\n";

    for(auto dependency : dataDependencies)
    {
        map << "    " << std::left << std::setw(10) << dependency.first << " : " << dependency.second.symbol << "\n";
    }

    map << "\nDependencies in code section\n";

    for(auto dependency : codeDependencies)
    {
        map << "    " << std::left << std::setw(10) << dependency.first << " : " << dependency.second.symbol << "\n";
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

    dataSymbols[symbol] = {tryte{currentDataPosition},currentLineNumber};
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

    codeSymbols[symbol] = {tryte{currentCodePosition},currentLineNumber};
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

