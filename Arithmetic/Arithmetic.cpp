
#include "Arithmetic.h"

tryte::tryte(int i)
{

}



tryte::operator int() const
{
    return   t5 * pow3(5)
           + t4 * pow3(4)
           + t3 * pow3(3)
           + t2 * pow3(2)
           + t1 * pow3(1)
           + t0 * pow3(0);
}



constexpr char to_string(const trit &t)
{
    switch(t.t)
    {
    case -1:    return '-';        
    case  0:    return '0';        
    case +1:    return '+';        
    default:    throw 0;
    }
}

std::string  to_string(const tryte &t)
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



std::ostream &operator<<(std::ostream &out, const tryte &t)
{
    out << to_string(t) << '(' << static_cast<int>(t) << ')';
    return out;
}
