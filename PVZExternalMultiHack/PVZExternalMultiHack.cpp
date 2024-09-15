#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

int main()
{
    // get process id
    HANDLE hSnapshot{CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)};

    // need to get the size according to MSDN
    PROCESSENTRY32 pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // loop through all the processes in hSnapshot
    do
    {
        if (!_wcsicmp(pe32.szExeFile, L"popcapgame1.exe")) break;
    }
    while (Process32Next(hSnapshot, &pe32));
    CloseHandle(hSnapshot);

    // display information on the process
    std::wcout << "PVZ Process ID: " << pe32.th32ProcessID << '\n' << "Process name: " << pe32.szExeFile << '\n';

    // get handle to pvz
    HANDLE hPVZ {OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID)};

    // check if handle is valid and we can read sun amount
    DWORD sun {};
    std::uintptr_t sunAddress {0x21C851A8};
    ReadProcessMemory(hPVZ, reinterpret_cast<LPVOID>(sunAddress), &sun, sizeof(sun), nullptr);
    std::cout << "Current amount of sun: " << sun;
    
    CloseHandle(hPVZ);
    return 0;
}
