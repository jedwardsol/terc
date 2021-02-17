#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include <filesystem>
namespace fs=std::filesystem;

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/tryte.h"
#include "Arithmetic/tryte_std.h"



#include "Architecture/MemoryBlock.h"
#include "Architecture/sixTritCPU.h"




#pragma comment(lib,"Arithmetic")
#pragma comment(lib,"Architecture")


struct PrintingIOPorts : Architecture::IOPorts
{
    ~PrintingIOPorts() override = default;

    Architecture::Exception  out(const trybble    portNumber,  const tryte    data) override
    {
        auto port = static_cast<Architecture::KnownIOPorts>(static_cast<int>(portNumber));

        if(port == Architecture::KnownIOPorts::IO_tryte)
        {
            std::cout << data;
        }
        else if(port == Architecture::KnownIOPorts::IO_ASCII)
        {
            std::cout << static_cast<char>(data);
        }
        else if(port == Architecture::KnownIOPorts::O_Exception)
        {
            if(data != Architecture::Exception::Halted)
            {
                std::cout << "\nException " << data << '\n';
            }
        }
        else 
        {
            std::cout << "\nPort " << portNumber << " -> " << data << '\n';
        }

        return Architecture::Exception::Okay;
    }


    Architecture::Exception  in (const trybble    portNumber,        tryte   &result)
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
};


int main(int argc, const char *argv[])
try
{
    std::vector<std::string_view>   args(argv+1, argv+argc);

    if(args.empty())
    {
        throw std::runtime_error{"Missing input FileName"};
    }

    fs::path    codeFileName     {args[0]};
    fs::path    dataFileName     {args[0]};
    fs::path    finaldataFileName{args[0]};

    codeFileName.replace_extension(".code");
    dataFileName.replace_extension(".data");
    finaldataFileName.replace_extension(".data.final");

    const Architecture::MemoryBlock     code {codeFileName.string() };        
          Architecture::MemoryBlock     data {dataFileName.string() , 
                                              finaldataFileName.string()};        
    PrintingIOPorts                        ioPorts;

    Architecture::sixTrit::CPU    cpu{code,data,ioPorts};

          int instructionCount{};
    const int instructionCountLimit{5'000'000};
    do
    {
        instructionCount++;
        cpu.execute();
    } while(   cpu.reg(Architecture::sixTrit::Register::REXC) <= 0
            && instructionCount < instructionCountLimit);

    std::cout << "\n";

    if(cpu.reg(Architecture::sixTrit::Register::REXC) == Architecture::Exception::Halted)
    {
        std::cout << "Program ran successfully\n";
    }
    else if(instructionCount == instructionCountLimit)
    {
        std::cout << "Program timed out\n";
    }
    else
    {
        std::cout << "Program halted with exception " 
                  << cpu.reg(Architecture::sixTrit::Register::REXC) 
                  << " at PC " 
                  << cpu.reg(Architecture::sixTrit::Register::REXA) 
                  << "\n";
    }
}
catch(const std::exception &e)
{
    std::cout << "caught " << e.what() << '\n';
}