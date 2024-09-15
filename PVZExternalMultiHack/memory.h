#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <iostream>

DWORD SigScan(const HANDLE& gameHandle, const MODULEENTRY32W& me32, std::vector<int> sig);