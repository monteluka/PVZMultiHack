﻿#pragma once

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

void toggleHack(const HANDLE& gameHandle, std::tuple<bool, bool, uintptr_t>& hackOption, const char* hackName,
                const char* newBytes, const char* oldBytes, const size_t& bytesLen);

void toggleHack(const HANDLE& gameHandle, std::tuple<bool, bool, std::vector<uintptr_t>>& hackOption,
                const char* hackName,
                const std::vector<const char*> newBytes, const std::vector<const char*> oldBytes,
                const std::vector<size_t>& bytesLen);

void toggleHackWithHook(const HANDLE& gameHandle, std::tuple<bool, bool, uintptr_t>& hackOption, sHookInfo& hookInfo,
                        const char* hackName, const char* oldBytes,
                        const size_t& bytesLen);
