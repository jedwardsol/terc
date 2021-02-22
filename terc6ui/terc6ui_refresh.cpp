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

// TODO : read map file

void UI::refreshCode()
{
    std::ostringstream title;
    title << "Code. RPC : " << tryte{cpu.reg(Architecture::sixTrit::Register::RPC)};


// scroll

    auto RPC = cpu.reg(Architecture::sixTrit::Register::RPC);
    SendDlgItemMessage(dlg,IDC_DISASS,LB_SETCURSEL,codeWindowIndices[RPC], 0);

// current instruction : conditional

    auto operation = code[RPC];
    auto opCode    = static_cast<Architecture::sixTrit::OpCode>  (static_cast<int>(operation.halves().first));

    if(Architecture::sixTrit::isConditionalInstruction(opCode))
    {
        auto condition = static_cast<Architecture::Condition>  (static_cast<int>(operation.halves().second));

        if(cpu.isConditionTrue(condition))
        {
            title << "  Conditional=True";
        }
        else
        {
            title << "  Conditional=False";
        }
    }


    SetDlgItemText(dlg,IDC_CODE_BOX,title.str().c_str());


// exception registers

    if(cpu.reg(Architecture::sixTrit::Register::REXC) > tryte{0})
    {
        ResetEvent(events[event::step]);
        EnableWindow(GetDlgItem(dlg,IDC_STEP),FALSE);
        EnableWindow(GetDlgItem(dlg,IDC_RUN),FALSE);
    }
}

void UI::refreshRegisters()
{
    for(auto &reg : registers)
    {
        auto value    = cpu.reg(reg.reg);
        std::ostringstream str;
        str << value;
        SetDlgItemText(dlg,reg.control,str.str().c_str());
    }


// flags  register

    std::string flags;


    flags += "C:"s + to_string(cpu.getFlag(Architecture::Flag::Comparison));
    flags += " E:"s + to_string(cpu.getFlag(Architecture::Flag::ExecutedConditional));
    flags += " S:"s + to_string(cpu.getFlag(Architecture::Flag::Sign));
    flags += " O:"s + to_string(cpu.getFlag(Architecture::Flag::Overflow));

    SetDlgItemText(dlg,IDC_RFLAGS,flags.c_str());
}

void UI::refreshStack()
{
    std::ostringstream title;
    title << "Stack. RSP : " << tryte{cpu.reg(Architecture::sixTrit::Register::RSP)};
    SetDlgItemText(dlg,IDC_STACK_BOX,title.str().c_str());


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



void UI::refreshData()
{
    SendDlgItemMessage(dlg,IDC_DATA,LB_RESETCONTENT,0,0);

    auto    dataBottom  = 0;
    auto    dataTop     = data.positiveSize();

    for(int address =dataBottom; address < ((dataTop + 5)/6)*6; address+=6)
    {
        std::ostringstream str;

        str << std::left << std::setw(12) << tryte{address} << " : " ;

        for(int i = address;  i < address + 6 && i < dataTop; i++)
        {
            auto t = data[address];

            str << std::left << std::setw(12) << t << " " ;
        }

        SendDlgItemMessage(dlg,IDC_DATA,LB_ADDSTRING,0, reinterpret_cast<LPARAM>(str.str().c_str()));
    }
}
