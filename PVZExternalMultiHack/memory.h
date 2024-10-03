#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <iostream>

struct sHookInfo
{
    sHookInfo(const char* hookInstructionBytes, const size_t& hookBytesLen, const char* jumpBackInstructionBytes,
              const size_t& jumpBackBytesLen);
    std::vector<BYTE> hookInstruction{};
    std::vector<BYTE> jumpBackInstruction{};
    uintptr_t locAllocatedMemory{NULL};
};

uintptr_t SigScan(const HANDLE& gameHandle, const MODULEENTRY32W& me32, const std::vector<WORD>& sig);

void toggleHack(const HANDLE& gameHandle,
                std::tuple<bool, bool, uintptr_t, const char*, const char*, size_t>& hackOption);

void toggleHack(const HANDLE& gameHandle,
                std::tuple<bool, bool, std::vector<uintptr_t>, std::vector<const char*>,
                std::vector<const char*>, std::vector<size_t>>& hackOption);

void toggleHackWithHook(const HANDLE& gameHandle,
                        std::tuple<bool, bool, uintptr_t, const char*, const char*, size_t>& hackOption,
                        sHookInfo& hookInfo);
