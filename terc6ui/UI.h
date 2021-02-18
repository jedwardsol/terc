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
             Architecture::MemoryBlock  &data) : cpu{code,data,*this}  , dlg{nullptr}
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


    Architecture::sixTrit::CPU          cpu;
    HWND                                dlg;


    INT_PTR proc(UINT m,WPARAM w,LPARAM l)
    {
        switch(m)
        {
        case WM_INITDIALOG:
            return TRUE;
            ;
        }

        return 0;
    }


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

};
