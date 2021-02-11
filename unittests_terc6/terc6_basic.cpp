#define NOMINMAX
#include <Windows.h>
#include <gtest/gtest.h>
#include <string>
using namespace std::literals;
#include <sstream>

#include <iostream>

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/Arithmetic_std.h"
#include "Arithmetic/tryte.h"

#include "Architecture/MemoryBlock.h"
#include "Architecture/sixTritCPU.h"
#include "Architecture/exception.h"
#include "Architecture/IOPorts.h"
