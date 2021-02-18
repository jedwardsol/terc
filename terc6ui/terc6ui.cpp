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




int WinMain(HINSTANCE hInstance,HINSTANCE ,LPSTR ,int )
try
{
    int argc{};
    auto argv = CommandLineToArgvW(GetCommandLineW(),&argc);

    std::vector<std::wstring_view>   args(argv+1, argv+argc);

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
 
    UI{code, data}.messageLoop();


/* 
    do
    {
        cpu.execute();
    } while(   cpu.reg(Architecture::sixTrit::Register::REXC) <= tryte{0}
            && instructionCount < instructionCountLimit);
*/


}
catch(const std::exception &e)
{
    MessageBox(nullptr,e.what(),"Terc6UI",MB_OK|MB_ICONEXCLAMATION);
}