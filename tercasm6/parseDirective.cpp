#include "assembler.h"

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
    else if(directive == ".address")
    {
        auto newAddress = source.asTryte(1);

        if(!newAddress)
        {
            error(".address directive missing a valid address");
        }

        switch(mode)
        {
        case Mode::data:
            setDataPosition(static_cast<int>(newAddress.value()));
            break;

        case Mode::code:
            setCodePosition(static_cast<int>(newAddress.value()));
            break;

        default:
            error(".address meaningless outside of .code or .data ");
            break;
        }
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
