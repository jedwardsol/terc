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

    else if(first == "tryte")
    {
        for(int i=1;i < source.tokens().size(); i++)
        {
            tryte  t{};

            auto string = source.asString(i).value();

            if(    string[0]=='&'
               ||  string[0]=='$')
            {
                t= tryte{0};

                addDataDependency(string);
            }
            else
            {
                auto value = source.asTryte(i);

                if(!value)
                {
                    error("Unparsable tryte at "s + std::to_string(i));
                }

                t=value.value();
            }

            data[currentDataPosition] = t;
            incrementDataPosition();
        }
    }
    else if(first == "string")
    {
        for(int i=1;i < source.tokens().size(); i++)
        {
            auto string = source.asString(i).value();

            for(auto c : string)
            {
                // TODO escape sequences
                data[currentDataPosition] = tryte{c};
                incrementDataPosition();
            }

            // nul-terminate
            data[currentDataPosition] = tryte{0};
            incrementDataPosition();
        }
    }
    else
    {
        error("Unrecognised data type");
    }
}