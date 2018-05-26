#pragma once

#include "Structs.hpp"
#include "Singleton.hpp"

// Autowall Struct
struct FireBulletData
{
	Vector src;
	CGameTrace enter_trace;
	Vector direction;
	CTraceFilter filter;
	float trace_length;
	float trace_length_remaining;
	float current_damage;
	int penetrate_count;
};

class AimRage : public Singleton<AimRage>
{
public:
	// Ragebot Functions
	void AutoStop();
	void AutoCrouch();
	bool CockRevolver();
	void TargetEntities();
	bool CheckTarget(int i);
	void Work(CUserCmd* usercmd);
	void RemoveRecoil(CUserCmd* cmd);
	bool Bodyaim(C_BasePlayer* Target);
	bool TargetSpecificEnt(C_BasePlayer* pEnt);
	bool HitChance(QAngle angles, C_BasePlayer* ent, float chance);
	Vector CalculateBestPoint(C_BasePlayer* player, int prioritized, float minDmg, bool onlyPrioritized, matrix3x4_t matrix[]);
	float BestHitPoint(C_BasePlayer* player, int prioritized, float minDmg, mstudiohitboxset_t* hitset, matrix3x4_t matrix[], Vector& vecOut);

	// Variables
	int prev_aimtarget = NULL;
	CUserCmd* usercmd = nullptr;
	bool can_fire_weapon = false;
	C_BaseCombatWeapon* local_weapon = nullptr;

	// Autowall Functions
	bool IsBreakableEntity(C_BasePlayer* ent);
	bool IsArmored(C_BasePlayer* player, int armorVal, int hitgroup);
	bool HandleBulletPenetration(WeapInfo_t* wpn_data, FireBulletData& data);
	float GetDamageVec(const Vector& vecPoint, C_BasePlayer* player, int hitbox);
	void ScaleDamage(int hitgroup, C_BasePlayer* player, float weapon_armor_ratio, float& current_damage);
	bool TraceToExit(Vector& end, CGameTrace* enter_trace, Vector start, Vector dir, CGameTrace* exit_trace);
	bool SimulateFireBullet(C_BaseCombatWeapon* weap, FireBulletData& data, C_BasePlayer* player, int hitbox);
	void traceIt(Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, C_BasePlayer* ign, CGameTrace* tr);
	void ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, CGameTrace* tr);
};