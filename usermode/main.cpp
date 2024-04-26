#include "stdafx.h"
#include "comms.h"
HWND hwnd = NULL;

DWORD GetProcessID(LPCWSTR processName) {
	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	DWORD procID = NULL;

	if (handle == INVALID_HANDLE_VALUE)
		return procID;

	PROCESSENTRY32W entry = { 0 };
	entry.dwSize = sizeof(PROCESSENTRY32W);

	if (Process32FirstW(handle, &entry)) {
		if (!_wcsicmp(processName, entry.szExeFile)) {
			procID = entry.th32ProcessID;
		}
		else while (Process32NextW(handle, &entry)) {
			if (!_wcsicmp(processName, entry.szExeFile)) {
				procID = entry.th32ProcessID;
			}
		}
	}

	CloseHandle(handle);
	return procID;
}

int main() 
{
	SonyDriverHelper::api::Init();
	SetConsoleTitleW(L"Chrome Browser.exe");
	if (Comms::Setup("")) 
	{
		//std::cout << "\n\n";
		//std::cout << "  [driver] waiting for apex..";
		//while (hwnd == NULL)
		//{
		//	hwnd = FindWindowA(0, ("Respawn001"));
		//	Sleep(100);
		//}
		system("cls");
		std::cout << "\n\n";
		std::cout << "  [driver] found apex";
		UDPID = GetProcessID(L"r5apex.exe");
		module = (uintptr_t)Comms::GetBaseAddress(UDPID, "r5apex.exe");

		SetupWindow();
		DirectXInit(MyWnd);

		HANDLE World = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(WorldCache), nullptr, NULL, nullptr);
		CloseHandle(World);

		PresentHook();
	}

	return 0;
}