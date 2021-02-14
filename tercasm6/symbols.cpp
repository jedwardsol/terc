#include <fstream>
#include <iomanip>
#include <string>
#include <cassert>
#include "assembler.h"

#pragma comment(lib,"Arithmetic")

using namespace std::literals;


void Assembler::makeMap()
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
    for(auto symbol : codeSymbols)
    {
        map << "    " << std::left << std::setw(16) << symbol.first << " : " << symbol.second.address << "\n";
    }

    map << "\nDependencies in code section\n";
    for(auto dependency : codeDependencies)
    {
        map << "    " << std::left << std::setw(16) << dependency.first << " : " << dependency.second.symbol << "\n";
    }

    map << "\nSymbols in data section\n";
    for(auto symbol : dataSymbols)
    {
        map << "    " << std::left << std::setw(16) << symbol.first << " : " << symbol.second.address << "\n";
    }

    map << "\nDependencies in data section\n";
    for(auto dependency : dataDependencies)
    {
        map << "    " << std::left << std::setw(16) << dependency.first << " : " << dependency.second.symbol << "\n";
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