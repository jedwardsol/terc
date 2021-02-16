#pragma once

#include <cstdint>
#include <string>
using namespace std::literals;
#include <exception>
#include <stdexcept>



class trit
{
public:
    int8_t      t : 2;

    constexpr trit() noexcept : t{0}
    {}

    constexpr explicit trit(char c) :t{0}
    {
        switch(c)
        {
        case '-':   t=-1; break;
        case '0':   t= 0; break;
        case '+':   t= 1; break;
        default :   throw std::out_of_range("trit constructor  '"s + c +"'"s);
        }
    }

    constexpr explicit trit(int8_t i)  : t{i}
    {
        if(i < -1 || i > 1)
        {
            throw std::out_of_range("trit constructor  "s + std::to_string(i));
        }
    }

    constexpr explicit trit(int i)     : trit{static_cast<int8_t>(i)}
    {}


    constexpr explicit trit(int16_t i) : trit{static_cast<int8_t>(i)}
    {}

    constexpr trit(const trit&)             noexcept = default;
    constexpr trit(      trit&&)            noexcept = default;
    constexpr trit &operator=(const trit&)  noexcept = default;
    constexpr trit &operator=(      trit&&) noexcept = default;

    constexpr explicit operator    int() const
    {
        return t;
    }

    constexpr explicit operator    int16_t() const
    {
        return t;
    }

    constexpr explicit operator bool() const noexcept
    {
        return t!=0;
    }

    constexpr trit operator-() const noexcept
    {
        return trit{-t};
    }

    constexpr trit &operator++() 
    {
        if(t == 1)
        {
            throw std::overflow_error("trit++pre"s);
        }

        t++;

        return *this;
    }

    constexpr trit operator++(int) 
    {
        if(t == 1)
        {
            throw std::overflow_error("trit++post"s);
        }

        auto previous = *this;
        operator++();
        return previous;
    }


    constexpr trit &operator--() 
    {
        if(t == -1)
        {
            throw std::overflow_error("trit--pre"s);
        }

        t--;

        return *this;
    }


    constexpr trit operator--(int) 
    {
        if(t == -1)
        {
            throw std::overflow_error("trit--post"s);
        }

        auto previous = *this;
        operator--();
        return previous;
    }



    constexpr trit &operator+=(const trit &rhs) 
    {
        if(t == 1 && rhs.t == 1)
        {
            throw std::overflow_error("trit+= overflow"s);
        }

        if(t == -1 && rhs.t == -1)
        {
            throw std::overflow_error("trit+= underflow"s);
        }

        t+=rhs.t;

        return *this;
    }

    constexpr trit &operator-=(const trit &rhs) 
    {
        if(t == 1 && rhs.t == -1)
        {
            throw std::overflow_error("trit+= overflow"s);
        }

        if(t == -1 && rhs.t == 1)
        {
            throw std::overflow_error("trit+= underflow"s);
        }

        t-=rhs.t;

        return *this;
    }

};

static_assert(sizeof(trit) ==1);



constexpr inline bool operator==(const trit &lhs, const trit &rhs) noexcept { return lhs.t == rhs.t; }
constexpr inline bool operator!=(const trit &lhs, const trit &rhs) noexcept { return !operator==(lhs , rhs);  }

constexpr inline bool operator< (const trit &lhs, const trit &rhs) noexcept { return   lhs.t < rhs.t;}
constexpr inline bool operator> (const trit &lhs, const trit &rhs) noexcept { return   rhs < lhs;    }
constexpr inline bool operator<=(const trit &lhs, const trit &rhs) noexcept { return !(lhs  > rhs);  }
constexpr inline bool operator>=(const trit &lhs, const trit &rhs) noexcept { return !(lhs  < rhs);  }


constexpr inline trit operator+(trit lhs, const trit &rhs){   lhs += rhs; return lhs; }
constexpr inline trit operator-(trit lhs, const trit &rhs){   lhs -= rhs; return lhs; }


constexpr inline trit  halfAdder(const trit &lhs, const trit &rhs,  trit &carry)
{
    if(rhs == trit{1} && lhs==trit{1})
    {
        carry=trit{1};
        return trit{-1};
    }
    else if(rhs == trit{-1} && lhs==trit{-1})
    {
        carry=trit{-1};
        return trit{1};
    }
    else
    {
        carry=trit{0};
        return lhs+rhs;
    }
}

constexpr inline trit  fullAdder(const trit &lhs, const trit &rhs,  trit &carry)
{
    trit partialCarry{};
    trit partialSum  {halfAdder(lhs,rhs, partialCarry)};
    trit finalCarry  {};
    trit finalSum    {halfAdder(partialSum,carry, finalCarry)};

    carry = partialCarry+finalCarry;

    return finalSum;
}


