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
 
    UI{code, data}.messageLoop();


}
catch(const std::exception &e)
{
    MessageBox(nullptr,e.what(),"Terc6UI",MB_OK|MB_ICONEXCLAMATION);
}




void UI::command  (int  control, int message)
{
    switch(control)
    {
    case IDCANCEL:
        EndDialog(dlg,0);
        break;

    case IDC_STEP:
    
        cpu.execute();
        refreshUI();
        break;

    case IDC_STEP10:
    
        for(int i=0;i<10 && cpu.reg(Architecture::sixTrit::Register::REXC) <= tryte{0} ;i++)
        {
            cpu.execute();
        }
        refreshUI();
        break;


    case IDC_DISASS:
        if(message == LBN_SELCHANGE)
        {
            auto RPC = cpu.reg(Architecture::sixTrit::Register::RPC);
            SendDlgItemMessage(dlg,IDC_DISASS,LB_SETCURSEL,codeWindowIndices[RPC], 0);
        }
        break;

    case IDC_STACK:

        if(message == LBN_SELCHANGE)
        {
            SendDlgItemMessage(dlg,IDC_STACK, LB_SETCURSEL,currentStackIndex, 0);
        }
        break;
    }
}


