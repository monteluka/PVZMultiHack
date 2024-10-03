#include "memory.h"

#include <tuple>

sHookInfo::sHookInfo(const char* hookInstructionBytes, const size_t& hookBytesLen, const char* jumpBackInstructionBytes,
                     const size_t& jumpBackBytesLen)
{
    for (size_t i{0}; i < hookBytesLen; ++i)
    {
        hookInstruction.push_back(hookInstructionBytes[i]);
    }
    for (size_t i{0}; i < jumpBackBytesLen; ++i)
    {
        jumpBackInstruction.push_back(jumpBackInstructionBytes[i]);
    }
}

uintptr_t SigScan(const HANDLE& gameHandle, const MODULEENTRY32W& me32, const std::vector<WORD>& sig)
{
    std::vector<BYTE> memBuffer(me32.modBaseSize, 0);

    ReadProcessMemory(gameHandle, me32.modBaseAddr, memBuffer.data(), memBuffer.size(), nullptr);

    for (size_t i{0}; i < memBuffer.size(); ++i)
    {
        for (size_t j{0}; j < sig.size(); ++j)
        {
            if (sig[j] != 0xFFFF && sig[j] != memBuffer[i + j]) break;
            if (j + 1 == sig.size()) return reinterpret_cast<uintptr_t>(me32.modBaseAddr) + i;
        }
    }

    return -1;
}

void toggleHack(const HANDLE& gameHandle,
                std::tuple<bool, bool, uintptr_t, const char*, const char*, size_t>& hackOption)
{
    // make sure bool that holds if box was clicked is set to false
    std::get<0>(hackOption) = false;

    if (std::get<1>(hackOption))
    {
        WriteProcessMemory(gameHandle, reinterpret_cast<uintptr_t*>(std::get<2>(hackOption)),
                           std::get<3>(hackOption),
                           std::get<5>(hackOption), nullptr);
    }
    else
    {
        WriteProcessMemory(gameHandle, reinterpret_cast<uintptr_t*>(std::get<2>(hackOption)),
                           std::get<4>(hackOption),
                           std::get<5>(hackOption), nullptr);
    }
}

void toggleHack(const HANDLE& gameHandle,
                std::tuple<bool, bool, std::vector<uintptr_t>, std::vector<const char*>,
                std::vector<const char*>, std::vector<size_t>>& hackOption)
{
    // make sure bool that holds if box was clicked is set to false
    std::get<0>(hackOption) = false;

    if (std::get<1>(hackOption))
    {
        for (size_t i{0}; i < std::get<2>(hackOption).size(); ++i)
            WriteProcessMemory(
                gameHandle, reinterpret_cast<uintptr_t*>(std::get<2>(hackOption)[i]),
                std::get<3>(hackOption)[i],
                std::get<5>(hackOption)[i], nullptr);
    }
    else
    {
        for (size_t i{0}; i < std::get<2>(hackOption).size(); ++i)
            WriteProcessMemory(
                gameHandle, reinterpret_cast<uintptr_t*>(std::get<2>(hackOption)[i]),
                std::get<4>(hackOption)[i],
                std::get<5>(hackOption)[i], nullptr);
    }
}

void toggleHackWithHook(const HANDLE& gameHandle,
                        std::tuple<bool, bool, uintptr_t, const char*, const char*, size_t>& hackOption,
                        sHookInfo& hookInfo)
{
    // make sure bool that holds if box was clicked is set to false
    std::get<0>(hackOption) = false;

    if (std::get<1>(hackOption))
    {
        // allocate memory for new instructions
        hookInfo.locAllocatedMemory = reinterpret_cast<uintptr_t>(VirtualAllocEx(
            gameHandle, nullptr, 1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));

        // calculate offset to allocated memory from original instruction
        uintptr_t offsetToAllocatedMem{hookInfo.locAllocatedMemory - std::get<2>(hackOption) - 5};
        // now add that offset after jmp instruction that is used where we hook from
        memcpy(hookInfo.hookInstruction.data() + 1, &offsetToAllocatedMem, sizeof(uintptr_t));

        // calculate offset back to original hook location from location of newly allocated memory
        uintptr_t jumpBackAddress{
            std::get<2>(hackOption) - hookInfo.locAllocatedMemory - hookInfo.jumpBackInstruction.size() +
            hookInfo.
            hookInstruction.size()
        };
        // add that offset right after jmp instruction that will be used in newly allocated memory
        memcpy(hookInfo.jumpBackInstruction.data() + (hookInfo.jumpBackInstruction.size() - 4), &jumpBackAddress,
               sizeof(uintptr_t));

        // now write new instructions + jump back instruction to the newly allocated memory
        WriteProcessMemory(gameHandle, reinterpret_cast<uintptr_t*>(hookInfo.locAllocatedMemory),
                           hookInfo.jumpBackInstruction.data(), hookInfo.jumpBackInstruction.size(), nullptr);


        WriteProcessMemory(gameHandle, reinterpret_cast<uintptr_t*>(std::get<2>(hackOption)),
                           hookInfo.hookInstruction.data(),
                           std::get<5>(hackOption), nullptr);
    }
    else
    {
        // release the memory we allocated when activating the hack
        VirtualFreeEx(gameHandle, reinterpret_cast<uintptr_t*>(hookInfo.locAllocatedMemory), 0,
                      MEM_RELEASE);
        hookInfo.locAllocatedMemory = NULL;
        WriteProcessMemory(gameHandle, reinterpret_cast<uintptr_t*>(std::get<2>(hackOption)),
                           std::get<4>(hackOption),
                           std::get<5>(hackOption), nullptr);
    }
}
