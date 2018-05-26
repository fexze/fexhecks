#pragma once

#include <time.h>
#include "Math.hpp"
#include "Utils.hpp"
#include "Structs.hpp"
#include "Options.hpp"
#include "Structs.hpp"
#include "AimRage.hpp"
#include "Resolver.hpp"
#include "pIncludes.hpp"
#include "Singleton.hpp"
#include "Gamehooking.hpp"
#include "Miscellaneous.hpp"
#include "PredictionSystem.hpp"
#include "RebuildGameMovement.hpp"

class Vector;
class CUserCmd;
class CRecvProxyData;

struct AntiaimData_t
{
	AntiaimData_t(const float& dist, const bool& inair, int player)
	{
		this->flDist = dist;
		this->bInAir = inair;
		this->iPlayer = player;
	}

	float flDist;
	bool bInAir;
	int	iPlayer;
};

class AntiAim : public Singleton<AntiAim>
{
public:
	int GetFPS();
	void RealYaw();
	void FakeYaw();
	float GetPitch();
	void SelectTarget();
	void Work(CUserCmd* usercmd);
	void LBYBreaker(CUserCmd* cmd);
	void Fakewalk(CUserCmd* userCMD);
	void Freestanding(CUserCmd* cmd);
	bool MeetsRequirements(C_BasePlayer* pPlayer);

private:
	CUserCmd* usercmd = nullptr;
	std::vector<AntiaimData_t> Entities;
	CBaseHandle* m_ulEntHandle = nullptr;
};