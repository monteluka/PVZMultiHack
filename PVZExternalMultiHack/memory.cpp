#include "memory.h"

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

void toggleHack(const HANDLE& gameHandle, std::pair<bool, uintptr_t>& hackOption, const char* hackName,
                const char* newBytes, const char* oldBytes, const size_t& bytesLen)
{
    hackOption.first = !hackOption.first;
    if (hackOption.first)
    {
        std::cout << hackName << " hack activated" << std::endl;
        WriteProcessMemory(gameHandle, reinterpret_cast<uintptr_t*>(hackOption.second), newBytes, bytesLen, nullptr);
    }
    else
    {
        std::cout << hackName << " hack deactivated" << std::endl;;
        WriteProcessMemory(gameHandle, reinterpret_cast<uintptr_t*>(hackOption.second), oldBytes, bytesLen, nullptr);
    }
}

void toggleHack(const HANDLE& gameHandle, std::pair<bool, std::vector<uintptr_t>>& hackOption,
                const char* hackName,
                const std::vector<const char*> newBytes, const std::vector<const char*> oldBytes,
                const std::vector<size_t>& bytesLen)
{
    hackOption.first = !hackOption.first;
    if (hackOption.first)
    {
        std::cout << hackName << " hack activated" << std::endl;
        for (size_t i{0}; i < hackOption.second.size(); ++i)
            WriteProcessMemory(
                gameHandle, reinterpret_cast<uintptr_t*>(hackOption.second[i]), newBytes[i], bytesLen[i], nullptr);
    }
    else
    {
        std::cout << hackName << " hack deactivated" << std::endl;;
        for (size_t i{0}; i < hackOption.second.size(); ++i)
            WriteProcessMemory(
                gameHandle, reinterpret_cast<uintptr_t*>(hackOption.second[i]), oldBytes[i], bytesLen[i], nullptr);
    }
}

void toggleHackWithHook(const HANDLE& gameHandle, std::pair<bool, uintptr_t>& hackOption, sHookInfo& hookInfo,
                        const char* hackName, const char* oldBytes, const size_t& bytesLen)
{
    hackOption.first = !hackOption.first;
    if (hackOption.first)
    {
        // allocate memory for new instructions
        hookInfo.locAllocatedMemory = reinterpret_cast<uintptr_t>(VirtualAllocEx(
            gameHandle, nullptr, 1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));

        // calculate offset to allocated memory from original instruction
        uintptr_t offsetToAllocatedMem{hookInfo.locAllocatedMemory - hackOption.second - 5};
        // now add that offset after jmp instruction that is used where we hook from
        memcpy(hookInfo.hookInstruction.data() + 1, &offsetToAllocatedMem, sizeof(uintptr_t));

        // calculate offset back to original hook location from location of newly allocated memory
        uintptr_t jumpBackAddress{
            hackOption.second - hookInfo.locAllocatedMemory - hookInfo.jumpBackInstruction.size() + hookInfo.
            hookInstruction.size()
        };
        // add that offset right after jmp instruction that will be used in newly allocated memory
        memcpy(hookInfo.jumpBackInstruction.data() + (hookInfo.jumpBackInstruction.size() - 4), &jumpBackAddress,
               sizeof(uintptr_t));

        // now write new instructions + jump back instruction to the newly allocated memory
        WriteProcessMemory(gameHandle, reinterpret_cast<uintptr_t*>(hookInfo.locAllocatedMemory),
                           hookInfo.jumpBackInstruction.data(), hookInfo.jumpBackInstruction.size(), nullptr);


        std::cout << hackName << " hack activated" << std::endl;
        WriteProcessMemory(gameHandle, reinterpret_cast<uintptr_t*>(hackOption.second), hookInfo.hookInstruction.data(),
                           bytesLen, nullptr);
    }
    else
    {
        // release the memory we allocated when activating the hack
        VirtualFreeEx(gameHandle, reinterpret_cast<uintptr_t*>(hookInfo.locAllocatedMemory), 0,
                      MEM_RELEASE);
        hookInfo.locAllocatedMemory = NULL;
        std::cout << hackName << " hack deactivated" << std::endl;;
        WriteProcessMemory(gameHandle, reinterpret_cast<uintptr_t*>(hackOption.second), oldBytes, bytesLen, nullptr);
    }
}
