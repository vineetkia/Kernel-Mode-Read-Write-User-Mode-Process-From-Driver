#include <vector>
#include <iomanip>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include "KeInterface.h"

class BlueMethMem
{
public:
	 BlueMethMem();
	~BlueMethMem();

	template <class val>
	val readMem(uintptr_t addr)
	{
		val x;
		ReadProcessMemory(handle, (LPBYTE*)addr, &x, sizeof(x), NULL);
		return x;
	}

	template <class val>
	val WMemory(uintptr_t addr, val x)
	{
		WriteProcessMemory(handle, (LPBYTE*)addr, &x, sizeof(x), NULL);
		return x;
	}

	uintptr_t getProcess(const wchar_t*);
	uintptr_t getModule(uintptr_t, const wchar_t*);
	uintptr_t getAddress(uintptr_t, std::vector<uintptr_t>);

private:
	HANDLE handle;
};

BlueMethMem::BlueMethMem() { handle = NULL; }

BlueMethMem::~BlueMethMem()
{
	CloseHandle(handle);
}

uintptr_t BlueMethMem::getProcess(const wchar_t* proc)
{
	HANDLE hProcessId = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	uintptr_t process;
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	do
	{
		if (!_wcsicmp(pEntry.szExeFile, proc))
		{
			process = pEntry.th32ProcessID;
			CloseHandle(hProcessId);
			handle = OpenProcess(PROCESS_ALL_ACCESS, false, process);
		}
	} while (Process32Next(hProcessId, &pEntry));
	return process;
}

uintptr_t BlueMethMem::getModule(uintptr_t procId, const wchar_t* modName)
{
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);
	do
	{
		if (!_wcsicmp(mEntry.szModule, modName))
		{
			CloseHandle(hModule);
			return (uintptr_t)mEntry.hModule;
		}
	} while (Module32Next(hModule, &mEntry));
	return 0;
}

uintptr_t BlueMethMem::getAddress(uintptr_t addr, std::vector<uintptr_t> vect)
{
	for (int i = 0; i < vect.size(); i++)
	{
		ReadProcessMemory(handle, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += vect[i];
	}
	return addr;
}

BlueMethMem MethClass;

struct offset
{
	DWORD m_iTeamNum = 0xF4;
	DWORD m_iGlowIndex = 0xA40C;
	DWORD dwEntityList = 0x4D07DD4;
	DWORD dwLocalPlayer = 0xCF5A4C;
	DWORD dwGlowObjectManager = 0x5248228;
}offset;

struct variables
{
	DWORD localPlayer;
	DWORD gameModule;
}val;

int main()
{
	std::string UserMain = "HEiSENBERG", Passwd = "spacex", USER, PASS; int LL = 0;
	std::cout << "[-------------------   [HEiSENBERG.IO]   ------------------]\n";
	std::cout << "[------ ENTER YOUR LOGiN CREDENTiALS [HEiSENBERG.ME] ------]\n";
	std::cout << "ENTER YOUR USERNAME: >>> "; std::cin >> USER;
	std::cout << "ENTER YOUR PASSWORD: >>> "; std::cin >> PASS;

	MessageBoxA(0, "HEiSENBERG WiLL NOW INJECT BLUE METH IN YOUR RAM", "HEiSENBERG.ME", 0);
	MessageBoxA(0, "[SUCCESS] - FEEL THE GAMESENSE", "HEiSENBERG.ME", 0);
	MessageBoxA(0, "[1] DiSCORD - HEiSENBERG#0487\n[2] STEAM - https://steamcommunity.com/id/-heisen-official/", "CONTACT", 0);
	printf("\n\n[USAGE]\n[1]-ViSHNU-THiRD-EYE-ESP  [INSERT-KEY]\n[2]-EXiT-UNHOOK [END-KEY]\n");

	KeInterface Driver("\\\\.\\kernelhop");
	DWORD ProcessId = MethClass.getProcess(L"csgo.exe");
	DWORD ClientAddress = MethClass.getModule(ProcessId, L"client_panorama.dll");

	std::cout << "\n[1] Process ID: " << ProcessId << std::endl;
	std::cout << "\n[2] Client_Panorama.dll Address: 0x" << std::uppercase<< std::hex << ClientAddress << std::endl;
	bool KEY_TOG = false;
	while (true)
	{
		DWORD glowObjectManager = Driver.ReadVirtualMemory<DWORD>(ProcessId, ClientAddress + offset.dwGlowObjectManager, sizeof(ULONG));
		int glowObjectCount = Driver.ReadVirtualMemory<int>(ProcessId, ClientAddress + offset.dwGlowObjectManager + 0x4, sizeof(int));

		for (short int i = 0; i < 60; i++)
		{
			int currentPlayer = Driver.ReadVirtualMemory<int>(ProcessId, ClientAddress + offset.dwEntityList + (i * 16), sizeof(int));
			int glowIndex = Driver.ReadVirtualMemory<int>(ProcessId, currentPlayer + offset.m_iGlowIndex, sizeof(int));
			int team = Driver.ReadVirtualMemory<int>(ProcessId, currentPlayer + offset.m_iTeamNum, sizeof(int));
			int glowAdress = glowObjectManager + (glowIndex * 56);

			float coloronf = 1.0f;ULONG coloron = *((unsigned long*)& coloronf);
			float colorofff = 0.0f;ULONG coloroff = *((unsigned long*)& colorofff);
			if (KEY_TOG && glowObjectManager != NULL)
			{
				if (team == 2)
				{
					Driver.WriteVirtualMemory(ProcessId, glowAdress + 0x4, coloroff, sizeof(float));
					Driver.WriteVirtualMemory(ProcessId, glowAdress + 0x8, coloron, sizeof(ULONG));
					Driver.WriteVirtualMemory(ProcessId, glowAdress + 0xC, coloroff, sizeof(float));
					Driver.WriteVirtualMemory(ProcessId, glowAdress + 0x10, coloron, sizeof(float));

				}
				if (team == 3)
				{
					Driver.WriteVirtualMemory(ProcessId, glowAdress + 0x4, coloron, sizeof(float));
					Driver.WriteVirtualMemory(ProcessId, glowAdress + 0x8, coloroff, sizeof(float));
					Driver.WriteVirtualMemory(ProcessId, glowAdress + 0xC, coloroff, sizeof(float));
					Driver.WriteVirtualMemory(ProcessId, glowAdress + 0x10, coloron, sizeof(float));
				}
				Driver.WriteVirtualMemory(ProcessId, glowAdress + 0x24, true, sizeof(BOOL));
				Driver.WriteVirtualMemory(ProcessId, glowAdress + 0x25, false, sizeof(BOOL));
			}
		}
		if (GetAsyncKeyState(VK_INSERT))
		{
			if (KEY_TOG == false)
			{
				Sleep(250);
				KEY_TOG = true;
			}
			else if (KEY_TOG == true)
			{
				Sleep(250);
				KEY_TOG = false;
			}
		}
		if (GetAsyncKeyState(VK_END))
		{
			exit(0);
		}
		Sleep(1);
	}
	return 0;
}