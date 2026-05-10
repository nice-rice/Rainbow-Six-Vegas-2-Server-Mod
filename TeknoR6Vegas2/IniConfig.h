#pragma once
#include <windows.h>

#include <string>
#include <cstring>

#define NUM(a) (sizeof(a) / sizeof(*a))
#define Roundby1000(a) (a / 0x1000 + ((a % 0x1000) > 0 ? 1 : 0)) * 0x1000



class IniConfig {
public:
	IniConfig();
	~IniConfig();
	
	bool WriteINI(LPCSTR file, LPCSTR section, LPCSTR key, LPCSTR value);
	void RestoreBackup();
	
	void SavePreferences(int index, int value);
	void SetServer(std::string name, std::string key);
	void SetInternet(bool set);
	void SetSound(bool set);
	void SetWeapons(bool set);
	void SetGadgets(bool set);
	void SetGraphics(bool set);

	void SetMap(int map);
	void SetRespawn(bool on, int count);
	void SetSpawnRate(int count);
	void SetTimeLimit(int time);

	const std::string g_aMapList[17] = {
"Mb01_Import",
"Mb02_Dantes_01",
"Mb03_Neon_01",
"Mb04_Oil",
"Mb05_Chinese",
"Mb06_Trainyard",
"Mb07_Penthouse_01",
"Mb08_Conven_01",
"Mb_09_Murdertown",
"Mb10_Training_01",
"Mb11_Streets_01_linkapp",
"Mb12_KillHouse",
"Mb13_Presidio",
"DLC_MP_Casino_01",
"DLC_Murdertown_2",
"DLC_Training_02",
"MB14_Convent_MLG"
	};

	const std::string g_aWeaponList[37] = {
		"ConfigR6SubMP5N", "ConfigR6SubMP7A1", "ConfigR6SubUMP45", "ConfigR6SubP90",
		"ConfigR6SubMP9","ConfigR6SubMAC11","ConfigR6SubVZ83","ConfigR6SubType05JS","ConfigR6SubAUGPara", //9
		
		"ConfigR6AssaultSCARHCQC","ConfigR6AssaultM8","ConfigR6AssaultAUGA3","ConfigR6AssaultG3KA4","ConfigR6Assault552Commando",
		"ConfigR6AssaultG36C","ConfigR6AssaultMTAR21","ConfigR6AssaultFamas","ConfigR6AssaultAK47","ConfigR6AssaultM468",
		"ConfigR6AssaultFNC","ConfigR6AssaultL85A2","ConfigR6AssaultAKS74U", //13
		
		"ConfigR6LMGMK46","ConfigR6LMG21E","ConfigR6LMGM249SPW","ConfigR6LMGMG36", //4
		
		"ConfigR6ShotgunM3","ConfigR6ShotgunSpas12","ConfigR6Shotgun870MCS","ConfigR6ShotgunXM26LSS","ConfigR6Shotgun500Tactical", //5

		"ConfigR6SniperPSG1","ConfigR6SniperM40A1","ConfigR6SniperSV98","ConfigR6SniperScoutTactical","ConfigR6SniperSR25SD", //6
		"ConfigR6SniperL96ArticWarfare"
	};
};