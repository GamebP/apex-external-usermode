

#pragma once

__int64(__fastcall* function_address)(void*) = nullptr;

DWORD UDPID;
uintptr_t baseaddy;

enum Request {
	GETBASE = 0,
	READPROCESSMEMORY = 1,
	WRITEPROCESSMEMORY = 2,
	OPENHANDLE = 3,
};

struct Communication {

	Request Request;
	DWORD processID;
	DWORD Reason; // must be 0xDEADBEEF....
	PVOID Outbase; // output image base for process.

	/*
	* READ/WRITE PROCESS MEMORY.
	*/
	uintptr_t Address;
	uintptr_t result;
	size_t size;
	const char* moduleName;
};


BOOL Setup() {

	unsigned char Shellcode[] =
	{

		0x16, 0xa3, 0xc2, 0xd3, 0x0, 0x2, 0x30, 0xb3,
		0xc3, 0x20, 0x6, 0xdf, 0x40, 0x57, 0x8f, 0x6,
		0xc8, 0x38, 0x7a, 0x8, 0xea, 0x50, 0xd4, 0x8a,
		0x8, 0x55, 0x48, 0x65, 0xd7, 0xd6, 0x42
	};

	for (unsigned int m = 0; m < sizeof(Shellcode); ++m)
	{
		unsigned char c = Shellcode[m];
		c -= 0xf4;
		c = (c >> 0x3) | (c << 0x5);
		c ^= m;
		c += m;
		c = ~c;
		c = (c >> 0x3) | (c << 0x5);
		c -= 0xa0;
		c ^= m;
		c += m;
		c = (c >> 0x6) | (c << 0x2);
		c = -c;
		c ^= m;
		c = ~c;
		c ^= 0x60;
		c -= m;
		c = ~c;
		c = -c;
		c += m;
		c = -c;
		c ^= 0xb7;
		c += 0xcd;
		c ^= 0xd;
		Shellcode[m] = c;
	}


	LoadLibrary(L"user32.dll");

	const auto win32k = LoadLibrary(L"win32u.dll");

	if (!win32k)
	{
		return false;
	}

	*(void**)&function_address = GetProcAddress(win32k, (LPCSTR)Shellcode);

	if (!function_address)
	{
		return false;
	}

	return true;
}


PVOID GetBaseAddress(DWORD processID, const char* name) {
	Communication request = {};
	SecureZeroMemory(&request, sizeof(Communication));

	request.Request = Request::GETBASE;
	request.Reason = 0x4569;
	request.processID = processID;
	request.moduleName = name;
	request.Outbase = 0;

	function_address(&request);
	return request.Outbase;
}

NTSTATUS ReadProcessMemory(DWORD processID, uintptr_t src, uint8_t* dest, uint32_t size) {


	Communication request = {};
	SecureZeroMemory(&request, sizeof(Communication));

	request.Request = Request::READPROCESSMEMORY;
	request.Reason = 0x4569;
	request.processID = processID;
	request.Address = src;
	request.result = (uintptr_t)dest;
	request.size = size;

	return function_address(&request);

}

NTSTATUS WriteProcessMemory(DWORD processID, uintptr_t src, uint8_t* dest, uint32_t size) {


	Communication request = {};
	SecureZeroMemory(&request, sizeof(Communication));

	request.Request = Request::WRITEPROCESSMEMORY;
	request.Reason = 0x4569;
	request.processID = processID;
	request.Address = src;
	request.result = (uintptr_t)dest;
	request.size = size;

	return function_address(&request);

}

template <typename T>
T readd(const uintptr_t address)
{
	T buffer{ };
	ReadProcessMemory(UDPID, address, (uint8_t*)&buffer, sizeof(T));
	return buffer;
}


template <typename T>
void Writee(const uintptr_t address, T value)
{
	WriteProcessMemory(UDPID, address, (uint8_t*)&value, sizeof(T));
}
