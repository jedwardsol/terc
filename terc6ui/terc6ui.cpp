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
}


void UI::refreshUICode()
{
    SendDlgItemMessage(dlg,IDC_DISASS,LB_RESETCONTENT,0,0);

    auto    RPC  = static_cast<int>(cpu.reg(Architecture::sixTrit::Register::RPC));

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

    auto                opCode = static_cast<Architecture::sixTrit::OpCode>  (static_cast<int>(operation.halves().first));

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

    if(cpu.reg(Architecture::sixTrit::Register::REXC) > tryte{0})
    {
        EnableWindow(GetDlgItem(dlg,IDC_STEP),FALSE);
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

    case IDC_DISASS:

        if(message == LBN_SELCHANGE)
        {
            SendDlgItemMessage(dlg,IDC_DISASS,LB_SETCURSEL,currentCodeIndex, 0);
        }

        break;


    }


}
