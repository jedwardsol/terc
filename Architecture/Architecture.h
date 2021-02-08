#pragma once

#include <fstream>
#include <vector>

#include "Arithmetic/Arithmetic.h"

constexpr int     tritsInTryte       {6};




namespace  sixTritArchitecture
{

    constexpr int     instructionBusSize        {tritsInTryte};
    constexpr int     dataBusSize               {tritsInTryte};


    constexpr int     instructionSectionSize    {maxValue(instructionBusSize)};
    constexpr int     dataSectionSize           {maxValue(dataBusSize)};
    constexpr int     stackSectionSize          {maxValue(dataBusSize)};

}


