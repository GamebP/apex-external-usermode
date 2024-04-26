#pragma once
#include <wtypes.h>
#include <cstdint>
#include "lib.hpp"

DWORD UDPID;
uintptr_t module;

enum Request {
	GETBASE = 0,
	READPROCESSMEMORY = 1,
	WRITEPROCESSMEMORY = 2,
	OPENHANDLE = 3,
	MOUSE = 4,
};
struct Communication {

	Request Request;
	DWORD processID;
	DWORD Reason; // must be 0xDEADBEEF....
	PVOID Outbase; // output image base for process.

	//mouse
	long x;
	long y;
	unsigned short button_flags;

	/*
	* READ/WRITE PROCESS MEMORY.
	*/
	uintptr_t Address;
	uintptr_t result;
	size_t size;
	const char* moduleName;
};

namespace Comms {
	BOOL Setup(LPCSTR routineName);
	PVOID GetBaseAddress(DWORD processID, const char* name);
	NTSTATUS ReadProcessMemory(DWORD processID, uintptr_t src, void* dest, uint32_t size);
	NTSTATUS WriteProcessMemory(DWORD processID, uintptr_t src, uint8_t* dest, uint32_t size);
}

template <typename T>
T vmread(const uintptr_t address)
{
	T buffer{ };
	Comms::ReadProcessMemory(UDPID, address, (uint8_t*)&buffer, sizeof(T));
	return buffer;
}


template <typename T>
void vmwrite(const uintptr_t address, T value)
{
	Comms::WriteProcessMemory(UDPID, address, (uint8_t*)&value, sizeof(T));
}