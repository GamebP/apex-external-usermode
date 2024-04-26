#include "stdafx.h"

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

HWND hwnd = NULL;
int main() {
	SetConsoleTitleA("ApexLauncher For Emos");
	system("color e");

	if (!Setup())
	{
		std::cout << "\n\n Could Not Load The Driver, Please Restart Your Pc";
		Sleep(2000);
		exit(0);
	}

	system("cls");
	std::cout << "\n\n Waiting For Apex Legends \n\n";

	//while (hwnd == NULL)
	//{
	//	hwnd = FindWindowA(0, ("Respawn001"));
	//	Sleep(100);
	//}

	std::cout << " Found Apex Legends, Please Wait While We Hook \n";

	UDPID = GetProcessID(L"r5apex.exe");
	if (!UDPID)
	{
		std::cout << _("[driver] pid fail.") << std::endl;
	}

	baseaddy = (uintptr_t)GetBaseAddress(UDPID, "r5apex.exe");
	if (!baseaddy) {
		std::cout << _("[driver] baseaddress fail.") << std::endl;
	}



	SetupWindow();
	DirectXInit(MyWnd);

	_beginthread(WorldCache, 0, nullptr);

	PresentHook();
}