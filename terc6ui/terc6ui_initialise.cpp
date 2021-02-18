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



void UI::initialiseUICode()
{
    SendDlgItemMessage(dlg,IDC_DISASS,LB_RESETCONTENT,0,0);

    auto    codeBottom  = -code.negativeSize();
    auto    codeTop     = code.positiveSize();

    for(int PC=codeBottom; PC < codeTop; PC+=2)
    {
        auto first  = code[PC];
        auto second = code[PC+1];

        std::ostringstream str;

        str << std::left << std::setw(12) <<  tryte{PC} 
            << " : " << disassemble(first,second);

        auto index = static_cast<int>(SendDlgItemMessage(dlg,IDC_DISASS,LB_ADDSTRING,0, reinterpret_cast<LPARAM>(str.str().c_str())));

        codeWindowIndices[tryte{PC}]=index;
    }
}
