#include <Windows.h>
#include <assert.h>
#include "game\q_shared.h"

static HMODULE g_Self = NULL;
static TCHAR moduleName[1024];


void PrintConsole( char *fmt, ...)
{
	int		len;
	va_list		va;
	char	bigbuffer[1024];

	va_start(va, fmt);
	len = vsprintf_s(bigbuffer, sizeof(bigbuffer), fmt, va);
	va_end(va);

	OutputDebugString(bigbuffer);
}

int LockLibraryIntoProcessMem(HMODULE DllHandle, HMODULE *LocalDllHandle)
{
	if (NULL == LocalDllHandle)
		return ERROR_INVALID_PARAMETER;

	*LocalDllHandle = NULL;
	
	if (0 == GetModuleFileName(
		DllHandle,
		moduleName,
		sizeof(moduleName) / sizeof(TCHAR)))
		return GetLastError();
	*LocalDllHandle = LoadLibrary(moduleName);
	if (NULL == *LocalDllHandle)
		return GetLastError();
	return NO_ERROR;
}



BOOL APIENTRY DllMain(HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		assert(LockLibraryIntoProcessMem(hModule, &g_Self) == NO_ERROR);
		PrintConsole("Attaching '%s'\n", moduleName);
		break;
	case DLL_PROCESS_DETACH:
		//PrintConsole("Detatching '%s'\n", moduleName);
		break;
	}
	return TRUE;
}