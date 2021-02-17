#pragma once

#include <tuple>
#include <string>
#include <array>
#include <cassert>

#include "Arithmetic.h"
#include "trit.h"


template <int N>
class trint
{
public:
    static constexpr int numTrits{N};

private:
    static constexpr int numBytes{  ((N * 2)+7)/8 };

    std::array<uint8_t,numBytes>   trits;    

public:

	constexpr trit	getTrit(int i) const
	{
		assert(i < N);

		auto byte  = trits.at(i / 4);
		auto shift = (i % 4) * 2;

		byte >>= shift;
		byte &= 0b11;
		
		trit t{};

		memcpy(&t,&byte,1);

		return t;
	}

	constexpr void	setTrit(int i, trit  t)
	{
		assert(i < N);

		auto &byte = trits.at(i / 4);
		auto shift = (i % 4) * 2;

		auto	mask = 0b11   << shift;
		int8_t  data{t.t};

		data <<= shift;
		data &=  mask;
		byte &= ~mask;
		byte |=  data;

	}



    constexpr trint() noexcept : trits{0}
    {}


    constexpr explicit trint(int value) : trint{}
	{
		if(    value >  BalancedTernary::maxValue(numTrits)
		   ||  value < -BalancedTernary::maxValue(numTrits))
		{
            throw std::out_of_range("trint<"s + std::to_string(N) + "> constructor "s + std::to_string(value));
		}


		bool negative{false};

		if(value<0)
		{
			negative=true;
			value=-value;
		}


		for(int i=0;i<N;i++)
		{
			*this >>= 1;

			int r = value % 3;
			
			if (r == 0)
			{
				setTrit(N-1, trit{0});
			}
			else if (r == 1)
			{
				setTrit(N-1, trit{1});
			}
			else 
			{
				setTrit(N-1, trit{-1});
				value++;
			}
 
			value /= 3;
		}
		
		if(negative)
		{
			*this=-*this;
		}
	}


    explicit trint(const std::string_view s) : trint{}
	{
		if(s.size() != N)
		{
            throw std::out_of_range("trint<"s + std::to_string(N) + "> constructor size "s + std::string{s});
		}


		auto toTrit = [](char c) ->trit
		{
			switch(c)
			{
			case '-': return trit{-1};
			case '0': return trit{ 0};
			case '+': return trit{ 1};
			default : throw std::out_of_range("tryte constructor char "s + c);
			}
		};

		for(int i=0;i<N;i++)
		{
			setTrit(i, toTrit( s[N-i-1LL] ));
		}
	}



    constexpr explicit trint(trint<N/2> low,  trint<N/2> high) : trint{}
	{
		static_assert(low.numTrits * 2 ==numTrits);

		for(int i=0;i<N/2;i++)
		{
			setTrit(i, high.getTrit(i));
		}

		*this <<= N/2;

		for(int i=0;i<N/2;i++)
		{
			setTrit(i, low.getTrit(i));
		}
	}


	constexpr std::pair<trint<N/2>,trint<N/2>> halves() const
	{
		trint<N/2>	low{};
		trint<N/2>	high{};

		static_assert(low.numTrits * 2 ==numTrits);

		for(int i=0;i<N/2;i++)
		{
			low.setTrit (i, getTrit(i));
			high.setTrit(i,	getTrit(i + N/2));
		}


		return std::make_pair(low,high);
	}



    trint(const trint&)             noexcept = default;
    trint(      trint&&)            noexcept = default;
    trint &operator=(const trint&)  noexcept = default;
    trint &operator=(      trint&&) noexcept = default;

public:

	constexpr trint operator-() const noexcept
    {
		trint negative;

		for(int i=0;i<N;i++)
		{
			negative.setTrit(i,  -getTrit(i));
		}

		return negative;
    }



    constexpr trint &operator<<=(int shift) noexcept
    {
		assert(shift >= 0);

		if(shift < 0) shift = 0;
		if(shift > N) shift = N;

		// shift the top trits left
		for(int i=N-1 ;i>=shift;i--)
		{
			setTrit(i, getTrit(i-shift));
		}

		// zero the remaining low trits			
		for(int i=shift-1;i>=0;i--)
		{
			setTrit(i, trit{0});
		}

		return *this;
    }

    constexpr trint &operator>>=(int shift) noexcept
    {
		assert(shift >= 0);

		if(shift < 0) shift = 0;
		if(shift > N) shift = N;

		// shift the low trits right
		for(int i=0;i<N-shift;i++)
		{
			setTrit(i, getTrit(i+shift));
		}

		// zero the remain top trits
		for(int i=N-shift;i<N;i++)
		{
			setTrit(i, trit{0});
		}

		return *this;
    }


    constexpr explicit operator    int() const
	{
		int result{};

		for(int i=0; i<N;i++)
		{
			result += static_cast<int>(getTrit(i)) * pow3(i);
		}

		return result;
	}
};

template <int N>
constexpr inline bool operator==(const trint<N> &lhs, const trint<N> &rhs) noexcept 
{ 
	for(int i=0;i<N;i++)
	{
		if( lhs.getTrit(i) != rhs.getTrit(i))
		{
			return false;
		}
	}

	return true;
}

template <int N>
constexpr inline bool operator!=(const trint<N> &lhs, const trint<N> &rhs) noexcept { return !(lhs == rhs);  }

template <int N>
constexpr inline bool operator< (const trint<N> &lhs, const trint<N> &rhs) noexcept 
{ 
	for(int i=N-1; i>=0; i--)
	{
		if(lhs.getTrit(i) < rhs.getTrit(i)) return true;
		if(lhs.getTrit(i) > rhs.getTrit(i)) return false;
	}

	// they're equal
	return false;
}


template <int N>
constexpr inline bool operator> (const trint<N> &lhs, const trint<N> &rhs) noexcept { return   rhs < lhs;    }

template <int N>
constexpr inline bool operator<=(const trint<N> &lhs, const trint<N> &rhs) noexcept { return !(lhs  > rhs);  }

template <int N>
constexpr inline bool operator>=(const trint<N> &lhs, const trint<N> &rhs) noexcept { return !(lhs  < rhs);  }



template <int N>
constexpr inline trint<N> halfAdder(const trint<N> &lhs, const trint<N> &rhs,  trit &carry)
{
	trit		partialCarry;    
	trint<N>	result;	

	result.setTrit(0,  halfAdder(lhs.getTrit(0), rhs.getTrit(0), partialCarry));

	for(int i=1;i<N;i++)
	{
		result.setTrit(i,  fullAdder(lhs.getTrit(i), rhs.getTrit(i), partialCarry));
	}

	carry = partialCarry;

	return result;
}




// useful types

using trybble = trint<3>;
using tryte   = trint<6>;
using trord   = trint<12>;

static_assert(sizeof(trybble) == 1);
static_assert(sizeof(tryte)   == 2);
static_assert(sizeof(trord)   == 3);