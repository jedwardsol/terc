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


std::string  disassemble(tryte  first, tryte second);

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


void UI::setFonts()
{
    LOGFONTA font{};

    auto defaultFont =  reinterpret_cast<HFONT>(SendDlgItemMessage(dlg,GPRs[0].control, WM_GETFONT, 0,0));
    GetObject(defaultFont,sizeof(font),&font);

    strcpy_s(font.lfFaceName,"Consolas");
    HFONT monospaced=CreateFontIndirectA(&font);//reinterpret_cast<HFONT>(GetStockObject(OEM_FIXED_FONT));

    for(auto &GPR : GPRs)
    {
        SendDlgItemMessage(dlg,GPR.control,WM_SETFONT,reinterpret_cast<WPARAM>(monospaced),FALSE);
    }

    for(auto &otherReg : otherRegisters)
    {
        SendDlgItemMessage(dlg,otherReg.control,WM_SETFONT,reinterpret_cast<WPARAM>(monospaced),FALSE);
    }

    SendDlgItemMessage(dlg,IDC_DISASS,WM_SETFONT,reinterpret_cast<WPARAM>(monospaced),FALSE);
    SendDlgItemMessage(dlg,IDC_RFLAGS,WM_SETFONT,reinterpret_cast<WPARAM>(monospaced),FALSE);
    SendDlgItemMessage(dlg,IDC_STDOUT,WM_SETFONT,reinterpret_cast<WPARAM>(monospaced),FALSE);
    SendDlgItemMessage(dlg,IDC_STACK,WM_SETFONT,reinterpret_cast<WPARAM>(monospaced),FALSE);
}


void UI::refreshUICode()
{
    SendDlgItemMessage(dlg,IDC_DISASS,LB_RESETCONTENT,0,0);

    auto    RPC  = static_cast<int>(cpu.reg(Architecture::sixTrit::Register::RPC));

    // TODO : disassemble whole program and just move cursor?
    for(int PC=RPC-10; PC < RPC+40; PC+=2)
    {
        if(    PC >= -code.negativeSize()
           &&  PC <=  code.positiveSize()-2)
        {
            auto first  = code[PC];
            auto second = code[PC+1];

            std::ostringstream str;

            str << std::left << std::setw(12) <<  tryte{PC} << " : " << disassemble(first,second);

            auto index = static_cast<int>(SendDlgItemMessage(dlg,IDC_DISASS,LB_ADDSTRING,0, reinterpret_cast<LPARAM>(str.str().c_str())));

            if(PC == RPC)
            {
                currentCodeIndex = index;
            }
        }
    }

    SendDlgItemMessage(dlg,IDC_DISASS,LB_SETCURSEL,currentCodeIndex, 0);
    auto operation = code[RPC];
    auto opCode    = static_cast<Architecture::sixTrit::OpCode>  (static_cast<int>(operation.halves().first));

    if(Architecture::sixTrit::isConditionalInstruction(opCode))
    {
        auto condition = static_cast<Architecture::Condition>  (static_cast<int>(operation.halves().second));

        if(cpu.isConditionTrue(condition))
        {
            SetDlgItemText(dlg,IDC_CONDITION,"True");
        }
        else
        {
            SetDlgItemText(dlg,IDC_CONDITION,"False");
        }
    }
    else
    {
        SetDlgItemText(dlg,IDC_CONDITION,"N/A");
    }

    std::string flags;


    flags += "C:"s + to_string(cpu.getFlag(Architecture::Flag::Comparison));
    flags += " E:"s + to_string(cpu.getFlag(Architecture::Flag::ExecutedConditional));
    flags += " S:"s + to_string(cpu.getFlag(Architecture::Flag::Sign));
    flags += " O:"s + to_string(cpu.getFlag(Architecture::Flag::Overflow));

    SetDlgItemText(dlg,IDC_RFLAGS,flags.c_str());

    if(cpu.reg(Architecture::sixTrit::Register::REXC) > tryte{0})
    {
        EnableWindow(GetDlgItem(dlg,IDC_STEP),FALSE);
        EnableWindow(GetDlgItem(dlg,IDC_STEP10),FALSE);
    }

}

void UI::refreshUIRegisters()
{
    for(auto &GPR : GPRs)
    {
        auto reg    = cpu.reg(GPR.reg);
        std::ostringstream str;
        str << reg;
        SetDlgItemText(dlg,GPR.control,str.str().c_str());
    }

    for(auto &otherReg : otherRegisters)
    {
        auto reg    = cpu.reg(otherReg.reg);
        std::ostringstream str;
        str << reg;
        SetDlgItemText(dlg,otherReg.control,str.str().c_str());
    }
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
    case IDC_STACK:

        if(message == LBN_SELCHANGE)
        {
            SendDlgItemMessage(dlg,IDC_DISASS,LB_SETCURSEL,currentCodeIndex, 0);
            SendDlgItemMessage(dlg,IDC_STACK, LB_SETCURSEL,currentStackIndex, 0);
        }
        break;
    }
}



void UI::refreshUIStack()
{
    SendDlgItemMessage(dlg,IDC_STACK,LB_RESETCONTENT,0,0);

    auto    RSP         = static_cast<int>(cpu.reg(Architecture::sixTrit::Register::RSP));
    auto    stackTop    = -1;
    auto    stackBottom = data.negativeSize();

    currentStackIndex = -1;

    for(int SP=stackTop; SP >= stackBottom; SP--)
    {
        auto t = data[SP];

        std::ostringstream str;

        str << std::left << std::setw(12) << tryte{SP} << " : " << t;

        auto index = static_cast<int>(SendDlgItemMessage(dlg,IDC_STACK,LB_ADDSTRING,0, reinterpret_cast<LPARAM>(str.str().c_str())));

        if(SP == RSP)
        {
            currentStackIndex = index;
        }
    }

    SendDlgItemMessage(dlg,IDC_STACK,LB_SETCURSEL,currentStackIndex, 0);
}


