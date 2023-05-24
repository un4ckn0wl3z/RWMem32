#include <locale>
#include <codecvt>
#include "Common.hpp"

int main()
{
	PVOID modulebase_address = NULL;
	PPLAYER player = new PLAYER();
	player->health = new PLAYER_DATA();
	DWORD process_id = NULL;
	const wchar_t* process_name = L"ac_client.exe";

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string process_name_utf8 = converter.to_bytes(process_name);

	player->health->offsets = new std::vector<PVOID>();
	player->health->offsets->push_back(reinterpret_cast<PVOID>(0xEC));

	if (!GetProcId(process_name, &process_id))
	{
		std::cout << "[!] GetProcId failed with error code: " << GetLastError() << std::endl;
		system("pause");
		return 0;
	}
	std::cout << "[+] Found " << process_name_utf8 << "'s ID: " << process_id << std::endl;


	if (!GetModuleBaseAddress(process_id, process_name, &modulebase_address))
	{
		std::cout << "[!] GetModuleBaseAddress failed with error code: " << GetLastError() << std::endl;
		system("pause");
		return 0;
	}

	std::cout << "[+] Found " << process_name_utf8 << "'s Module base address: " << modulebase_address << std::endl;




	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
	if (hProcess == NULL)
	{
		std::cout << "[!] OpenProcess failed with error code: " << GetLastError() << std::endl;
		system("pause");
		return 0;
	}

	player->health->address = reinterpret_cast<PVOID>(reinterpret_cast<uintptr_t>(modulebase_address) + 0x0017E0A8);
	std::cout << "[+] Dynamic Health address: " << player->health->address << std::endl;

	if (!FindDMAAddy(hProcess, player->health->address, *player->health->offsets, &player->health->address))
	{
		std::cout << "[!] FindDMAAddy failed with error code: " << GetLastError() << std::endl;
		system("pause");
		return 0;
	}


	if ((ReadMem(hProcess, player)) == FALSE)
	{
		std::cout << "[!] ReadProcessMemory failed with error code: " << GetLastError() << std::endl;
		system("pause");
		return 0;
	};
	std::cout << "[+] Current Player Health: " << player->health->value << std::endl;

	std::cout << "Press any key to setting health to 1337" << std::endl;
	system("pause");

	player->health->value = 1337;

	if ((WriteMem(hProcess, player)) == FALSE)
	{
		std::cout << "[!] WriteProcessMemory failed with error code: " << GetLastError() << std::endl;
		system("pause");
		return 0;
	};

	delete player->health->offsets;
	delete player->health;
	delete player;
	CloseHandle(hProcess);

	system("pause");
	return 0;
}
