#include "memory.h"

DWORD SigScan(const HANDLE& gameHandle, const MODULEENTRY32W& me32, std::vector<int> sig)
{
    std::vector<BYTE> memBuffer(me32.modBaseSize, 0);

    ReadProcessMemory(gameHandle, me32.modBaseAddr, memBuffer.data(), memBuffer.size(), nullptr);

    for (size_t i{0}; i < memBuffer.size(); ++i)
    {
        for (size_t j{0}; j < sig.size(); ++j)
        {
            if (sig[j] != -1 && sig[j] != memBuffer[i + j]) break;
            if (j + 1 == sig.size()) return reinterpret_cast<DWORD>(me32.modBaseAddr) + i;
        }
    }

    return -1;
}
