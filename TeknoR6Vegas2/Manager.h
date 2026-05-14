#pragma once
#include <windows.h>
#include "PE.h"
#include "memory.h"
#include <time.h>
#include "IniConfig.h"

#include <tlhelp32.h> //Added for GetModuleBaseAddress
#include <string>
#include <cstring>
#include <functional>

#define NUM(a) (sizeof(a) / sizeof(*a))
#define Roundby1000(a) (a / 0x1000 + ((a % 0x1000) > 0 ? 1 : 0)) * 0x1000


class ModManager {
public:
	ModManager();
	~ModManager();

	void Init();
	bool LoadProcess(LPCSTR Filename);
	int RunTo(DWORD Address, DWORD Mode, DWORD Eip);
	uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName); 
	static ModManager& GetInstance();

	void SetOtherMods(bool internet, bool sound, bool graphics, bool ammo, bool gadgets);
	void StartProcess(bool start);
	void ModifyMemory();

	std::string GetMapName(int index);
	void SetPreferences(int index, int value);

	void SetGameMode(System::String^ mode);
	void SetServer(bool s, System::String^ name, System::String^ pwd);
	void SetRespawn(System::String^ val);
	void SetDifficulty(System::String^ diff);
	void SetTimeLimit(int val);
	void SetSpawnRate(System::Object^ val);
	void SetTerrorCount(System::Object^ val);
	void SetMaxPlayers(int val);
	void SetMap(int map);
	void SetReadyUp(bool val);
	void SetCycleMaps(bool cycle);
	
	//Message logging system for sending messages to the UI from this class without coupling
	typedef void (*LogFn)(void* ctx, const char* msg);
	void SetLogCallback(LogFn fn, void* ctx);
	void Log(const std::string& msg);

private:
	_CONTEXT mycontext;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	
	IniConfig* m_pIniConfig;

	LogFn  m_logFn;
	void* m_logCtx;

	bool m_bStartProcess;
	bool m_bIsHost;
	bool m_bDefaultPlayers;
	bool m_bDefaultTerrorCount;
	bool m_bCycleMaps;
	std::string m_sServerName;
	std::string m_sServerPassword;
	std::string m_sGameMode;

	int m_iMaxPlayers;
	int m_iTerrorCount;
	int m_iSpawnRate;
	int m_iTimeLimit;
	int m_iDifficulty;
	int m_iRespawnCount;
	std::string m_sCurrentMap;
	int m_iTimeBetween;
	//std::vector<std::string> m_vMapList;
};