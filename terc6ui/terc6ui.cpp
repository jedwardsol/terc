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
#include "symbols.h"


std::vector<symbol>  loadSymbols(const fs::path& mapFileName)
{
    std::vector<symbol> symbols;
    std::ifstream       map{mapFileName};

    if(!map)
    {
        return symbols;
    }

    std::string line;

    while(std::getline(map,line))
    {
//        $GCD             : 0++0+-(110)         Referenced 1  times 

        std::istringstream  entry{line};

        std::string symbol;
        std::string separator;
        tryte       address;
        
        entry >> symbol >> separator >> address;

        if(   entry 
           && symbol.size()
           && symbol[0] == '$'
           && separator == ":")
        {
            symbols.push_back({address,symbol});
        }
    }

    return symbols;
}


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
    fs::path    mapFileName      {args[0]};

    codeFileName.replace_extension(".code");
    dataFileName.replace_extension(".data");
    finaldataFileName.replace_extension(".data.final");
    mapFileName.replace_extension(".map");

    const Architecture::MemoryBlock     code {codeFileName.string() };        
          Architecture::MemoryBlock     data {dataFileName.string() , 
                                              finaldataFileName.string()};        

    auto symbols{loadSymbols(mapFileName)};                    


    INITCOMMONCONTROLSEX init{sizeof(init),ICC_WIN95_CLASSES};
    InitCommonControlsEx(&init);
 
    UI{code, symbols, data}.run();


}
catch(const std::exception &e)
{
    MessageBox(nullptr,e.what(),"Terc6UI",MB_OK|MB_ICONEXCLAMATION);
}

