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

#include <commctrl.h>
#pragma comment(lib,"Comctl32")
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

    INITCOMMONCONTROLSEX init{sizeof(init),ICC_WIN95_CLASSES};
    InitCommonControlsEx(&init);
 
    UI{code, data}.run();


}
catch(const std::exception &e)
{
    MessageBox(nullptr,e.what(),"Terc6UI",MB_OK|MB_ICONEXCLAMATION);
}





