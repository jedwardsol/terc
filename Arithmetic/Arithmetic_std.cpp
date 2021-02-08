
#include "Arithmetic_std.h"


std::string  to_string(const trit &t)
{
    return std::string(1,to_char(t));

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



std::ostream &operator<<(std::ostream &out, const trit &t)
{
    out << to_char(t);
    return out;
}

std::ostream &operator<<(std::ostream &out, const tryte &t)
{
    out << to_string(t) << '(' << static_cast<int>(t) << ')';
    return out;
}
