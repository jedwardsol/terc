#pragma once
#include <cstdint>
#include <string>
using namespace std::literals;
#include <exception>
#include <stdexcept>



constexpr int     pow3(int n)
{
    int result = 1;

    for(int i=0;i<n;i++)
    {
        result *= 3;
    }

    return result;
}


namespace BalancedTernary
{

constexpr int     numValues(int numTrits)
{
    return pow3(numTrits);
}

constexpr int     maxValue(int numTrits)
{
    return (numValues(numTrits) - 1) / 2;
}

constexpr int     minValue(int numTrits)
{
    return -maxValue(numTrits);
}


}



