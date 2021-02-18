#include <Windows.h>

#include "Arithmetic/Arithmetic.h"
#include "Arithmetic/trint.h"
#include "Arithmetic/trint_std.h"

#include "Architecture/MemoryBlock.h"
#include "terc6cpu/sixTritCPU.h"

#include    "resource.h"
#include    "resource.h"



struct UI : Architecture::IOPorts
{
    UI(const Architecture::MemoryBlock  &code,
             Architecture::MemoryBlock  &data) : code{code}, data{data},cpu{code,data,*this}  , dlg{nullptr}
    {}

    ~UI() override = default;


    void messageLoop()
    {
        auto result = DialogBoxParam(GetModuleHandle(nullptr),
                                     MAKEINTRESOURCE(IDD_DIALOG1),
                                     nullptr,
                                     &UI::procWrapper,
                                     reinterpret_cast<LPARAM>(this));

        if(result < 0)
        {
            throw std::system_error{ static_cast<int>(GetLastError()), std::system_category(), "DialogProc"};
        }
    }


private:

    Architecture::MemoryBlock const    &code;
    Architecture::MemoryBlock          &data;
    Architecture::sixTrit::CPU          cpu;
    HWND                                dlg;

    int                                 currentCodeIndex{};
    int                                 currentStackIndex{};
    std::ostringstream                  stdOut;


    INT_PTR proc(UINT m,WPARAM w,LPARAM l)
    {
        switch(m)
        {
        case WM_INITDIALOG:
            setFonts();
            refreshUI();
            return TRUE;

        case WM_COMMAND:
            command(LOWORD(w), HIWORD(w));
            break;



        }

        return 0;
    }

    void setFonts ();
    void refreshUI()
    {
        refreshUICode();
        refreshUIStack();
        refreshUIRegisters();
    }

    void refreshUICode();
    void refreshUIStack();
    void refreshUIRegisters();


    void command  (int  control,  int message);


    static INT_PTR procWrapper(HWND h,UINT m,WPARAM w,LPARAM l)
    {
        if(m==WM_INITDIALOG)
        {
            SetWindowLongPtr(h,DWLP_USER,l);
            reinterpret_cast<UI*>(l)->dlg=h;
        }


        auto This = reinterpret_cast<UI*>(GetWindowLongPtr(h,DWLP_USER));

        if(This)
        {
            return This->proc(m,w,l);
        }
        else
        {
            return 0;
        }
    }



private:

    Architecture::Exception  out(const trybble    portNumber,  const tryte    data) override;
    Architecture::Exception  in (const trybble    portNumber,        tryte   &result) override;



private:

    struct  regToControl
    {
        Architecture::sixTrit::Register     reg;
        int                                 control;
    };
    
    const static inline regToControl GPRs[]
    {
        {Architecture::sixTrit::Register::Rn7, IDC_Rn7},
        {Architecture::sixTrit::Register::Rn6, IDC_Rn6},
        {Architecture::sixTrit::Register::Rn5, IDC_Rn5},
        {Architecture::sixTrit::Register::Rn4, IDC_Rn4},
        {Architecture::sixTrit::Register::Rn3, IDC_Rn3},
        {Architecture::sixTrit::Register::Rn2, IDC_Rn2},
        {Architecture::sixTrit::Register::Rn1, IDC_Rn1},
        {Architecture::sixTrit::Register::R0,  IDC_R0},
        {Architecture::sixTrit::Register::R1,  IDC_R1},
        {Architecture::sixTrit::Register::R2,  IDC_R2},
        {Architecture::sixTrit::Register::R3,  IDC_R3},
        {Architecture::sixTrit::Register::R4,  IDC_R4},
        {Architecture::sixTrit::Register::R5,  IDC_R5},
        {Architecture::sixTrit::Register::R6,  IDC_R6},
        {Architecture::sixTrit::Register::R7,  IDC_R7},
        {Architecture::sixTrit::Register::R8,  IDC_R8},
        {Architecture::sixTrit::Register::R9,  IDC_R9},
        {Architecture::sixTrit::Register::R10, IDC_R10},
        {Architecture::sixTrit::Register::R11, IDC_R11},
        {Architecture::sixTrit::Register::R12, IDC_R12},
        {Architecture::sixTrit::Register::R13, IDC_R13}
    };

    const static inline regToControl otherRegisters[]
    {
        {Architecture::sixTrit::Register::RPC,  IDC_RPC},
        {Architecture::sixTrit::Register::RRA,  IDC_RRA},
        {Architecture::sixTrit::Register::REXC, IDC_REXC},
        {Architecture::sixTrit::Register::REXA, IDC_REXA},
        {Architecture::sixTrit::Register::RSP,  IDC_RSP},
    };




};
