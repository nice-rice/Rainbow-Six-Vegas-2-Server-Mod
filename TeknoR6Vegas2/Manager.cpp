#pragma once
#include "Manager.h"
#include <msclr\marshal_cppstd.h>

ModManager::ModManager() {

	m_pIniConfig = new IniConfig;

	m_bStartProcess = false;
	m_sCurrentMap = "Mb01_Import";

	m_bIsHost = true;
	m_sServerName = "Default Server";
	m_sServerPassword = "";

	m_iMaxPlayers = 0;
	m_iTerrorCount = 0;
	m_iSpawnRate = 0;
	m_iTimeLimit = 0;
	m_iDifficulty = 2;
	m_iRespawnCount = 1;
	m_iTimeBetween = 60;

}

ModManager::~ModManager() {
	delete m_pIniConfig;
}
ModManager& ModManager::GetInstance() {
	static ModManager instance;
	return instance;
}
void ModManager::Init() {
	bool ret = false;

	while (!ret) {
		if (m_bStartProcess) {
			ModifyMemory();
			m_bStartProcess = false;
		}
		else {
			Sleep(100);
		}
	}

}
void ModManager::StartProcess(bool start) {
	m_bStartProcess = start;


}
void ModManager::SetOtherMods(bool internet, bool sound, bool graphics, bool ammo, bool gadgets) {
	m_pIniConfig->SetGadgets(gadgets);
	m_pIniConfig->SetWeapons(ammo);

	//Ordering is currently important, otherwise they overwrite each other
	//If grapgics is set off, do the back up restore first
	if (!graphics)
		m_pIniConfig->SetGraphics(graphics);
	if (!internet) {
		m_pIniConfig->SetInternet(internet);
		m_pIniConfig->SetSound(sound);
	}
	else {
		m_pIniConfig->SetSound(sound);
		m_pIniConfig->SetInternet(internet);
	}
	//Check again if graphics is on and skipped earlier
	if (graphics)
		m_pIniConfig->SetGraphics(graphics);
}
bool ModManager::LoadProcess(LPCSTR Filename) {
	if (!m_bIsHost) {
		if (!CreateProcess(Filename, // No module name (use command line). 
			NULL,			  // Command line.
			NULL,             // Process handle not inheritable. 
			NULL,             // Thread handle not inheritable. 
			FALSE,            // Set handle inheritance to FALSE. 
			CREATE_SUSPENDED | CREATE_NEW_PROCESS_GROUP, // suspended creation flags. 
			NULL,             // Use parent's environment block. 
			NULL,             // Use parent's starting directory. 
			&si,              // Pointer to STARTUPINFO structure.
			&pi)             // Pointer to PROCESS_INFORMATION structure.
			)
		{
			return false;
		}
		mycontext.ContextFlags = 0x00010000 + 1 + 2 + 4 + 8 + 0x10;
		return true;

	}
	// Mutable string required by CreateProcess
	std::string commandLine = "../Binaries/RainbowSixVegas2_SADS.exe engine.servercommandlet " +
		m_sCurrentMap + "?AgO=0?AgU=" +
		m_sServerName + "?AgP=?SrvOptionFile=R6VegasServerConfig?PW=" +
		m_sServerPassword + "?GAME=R6Game.R6" +
		m_sGameMode + "Game?GD=" +
		std::to_string(m_iDifficulty) + "?BT=30?TBR=" +
		std::to_string(m_iTimeBetween) + "?RD=" +
		std::to_string(m_iTimeLimit) + "?";
	char* cmdLineMutable = &commandLine[0];

	if (!CreateProcess(NULL, // No module name (use command line). 
		cmdLineMutable,			  // Command line.
		NULL,             // Process handle not inheritable. 
		NULL,             // Thread handle not inheritable. 
		FALSE,            // Set handle inheritance to FALSE. 
		CREATE_SUSPENDED | CREATE_NEW_PROCESS_GROUP, // suspended creation flags. 
		NULL,             // Use parent's environment block. 
		NULL,             // Use parent's starting directory. 
		&si,              // Pointer to STARTUPINFO structure.
		&pi)             // Pointer to PROCESS_INFORMATION structure.
		)
	{
		return false;
	}
	mycontext.ContextFlags = 0x00010000 + 1 + 2 + 4 + 8 + 0x10;
	return true;
}

int ModManager::RunTo(DWORD Address, DWORD Mode, DWORD Eip) {

	char tempbuf[4];
	int count = 0;
	if (Eip != 0)
	{
		GetThreadContext(pi.hThread, &mycontext);
		mycontext.Eip = Eip;
		SetThreadContext(pi.hThread, &mycontext);
	}
	ReadProcessMemory(pi.hProcess, (LPVOID)Address, tempbuf, 4, 0);
	WriteProcessMemory(pi.hProcess, (LPVOID)Address, "\xEB\xFE", 2, 0);
	ResumeThread(pi.hThread);
	while (1)
	{
		if (count == 10) return 0;
		if (!GetThreadContext(pi.hThread, &mycontext)) count++;
		else count = 0;
		if (Mode == 0) Sleep(500);
		if (Mode == 1) WriteProcessMemory(pi.hProcess, (LPVOID)Address, "\xEB\xFE", 2, 0);
		if (mycontext.Eip == Address) break;
	}
	SuspendThread(pi.hThread);
	if (!GetThreadContext(pi.hThread, &mycontext)) return 0;
	WriteProcessMemory(pi.hProcess, (LPVOID)Address, tempbuf, 4, 0);
	return 1;
}



void ModManager::ModifyMemory() {
	LPCSTR filename;
	PEStruct FilePEFile;
	if (m_bIsHost) {
		filename = "../Binaries/RainbowSixVegas2_SADS.exe";
		FilePEFile = getPEFileInformation("../Binaries/RainbowSixVegas2_SADS.exe");
	}
	else {
		filename = "../Binaries/R6Vegas2_Game.exe";
		FilePEFile = getPEFileInformation("../Binaries/R6Vegas2_Game.exe");
	}

	if (!LoadProcess(filename))
	{
		//WriteLog("Unable to create process for R6Vegas2_Game.exe");
		return;
	}
	//else WriteLog("Created process for R6Vegas2_Game.exe");
	DWORD OEP = FilePEFile.image_nt_headers.OptionalHeader.AddressOfEntryPoint + FilePEFile.image_nt_headers.OptionalHeader.ImageBase;
	if (!RunTo(OEP, 1, 0)) {
		//	WriteLog("Process crashed on init");
		return;
	}


	//else WriteLog("Process initialized");
	DWORD codesize = Roundby1000(FilePEFile.image_section_header[0].Misc.VirtualSize);
	DWORD codebase = FilePEFile.image_section_header[0].VirtualAddress + FilePEFile.image_nt_headers.OptionalHeader.ImageBase;
	DWORD serverBase = FilePEFile.image_nt_headers.OptionalHeader.ImageBase;
	DWORD oldprot = 0;
	DWORD newprot = 0;
	VirtualProtectEx(pi.hProcess, (LPVOID)codebase, codesize, 0x40, &oldprot);

	if (m_bIsHost)
	{
		VirtualProtectEx(pi.hProcess, (LPVOID)codebase, codesize, 0x40, &oldprot);
		// Player Cap
		if (!m_bDefaultPlayers)
		{
			DWORD myval = m_iMaxPlayers;


			//DWORD patchOffset = 0x3EFCD6;
			//DWORD patchAddress = serverBase + patchOffset;
			//overwrites extra compare ??
			//WriteProcessMemory(pi.hProcess, (LPVOID)patchAddress, "\x83\xF8\x10\x90\x90\x90", 6, 0);

			//Update state count (start and death)
			WriteProcessMemory(pi.hProcess, (LPVOID)0x113B7B39, "\x90\x90\x90\x90\x90\x90\x90", 7, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x113BA3B0, "\x90\x90\x90\x90\x90\x90", 6, 0);

			// Group 2 - SADS equivalents
			WriteProcessMemory(pi.hProcess, (LPVOID)0x11162300, "\x90\x90\x90\x90\x90\x90\x90", 7, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x11162220, "\x90\x90\x90\x90\x90\x90", 6, 0);

			// Group 3 - SADS equivalents
			WriteProcessMemory(pi.hProcess, (LPVOID)0x113DE841, "\xBA\x01\x00\x00\x00\x90", 6, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x113DE129, "\xBA\x01\x00\x00\x00\x90", 6, 0);


			WriteProcessMemory(pi.hProcess, (LPVOID)0x10d4d7f2, "\x90\x90", 2, 0);
			// Player count #1 here
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10d4d7f4, "\xBE\x00\x00\x00\x00", 5, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10d4d7f5, &myval, 4, 0);
			// Player count #2 here

			WriteProcessMemory(pi.hProcess, (LPVOID)0x10d679c1, "\xC7\x46\x14\x10\x00\x00\x00\x8B\x7E\x24\xEB\x1D", 12, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10d679c4, &myval, 4, 0);
			//
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10d679e6, "\xEB\xD8\x90", 3, 0);
			// Player count #3 here
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10d63691, "\x83\x7E\x18\x02\x75\x47\xC7\x46\x14\x10\x00\x00\x00\xEB\x18", 15, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10d6369a, &myval, 4, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10d636b6, "\xEB\xD9\x83\x7E\x04\x00\x90", 7, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10d636de, "\xEB\x4D", 2, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10d6372d, "\xEB\x42", 2, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10d63771, "\x89\x46\x14\xE9\x3F\xFF\xFF\xFF", 8, 0);
		}
		// Terrorist Count
		if (!m_bDefaultTerrorCount)
		{
			DWORD myval = System::Convert::ToInt32(m_iTerrorCount);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10A2B38C, "\xEB\x45\x90\x90", 4, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10A2B3D3, "\xB8\x00\x00\x00\x00\xEB\xB6", 7, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10A2B3D4, &myval, 4, 0);
		}
	}
	else {
		// Player Cap

		if (!m_bDefaultPlayers)
		{


			DWORD myval = m_iMaxPlayers;
			//Update state count (start and death)
			WriteProcessMemory(pi.hProcess, (LPVOID)0x113B7DB9, "\x90\x90\x90\x90\x90\x90\x90", 7, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x113BA630, "\x90\x90\x90\x90\x90\x90", 6, 0);
			//Networking count
			WriteProcessMemory(pi.hProcess, (LPVOID)0x11162580, "\x90\x90\x90\x90\x90\x90\x90", 7, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x111624A0, "\x90\x90\x90\x90\x90\x90", 6, 0);
			//Skip check
			WriteProcessMemory(pi.hProcess, (LPVOID)0x113DEAC1, "\xBA\x01\x00\x00\x00\x90", 6, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x113DE3A9, "\xBA\x01\x00\x00\x00\x90", 6, 0);


			WriteProcessMemory(pi.hProcess, (LPVOID)0x10D4DA72, "\x90\x90", 2, 0);
			// Player count #1 here
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10D4DA74, "\xBE\x00\x00\x00\x00", 5, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10D4DA75, &myval, 4, 0);
			// Player count #2 here
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10D67C41, "\xC7\x46\x14\x10\x00\x00\x00\x8B\x7E\x24\xEB\x1D", 12, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10D67C44, &myval, 4, 0);
			//
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10D67C67, "\xEB\xD8\x90", 3, 0);
			// Player count #3 here
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10D63911, "\x83\x7E\x18\x02\x75\x47\xC7\x46\x14\x10\x00\x00\x00\xEB\x18", 15, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10D6391A, &myval, 4, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10D63936, "\xEB\xD9\x83\x7E\x04\x00\x90", 7, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10D6395E, "\xEB\x4D", 2, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10D639AD, "\xEB\x42", 2, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10D639F1, "\x89\x46\x14\xE9\x3F\xFF\xFF\xFF", 8, 0);
		}
		// Terrorist Count
		if (!m_bDefaultTerrorCount) {
			DWORD myval = System::Convert::ToInt32(m_iTerrorCount);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10A2B60C, "\xEB\x45\x90\x90", 4, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10A2B653, "\xB8\x00\x00\x00\x00\xEB\xB6", 7, 0);
			WriteProcessMemory(pi.hProcess, (LPVOID)0x10A2B654, &myval, 4, 0);
		}


	}


	VirtualProtectEx(pi.hProcess, (LPVOID)codebase, codesize, oldprot, &newprot);
	ResumeThread(pi.hThread);
	WaitForSingleObject(pi.hThread, INFINITE);
	while (GetThreadContext(pi.hThread, &mycontext))
		Sleep(1000);
	CloseHandle(pi.hThread);
	for (unsigned int i = 0; i < FilePEFile.numOfSecs; i++) {
		free(FilePEFile.image_section[i]);
		FilePEFile.image_section[i] = NULL;
	}
	free(FilePEFile.fileImage);
	FilePEFile.fileImage = NULL;
	//WriteLog("Process exited");

}

std::string ModManager::GetMapName(int index) {

	return m_pIniConfig->g_aMapList[index];
}
void ModManager::SetGameMode(System::String^ mode) {
	msclr::interop::marshal_context context;
	m_sGameMode = context.marshal_as<std::string>(mode);
}
void ModManager::SetServer(bool s, System::String^ name, System::String^ pwd) {
	m_bIsHost = s;
	msclr::interop::marshal_context context;
	m_sServerName = context.marshal_as<std::string>(name);
	m_sServerPassword = context.marshal_as<std::string>(pwd);
	m_pIniConfig->SetServer(m_sServerName, m_sServerPassword);
}
void ModManager::SetRespawn(System::String^ val) {
	bool respawn_on = true;
	if (val == "None") {
		m_iRespawnCount = 1;
		respawn_on = false;
	}
	else if (val == "Unlimited") {
		m_iRespawnCount = 0;
	}
	else {
		m_iRespawnCount = System::Convert::ToInt32(val) + 1;
	}
	m_pIniConfig->SetRespawn(respawn_on, m_iRespawnCount);

}
void ModManager::SetDifficulty(System::String^ diff) {
	msclr::interop::marshal_context context;
	std::string difficulty_name = context.marshal_as<std::string>(diff);
	if (difficulty_name == "Realistic") { m_iDifficulty = 3; }
	else if (difficulty_name == "Normal") { m_iDifficulty = 2; }
	else if (difficulty_name == "Easy") { m_iDifficulty = 1; }
	else { m_iDifficulty = 3; }

}
void ModManager::SetTimeLimit(int val) {
	m_iTimeLimit = val * 60;
	m_pIniConfig->SetTimeLimit(m_iTimeLimit);

}
void ModManager::SetSpawnRate(System::Object^ val) {
	if (val != "Default") {
		m_iSpawnRate = System::Convert::ToInt32(val);
	}
	else {
		m_iSpawnRate = 8;
	}
	m_pIniConfig->SetSpawnRate(m_iSpawnRate);

}
void ModManager::SetTerrorCount(System::Object^ val) {
	if (val != "Default") {
		m_bDefaultTerrorCount = false;
		m_iTerrorCount = System::Convert::ToInt32(val);
	}
	else {
		m_bDefaultTerrorCount = true;
	}
}
void ModManager::SetMaxPlayers(int val) {
	if (val == 0) {
		m_bDefaultPlayers = true;
	}
	else {
		m_bDefaultPlayers = false;
		m_iMaxPlayers = val;
	}
}
void ModManager::SetMap(int map) {

	m_sCurrentMap = GetMapName(map);
	m_pIniConfig->SetMap(map);

}
void ModManager::SetReadyUp(bool val) {
	if (val) {
		m_iTimeBetween = 0;
	}
	else
		m_iTimeBetween = 60;
}
void ModManager::SetPreferences(int index, int value) {
	m_pIniConfig->SavePreferences(index, value);
}