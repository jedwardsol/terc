#define NOMINMAX
#include <Windows.h>



#include <vector>
#include <string>
#include <exception>
#include <filesystem>
namespace fs=std::filesystem;
#include <chrono>

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/trint.h"
#include "Arithmetic/trint_std.h"

#include "Architecture/MemoryBlock.h"
#include "terc6cpu/sixTritCPU.h"

#include "UI.h"

Architecture::Exception  UI::out(const trybble    portNumber,  const tryte    data) 
{
    auto port = static_cast<Architecture::KnownIOPorts>(static_cast<int>(portNumber));

    if(port == Architecture::KnownIOPorts::IO_tryte)
    {
        stdOut << data;
    }
    else if(port == Architecture::KnownIOPorts::IO_ASCII)
    {
        auto c = static_cast<char>(static_cast<int>(data));

        if(c == '\n')
        {
            stdOut << '\r';
        }

        stdOut << c;
    }
    else if(port == Architecture::KnownIOPorts::O_Exception)
    {
        if(data != tryte{Architecture::Exception::Halted})
        {
            stdOut << "\nException " << data << '\n';
        }
    }
    else 
    {
        stdOut << "\nPort " << portNumber << " -> " << data << '\n';
    }

    SetDlgItemText(dlg,IDC_STDOUT,stdOut.str().c_str());

    return Architecture::Exception::Okay;
}


Architecture::Exception  UI::in (const trybble    portNumber,        tryte   &result) 
{
    auto port = static_cast<Architecture::KnownIOPorts>(static_cast<int>(portNumber));

    if(port==Architecture::KnownIOPorts::IO_tryte)
    {
        do
        {
            std::cin.clear();
            std::cin >> result;
        } while(!std::cin);
    }
    else
    {
        std::cout << "Port " << portNumber << " <- " << 0 << '\n';

        result = tryte{0};
    }

    return Architecture::Exception::Okay;
}
