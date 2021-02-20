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
    bool                    goodInput{false};

    while(!goodInput)
    {
        auto signal = WaitForMultipleObjects(2,&events[1],FALSE,INFINITE);

        if(signal == WAIT_OBJECT_0)     // stop
        {
            result = tryte{0};
            return Architecture::Exception::Halted;
        }

        char    data[100];
        GetDlgItemText(dlg,IDC_STDIN,data,sizeof(data));

        std::string         string{data};
        std::istringstream  in{data};


        auto port = static_cast<Architecture::KnownIOPorts>(static_cast<int>(portNumber));


        if(port==Architecture::KnownIOPorts::IO_tryte)
        {
            in >> result;
        }
        else
        {
            result = tryte{0};
        }

        if(in)
        {
            goodInput=true;
            auto pos=in.tellg();
            string.erase(0,pos);
            SetDlgItemText(dlg,IDC_STDIN,string.c_str());
        }
    }


    return Architecture::Exception::Okay;
}
