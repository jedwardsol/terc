#include <iostream>

#include "Arithmetic/trint.h"
#include "Arithmetic/trint_std.h"




int main()
{
    for(int i =std::numeric_limits<tryte>::min();
            i<=std::numeric_limits<tryte>::max();
            i++)
    {
        std::cout << tryte{i} << "\n";
    }
}