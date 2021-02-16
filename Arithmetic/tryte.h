#pragma once

#include "Arithmetic.h"
#include "trit.h"
#include <tuple>
#include <string>

constexpr int     tritsInTryte       {6};


class tryte;
using trybble=tryte;			  // 2 trybbles in a tryte.  Should be 3 really,  but then they're too small ( -4 to +4 )

constexpr inline tryte  halfAdder(const tryte &lhs, const tryte &rhs,  trit &carry);
std::string  to_string(const tryte&);


class tryte  // TODO : templatise on number of trits to implement  trybble, tryte and trord
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

    constexpr tryte() noexcept : t0{0}, t1{0}, t2{0}, t3{0}, t4{0}, t5{0}, unused{0}
    {}

    constexpr explicit tryte(int i) : tryte{}
	{
		if(    i >  BalancedTernary::maxValue(6)
		   ||  i < -BalancedTernary::maxValue(6))
		{
            throw std::out_of_range("tryte constructor  "s + std::to_string(i));
		}


		bool negative{false};

		if(i<0)
		{
			negative=true;
			i=-i;
		}


		for(int trit=0;trit<6;trit++)
		{
			*this >>= 1;

			int r = i % 3;
			
			if (r == 0)
			{
				t5 = 0;
			}
			else if (r == 1)
			{
				t5 = 1;
			}
			else 
			{
				t5=-1;
				i++;
			}
 
			i /= 3;
		}
		
		if(negative)
		{
			*this=-*this;
		}
	}


    explicit tryte(const std::string_view s) : tryte{}
	{
		if(s.size() != 6)
		{
            throw std::out_of_range("tryte constructor size "s + std::string{s});
		}

		auto toTrit = [](char c) 
		{
			switch(c)
			{
			case '-': return -1;
			case '0': return  0;
			case '+': return  1;
			default : throw std::out_of_range("tryte constructor char "s + c);
			}

			return 0;
		};

		t5 = toTrit(s[0]);
		t4 = toTrit(s[1]);
		t3 = toTrit(s[2]);
		t2 = toTrit(s[3]);
		t1 = toTrit(s[4]);
		t0 = toTrit(s[5]);
	}



	constexpr tryte(const trybble &low, const trybble &high) : tryte{}
	{
		if(   low > tryte{"000+++"}
			|| low < tryte{"000---"})
		{
			throw std::out_of_range("tryte constructor low "s + to_string(low));
		}

		if(   high > tryte{"000+++"}
			|| high < tryte{"000---"})
		{
			throw std::out_of_range("tryte constructor high "s + to_string(high));
		}


		tryte	higher{high};

		higher <<= 3;

		trit carry;
		*this = halfAdder(higher,low, carry);
	}


    tryte(const tryte&)             noexcept = default;
    tryte(      tryte&&)            noexcept = default;
    tryte &operator=(const tryte&)  noexcept = default;
    tryte &operator=(      tryte&&) noexcept = default;

public:

    constexpr operator    int() const
	{
		return   t5 * pow3(5)
			   + t4 * pow3(4)
			   + t3 * pow3(3)
			   + t2 * pow3(2)
			   + t1 * pow3(1)
			   + t0 * pow3(0);
	}


	constexpr std::pair<trybble,trybble> trybbles() const
	{
		trybble  low {*this};
		trybble  high{*this};

		low.t3 = low.t4 = low.t5 = 0;
		high >>= 3;			

		return std::make_pair(low,high);
	}


    constexpr tryte operator-() const noexcept
    {
		tryte negative{*this};

		negative.t0=-negative.t0;
		negative.t1=-negative.t1;
		negative.t2=-negative.t2;
		negative.t3=-negative.t3;
		negative.t4=-negative.t4;
		negative.t5=-negative.t5;

		return negative;
    }


    constexpr tryte &operator<<=(int shift) noexcept
    {
		for(int i=0;i<shift;i++)
		{
			t5=t4;
			t4=t3;
			t3=t2;
			t2=t1;
			t1=t0;
			t0=0;
		}

		return *this;
    }

    constexpr tryte &operator>>=(int shift) noexcept
    {
		for(int i=0;i<shift;i++)
		{
			t0=t1;
			t1=t2;
			t2=t3;
			t3=t4;
			t4=t5;
			t5=0;
		}

		return *this;
    }


	constexpr trit getTrit(int index) const
	{
		switch(index)
		{
		case 0:  return trit{t0};
		case 1:  return trit{t1};
		case 2:  return trit{t2};
		case 3:  return trit{t3};
		case 4:  return trit{t4};
		case 5:  return trit{t5};
		default: throw std::out_of_range("tryte getTrit bad index "s + std::to_string(index));
		}
	}

	constexpr void setTrit(int index, trit newTrit)
	{
		auto newBits = static_cast<int16_t>(newTrit); 

		switch(index)
		{
		case 0:  t0 = newBits; break;
		case 1:  t1 = newBits; break;
		case 2:  t2 = newBits; break;
		case 3:  t3 = newBits; break;
		case 4:  t4 = newBits; break;
		case 5:  t5 = newBits; break;
		default: throw std::out_of_range("tryte setTrit bad index "s + std::to_string(index));
		}
	}




};

static_assert(sizeof(tryte)==2);


constexpr inline bool operator==(const tryte &lhs, const tryte &rhs) noexcept 
{ 
	return    lhs.t5 == rhs.t5
	       && lhs.t4 == rhs.t4
	       && lhs.t3 == rhs.t3
	       && lhs.t2 == rhs.t2
	       && lhs.t1 == rhs.t1
	       && lhs.t0 == rhs.t0;
}
constexpr inline bool operator!=(const tryte &lhs, const tryte &rhs) noexcept { return !(lhs == rhs);  }

constexpr inline bool operator< (const tryte &lhs, const tryte &rhs) noexcept 
{ 
	return  (lhs.t5  < rhs.t5) ? true  :
			(lhs.t5  > rhs.t5) ? false :
			(lhs.t4  < rhs.t4) ? true  :
			(lhs.t4  > rhs.t4) ? false :
			(lhs.t3  < rhs.t3) ? true  :
			(lhs.t3  > rhs.t3) ? false :
			(lhs.t2  < rhs.t2) ? true  :
			(lhs.t2  > rhs.t2) ? false :
			(lhs.t1  < rhs.t1) ? true  :
			(lhs.t1  > rhs.t1) ? false :
			(lhs.t0  < rhs.t0) ? true  :
			(lhs.t0  > rhs.t0) ? false :
			false;
}


constexpr inline bool operator> (const tryte &lhs, const tryte &rhs) noexcept { return   rhs < lhs;    }
constexpr inline bool operator<=(const tryte &lhs, const tryte &rhs) noexcept { return !(lhs  > rhs);  }
constexpr inline bool operator>=(const tryte &lhs, const tryte &rhs) noexcept { return !(lhs  < rhs);  }


constexpr inline tryte  halfAdder(const tryte &lhs, const tryte &rhs,  trit &carry)
{
	trit	partialCarry;    
	tryte	result;	

	result.setTrit(0, halfAdder(trit{lhs.t0}, trit{rhs.t0}, partialCarry));
	result.setTrit(1, fullAdder(trit{lhs.t1}, trit{rhs.t1}, partialCarry));
	result.setTrit(2, fullAdder(trit{lhs.t2}, trit{rhs.t2}, partialCarry));
	result.setTrit(3, fullAdder(trit{lhs.t3}, trit{rhs.t3}, partialCarry));
	result.setTrit(4, fullAdder(trit{lhs.t4}, trit{rhs.t4}, partialCarry));
	result.setTrit(5, fullAdder(trit{lhs.t5}, trit{rhs.t5}, partialCarry));

	carry = partialCarry;

	return result;
}



