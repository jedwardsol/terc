#include "assembler.h"

void Assembler::parseData(const SourceLine &source)
{
    const auto   &first = source.asString(0).value();

// Label

    if(first[0] == ':')
    {
        addDataSymbol(first);

        if(source.tokens().size() > 1)
        {
            SourceLine rest{source};

            rest.tokens().pop_front();

            parseData(rest);
        }
    }


// Data


}