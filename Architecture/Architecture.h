#pragma once

#include "Arithmetic/Arithmetic.h"

constexpr int     tritsInTryte       {6};
constexpr int     instructionBusSize {6};
constexpr int     dataBusSize       {12};




constexpr int     instructionSectionSize {maxValue(instructionBusSize)};
constexpr int     dataSectionSize        {maxValue(dataBusSize)};

