#pragma once
#include <Windows.h>
#include <TlHelp32.h>

typedef struct _PLAYER_DATA {

	PVOID address;
	DWORD value;

} PLAYER_DATA, * PPLAYER_DATA;

typedef struct _PLAYER
{
	PPLAYER_DATA health;

} PLAYER, * PPLAYER;


BOOL GetProcId(IN const wchar_t* process_name, OUT DWORD* pprocess_id);
BOOL ReadMem(IN HANDLE hProcess, IN PPLAYER pplayer);
BOOL WriteMem(IN HANDLE hProcess, IN PPLAYER pplayer);
BOOL GetModuleBaseAddress(IN DWORD process_id, IN const wchar_t* module_name, OUT PVOID* pbaseAddress);