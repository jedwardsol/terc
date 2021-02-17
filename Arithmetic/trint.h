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
    static constexpr int numBytes{ bytesNeeded(N) };

    std::array<uint8_t,numBytes>   trits;    


public:

	constexpr trit	getTrit(int i) const
	{
		assert(i < N);

		auto byte = trits.at(i / 4);
		byte >>= (i % 4);

		return trit{ int{ byte & 0b11 } };
	}

	constexpr void	setTrit(int i, trit  t)
	{
		assert(i < N);

		auto &byte = trits.at(i / 4);

		auto	mask = 0b11   << (i%4);
		uint8_t	data;
		memcpy(&data,&t,1);
		data <<=i%4;

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
            throw std::out_of_range("trint constructor  "s + std::to_string(value));
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
				i++;
			}
 
			value /= 3;
		}
		
		if(negative)
		{
			*this=-*this;
		}
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
		auto offset=std::max( shift-N, N);

		for(int i=N ;i<offset ;i--)
		{
			setTrit(i, getTrit(i-offset));
		}

		for(int i=offset ;i<=0;i--)
		{
			setTrit(i, trit{0});
		}

		return *this;
    }

    constexpr trint &operator>>=(int shift) noexcept
    {
		auto offset=std::max( shift-N, N);

		for(int i=0;i<N-offset;i++)
		{
			setTrit(i, getTrit(i+offset));
		}

		for(int i=N-offset;i<N;i++)
		{
			setTrit(i, trit{0});
		}

		return *this;
    }


};

using trybble = trint<3>;
using tryte   = trint<6>;
using trord   = trint<12>;

static_assert(sizeof(trybble) == 1);
static_assert(sizeof(tryte)   == 2);
static_assert(sizeof(trord)   == 3);