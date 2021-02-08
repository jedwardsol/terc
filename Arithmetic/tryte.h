#pragma once

#include "Arithmetic.h"
#include "trit.h"

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

    constexpr tryte() noexcept : t0{0}, t1{0}, t2{0}, t3{0}, t4{0}, t5{0}, unused{0}
    {}

    constexpr explicit tryte(int i) : t0{0}, t1{0}, t2{0}, t3{0}, t4{0}, t5{0}, unused{0}
	{
		if(    i >  maxValue(6)
		   ||  i < -maxValue(6))
		{
            throw std::out_of_range("trit constructor  "s + std::to_string(i));
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

	result.t0 = halfAdder(trit{lhs.t0}, trit{rhs.t0}, partialCarry);
	result.t1 = fullAdder(trit{lhs.t1}, trit{rhs.t1}, partialCarry);
	result.t2 = fullAdder(trit{lhs.t2}, trit{rhs.t2}, partialCarry);
	result.t3 = fullAdder(trit{lhs.t3}, trit{rhs.t3}, partialCarry);
	result.t4 = fullAdder(trit{lhs.t4}, trit{rhs.t4}, partialCarry);
	result.t5 = fullAdder(trit{lhs.t5}, trit{rhs.t5}, partialCarry);

	carry = partialCarry;

	return result;
}



