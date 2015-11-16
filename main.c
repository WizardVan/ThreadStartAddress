#include <windows.h>



typedef LONG    NTSTATUS;

typedef NTSTATUS(WINAPI *pNtQIT)(HANDLE, LONG, PVOID, ULONG, PULONG);

#define STATUS_SUCCESS    ((NTSTATUS)0x00000000L)

#define ThreadQuerySetWin32StartAddress 9





DWORD WINAPI GetThreadStartAddress(HANDLE hThread)

{

	NTSTATUS ntStatus;

	HANDLE hDupHandle;

	DWORD dwStartAddress;



	pNtQIT NtQueryInformationThread = (pNtQIT)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQueryInformationThread");

	if (NtQueryInformationThread == NULL) return 0;



	HANDLE hCurrentProcess = GetCurrentProcess();

	if (!DuplicateHandle(hCurrentProcess, hThread, hCurrentProcess, &hDupHandle, THREAD_QUERY_INFORMATION, FALSE, 0)){

		SetLastError(ERROR_ACCESS_DENIED);

		return 0;

	}

	ntStatus = NtQueryInformationThread(hDupHandle, ThreadQuerySetWin32StartAddress, &dwStartAddress, sizeof(DWORD), NULL);

	CloseHandle(hDupHandle);



	if (ntStatus != STATUS_SUCCESS) return 0;

	return dwStartAddress;

}



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nShowCmd)

{

	CHAR cMessage[9];

	wsprintf(cMessage, "%p", GetThreadStartAddress(GetCurrentThread()));

	MessageBox(HWND_DESKTOP, cMessage, "Start Address", MB_ICONINFORMATION | MB_OK);

	return 0;

}