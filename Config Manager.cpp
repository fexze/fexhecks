#include <string>
#include <sstream>
#include <ShlObj.h>
#include <winerror.h>
#include "Options.hpp"
#include "Config Manager.hpp"
#include <experimental/filesystem>

ConfigMgr* ConSys = new ConfigMgr();

bool ConfigMgr::FileExists(std::string file)
{
	return std::experimental::filesystem::exists(file);
}

inline bool Check(std::string File)
{
	struct stat buf;
	return (stat(File.c_str(), &buf) == 0);
}

bool ConfigMgr::CheckConfigs()
{
	strcat(Path, "C:\\faxhook");
	CreateDirectoryA(Path, nullptr);

	strcpy(Path1, Path);
	strcpy(Path2, Path);
	strcpy(Path3, Path);
	strcpy(Path4, Path);
	strcpy(Path5, Path);

	strcat(Path, "\\Legit.ini");
	strcat(Path1, "\\Rage.ini");
	strcat(Path2, "\\AWP.ini");
	strcat(Path3, "\\Auto.ini");
	strcat(Path4, "\\Scout.ini");
	strcat(Path5, "\\Revolver.ini");

	return true;
}

char* ConfigMgr::Handle()
{
	if (Clientvariables->Miscellaneous.iConfigs == 0)
	{
		ConSys->Legit = true;
		ConSys->Rage = false;
		ConSys->AWP = false;
		ConSys->Auto = false;
		ConSys->Scout = false;
		ConSys->Revolver = false;
	}

	if (Clientvariables->Miscellaneous.iConfigs == 1)
	{
		ConSys->Legit = false;
		ConSys->Rage = true;
		ConSys->AWP = false;
		ConSys->Auto = false;
		ConSys->Scout = false;
		ConSys->Revolver = false;
	}

	if (Clientvariables->Miscellaneous.iConfigs == 2)
	{
		ConSys->Legit = false;
		ConSys->Rage = false;
		ConSys->AWP = true;
		ConSys->Auto = false;
		ConSys->Scout = false;
		ConSys->Revolver = false;
	}

	if (Clientvariables->Miscellaneous.iConfigs == 3)
	{
		ConSys->Legit = false;
		ConSys->Rage = false;
		ConSys->AWP = false;
		ConSys->Auto = true;
		ConSys->Scout = false;
		ConSys->Revolver = false;
	}

	if (Clientvariables->Miscellaneous.iConfigs == 4)
	{
		ConSys->Legit = false;
		ConSys->Rage = false;
		ConSys->AWP = false;
		ConSys->Auto = false;
		ConSys->Scout = true;
		ConSys->Revolver = false;
	}

	if (Clientvariables->Miscellaneous.iConfigs == 5)
	{
		ConSys->Legit = false;
		ConSys->Rage = false;
		ConSys->AWP = false;
		ConSys->Auto = false;
		ConSys->Scout = false;
		ConSys->Revolver = true;
	}

	if (Legit)
		return Path;

	if (Rage)
		return Path1;

	if (AWP)
		return Path2;

	if (Auto)
		return Path3;

	if (Scout)
		return Path4;

	if (Revolver)
		return Path5;
}

// Ints
int ConfigMgr::ReadInt(const char* Cata, const char* Name, int DefaultVal)
{
	int iResult;
	iResult = GetPrivateProfileIntA(Cata, Name, DefaultVal, this->Handle());
	return iResult;
}

void ConfigMgr::WriteInt(const char* Cata, const char* Name, int SetVal)
{
	char buf[255];
	sprintf(buf, "%d", SetVal);
	WritePrivateProfileStringA(Cata, Name, buf, this->Handle());
}

// Floats
void ConfigMgr::WriteFloat(const char* Cata, const char* Name, float SetVal)
{
	char buf[255];
	sprintf(buf, "%f", SetVal);
	WritePrivateProfileStringA(Cata, Name, buf, this->Handle());
}

float ConfigMgr::ReadFloat(const char* Cata, const char* Name, float DefaultVal)
{
	char result[255];
	char cDefault[255];
	double fresult;
	sprintf(cDefault, "%f", DefaultVal);
	GetPrivateProfileStringA(Cata, Name, cDefault, result, 255, this->Handle());
	fresult = atof(result);
	return fresult;
}

// Strings
void ConfigMgr::WriteString(char* Cata, char* Name, char* SetVal)
{
	WritePrivateProfileStringA(Cata, Name, SetVal, this->Handle());
}

char* ConfigMgr::ReadString(char* Cata, char* Name, char* DefaultVal)
{
	auto result = new char[255];
	DWORD oProtection;
	VirtualProtect(reinterpret_cast<void*>(result), 4, PAGE_READWRITE, &oProtection);
	memset(result, 0x00, 255);
	VirtualProtect(reinterpret_cast<void*>(result), 4, oProtection, &oProtection);
	GetPrivateProfileStringA(Cata, Name, DefaultVal, result, 255, this->Handle());
	return result;
}

void ConfigMgr::SaveConfig()
{
	// Legit
	WriteInt(("Legit"), ("bBacktrack"), Clientvariables->Legit.bBacktrack);

	// Rage Page 1
	WriteInt(("Rage"), ("bEnable"), Clientvariables->Rage.bEnable);
	WriteInt(("Rage"), ("bSilentAimbot"), Clientvariables->Rage.bSilentAimbot);
	WriteInt(("Rage"), ("bRemoveRecoil"), Clientvariables->Rage.bRemoveRecoil);
	WriteInt(("Rage"), ("bAutoFire"), Clientvariables->Rage.bAutoFire);
	WriteInt(("Rage"), ("bAutoScope"), Clientvariables->Rage.bAutoScope);
	WriteInt(("Rage"), ("bResolver"), Clientvariables->Rage.bResolver);
	WriteInt(("Rage"), ("bOverride"), Clientvariables->Rage.bOverride);
	WriteInt(("Rage"), ("bLagComp"), Clientvariables->Rage.bLagComp);
	WriteInt(("Rage"), ("iLagCompType"), Clientvariables->Rage.iLagCompType);
	WriteInt(("Rage"), ("iFov"), Clientvariables->Rage.iFov);
	WriteInt(("Rage"), ("iHitchance"), Clientvariables->Rage.iHitchance);
	WriteInt(("Rage"), ("iMinDmg"), Clientvariables->Rage.iMinDmg);

	// Rage Page 2
	WriteInt(("Rage"), ("iHitbox"), Clientvariables->Rage.iHitbox);
	WriteInt(("Rage"), ("iHitscan"), Clientvariables->Rage.iHitscan);
	WriteInt(("Rage"), ("bMultiPoint"), Clientvariables->Rage.bMultiPoint);
	WriteInt(("Rage"), ("iHeadScale"), Clientvariables->Rage.iHeadScale);
	WriteInt(("Rage"), ("iBodyScale"), Clientvariables->Rage.iBodyScale);
	WriteInt(("Rage"), ("bAutoBaim"), Clientvariables->Rage.bAutoBaim);
	WriteInt(("Rage"), ("iBaimShots"), Clientvariables->Rage.iBaimShots);
	WriteInt(("Rage"), ("iBaimHealth"), Clientvariables->Rage.iBaimHealth);

	// Anti-Aim
	WriteInt(("Rage"), ("bAntiAim"), Clientvariables->Rage.bAntiAim);
	WriteInt(("Rage"), ("iPitchType"), Clientvariables->Rage.iPitchType);
	WriteInt(("Rage"), ("iYawType"), Clientvariables->Rage.iYawType);
	WriteInt(("Rage"), ("iYawAdd"), Clientvariables->Rage.iYawAdd);
	WriteInt(("Rage"), ("iYawTypeMoving"), Clientvariables->Rage.iYawTypeMoving);
	WriteInt(("Rage"), ("iYawMovingAdd"), Clientvariables->Rage.iYawMovingAdd);
	WriteInt(("Rage"), ("bLBYBreaker"), Clientvariables->Rage.bLBYBreaker);
	WriteInt(("Rage"), ("iLBYDelta"), Clientvariables->Rage.iLBYDelta);
	WriteInt(("Rage"), ("bFreestanding"), Clientvariables->Rage.bFreestanding);
	WriteInt(("Rage"), ("bFakewalk"), Clientvariables->Rage.bFakewalk);

	// Visuals Normal Page 1
	WriteInt(("Visuals"), ("bEnable"), Clientvariables->Visuals.bEnable);
	WriteInt(("Visuals"), ("bBox"), Clientvariables->Visuals.bBox);
	WriteInt(("Visuals"), ("bFlags"), Clientvariables->Visuals.bFlags);
	WriteInt(("Visuals"), ("bName"), Clientvariables->Visuals.bName);
	WriteInt(("Visuals"), ("bHealth"), Clientvariables->Visuals.bHealth);
	WriteInt(("Visuals"), ("bWeapon"), Clientvariables->Visuals.bWeapon);
	WriteInt(("Visuals"), ("bDistance"), Clientvariables->Visuals.bDistance);
	WriteInt(("Visuals"), ("bCrosshair"), Clientvariables->Visuals.bCrosshair);
	WriteInt(("Visuals"), ("bHitSound"), Clientvariables->Visuals.bHitSound);
	WriteInt(("Visuals"), ("bHitMarker"), Clientvariables->Visuals.bHitMarker);
	WriteInt(("Visuals"), ("bEventLogs"), Clientvariables->Visuals.bEventLogs);
	WriteInt(("Visuals"), ("bFovArrows"), Clientvariables->Visuals.bFovArrows);
	WriteInt(("Visuals"), ("bAmmo"), Clientvariables->Visuals.bAmmo);
	WriteInt(("Visuals"), ("bThirdPerson"), Clientvariables->Visuals.bThirdPerson);
	WriteInt(("Visuals"), ("bBeamTracers"), Clientvariables->Visuals.bBeamTracers);

	// Visuals Normal Page 2
	WriteInt(("Visuals"), ("bSkinChanger"), Clientvariables->Visuals.bSkinChanger);
	WriteInt(("Visuals"), ("bMenuSkinChanger"), Clientvariables->Visuals.bMenuSkinChanger);
	WriteInt(("Visuals"), ("bNightmode"), Clientvariables->Visuals.bNightMode);
	WriteInt(("Visuals"), ("iSkyBox"), Clientvariables->Visuals.iSkyBox);
	WriteInt(("Visuals"), ("bNadePrediction"), Clientvariables->Visuals.bNadePrediction);
	WriteInt(("Visuals"), ("bSnapLines"), Clientvariables->Visuals.bSnapLines);
	WriteInt(("Visuals"), ("bDroppedWeapons"), Clientvariables->Visuals.bDroppedWeapons);
	WriteInt(("Visuals"), ("bShowBomb"), Clientvariables->Visuals.bShowBomb);
	WriteInt(("Visuals"), ("bSkeleton"), Clientvariables->Visuals.bSkeleton);
	WriteInt(("Visuals"), ("bBacktrackedSkeleton"), Clientvariables->Visuals.bBacktrackedHead);
	WriteInt(("Visuals"), ("bAngleLines"), Clientvariables->Visuals.bAngleLines);
	WriteInt(("Visuals"), ("bSpecList"), Clientvariables->Visuals.bSpecList);
	WriteInt(("Visuals"), ("bEnemyThrowables"), Clientvariables->Visuals.bEnemyThrowables);

	// Visuals Miscellaneous Page 1
	WriteInt(("Visuals"), ("iSmokeType"), Clientvariables->Visuals.iSmokeType);
	WriteInt(("Visuals"), ("bRemoveFlash"), Clientvariables->Visuals.bRemoveFlash);
	WriteInt(("Visuals"), ("bRemoveZoom"), Clientvariables->Visuals.bRemoveZoom);
	WriteInt(("Visuals"), ("bRemoveScope"), Clientvariables->Visuals.bRemoveScope);
	WriteInt(("Visuals"), ("bRemoveVisualRecoil"), Clientvariables->Visuals.bRemoveVisualRecoil);
	WriteInt(("Visuals"), ("bFov1"), Clientvariables->Visuals.bFov1);
	WriteInt(("Visuals"), ("iFov1"), Clientvariables->Visuals.iFov1);
	WriteInt(("Visuals"), ("bFov2"), Clientvariables->Visuals.bFov2);
	WriteInt(("Visuals"), ("iFov2"), Clientvariables->Visuals.iFov2);

	// Visuals Miscellaneous Page 2
	WriteInt(("Visuals"), ("bGlowEnemy"), Clientvariables->Visuals.bGlowEnemy);
	WriteInt(("Visuals"), ("iGlowStyleEnemy"), Clientvariables->Visuals.iGlowStyleEnemy);
	WriteInt(("Visuals"), ("bChamsEnemy"), Clientvariables->Visuals.bChamsEnemy);
	WriteInt(("Visuals"), ("bGlowLocal"), Clientvariables->Visuals.bGlowLocal);
	WriteInt(("Visuals"), ("iGlowStyleLocal"), Clientvariables->Visuals.iGlowStyleLocal);
	WriteInt(("Visuals"), ("bChamsLocal"), Clientvariables->Visuals.bChamsLocal);
	WriteInt(("Visuals"), ("bChamsLocalScoped"), Clientvariables->Visuals.bChamsLocalScoped);

	// Miscellaneous
	WriteInt(("Miscellaneous"), ("bEnable"), Clientvariables->Miscellaneous.bEnable);
	WriteInt(("Miscellaneous"), ("bAntiUntrusted"), Clientvariables->Miscellaneous.bAntiUntrusted);
	WriteInt(("Miscellaneous"), ("bAutoAccept"), Clientvariables->Miscellaneous.bAutoAccept);
	WriteInt(("Miscellaneous"), ("bRevealRanks"), Clientvariables->Miscellaneous.bRevealRanks);
	WriteInt(("Miscellaneous"), ("bClanTag"), Clientvariables->Miscellaneous.bClanTag);
	WriteInt(("Miscellaneous"), ("bChatSpammer"), Clientvariables->Miscellaneous.bChatSpammer);
	WriteInt(("Miscellaneous"), ("bAutoJump"), Clientvariables->Miscellaneous.bAutoJump);
	WriteInt(("Miscellaneous"), ("bAutoStrafe"), Clientvariables->Miscellaneous.bAutoStrafe);
	WriteInt(("Miscellaneous"), ("bFakelag"), Clientvariables->Miscellaneous.bFakelag);
	WriteInt(("Miscellaneous"), ("iFakelagMove"), Clientvariables->Miscellaneous.iFakelagMove);
	WriteInt(("Miscellaneous"), ("iFakelagJump"), Clientvariables->Miscellaneous.iFakelagJump);
	WriteInt(("Miscellaneous"), ("iFakelagStand"), Clientvariables->Miscellaneous.iFakelagStand);
	WriteInt(("Miscellaneous"), ("bLagOnLand"), Clientvariables->Miscellaneous.bLagOnLand);
	WriteInt(("Miscellaneous"), ("bBreakLagComp"), Clientvariables->Miscellaneous.bBreakLagComp);

	// Box Color
	WriteFloat(("Colors"), ("cBoxR"), Clientvariables->Visuals.cBox[0]);
	WriteFloat(("Colors"), ("cBoxG"), Clientvariables->Visuals.cBox[1]);
	WriteFloat(("Colors"), ("cBoxB"), Clientvariables->Visuals.cBox[2]);

	// Name Color
	WriteFloat(("Colors"), ("cNameR"), Clientvariables->Visuals.cName[0]);
	WriteFloat(("Colors"), ("cNameG"), Clientvariables->Visuals.cName[1]);
	WriteFloat(("Colors"), ("cNameB"), Clientvariables->Visuals.cName[2]);

	// Weapon Color
	WriteFloat(("Colors"), ("cWeaponR"), Clientvariables->Visuals.cWeapon[0]);
	WriteFloat(("Colors"), ("cWeaponG"), Clientvariables->Visuals.cWeapon[1]);
	WriteFloat(("Colors"), ("cWeaponB"), Clientvariables->Visuals.cWeapon[2]);

	// Distance Color
	WriteFloat(("Colors"), ("cDistanceR"), Clientvariables->Visuals.cDistance[0]);
	WriteFloat(("Colors"), ("cDistanceG"), Clientvariables->Visuals.cDistance[1]);
	WriteFloat(("Colors"), ("cDistanceB"), Clientvariables->Visuals.cDistance[2]);

	// Event Logs Color
	WriteFloat(("Colors"), ("cEventLogsR"), Clientvariables->Visuals.cEventLogs[0]);
	WriteFloat(("Colors"), ("cEventLogsG"), Clientvariables->Visuals.cEventLogs[1]);
	WriteFloat(("Colors"), ("cEventLogsB"), Clientvariables->Visuals.cEventLogs[2]);

	// Ammunition Color
	WriteFloat(("Colors"), ("cAmmoR"), Clientvariables->Visuals.cAmmo[0]);
	WriteFloat(("Colors"), ("cAmmoG"), Clientvariables->Visuals.cAmmo[1]);
	WriteFloat(("Colors"), ("cAmmoB"), Clientvariables->Visuals.cAmmo[2]);

	// Bullet Tracers Color
	WriteFloat(("Colors"), ("cBeamTracersR"), Clientvariables->Visuals.cBeamTracers[0]);
	WriteFloat(("Colors"), ("cBeamTracersG"), Clientvariables->Visuals.cBeamTracers[1]);
	WriteFloat(("Colors"), ("cBeamTracersB"), Clientvariables->Visuals.cBeamTracers[2]);

	// Glow Enemy Color
	WriteFloat(("Colors"), ("cGlowEnemyR"), Clientvariables->Visuals.cGlowEnemy[0]);
	WriteFloat(("Colors"), ("cGlowEnemyG"), Clientvariables->Visuals.cGlowEnemy[1]);
	WriteFloat(("Colors"), ("cGlowEnemyB"), Clientvariables->Visuals.cGlowEnemy[2]);

	// Chams Enemy Color
	WriteFloat(("Colors"), ("cChamsEnemyR"), Clientvariables->Visuals.cChamsEnemy[0]);
	WriteFloat(("Colors"), ("cChamsEnemyG"), Clientvariables->Visuals.cChamsEnemy[1]);
	WriteFloat(("Colors"), ("cChamsEnemyB"), Clientvariables->Visuals.cChamsEnemy[2]);

	// Glow Local Color
	WriteFloat(("Colors"), ("cGlowLocalR"), Clientvariables->Visuals.cGlowLocal[0]);
	WriteFloat(("Colors"), ("cGlowLocalG"), Clientvariables->Visuals.cGlowLocal[1]);
	WriteFloat(("Colors"), ("cGlowLocalB"), Clientvariables->Visuals.cGlowLocal[2]);

	// Chams Local Color
	WriteFloat(("Colors"), ("cChamsLocalR"), Clientvariables->Visuals.cChamsLocal[0]);
	WriteFloat(("Colors"), ("cChamsLocalG"), Clientvariables->Visuals.cChamsLocal[1]);
	WriteFloat(("Colors"), ("cChamsLocalB"), Clientvariables->Visuals.cChamsLocal[2]);

	// Nade Prediction Color
	WriteFloat(("Colors"), ("cNadePredictionR"), Clientvariables->Visuals.cNadePrediction[0]);
	WriteFloat(("Colors"), ("cNadePredictionG"), Clientvariables->Visuals.cNadePrediction[1]);
	WriteFloat(("Colors"), ("cNadePredictionB"), Clientvariables->Visuals.cNadePrediction[2]);

	// Dropped Weapons Color
	WriteFloat(("Colors"), ("cDroppedWeaponsR"), Clientvariables->Visuals.cDroppedWeapons[0]);
	WriteFloat(("Colors"), ("cDroppedWeaponsG"), Clientvariables->Visuals.cDroppedWeapons[1]);
	WriteFloat(("Colors"), ("cDroppedWeaponsB"), Clientvariables->Visuals.cDroppedWeapons[2]);

	// Enemy Nade ESP Color
	WriteFloat(("Colors"), ("cEnemyThrowablesR"), Clientvariables->Visuals.cEnemyThrowables[0]);
	WriteFloat(("Colors"), ("cEnemyThrowablesG"), Clientvariables->Visuals.cEnemyThrowables[1]);
	WriteFloat(("Colors"), ("cEnemyThrowablesB"), Clientvariables->Visuals.cEnemyThrowables[2]);
}

void ConfigMgr::LoadConfig()
{
	// Legit
	Clientvariables->Legit.bBacktrack = ReadInt(("Legit"), ("bBacktrack"), 0);

	// Rage Page 1
	Clientvariables->Rage.bEnable = ReadInt(("Rage"), ("bEnable"), 0);
	Clientvariables->Rage.bSilentAimbot = ReadInt(("Rage"), ("bSilentAimbot"), 0);
	Clientvariables->Rage.bRemoveRecoil = ReadInt(("Rage"), ("bRemoveRecoil"), 0);
	Clientvariables->Rage.bAutoFire = ReadInt(("Rage"), ("bAutoFire"), 0);
	Clientvariables->Rage.bAutoScope = ReadInt(("Rage"), ("bAutoScope"), 0);
	Clientvariables->Rage.bResolver = ReadInt(("Rage"), ("bResolver"), 0);
	Clientvariables->Rage.bOverride = ReadInt(("Rage"), ("bOverride"), 0);
	Clientvariables->Rage.bLagComp = ReadInt(("Rage"), ("bLagComp"), 0);
	Clientvariables->Rage.iLagCompType = ReadInt(("Rage"), ("iLagCompType"), 0);
	Clientvariables->Rage.iFov = ReadInt(("Rage"), ("iFov"), 0);
	Clientvariables->Rage.iHitchance = ReadInt(("Rage"), ("iHitchance"), 0);
	Clientvariables->Rage.iMinDmg = ReadInt(("Rage"), ("iMinDmg"), 0);

	// Rage Page 2
	Clientvariables->Rage.iHitbox = ReadInt(("Rage"), ("iHitbox"), 0);
	Clientvariables->Rage.iHitscan = ReadInt(("Rage"), ("iHitscan"), 0);
	Clientvariables->Rage.bMultiPoint = ReadInt(("Rage"), ("bMultiPoint"), 0);
	Clientvariables->Rage.iHeadScale = ReadInt(("Rage"), ("iHeadScale"), 0);
	Clientvariables->Rage.iBodyScale = ReadInt(("Rage"), ("iBodyScale"), 0);
	Clientvariables->Rage.bAutoBaim = ReadInt(("Rage"), ("bAutoBaim"), 0);
	Clientvariables->Rage.iBaimShots = ReadInt(("Rage"), ("iBaimShots"), 0);
	Clientvariables->Rage.iBaimHealth = ReadInt(("Rage"), ("iBaimHealth"), 0);

	// Anti-Aim
	Clientvariables->Rage.bAntiAim = ReadInt(("Rage"), ("bAntiAim"), 0);
	Clientvariables->Rage.iPitchType = ReadInt(("Rage"), ("iPitchType"), 0);
	Clientvariables->Rage.iYawType = ReadInt(("Rage"), ("iYawType"), 0);
	Clientvariables->Rage.iYawAdd = ReadInt(("Rage"), ("iYawAdd"), 0);
	Clientvariables->Rage.iYawTypeMoving = ReadInt(("Rage"), ("iYawTypeMoving"), 0);
	Clientvariables->Rage.iYawMovingAdd = ReadInt(("Rage"), ("iYawMovingAdd"), 0);
	Clientvariables->Rage.bLBYBreaker = ReadInt(("Rage"), ("bLBYBreaker"), 0);
	Clientvariables->Rage.iLBYDelta = ReadInt(("Rage"), ("iLBYDelta"), 0);
	Clientvariables->Rage.bFreestanding = ReadInt(("Rage"), ("bFreestanding"), 0);
	Clientvariables->Rage.bFakewalk = ReadInt(("Rage"), ("bFakewalk"), 0);

	// Visuals Normal Page 1
	Clientvariables->Visuals.bEnable = ReadInt(("Visuals"), ("bEnable"), 0);
	Clientvariables->Visuals.bBox = ReadInt(("Visuals"), ("bBox"), 0);
	Clientvariables->Visuals.bFlags = ReadInt(("Visuals"), ("bFlags"), 0);
	Clientvariables->Visuals.bName = ReadInt(("Visuals"), ("bName"), 0);
	Clientvariables->Visuals.bHealth = ReadInt(("Visuals"), ("bHealth"), 0);
	Clientvariables->Visuals.bWeapon = ReadInt(("Visuals"), ("bWeapon"), 0);
	Clientvariables->Visuals.bDistance = ReadInt(("Visuals"), ("bDistance"), 0);
	Clientvariables->Visuals.bCrosshair = ReadInt(("Visuals"), ("bCrosshair"), 0);
	Clientvariables->Visuals.bHitSound = ReadInt(("Visuals"), ("bHitSound"), 0);
	Clientvariables->Visuals.bHitMarker = ReadInt(("Visuals"), ("bHitMarker"), 0);
	Clientvariables->Visuals.bEventLogs = ReadInt(("Visuals"), ("bEventLogs"), 0);
	Clientvariables->Visuals.bFovArrows = ReadInt(("Visuals"), ("bFovArrows"), 0);
	Clientvariables->Visuals.bAmmo = ReadInt(("Visuals"), ("bAmmo"), 0);
	Clientvariables->Visuals.bThirdPerson = ReadInt(("Visuals"), ("bThirdPerson"), 0);
	Clientvariables->Visuals.bBeamTracers = ReadInt(("Visuals"), ("bBeamTracers"), 0);

	// Visuals Normal Page 2
	Clientvariables->Visuals.bSkinChanger = ReadInt(("Visuals"), ("bSkinChanger"), 0);
	Clientvariables->Visuals.bMenuSkinChanger = ReadInt(("Visuals"), ("bMenuSkinChanger"), 0);
	Clientvariables->Visuals.bNightMode = ReadInt(("Visuals"), ("bNightmode"), 0);
	Clientvariables->Visuals.iSkyBox = ReadInt(("Visuals"), ("iSkyBox"), 0);
	Clientvariables->Visuals.bNadePrediction = ReadInt(("Visuals"), ("bNadePrediction"), 0);

	Clientvariables->Visuals.bSnapLines = ReadInt(("Visuals"), ("bSnapLines"), 0);
	Clientvariables->Visuals.bDroppedWeapons = ReadInt(("Visuals"), ("bDroppedWeapons"), 0);
	Clientvariables->Visuals.bShowBomb = ReadInt(("Visuals"), ("bShowBomb"), 0);
	Clientvariables->Visuals.bSkeleton = ReadInt(("Visuals"), ("bSkeleton"), 0);
	Clientvariables->Visuals.bBacktrackedHead = ReadInt(("Visuals"), ("bBacktrackedSkeleton"), 0);
	Clientvariables->Visuals.bAngleLines = ReadInt(("Visuals"), ("bAngleLines"), 0);
	Clientvariables->Visuals.bSpecList = ReadInt(("Visuals"), ("bSpecList"), 0);
	Clientvariables->Visuals.bEnemyThrowables = ReadInt(("Visuals"), ("bEnemyThrowables"), 0);

	// Visuals Miscellaneous Page 1
	Clientvariables->Visuals.iSmokeType = ReadInt(("Visuals"), ("iSmokeType"), 0);
	Clientvariables->Visuals.bRemoveFlash = ReadInt(("Visuals"), ("bRemoveFlash"), 0);
	Clientvariables->Visuals.bRemoveZoom = ReadInt(("Visuals"), ("bRemoveZoom"), 0);
	Clientvariables->Visuals.bRemoveScope = ReadInt(("Visuals"), ("bRemoveScope"), 0);
	Clientvariables->Visuals.bRemoveVisualRecoil = ReadInt(("Visuals"), ("bRemoveVisualRecoil"), 0);
	Clientvariables->Visuals.bFov1 = ReadInt(("Visuals"), ("bFov1"), 0);
	Clientvariables->Visuals.iFov1 = ReadInt(("Visuals"), ("iFov1"), 0);
	Clientvariables->Visuals.bFov2 = ReadInt(("Visuals"), ("bFov2"), 0);
	Clientvariables->Visuals.iFov2 = ReadInt(("Visuals"), ("iFov2"), 0);

	// Visuals Miscellaneous Page 2
	Clientvariables->Visuals.bGlowEnemy = ReadInt(("Visuals"), ("bGlowEnemy"), 0);
	Clientvariables->Visuals.iGlowStyleEnemy = ReadInt(("Visuals"), ("iGlowStyleEnemy"), 0);
	Clientvariables->Visuals.bChamsEnemy = ReadInt(("Visuals"), ("bChamsEnemy"), 0);
	Clientvariables->Visuals.bGlowLocal = ReadInt(("Visuals"), ("bGlowLocal"), 0);
	Clientvariables->Visuals.iGlowStyleLocal = ReadInt(("Visuals"), ("iGlowStyleLocal"), 0);
	Clientvariables->Visuals.bChamsLocal = ReadInt(("Visuals"), ("bChamsLocal"), 0);
	Clientvariables->Visuals.bChamsLocalScoped = ReadInt(("Visuals"), ("bChamsLocalScoped"), 0);

	// Miscellaneous
	Clientvariables->Miscellaneous.bEnable = ReadInt(("Miscellaneous"), ("bEnable"), 0);
	Clientvariables->Miscellaneous.bAntiUntrusted = ReadInt(("Miscellaneous"), ("bAntiUntrusted"), 0);
	Clientvariables->Miscellaneous.bAutoAccept = ReadInt(("Miscellaneous"), ("bAutoAccept"), 0);
	Clientvariables->Miscellaneous.bRevealRanks = ReadInt(("Miscellaneous"), ("bRevealRanks"), 0);
	Clientvariables->Miscellaneous.bClanTag = ReadInt(("Miscellaneous"), ("bClanTag"), 0);
	Clientvariables->Miscellaneous.bChatSpammer = ReadInt(("Miscellaneous"), ("bChatSpammer"), 0);
	Clientvariables->Miscellaneous.bAutoJump = ReadInt(("Miscellaneous"), ("bAutoJump"), 0);
	Clientvariables->Miscellaneous.bAutoStrafe = ReadInt(("Miscellaneous"), ("bAutoStrafe"), 0);
	Clientvariables->Miscellaneous.bFakelag = ReadInt(("Miscellaneous"), ("bFakelag"), 0);
	Clientvariables->Miscellaneous.iFakelagMove = ReadInt(("Miscellaneous"), ("iFakelagMove"), 0);
	Clientvariables->Miscellaneous.iFakelagJump = ReadInt(("Miscellaneous"), ("iFakelagJump"), 0);
	Clientvariables->Miscellaneous.iFakelagStand = ReadInt(("Miscellaneous"), ("iFakelagStand"), 0);
	Clientvariables->Miscellaneous.bLagOnLand = ReadInt(("Miscellaneous"), ("bLagOnLand"), 0);
	Clientvariables->Miscellaneous.bBreakLagComp = ReadInt(("Miscellaneous"), ("bBreakLagComp"), 0);

	// Box Color
	Clientvariables->Visuals.cBox[0] = ReadFloat(("Colors"), ("cBoxR"), 0.f);
	Clientvariables->Visuals.cBox[1] = ReadFloat(("Colors"), ("cBoxG"), 0.f);
	Clientvariables->Visuals.cBox[2] = ReadFloat(("Colors"), ("cBoxB"), 0.f);

	// Name Color
	Clientvariables->Visuals.cName[0] = ReadFloat(("Colors"), ("cNameR"), 0.f);
	Clientvariables->Visuals.cName[1] = ReadFloat(("Colors"), ("cNameG"), 0.f);
	Clientvariables->Visuals.cName[2] = ReadFloat(("Colors"), ("cNameB"), 0.f);

	// Weapon Color
	Clientvariables->Visuals.cWeapon[0] = ReadFloat(("Colors"), ("cWeaponR"), 0.f);
	Clientvariables->Visuals.cWeapon[1] = ReadFloat(("Colors"), ("cWeaponG"), 0.f);
	Clientvariables->Visuals.cWeapon[2] = ReadFloat(("Colors"), ("cWeaponB"), 0.f);

	// Distance Color
	Clientvariables->Visuals.cDistance[0] = ReadFloat(("Colors"), ("cDistanceR"), 0.f);
	Clientvariables->Visuals.cDistance[1] = ReadFloat(("Colors"), ("cDistanceG"), 0.f);
	Clientvariables->Visuals.cDistance[2] = ReadFloat(("Colors"), ("cDistanceB"), 0.f);

	// Event Logs Color
	Clientvariables->Visuals.cEventLogs[0] = ReadFloat(("Colors"), ("cEventLogsR"), 0.f);
	Clientvariables->Visuals.cEventLogs[1] = ReadFloat(("Colors"), ("cEventLogsG"), 0.f);
	Clientvariables->Visuals.cEventLogs[2] = ReadFloat(("Colors"), ("cEventLogsB"), 0.f);

	// Ammunition Color
	Clientvariables->Visuals.cAmmo[0] = ReadFloat(("Colors"), ("cAmmoR"), 0.f);
	Clientvariables->Visuals.cAmmo[1] = ReadFloat(("Colors"), ("cAmmoG"), 0.f);
	Clientvariables->Visuals.cAmmo[2] = ReadFloat(("Colors"), ("cAmmoB"), 0.f);

	// Bullet Tracers Color
	Clientvariables->Visuals.cBeamTracers[0] = ReadFloat(("Colors"), ("cBeamTracersR"), 0.f);
	Clientvariables->Visuals.cBeamTracers[1] = ReadFloat(("Colors"), ("cBeamTracersG"), 0.f);
	Clientvariables->Visuals.cBeamTracers[2] = ReadFloat(("Colors"), ("cBeamTracersB"), 0.f);

	// Glow Enemy Color
	Clientvariables->Visuals.cGlowEnemy[0] = ReadFloat(("Colors"), ("cGlowEnemyR"), 0.f);
	Clientvariables->Visuals.cGlowEnemy[1] = ReadFloat(("Colors"), ("cGlowEnemyG"), 0.f);
	Clientvariables->Visuals.cGlowEnemy[2] = ReadFloat(("Colors"), ("cGlowEnemyB"), 0.f);

	// Chams Enemy Color
	Clientvariables->Visuals.cChamsEnemy[0] = ReadFloat(("Colors"), ("cChamsEnemyR"), 0.f);
	Clientvariables->Visuals.cChamsEnemy[1] = ReadFloat(("Colors"), ("cChamsEnemyG"), 0.f);
	Clientvariables->Visuals.cChamsEnemy[2] = ReadFloat(("Colors"), ("cChamsEnemyB"), 0.f);

	// Glow Local Color
	Clientvariables->Visuals.cGlowLocal[0] = ReadFloat(("Colors"), ("cGlowLocalR"), 0.f);
	Clientvariables->Visuals.cGlowLocal[1] = ReadFloat(("Colors"), ("cGlowLocalG"), 0.f);
	Clientvariables->Visuals.cGlowLocal[2] = ReadFloat(("Colors"), ("cGlowLocalB"), 0.f);

	// Chams Local Color
	Clientvariables->Visuals.cChamsLocal[0] = ReadFloat(("Colors"), ("cChamsLocalR"), 0.f);
	Clientvariables->Visuals.cChamsLocal[1] = ReadFloat(("Colors"), ("cChamsLocalG"), 0.f);
	Clientvariables->Visuals.cChamsLocal[2] = ReadFloat(("Colors"), ("cChamsLocalB"), 0.f);

	// Nade Prediction Color
	Clientvariables->Visuals.cNadePrediction[0] = ReadFloat(("Colors"), ("cNadePredictionR"), 0.f);
	Clientvariables->Visuals.cNadePrediction[1] = ReadFloat(("Colors"), ("cNadePredictionG"), 0.f);
	Clientvariables->Visuals.cNadePrediction[2] = ReadFloat(("Colors"), ("cNadePredictionB"), 0.f);

	// Dropped Weapons Color
	Clientvariables->Visuals.cDroppedWeapons[0] = ReadFloat(("Colors"), ("cDroppedWeaponsR"), 0.f);
	Clientvariables->Visuals.cDroppedWeapons[1] = ReadFloat(("Colors"), ("cDroppedWeaponsG"), 0.f);
	Clientvariables->Visuals.cDroppedWeapons[2] = ReadFloat(("Colors"), ("cDroppedWeaponsB"), 0.f);

	// Enemy Nade ESP Color
	Clientvariables->Visuals.cEnemyThrowables[0] = ReadFloat(("Colors"), ("cEnemyThrowablesR"), 0.f);
	Clientvariables->Visuals.cEnemyThrowables[1] = ReadFloat(("Colors"), ("cEnemyThrowablesG"), 0.f);
	Clientvariables->Visuals.cEnemyThrowables[2] = ReadFloat(("Colors"), ("cEnemyThrowablesB"), 0.f);
}

void ConfigMgr::Reset()
{
	// Legit
	Clientvariables->Legit.bBacktrack = 0;

	// Rage Page 1
	Clientvariables->Rage.bEnable = 0;
	Clientvariables->Rage.bSilentAimbot = 0;
	Clientvariables->Rage.bRemoveRecoil = 0;
	Clientvariables->Rage.bAutoFire = 0;
	Clientvariables->Rage.bAutoScope = 0;
	Clientvariables->Rage.iFov = 0;
	Clientvariables->Rage.iHitchance = 0;
	Clientvariables->Rage.iMinDmg = 0;
	Clientvariables->Rage.bResolver = 0;
	Clientvariables->Rage.bOverride = 0;
	Clientvariables->Rage.bLagComp = 0;
	Clientvariables->Rage.iLagCompType  = 0;

	// Rage Page 2
	Clientvariables->Rage.iHitbox = 0;
	Clientvariables->Rage.iHitscan = 0;
	Clientvariables->Rage.bMultiPoint = 0;
	Clientvariables->Rage.iHeadScale = 0;
	Clientvariables->Rage.iBodyScale = 0;
	Clientvariables->Rage.bAutoBaim = 0;
	Clientvariables->Rage.iBaimShots = 0;
	Clientvariables->Rage.iBaimHealth = 0;

	// Anti-Aim
	Clientvariables->Rage.bAntiAim = 0;
	Clientvariables->Rage.iPitchType = 0;
	Clientvariables->Rage.iYawType = 0;
	Clientvariables->Rage.iYawAdd = 0;
	Clientvariables->Rage.iYawTypeMoving = 0;
	Clientvariables->Rage.iYawMovingAdd = 0;
	Clientvariables->Rage.bLBYBreaker = 0;
	Clientvariables->Rage.iLBYDelta = 0;
	Clientvariables->Rage.bFreestanding = 0;
	Clientvariables->Rage.bFakewalk = 0;

	// Visuals Normal Page 1
	Clientvariables->Visuals.bEnable = 0;
	Clientvariables->Visuals.bBox = 0;
	Clientvariables->Visuals.bFlags = 0;
	Clientvariables->Visuals.bName = 0;
	Clientvariables->Visuals.bHealth = 0;
	Clientvariables->Visuals.bWeapon = 0;
	Clientvariables->Visuals.bDistance = 0;
	Clientvariables->Visuals.bCrosshair = 0;
	Clientvariables->Visuals.bHitSound = 0;
	Clientvariables->Visuals.bHitMarker = 0;
	Clientvariables->Visuals.bEventLogs = 0;
	Clientvariables->Visuals.bFovArrows = 0;
	Clientvariables->Visuals.bAmmo = 0;
	Clientvariables->Visuals.bThirdPerson = 0;
	Clientvariables->Visuals.bBeamTracers = 0;

	// Visuals Normal Page 2
	Clientvariables->Visuals.bSkinChanger = 0;
	Clientvariables->Visuals.bMenuSkinChanger = 0;
	Clientvariables->Visuals.bNightMode = 0;
	Clientvariables->Visuals.iSkyBox = 0;
	Clientvariables->Visuals.bNadePrediction = 0;
	Clientvariables->Visuals.bSnapLines = 0;
	Clientvariables->Visuals.bDroppedWeapons = 0;
	Clientvariables->Visuals.bShowBomb = 0;
	Clientvariables->Visuals.bSkeleton = 0;
    Clientvariables->Visuals.bBacktrackedHead = 0;
	Clientvariables->Visuals.bAngleLines = 0;
	Clientvariables->Visuals.bSpecList = 0;
	Clientvariables->Visuals.bEnemyThrowables = 0;

	// Visuals Miscellaneous Page 1
	Clientvariables->Visuals.iSmokeType = 0;
	Clientvariables->Visuals.bRemoveFlash = 0;
	Clientvariables->Visuals.bRemoveZoom = 0;
	Clientvariables->Visuals.bRemoveScope = 0;
	Clientvariables->Visuals.bRemoveVisualRecoil = 0;
	Clientvariables->Visuals.bFov1 = 0;
	Clientvariables->Visuals.iFov1 = 90;
	Clientvariables->Visuals.bFov2 = 0;
	Clientvariables->Visuals.iFov2 = 54;

	// Visuals Miscellaneous Page 2
	Clientvariables->Visuals.bGlowEnemy = 0;
	Clientvariables->Visuals.iGlowStyleEnemy = 0;
	Clientvariables->Visuals.bChamsEnemy = 0;
	Clientvariables->Visuals.bGlowLocal = 0;
	Clientvariables->Visuals.iGlowStyleLocal = 0;
	Clientvariables->Visuals.bChamsLocal = 0;
	Clientvariables->Visuals.bChamsLocalScoped = 0;

	// Miscellaneous
	Clientvariables->Miscellaneous.bEnable = 0;
	Clientvariables->Miscellaneous.bAntiUntrusted = 1;
	Clientvariables->Miscellaneous.bAutoAccept = 0;
	Clientvariables->Miscellaneous.bRevealRanks = 0;
	Clientvariables->Miscellaneous.bClanTag = 0;
	Clientvariables->Miscellaneous.bChatSpammer = 0;
	Clientvariables->Miscellaneous.bAutoJump = 0;
	Clientvariables->Miscellaneous.bAutoStrafe = 0;
	Clientvariables->Miscellaneous.bFakelag = 0;
	Clientvariables->Miscellaneous.iFakelagMove = 1;
	Clientvariables->Miscellaneous.iFakelagJump = 1;
	Clientvariables->Miscellaneous.iFakelagStand = 1;
	Clientvariables->Miscellaneous.bLagOnLand = 0;
	Clientvariables->Miscellaneous.bBreakLagComp = 0;

	// Box Color
	Clientvariables->Visuals.cBox[0] = 0.f;
	Clientvariables->Visuals.cBox[1] = 0.f;
	Clientvariables->Visuals.cBox[2] = 0.f;

	// Name Color
	Clientvariables->Visuals.cName[0] = 0.f;
	Clientvariables->Visuals.cName[1] = 0.f;
	Clientvariables->Visuals.cName[2] = 0.f;

	// Weapon Color
	Clientvariables->Visuals.cWeapon[0] = 0.f;
	Clientvariables->Visuals.cWeapon[1] = 0.f;
	Clientvariables->Visuals.cWeapon[2] = 0.f;

	// Distance Color
	Clientvariables->Visuals.cDistance[0] = 0.f;
	Clientvariables->Visuals.cDistance[1] = 0.f;
	Clientvariables->Visuals.cDistance[2] = 0.f;

	// Event Logs Color
	Clientvariables->Visuals.cEventLogs[0] = 0.f;
	Clientvariables->Visuals.cEventLogs[1] = 0.f;
	Clientvariables->Visuals.cEventLogs[2] = 0.f;

	// Ammunition Color
	Clientvariables->Visuals.cAmmo[0] = 0.f;
	Clientvariables->Visuals.cAmmo[1] = 0.f;
	Clientvariables->Visuals.cAmmo[2] = 0.f;

	// Bullet Tracers Color
	Clientvariables->Visuals.cBeamTracers[0] = 0.f;
	Clientvariables->Visuals.cBeamTracers[1] = 0.f;
	Clientvariables->Visuals.cBeamTracers[2] = 0.f;

	// Glow Enemy Color
	Clientvariables->Visuals.cGlowEnemy[0] = 0.f;
	Clientvariables->Visuals.cGlowEnemy[1] = 0.f;
	Clientvariables->Visuals.cGlowEnemy[2] = 0.f;

	// Chams Enemy Color
	Clientvariables->Visuals.cChamsEnemy[0] = 0.f;
	Clientvariables->Visuals.cChamsEnemy[1] = 0.f;
	Clientvariables->Visuals.cChamsEnemy[2] = 0.f;

	// Glow Local Color
	Clientvariables->Visuals.cGlowLocal[0] = 0.f;
	Clientvariables->Visuals.cGlowLocal[1] = 0.f;
	Clientvariables->Visuals.cGlowLocal[2] = 0.f;

	// Chams Local Color
	Clientvariables->Visuals.cChamsLocal[0] = 0.f;
	Clientvariables->Visuals.cChamsLocal[1] = 0.f;
	Clientvariables->Visuals.cChamsLocal[2] = 0.f;

	// Nade Prediction Color
	Clientvariables->Visuals.cNadePrediction[0] = 0.f;
	Clientvariables->Visuals.cNadePrediction[1] = 0.f;
	Clientvariables->Visuals.cNadePrediction[2] = 0.f;

	// Dropped Weapons Color
	Clientvariables->Visuals.cDroppedWeapons[0] = 0.f;
	Clientvariables->Visuals.cDroppedWeapons[1] = 0.f;
	Clientvariables->Visuals.cDroppedWeapons[2] = 0.f;

	// Enemy Nade ESP Color
	Clientvariables->Visuals.cEnemyThrowables[0] = 0.f;
	Clientvariables->Visuals.cEnemyThrowables[1] = 0.f;
	Clientvariables->Visuals.cEnemyThrowables[2] = 0.f;
}