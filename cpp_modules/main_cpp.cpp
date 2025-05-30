#include <iostream>
#include <iomanip>
//#include <pybind11/pybind11.h> //Działa

#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>

BOOL GetProcessList();
BOOL ListProcessModules(DWORD dwPID);
// BOOL ListProcessThreads(Dword dwOwnerPID);
void printError(TCHAR const* msg);
//namespace py = pybind11;

int main(void)
{
    GetProcessList();
    return 0;
}

BOOL GetProcessList()
{
    HANDLE hProcessSnap;
    HANDLE hProcess;
    PROCESSENTRY32 pe32;
    DWORD dwPriorityClass;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hProcessSnap == INVALID_HANDLE_VALUE)
    {
        printError(L"CreateToolhelp32Snapshot (of processes)");
        return(FALSE);
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if(!Process32First(hProcessSnap, &pe32))
    {
        printError(L"Process32First");
        CloseHandle(hProcessSnap);
        return(FALSE);
    }

    do
    {
        std::wcout << L">PROCESS NAME: " << pe32.szExeFile << std::endl;

        dwPriorityClass = 0;
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
        if(hProcess == NULL)
            printError(L"OpenProcess");
        else
        {
            dwPriorityClass = GetPriorityClass(hProcess);
            if(!dwPriorityClass)
                printError(L"GetPriorityClass");
            CloseHandle(hProcess);
        }

        std::wcout << L" Process ID        = "<< std::setw(8) << std::setfill(L'0') << pe32.th32ProcessID << std::endl;
        std::wcout << L" Thread count      = "<< std::dec << pe32.cntThreads << std::endl;
        std::wcout << L" Parent process ID = "<< std::setw(8) << std::setfill(L'0') << pe32.th32ParentProcessID << std::endl;
        std::wcout << L" Priority base     = "<< std::dec << pe32.pcPriClassBase << std::endl;
        if(dwPriorityClass)
        std::wcout << L" Priority class    = "<< std::dec << dwPriorityClass << std::endl;
        
        //ListProcessModules( pe32.th32ProcessID );
        //ListProcessThreads( pe32.th32ProcessID );
        
    } while(Process32Next(hProcessSnap, &pe32));
    
    CloseHandle(hProcessSnap);
    
    
    return(TRUE);
}

BOOL ListProcessModules(DWORD dwPID)
{
    HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
    MODULEENTRY32 me32;
    
    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
    if(hModuleSnap == INVALID_HANDLE_VALUE)
    {
        printError(L"CreateToolhelp32Snapshot (of modules)");
        return(FALSE);
    }
    
    me32.dwSize = sizeof(MODULEENTRY32);
    
    if(!Module32First(hModuleSnap, &me32))
    {
        printError(L"Module32First");
        CloseHandle(hModuleSnap);
        return(FALSE);
    }
    do
    {
        std::wcout << L"\n   Module Name:    " << me32.szModule << std::endl; //%s
        std::wcout << L"   Executable      =" << me32.szExePath << std::endl; //%s
        std::wcout << L"   Process ID      =" << std::setw(8) << std::setfill(L'0') << me32.th32ProcessID << std::endl;
        std::wcout << L"   Ref count (g)   =" << std::setw(4) << std::setfill(L'0') << me32.GlblcntUsage << std::endl;
        std::wcout << L"   Ref count (p)   =" << std::setw(4) << std::setfill(L'0') << me32.ProccntUsage << std::endl;
        std::wcout << L"   Base address    =" << std::setw(8) << std::setfill(L'0') << me32.th32ProcessID << std::endl;


    }while(Module32Next(hModuleSnap, &me32));


    CloseHandle(hModuleSnap);
    return(TRUE);
}


//----------------------------------------
int dodaj(int a, int b) {
    return a + b;
}

// PYBIND11_MODULE(main_cpp, m) {
//     m.def("dodaj", &dodaj, "Dodaje dwie liczby");
// }

void printError(const wchar_t* msg) {
    std::wcerr << L"Błąd: " << msg << L" (kod " << GetLastError() << L")" << std::endl;
}
