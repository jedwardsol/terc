#pragma once

#include <limits>
#include <string>
#include <iostream>
#include <sstream>

#include "trint.h"

#include "trit.h"
#include "trit_std.h"


namespace std
{
template<int N> 
class numeric_limits<trint<N>>
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
    static constexpr int                digits10            { 3 /* TODO */ };                        
    static constexpr int                max_digits10        { digits10};                        

    static constexpr int                radix               {3};                        
    static constexpr int                digits              {N};


// functions
    [[nodiscard]]static constexpr       int max()           noexcept { return  BalancedTernary::maxValue(digits); }
    [[nodiscard]]static constexpr       int min()           noexcept { return -max(); }
    [[nodiscard]]static constexpr       int lowest()        noexcept { return  min(); }


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



template <int N>
inline std::string  to_string(const trint<N> &t)
{
    std::string  result;

    for(int i=N-1;i>=0;i--)
    {
        result += to_string(t.getTrit(i));
    }
    
    return result;
}


template <int N>
inline std::ostream &operator<<(std::ostream &out, const trint<N> &t)
{
    std::ostringstream str;

    str << to_string(t) << '(' << static_cast<int>(t) << ')';
    return out << str.str();
}

template <int N>
inline std::istream &operator>>(std::istream &in,       trint<N> &t)
{
    std::string input;

    in >> input;

// try the string

    try
    {
        t = trint<N>{input};
        return in;
    }
    catch(...)
    {
    }

// try as an integer
    try
    {
        int i = std::stoi(input);

        t=trint<N>{i};

        return in;
    }
    catch(...)
    {
    }

    in.setstate(std::ios_base::failbit);
    return in;
}

