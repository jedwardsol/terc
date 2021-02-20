#include <Windows.h>
#include <string>
#include <map>
#include <thread>

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
             Architecture::MemoryBlock  &data) : code{code}, 
                                                 data{data},
                                                 cpu{code,data,*this}
    {}

    ~UI() override 
    {
        DeleteObject(monospaced);
        CloseHandle(stepStop[0]);
        CloseHandle(stepStop[1]);
    }

    void run()
    {
        UIthread.join();
        ExecuteThread.join();
    }

private:

    void messageLoop()
    {
        auto result = DialogBoxParam(GetModuleHandle(nullptr),
                                     MAKEINTRESOURCE(IDD_UI),
                                     nullptr,
                                     &UI::procWrapper,
                                     reinterpret_cast<LPARAM>(this));

        if(result < 0)
        {
            throw std::system_error{ static_cast<int>(GetLastError()), std::system_category(), "DialogProc"};
        }
    }


    void cpuThread()
    {
        bool done{};

        while(!done)
        {
            auto signal = WaitForMultipleObjects(2,stepStop.data(),FALSE,INFINITE);

            if(signal == WAIT_OBJECT_0)
            {
                cpu.execute();
                PostMessage(dlg,WM_REFRESH,0,0);
            }
            else 
            {
                done=true;
            }
        }
    }


private:


    Architecture::MemoryBlock const    &code;
    Architecture::MemoryBlock          &data;
    Architecture::sixTrit::CPU          cpu;
    std::array<HANDLE,2>                stepStop{CreateEvent(nullptr,FALSE,FALSE,nullptr),
                                                             CreateEvent(nullptr,FALSE,FALSE,nullptr)};

    HWND                                dlg           {nullptr};

    int                                 currentStackIndex{};
    std::map<tryte, int>                codeWindowIndices{};

    std::ostringstream                  stdOut;

    static constexpr auto               WM_REFRESH      {WM_APP};
    HFONT                               monospaced      {nullptr};

    std::thread                         ExecuteThread   { &UI::cpuThread,  this};;
    std::thread                         UIthread        { &UI::messageLoop,this};;


private:

    void initialiseUI()
    {
        setFonts();
        initialiseCode();
    }

    void refreshUI()
    {
        refreshCode();
        refreshStack();
        refreshData();
        refreshRegisters();
    }


    void setFonts ();
    void initialiseCode();

    void refreshCode();
    void refreshStack();
    void refreshData();
    void refreshRegisters();

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

    INT_PTR proc(UINT m,WPARAM w,LPARAM l)
    {
        switch(m)
        {
        case WM_INITDIALOG:
            initialiseUI();
            refreshUI();
            return TRUE;

        case WM_REFRESH:
            refreshUI();
            break;

        case WM_COMMAND:
            command(LOWORD(w), HIWORD(w));
            break;
        }

        return 0;
    }

    void command  (int  control, int message)
    {
        switch(control)
        {
        case IDCANCEL:
            SetEvent(stepStop[1]);
            EndDialog(dlg,0);
            break;

        case IDC_STEP:
            SetEvent(stepStop[0]);
            break;

    //    case IDC_STEP10:
    //    
    //        for(int i=0;i<10 && cpu.reg(Architecture::sixTrit::Register::REXC) <= tryte{0} ;i++)
    //        {
    //            cpu.execute();
    //        }
    //        refreshUI();
    //        break;
    //

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



private:

    Architecture::Exception  out(const trybble    portNumber,  const tryte    data) override;
    Architecture::Exception  in (const trybble    portNumber,        tryte   &result) override;


private:

    struct  regToControl
    {
        Architecture::sixTrit::Register     reg;
        int                                 control;
    };
    
    const static inline regToControl registers[]
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
        {Architecture::sixTrit::Register::R13, IDC_R13},
        {Architecture::sixTrit::Register::RRA,  IDC_RRA},
        {Architecture::sixTrit::Register::REXC, IDC_REXC},
        {Architecture::sixTrit::Register::REXA, IDC_REXA},
    };

};
