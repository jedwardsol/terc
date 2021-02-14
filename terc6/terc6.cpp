#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include <filesystem>
namespace fs=std::filesystem;

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/Arithmetic_std.h"

#include "Architecture/MemoryBlock.h"
#include "Architecture/sixTritCPU.h"

#pragma comment(lib,"Arithmetic")
#pragma comment(lib,"Architecture")


struct DummyIOPorts : Architecture::IOPorts
{
    ~DummyIOPorts() override = default;

    Architecture::Exception  out(const tryte    port,  const tryte    data) override
    {
        return Architecture::Exception::Okay;
    }

    Architecture::Exception  in (const tryte    port,        tryte   &result)
    {
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
    DummyIOPorts                        ioPorts;

    Architecture::sixTrit::CPU    cpu{code,data,ioPorts};

    do
    {
        cpu.execute();
    } while(cpu.reg(Architecture::sixTrit::Register::REXC) <= 0);

    if(cpu.reg(Architecture::sixTrit::Register::REXC) == Architecture::Exception::Halted)
    {
        std::cout << "Program ran successfully\n";
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