#pragma once

#include <vector>
#include "Arithmetic/trint.h"

struct symbol
{
    tryte       address;
    std::string name;
};

std::vector<symbol>  loadSymbols(const fs::path&);