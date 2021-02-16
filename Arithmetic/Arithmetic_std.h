#pragma once

#include <limits>
#include <string>
#include <iostream>

#include "trit.h"
#include "tryte.h"

namespace std
{

template<> 
class numeric_limits<trit>
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
    static constexpr int                digits10            {1};                        
    static constexpr int                max_digits10        {1};                        

    static constexpr int                radix               {3};                        
    static constexpr int                digits              {1};                       // 1 trits


// functions
    [[nodiscard]]static constexpr       int max()           noexcept { return 1; }
    [[nodiscard]]static constexpr       int min()           noexcept { return -1; }
    [[nodiscard]]static constexpr       int lowest()        noexcept { return -1; }


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



constexpr char to_char(const trit &t)
{
    switch(t.t)
    {
    case -1:    return '-';        
    case  0:    return '0';        
    case +1:    return '+';        
    default:    throw 0;
    }
}


std::string  to_string(const trit &);
std::string  to_string(const tryte&);

std::ostream &operator<<(std::ostream &, const trit  &);
std::ostream &operator<<(std::ostream &, const tryte &);
std::istream &operator>>(std::istream &,       tryte &);
