#include "AntiAim.hpp"

#define M_PI 3.14159265358979323846

bool AntiAim::MeetsRequirements(C_BasePlayer* pPlayer)
{
	if (!pPlayer || pPlayer == nullptr)
		return false;

	if (pPlayer == g_LocalPlayer)
		return false;

	if (!pPlayer->IsAlive() || pPlayer->IsDormant())
		return false;

	if (pPlayer->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
		return false;

	if (pPlayer->m_fFlags() & FL_FROZEN)
		return false;

	auto pWeapon = pPlayer->m_hActiveWeapon();

	if (pWeapon->IsMiscellaneousWeapon())
		return false;

	return true;
}

float Vector::Dist(const Vector& vOther) const
{
	Vector delta;
	delta.x = x - vOther.x;
	delta.y = y - vOther.y;
	delta.z = z - vOther.z;
	return delta.Length();
}

void AntiAim::SelectTarget()
{
	for (int index = 1; index <= g_GlobalVars->maxClients; index++)
	{
		auto pPlayer = reinterpret_cast< C_BasePlayer* >(g_EntityList->GetClientEntity(index));

		if (!MeetsRequirements(pPlayer))
			continue;

		float flDist = pPlayer->GetAbsOrigin().Dist(g_LocalPlayer->GetAbsOrigin());
		bool bInAir = !(pPlayer->m_fFlags() & FL_ONGROUND);
		AntiaimData_t data(index, flDist, bInAir);
		Entities.push_back(data);
	}
}

void AntiAim::Work(CUserCmd* usercmd)
{
	Entities.clear();
	this->usercmd = usercmd;

	if (usercmd->buttons & IN_USE)
		return;

	auto weapon = g_LocalPlayer->m_hActiveWeapon().Get();

	if (!weapon)
		return;

	if (weapon->m_iItemDefinitionIndex() == WEAPON_REVOLVER)
	{
		if (usercmd->buttons & IN_ATTACK2)
			return;

		if (weapon->CanFirePostPone() && (usercmd->buttons & IN_ATTACK))
			return;
	}
	else if (weapon->GetWeapInfo()->iWeaponType == WEAPONTYPE_GRENADE)
	{
		if (weapon->IsInThrow())
			return;
	}
	else
	{
		if (weapon->GetWeapInfo()->iWeaponType == WEAPONTYPE_KNIFE && ((usercmd->buttons & IN_ATTACK) || (usercmd->buttons & IN_ATTACK2)))
			return;

		else if ((usercmd->buttons & IN_ATTACK) && (weapon->m_iItemDefinitionIndex() != WEAPON_C4 || Clientvariables->Rage.iPitchType != 0))
			return;
	}

	if (g_LocalPlayer->GetMoveType() == MOVETYPE_NOCLIP || g_LocalPlayer->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (Clientvariables->Rage.bAntiAim && !Clientvariables->Miscellaneous.bFakelag && !g_InputSystem->IsButtonDown(MOUSE_4))
		Global::bSendPacket = usercmd->command_number % 2;

	usercmd->viewangles.pitch = GetPitch();

	if (!Global::bSendPacket)
	{
		if (Clientvariables->Rage.bFreestanding)
		{
			SelectTarget();
			Freestanding(usercmd);
		}
		else
			RealYaw();
	}
	else
		FakeYaw();
}

float AntiAim::GetPitch()
{
	switch (Clientvariables->Rage.iPitchType)
	{
	case 0:
		return usercmd->viewangles.pitch;
		break;

	case 1:
		return -88.99f;
		break;

	case 2:

		return 0.f;
		break;

	case 3:

		return 88.99f;
		break;

	case 4:

		return g_LocalPlayer->m_hActiveWeapon().Get()->IsSniper() ? (g_LocalPlayer->m_hActiveWeapon().Get()->m_zoomLevel() != 0 ? 87.f : 85.f) : 88.99f;
		break;
	}

	return usercmd->viewangles.pitch;
}

void AntiAim::RealYaw()
{
	if (Clientvariables->Rage.iYawType == 3)
	{
		usercmd->viewangles.yaw += 90;
	}
	else if (Clientvariables->Rage.iYawType == 5)
	{
		if (GetAsyncKeyState(VK_XBUTTON2) & 1)
			Global::AAFlip = !Global::AAFlip;

		if (Global::AAFlip)
			usercmd->viewangles.yaw -= 90;
		else
			usercmd->viewangles.yaw += 90;
	}
	else if (g_LocalPlayer->m_vecVelocity().Length() >= 0.1f)
	{
		float curYaw = usercmd->viewangles.yaw;

		static float add = 0;
		static int Ticks = 0;
		float temp_base = curYaw;
		static bool reset = true;

		switch (Clientvariables->Rage.iYawTypeMoving)
		{
		case 0:
			return;
			break;

		case 1:
			curYaw -= Ticks;
			Ticks += 2;

			if (Ticks > 240)
				Ticks = 120;
			break;

		case 2:
			if (reset)
			{
				add = 0;
				reset = false;
			}

			curYaw += 135;
			curYaw += add;
			add += 15;

			if (temp_base + 225 < curYaw)
			{
				reset = true;
				curYaw = temp_base + 225;
			}
			break;

		case 3:
			curYaw += 180 + ((rand() % 15) - (15 * 0.5f));
			break;

		case 4:
			curYaw += 180;
			break;
		}

		usercmd->viewangles.yaw = curYaw + Clientvariables->Rage.iYawMovingAdd;
	}
	else
	{
		float curYaw = usercmd->viewangles.yaw;

		static float add = 0;
		static int Ticks = 0;
		float temp_base = curYaw;
		static bool reset = true;

		switch (Clientvariables->Rage.iYawType)
		{
		case 0:
			return;
			break;

		case 1:
			curYaw -= Ticks;
			Ticks += 2;

			if (Ticks > 240)
				Ticks = 120;
			break;

		case 2:
			if (reset)
			{
				add = 0;
				reset = false;
			}

			curYaw += 135;
			curYaw += add;
			add += 15;

			if (temp_base + 225 < curYaw)
			{
				reset = true;
				curYaw = temp_base + 225;
			}
			break;

		case 3:
			break;

		case 4:
			curYaw += 180 + ((rand() % 15) - (15 * 0.5f));
			break;

		case 5:
			break;

		case 6:
			curYaw += 180;
			break;
		}

		usercmd->viewangles.yaw = curYaw + Clientvariables->Rage.iYawAdd;
	}
}

void AntiAim::FakeYaw()
{
	if (Clientvariables->Rage.iYawType > 0)
	{
		usercmd->viewangles.yaw = Utils::RandomInt(-180, 180);
	}
}

void AntiAim::Fakewalk(CUserCmd* userCMD)
{
	if (!Clientvariables->Rage.bFakewalk || !g_InputSystem->IsButtonDown(MOUSE_4))
		return;

	auto velocity = Global::vecUnpredictedVel;

	static auto predict_velocity = [](Vector* velocity) 
	{
		float speed = velocity->Length();
		static auto sv_friction = g_CVar->FindVar("sv_friction");
		static auto sv_stopspeed = g_CVar->FindVar("sv_stopspeed");

		if (speed >= 0.1f) 
		{
			float friction = sv_friction->GetFloat();
			float stop_speed = std::max< float >(speed, sv_stopspeed->GetFloat());
			float time = std::max< float >(g_GlobalVars->interval_per_tick, g_GlobalVars->frametime);
			*velocity *= std::max< float >(0.f, speed - friction * stop_speed * time / speed);
		}
	};

	static auto QuickStop = []()
	{
		Vector velocity = Global::vecUnpredictedVel;
		float speed = velocity.Length2D();

		if (speed > 13.f) 
		{
			QAngle direction;
			Math::VectorAngles(velocity, direction);
			direction.yaw = Global::userCMD->viewangles.yaw - direction.yaw;
			Vector forward;
			Math::AngleVectors(direction, forward);
			Vector negated_direction = forward * -speed;
			Global::userCMD->forwardmove = negated_direction.x;
			Global::userCMD->sidemove = negated_direction.y;
		}
		else
		{
			Global::userCMD->forwardmove = 0.f;
			Global::userCMD->sidemove = 0.f;
		}
	};

	int ticks_to_stop;
	int ticks_to_update = Global::NextPredictedLBYUpdate - 1;

	for (ticks_to_stop = 0; ticks_to_stop < 15; ++ticks_to_stop) 
	{
		if (velocity.Length2D() < 0.1f)
			break;

		predict_velocity(&velocity);
	}

	const int max_ticks = std::min< int >(Clientvariables->Miscellaneous.iFakelagStand, ticks_to_update);
	const int choked = g_ClientState->chokedcommands;
	int ticks_left = max_ticks - choked;

	if (choked < max_ticks || ticks_to_stop)
		Global::ChokeAmount = 14;

	if (ticks_to_stop > ticks_left || !choked || Global::bSendPacket)
		QuickStop();
}

void AntiAim::Freestanding(CUserCmd* cmd)
{
	if (Entities.empty())
	{
		RealYaw();
		return;
	}

	auto eyepos = g_LocalPlayer->GetEyePos();
	auto origin = g_LocalPlayer->GetAbsOrigin();
	auto headpos = g_LocalPlayer->GetHitboxPos(0);

	auto checkWallThickness = [&](C_BasePlayer* pPlayer, Vector newhead) -> float
	{
		Ray_t ray;
		trace_t trace1, trace2;
		Vector endpos1, endpos2;
		Vector eyepos = pPlayer->GetEyePos();
		CTraceFilterSkipTwoEntities filter(pPlayer, g_LocalPlayer);

		ray.Init(newhead, eyepos);
		g_EngineTrace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace1);

		if (trace1.DidHit())
			endpos1 = trace1.endpos;
		else
			return 0.f;

		ray.Init(eyepos, newhead);
		g_EngineTrace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace2);

		if (trace2.DidHit())
			endpos2 = trace2.endpos;

		float add = newhead.Dist(eyepos) - g_LocalPlayer->GetEyePos().Dist(eyepos) + 3.f;
		return endpos1.Dist(endpos2) + add / 3;
	};

	Vector besthead;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	float step = M_PI * 2.0 / 15;
	float radius = Vector(headpos - origin).Length2D();

	for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step)
	{
		float totalthickness = 0.f;
		Vector newhead(radius * cos(rotation) + eyepos.x, radius * sin(rotation) + eyepos.y, eyepos.z);

		for (auto current : Entities)
			totalthickness += checkWallThickness(reinterpret_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(current.iPlayer)), newhead);

		if (totalthickness > highestthickness)
		{
			highestthickness = totalthickness;
			bestrotation = rotation;
			besthead = newhead;
		}
	}

	if (highestthickness != 0)
	{
		cmd->viewangles.yaw = RAD2DEG(bestrotation);
	}
	else
	{
		RealYaw();
	}
}

int AntiAim::GetFPS()
{
	static int fps = 0;
	static int count = 0;
	using namespace std::chrono;
	auto now = high_resolution_clock::now();
	static auto last = high_resolution_clock::now();
	count++;

	if (duration_cast<milliseconds>(now - last).count() > 1000)
	{
		fps = count;
		count = 0;
		last = now;
	}

	return fps;
}

void AntiAim::LBYBreaker(CUserCmd* cmd)
{
	if (!Clientvariables->Rage.bLBYBreaker)
		return;

	if (Clientvariables->Rage.iYawType < 1)
		return;

	static int choked = 0;
	static bool wasMoving = true;
	static bool preBreak = false;
	static bool shouldBreak = false;
	static bool brokeThisTick = false;
	static float oldCurtime = g_GlobalVars->curtime;

	if (Global::bSendPacket)
	{
		brokeThisTick = false;
		choked = g_ClientState->chokedcommands;

		if (g_LocalPlayer->m_vecVelocity().Length2D() >= 0.1f && (g_LocalPlayer->m_fFlags() & FL_ONGROUND))
		{
			oldCurtime = g_GlobalVars->curtime;
			wasMoving = true;
		}
		else
		{
			if (wasMoving && g_GlobalVars->curtime - oldCurtime > 0.22f)
			{
				wasMoving = false;
				shouldBreak = true;
				Global::NextPredictedLBYUpdate = g_GlobalVars->curtime;
			}
			else if (g_GlobalVars->curtime - oldCurtime > 1.1f)
			{
				shouldBreak = true;
				Global::NextPredictedLBYUpdate = g_GlobalVars->curtime;
			}
			else if (g_GlobalVars->curtime - oldCurtime > 1.1f - TICKS_TO_TIME(choked) - TICKS_TO_TIME(2))
				preBreak = true;
		}
	}
	else if (shouldBreak)
	{
		shouldBreak = false;
		brokeThisTick = true;
		oldCurtime = g_GlobalVars->curtime;
		cmd->viewangles.yaw += Clientvariables->Rage.iLBYDelta;
	}
	else if (preBreak)
	{
		preBreak = false;
		brokeThisTick = true;
		float addAngle = GetFPS() >= (TIME_TO_TICKS(1.f) * 0.5f) ? (2.9 * max(choked, Global::prevChoked) + 100) : 145.f;
		cmd->viewangles.yaw += Clientvariables->Rage.iLBYDelta + addAngle;
	}

	if (brokeThisTick)
	{
		if (AimRage::Get().local_weapon->m_iItemDefinitionIndex() == WEAPON_REVOLVER)
		{
			cmd->buttons &= ~IN_ATTACK;
			cmd->buttons &= ~IN_ATTACK2;
		}

		else if (!AimRage::Get().local_weapon->IsMiscellaneousWeapon())
			cmd->buttons &= ~IN_ATTACK;
	}
}