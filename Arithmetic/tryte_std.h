#pragma once

#include <limits>
#include <string>
#include <iostream>
#include <sstream>

#include "tryte.h"

#include "trit.h"
#include "trit_std.h"


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
    static constexpr int                digits              {tritsInTryte};


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




inline std::string  to_string(const tryte &t)
{
    std::string  result;

    result += to_string(trit{t.t5});
    result += to_string(trit{t.t4});
    result += to_string(trit{t.t3});
    result += to_string(trit{t.t2});
    result += to_string(trit{t.t1});
    result += to_string(trit{t.t0});
    
    return result;
}




inline std::ostream &operator<<(std::ostream &out, const tryte &t)
{
    std::ostringstream str;

    str << to_string(t) << '(' << static_cast<int>(t) << ')';
    return out << str.str();
}


inline std::istream &operator>>(std::istream &in,       tryte &t)
{
    std::string input;

    in >> input;

// try the string

    try
    {
        t = tryte{input};
        return in;
    }
    catch(...)
    {
    }

// try as an integer
    try
    {
        int i = std::stoi(input);

        t=tryte{i};

        return in;
    }
    catch(...)
    {
    }

    in.setstate(std::ios_base::failbit);
    return in;
}
