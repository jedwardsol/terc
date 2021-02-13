#include <string>
#include <cassert>
#include "assembler.h"

using namespace std::literals;

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
