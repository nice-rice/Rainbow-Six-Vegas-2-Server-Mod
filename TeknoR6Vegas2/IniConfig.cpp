#include "IniConfig.h"

IniConfig::IniConfig() {
	
	//Create Back ups of any altered config files
	//Doesn't overwrite once copied
	CreateDirectory("../BackupConfig/",NULL);
	CopyFile("../KellerGame/Config/PCKellerEngine.ini", "../BackupConfig/PCKellerEngine.ini", true);
	CopyFile("../KellerGame/Config/DefaultEngine.ini", "../BackupConfig/DefaultEngine.ini", true);
	CopyFile("../Engine/Config/BaseEngine.ini", "../BackupConfig/BaseEngine.ini", true);
	CopyFile("../KellerGame/Config/DefaultWeaponsConfig.ini", "../BackupConfig/DefaultWeaponsConfig.ini", true);
	CopyFile("../KellerGame/Config/DefaultGadgetsConfig.ini", "../BackupConfig/DefaultGadgetsConfig.ini", true);
	CopyFile("../KellerGame/Config/PC/R6VegasServerConfig.ini", "../BackupConfig/R6VegasServerConfig.ini", true);
	CopyFile("../KellerGame/Config/PlatformSpecificConfigPC.ini", "../BackupConfig/PlatformSpecificConfigPC.ini", true);
	
}
IniConfig::~IniConfig() {}

void IniConfig::RestoreBackup() {
	CopyFile("../BackupConfig/PCKellerEngine.ini", "../KellerGame/Config/PCKellerEngine.ini", false);
	CopyFile("../BackupConfig/DefaultEngine.ini", "../KellerGame/Config/DefaultEngine.ini", false);
	CopyFile("../BackupConfig/BaseEngine.ini", "../Engine/Config/BaseEngine.ini", false);
	CopyFile("../BackupConfig/DefaultWeaponsConfig.ini", "../KellerGame/Config/DefaultWeaponsConfig.ini", false);
	CopyFile("../BackupConfig/DefaultGadgetsConfig.ini", "../KellerGame/Config/DefaultGadgetsConfig.ini", false);
	CopyFile("../BackupConfig/R6VegasServerConfig.ini", "../KellerGame/Config/PC/R6VegasServerConfig.ini", false);
	CopyFile("../BackupConfig/PlatformSpecificConfigPC.ini", "../KellerGame/Config/PlatformSpecificConfigPC.ini", false);

}

void IniConfig::SetInternet(bool set) {
	if (!set) {
		CopyFile("../BackupConfig/BaseEngine.ini", "../Engine/Config/BaseEngine.ini", false);
		CopyFile( "../BackupConfig/PCKellerEngine.ini", "../KellerGame/Config/PCKellerEngine.ini", false);
		CopyFile("../BackupConfig/DefaultEngine.ini", "../KellerGame/Config/DefaultEngine.ini", false);
	}
	else {
		LPCSTR filename = "../KellerGame/Config/PCKellerEngine.ini";
		for (int i = 0; i < 3; ++i) {

			LPCSTR section = "Engine.Player";
			WriteINI(filename, section, "ConfiguredInternetSpeed", "500000");
			WriteINI(filename, section, "ConfiguredLanSpeed", "500000");

			section = "IpDrv.TcpNetDriver";
			WriteINI(filename, section, "AllowDownloads", "True");
			WriteINI(filename, section, "EnableTimeout", "True");
			WriteINI(filename, section, "ConnectionTimeout", "30.0");
			WriteINI(filename, section, "InitialConnectTimeout", "30.0");

			WriteINI(filename, section, "AckTimeout", "0.5");
			WriteINI(filename, section, "KeepAliveTime", "0.1");
			WriteINI(filename, section, "MaxClientRate", "100000");
			WriteINI(filename, section, "MaxInternetClientRate", "100000");

			WriteINI(filename, section, "RelevantTimeout", "10.0");
			WriteINI(filename, section, "SpawnPrioritySeconds", "0.5");
			WriteINI(filename, section, "ServerTravelPause", "10.0");
			WriteINI(filename, section, "NetServerMaxTickRate", "60");
			WriteINI(filename, section, "LanServerMaxTickRate", "60");

			//Repeat for BaseEngine and Default engine file
			if(i ==1)
				filename = "../KellerGame/Config/DefaultEngine.ini";
			else
				filename = "../Engine/Config/BaseEngine.ini";
		}
	}
}

void IniConfig::SetSound(bool set) {
	if (!set) {
		CopyFile("../BackupConfig/BaseEngine.ini", "../Engine/Config/BaseEngine.ini", false);
		CopyFile("../BackupConfig/PCKellerEngine.ini", "../KellerGame/Config/PCKellerEngine.ini", false);
		CopyFile("../BackupConfig/DefaultEngine.ini", "../KellerGame/Config/DefaultEngine.ini", false);
	}
	else{
		LPCSTR filename = "../KellerGame/Config/PCKellerEngine.ini";
		LPCSTR section = "ALAudio.ALAudioDevice";

		for (int i = 0; i < 3; ++i) {
			WriteINI(filename, section, "MaxChannels", "32"); //Too high may cause issues with sounds not flagging off
			WriteINI(filename, section, "TimeBetweenHWUpdates", "10"); //Same with updates too frequent
			if (i == 1)
				filename = "../KellerGame/Config/DefaultEngine.ini";
			else
				filename = "../Engine/Config/BaseEngine.ini";
		}
	}
}

void IniConfig::SetWeapons(bool set) {
	if (!set) {
		CopyFile("../BackupConfig/DefaultWeaponsConfig.ini", "../KellerGame/Config/DefaultWeaponsConfig.ini", false);
	}
	else {
		LPCSTR filename = "../KellerGame/Config/DefaultWeaponsConfig.ini";
		LPCSTR key = "m_iDefaultAmmunitionRealistic";
		LPCSTR value = "500"; //Sets all ammo to 500 rounds
		LPCSTR section = "";

		for (int i = 0; i < NUM(g_aWeaponList); ++i) {
			std::string header = "R6Game." + g_aWeaponList[i];
			section = header.c_str();
			WriteINI(filename, section, key, value);
		}
	}
}

void IniConfig::SetGadgets(bool set) {
	if (!set) {
		CopyFile("../BackupConfig/DefaultGadgetsConfig.ini", "../KellerGame/Config/DefaultGadgetsConfig.ini", false);
	}
	else {
		LPCSTR filename = "../KellerGame/Config/DefaultGadgetsConfig.ini";
		LPCSTR a_section[8] = { "R6Game.R6GadgetGrenadeFlashbang","R6Game.R6GadgetGrenadeFrag",
								"R6Game.R6GadgetGrenadeIncendiary","R6Game.R6GadgetGrenadeMolotov",
								"R6Game.R6GadgetGrenadeSmoke", "R6Game.R6GadgetGrenadeTearGas",
								"R6Game.R6GadgetExplosiveBreaching","R6Game.R6GadgetExplosiveDemolition"
		};
		LPCSTR key = "m_iMaxAmmoRealistic";
		LPCSTR value = "9"; //Sets all gadgets to 9 items regardless of difficulty

		for (int i = 0; i < NUM(a_section); ++i) {
			WriteINI(filename, a_section[i], key, value);
		}
	}
}
void IniConfig::SetGraphics(bool set) {
	if (!set) {
		CopyFile("../BackupConfig/BaseEngine.ini", "../Engine/Config/BaseEngine.ini", false);
		CopyFile("../BackupConfig/PCKellerEngine.ini", "../KellerGame/Config/PCKellerEngine.ini", false);
		CopyFile("../BackupConfig/DefaultEngine.ini", "../KellerGame/Config/DefaultEngine.ini", false);
	}
	else {
		LPCSTR filename = "../KellerGame/Config/PCKellerEngine.ini";

		for (int i = 0; i < 3; ++i) {

			LPCSTR section = "Engine.Engine";
			WriteINI(filename, section, "MinStreamedInMips", "768");
			WriteINI(filename, section, "MaxStreamedInMips", "1120");

			WriteINI(filename, section, "TerrainMaterialMaxTextureCount", "64");
			WriteINI(filename, section, "TerrainTessellationCheckCount", "18");
			WriteINI(filename, section, "TerrainTessellationCheckBorder", "2.0");
			WriteINI(filename, section, "TerrainTessellationCheckDistance", "8192");
			WriteINI(filename, section, "bUsePixelShaderComplexity", "True");

			WriteINI(filename, section, "bUseAdditiveComplexity", "True");
			WriteINI(filename, section, "MaxPixelShaderAdditiveComplexityCount", "1000");
			WriteINI(filename, section, "MaxPixelShaderOpaqueComplexityCount", "400");
			WriteINI(filename, section, "MaxVertexShaderComplexityCount", "400");
			WriteINI(filename, section, "ShadowVolumeLightRadiusThreshold", "500");

			WriteINI(filename, section, "ShadowVolumePrimitiveScreenSpacePercentageThreshold", "0.1");
			WriteINI(filename, section, "bEnableVSMShadows", "True");
			WriteINI(filename, section, "bEnableBranchingPCFShadows", "True");
			WriteINI(filename, section, "ShadowFilterRadius", "1");
			WriteINI(filename, section, "DepthBias", "0.008");
			WriteINI(filename, section, "EnableHighPolyChars", "True");

			section = "Engine.RenderDevice";
			WriteINI(filename, section, "MeshLODRange", "4000");
			WriteINI(filename, section, "UserMaxTextureSize", "8192");

			section = "OpenGLDrv.OpenGLRenderDevice";
			WriteINI(filename, section, "LevelOfAnisotropy", "16");
			WriteINI(filename, section, "UseStaticLighting", "False");
			WriteINI(filename, section, "ShadowBufferResolution", "8192");
			WriteINI(filename, section, "MaxShadowResolution", "2040");
			WriteINI(filename, section, "MinShadowResolution", "512");

			WriteINI(filename, section, "MaxTranslucencyLayers", "4");
			WriteINI(filename, section, "ShadowResolutionScale", "3.0");
			WriteINI(filename, section, "BlurBufferResolutionDivisor", "2");
			WriteINI(filename, section, "DisableMultiLightPath", "False");
			WriteINI(filename, section, "bEnableFastShaderCaching", "True");
			WriteINI(filename, section, "DisableOcclusion", "True");

			section = "D3DDrv.D3DRenderDevice";
			WriteINI(filename, section, "LevelOfAnisotropy", "16");
			WriteINI(filename, section, "UseStaticLighting", "False");
			WriteINI(filename, section, "ShadowBufferResolution", "8192");
			WriteINI(filename, section, "MaxShadowResolution", "2040");
			WriteINI(filename, section, "MinShadowResolution", "512");

			WriteINI(filename, section, "DisableShaderModel3", "False");
			WriteINI(filename, section, "DisableMultiLightPath", "False");
			WriteINI(filename, section, "bEnableFastShaderCaching", "True");
			WriteINI(filename, section, "MaxTranslucencyLayers", "4");

			if (i == 1)
				filename = "../KellerGame/Config/DefaultEngine.ini";
			else
				filename = "../Engine/Config/BaseEngine.ini";
		}
	}
}
void IniConfig::SetTimeLimit(int time) {
	std::string time_limt = std::to_string(time);

	LPCSTR filename = "../KellerGame/Config/PC/R6VegasServerConfig.ini";
	LPCSTR section = "Engine.R6ServerOptions";
	LPCSTR key = "m_iRoundDuration";

	LPCSTR value = time_limt.c_str();
	WriteINI(filename, section, key, value);
}

void IniConfig::SetSpawnRate(int count) {
	std::string spawn = std::to_string(count);

	LPCSTR filename = "../KellerGame/Config/PlatformSpecificConfigPC.ini";
	LPCSTR section = "Gameplay";
	LPCSTR key = "MaxTerrorist";

	LPCSTR value = spawn.c_str();
	WriteINI(filename, section, key, value);
	
	key = "MaxTerroristTerroHunt";
	WriteINI(filename, section, key, value);
}
void IniConfig::SetGameMode(std::string mode) {
	
	LPCSTR filename = "../KellerGame/Config/PC/R6VegasServerConfig.ini";
	LPCSTR section = "Engine.R6ServerOptions";
	WriteINI(filename, section, "m_eGameMode", mode.c_str());

}
void IniConfig::SetMap(int map, bool cycle) {
	std::string map_code = std::to_string((map + 1) + 200);

	LPCSTR filename = "../KellerGame/Config/PC/R6VegasServerConfig.ini";
	LPCSTR section = "Engine.R6ServerOptions";
	LPCSTR value = map_code.c_str();

	if (!cycle) {
		//Default R6VegasServerConfig has 20 items and 17 maps
		for (int i = 0; i < NUM(g_aMapList) + 3; ++i) {
			std::string key_index = "m_iSelectedMaps[" + std::to_string(i) + "]";
			LPCSTR key = key_index.c_str();
			WriteINI(filename, section, key, value);
		}
	}
	else {
		std::string key_index = "m_iSelectedMaps[0]";
		LPCSTR key = key_index.c_str();
		WriteINI(filename, section, key, value);
		std::string initial_map = map_code;
		int map_slot = 1;

		for (int i = 0; i < NUM(g_aMapList); ++i) {
			map_code = std::to_string((i + 1) + 200);
			if (map_code == initial_map)
				continue;

			key_index = "m_iSelectedMaps[" + std::to_string(map_slot) + "]";
			key = key_index.c_str();
			value = map_code.c_str();
			WriteINI(filename, section, key, value);
			++map_slot;
		}
		while (map_slot < NUM(g_aMapList) + 3){
			map_code = std::to_string( ( (rand()%(NUM(g_aMapList)-1)) + 1) + 200);
			key_index = "m_iSelectedMaps[" + std::to_string(map_slot) + "]";
			key = key_index.c_str();
			value = map_code.c_str();
			WriteINI(filename, section, key, value);
			++map_slot;
		}
	}
}

void IniConfig::SetRespawn(bool on, int count) {
	LPCSTR filename = "../KellerGame/Config/PC/R6VegasServerConfig.ini";
	LPCSTR section = "Engine.R6ServerOptions";

	bool respawn_on = on;
	std::string respawn = std::to_string(count);
	
	WriteINI(filename, section, "m_iLimitRespawns", respawn.c_str());
	if (respawn_on)
		WriteINI(filename, section, "m_bRespawn", "True");
	else
		WriteINI(filename, section, "m_bRespawn", "False");

	filename = "../KellerGame/Config/DefaultServerOptions.ini";
	WriteINI(filename, section, "m_iLimitRespawns", respawn.c_str());
	WriteINI(filename, section, "m_bRespawn", "True");

}
bool IniConfig::WriteINI(LPCSTR file, LPCSTR section, LPCSTR key, LPCSTR value) {

	if (WritePrivateProfileStringA(section, key, value, file)) {
		return true;
	}
	else {
		return false;
	}
}
void IniConfig::SetServer(std::string name, std::string pass) {
	LPCSTR filename = "../BackupConfig/ModPreferences.ini";
	LPCSTR section = "Preferences";
	WriteINI(filename, section, "ServerName", name.c_str());
	WriteINI(filename, section, "ServerPassword", pass.c_str());

}
void IniConfig::SavePreferences(int index, int value) {

	LPCSTR filename = "../BackupConfig/ModPreferences.ini";
	LPCSTR section = "Preferences";

	static const std::string m_aPreferences[16] = { "IsServer","GameMode","Respawns","TimeLimit",
		"ReadyUpReq","MapIndex","MaxPlayers","SpawnRate","TerroristCount","Difficulty",
		"SetAmmo","SetGadgets", "SetInternet","SetSound","SetGraphics", "CycleMaps"
	};
	WriteINI(filename, section, m_aPreferences[index].c_str(), std::to_string(value).c_str());

}