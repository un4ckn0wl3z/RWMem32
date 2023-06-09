#include "Common.hpp"

BOOL GetProcId(IN const wchar_t* process_name, OUT DWORD* pprocess_id)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	PROCESSENTRY32 process_entity;
	process_entity.dwSize = sizeof(process_entity);

	if (Process32First(hSnapshot, &process_entity))
	{
		do {

			if (!_wcsicmp(process_entity.szExeFile, process_name))
			{
				*pprocess_id = process_entity.th32ProcessID;
				return TRUE;
			}

		} while (Process32Next(hSnapshot, &process_entity));
	}

	CloseHandle(hSnapshot);

	return FALSE;
}


BOOL ReadMem(IN HANDLE hProcess, IN PPLAYER pplayer) 
{
	return ReadProcessMemory(hProcess, pplayer->health->address, &pplayer->health->value, sizeof(pplayer->health->value), NULL);
}


BOOL WriteMem(IN HANDLE hProcess, IN PPLAYER pplayer)
{
	return WriteProcessMemory(hProcess, pplayer->health->address, &pplayer->health->value, sizeof(pplayer->health->value), NULL);
}

BOOL GetModuleBaseAddress(IN DWORD process_id, IN const wchar_t* module_name, OUT PVOID* pbaseAddress)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	MODULEENTRY32 module_entity;
	module_entity.dwSize = sizeof(module_entity);

	if (Module32First(hSnapshot, &module_entity))
	{
		do {

			if (!_wcsicmp(module_entity.szModule, module_name))
			{
				*pbaseAddress = module_entity.modBaseAddr;
				return TRUE;
			}

		} while (Module32Next(hSnapshot, &module_entity));
	}
	CloseHandle(hSnapshot);
	return FALSE;
}

BOOL FindDMAAddy(IN HANDLE hProcess, IN PVOID init_ptr, IN std::vector<PVOID> offsets, OUT PVOID* pbaseAddress)
{
	PVOID address = init_ptr;

	for (int i = 0; i < offsets.size(); ++i)
	{
		if ((ReadProcessMemory(hProcess, address, &address, sizeof(address), NULL)) == FALSE)
		{
			return FALSE;
		}

		address = reinterpret_cast<PVOID>(reinterpret_cast<uintptr_t>(address) + (uintptr_t)offsets[i]);

	}

	*pbaseAddress = address;
	return TRUE;

}