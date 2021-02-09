
#include <vector>
#include <array>

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/tryte.h"
#include "Arithmetic/trit.h"

#include "MemoryBlock.h"
#include "sixTritCPU.h"


namespace sixTritArchitecture
{

void CPU::execute()
{
    if( reg(Register::RPC) > tryte{ codeSize-2})
    {
        reg(Register::REXC) = tryte{ExceptionRanOffEnd};
        reg(Register::REXA) = reg(Register::RPC);
        return;
    }

    tryte   PC = reg(Register::RPC);

    auto  first  = code[reg(Register::RPC)];
    auto  second = code[reg(Register::RPC)];

    reg(Register::RPC) =  tryte { static_cast<int>(reg(Register::RPC))+2 };     // TODO do tryte arithmetic
                                                                                // TODO can overflow here.  new excption


    if(first == tryte{0})
    {
        reg(Register::REXC) = tryte{ExceptionHalted};
        reg(Register::REXA) = PC;
        return;
    }

}



}
