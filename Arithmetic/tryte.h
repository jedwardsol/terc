#pragma once


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



