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

    constexpr explicit trit(int i) : t{static_cast<int8_t>(i)}
    {
        checkConstructor(i);
    }

    constexpr explicit trit(int8_t i) : t{i}
    {
        checkConstructor(i);
    }

    constexpr explicit trit(int16_t i) : t{static_cast<int8_t>(i)}
    {
        checkConstructor(i);
    }


    constexpr trit(const trit&)             noexcept = default;
    constexpr trit(      trit&&)            noexcept = default;
    constexpr trit &operator=(const trit&)  noexcept = default;
    constexpr trit &operator=(      trit&&) noexcept = default;

    constexpr operator    int() const
    {
        return t;
    }


private:

    constexpr static void checkConstructor(int i)
    {
        if(i < -1 || i > 1)
        {
            throw std::out_of_range("trit constructor  "s + std::to_string(i));
        }
    }

};



constexpr inline bool operator< (const trit &lhs, const trit &rhs){ return   lhs.t < rhs.t;}
constexpr inline bool operator> (const trit &lhs, const trit &rhs){ return   rhs < lhs;    }
constexpr inline bool operator<=(const trit &lhs, const trit &rhs){ return !(lhs  > rhs);  }
constexpr inline bool operator>=(const trit &lhs, const trit &rhs){ return !(lhs  < rhs);  }
constexpr inline bool operator==(const trit &lhs, const trit &rhs){ return lhs.t == rhs.t; }
constexpr inline bool operator!=(const trit &lhs, const trit &rhs){ return !(lhs == rhs);  }


static_assert(sizeof(trit) ==1);
