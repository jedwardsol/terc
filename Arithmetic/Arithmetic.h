#pragma once
#include <cstdint>
#include <limits>

#include <string>
#include <iostream>

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
    int16_t      t : 2;
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

static_assert(sizeof(trit) ==2);
static_assert(sizeof(tryte)==2);




namespace std
{

template<> 
class numeric_limits<tryte>
{
public:
// constants

    static constexpr bool               is_specialized      {true};
    static constexpr bool               is_signed           {true};
    static constexpr bool               is_integer          {true};
    static constexpr bool               is_exact            {true};
    static constexpr bool               is_bounded          {true};
    static constexpr bool               traps               {true};

    static constexpr bool               is_modulo           {false};
    static constexpr int                digits10            {3};                        
    static constexpr int                max_digits10        {3};                        

    static constexpr int                radix               {3};                        
    static constexpr int                digits              {6};                       // 6 trits


// functions
    [[nodiscard]]static constexpr       int max()           noexcept { return 243+81+27+9+3+1; }
    [[nodiscard]]static constexpr       int min()           noexcept { return -max(); }
    [[nodiscard]]static constexpr       int lowest()        noexcept { return min(); }


// irrelevent float constants

    static constexpr bool               is_iec559           {false};
    static constexpr float_round_style  round_style         {std::round_toward_zero};
    static constexpr bool               has_infinity        {false};
    static constexpr bool               has_quiet_NaN       {false};
    static constexpr bool               has_signaling_NaN   {false};
    static constexpr bool               has_denorm          {false};
    static constexpr bool               has_denorm_loss     {false};
    static constexpr int                min_exponent        {0};
    static constexpr int                min_exponent10      {0};
    static constexpr int                max_exponent        {0};
    static constexpr int                max_exponent10      {0};
    static constexpr bool               tinyness_before     {false};
 
// irrelevent float functions

    [[nodiscard]]static constexpr       int epsilon()       noexcept { return 0; }
    [[nodiscard]]static constexpr       int round_error()   noexcept { return 0; }
    [[nodiscard]]static constexpr       int infinity()      noexcept { return 0; }
    [[nodiscard]]static constexpr       int quiet_NaN()     noexcept { return 0; }
    [[nodiscard]]static constexpr       int signaling_NaN() noexcept { return 0; }
    [[nodiscard]]static constexpr       int denorm_min()    noexcept { return 0; }
};

}



std::string  to_string(const tryte&);

std::ostream &operator<<(std::ostream &, const tryte &);
