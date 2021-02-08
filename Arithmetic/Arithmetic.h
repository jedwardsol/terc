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

constexpr int     numValues(int numTrits)
{
    return pow3(numTrits);
}

constexpr int     maxValue(int numTrits)
{
    return numValues(numTrits) - 1 / 2;
}


class trit
{
public:
    int8_t      t : 2;

    trit() noexcept : t{0}
    {}

    explicit trit(int i) : t{static_cast<int8_t>(i)}
    {
        if(i < -1 || i > 1)
        {
            throw std::out_of_range("trit init"s + std::to_string(i));
        }
    }

    explicit trit(int8_t i) : t{i}
    {
        if(i < -1 || i > 1)
        {
            throw std::out_of_range("trit init"s + std::to_string(i));
        }
    }

    explicit trit(int16_t i) : t{static_cast<int8_t>(i)}
    {
        if(i < -1 || i > 1)
        {
            throw std::out_of_range("trit init"s + std::to_string(i));
        }
    }


    trit(const trit&)             noexcept = default;
    trit(      trit&&)            noexcept = default;
    trit &operator=(const trit&)  noexcept = default;
    trit &operator=(      trit&&) noexcept = default;

    operator    int() const
    {
        return t;
    }

};

class tryte 
{
public:
    int16_t     t0 : 2;             // 3^0   1
    int16_t     t1 : 2;             // 3^1   3
    int16_t     t2 : 2;             // 3^2   9
    int16_t     t3 : 2;             // 3^3  27
    int16_t     t4 : 2;             // 3^4  81
    int16_t     t5 : 2;             // 3^5 243
    int16_t     unused : 4;

public:

    tryte() noexcept : t0{0}, t1{0}, t2{0}, t3{0}, t4{0}, t5{0}
    {}

    tryte(int i);

    tryte(const tryte&)             noexcept = default;
    tryte(      tryte&&)            noexcept = default;
    tryte &operator=(const tryte&)  noexcept = default;
    tryte &operator=(      tryte&&) noexcept = default;

public:

    operator    int() const;
};

static_assert(sizeof(trit) ==1);
static_assert(sizeof(tryte)==2);



