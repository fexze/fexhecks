#include "Fakelag.h"

int CFakelag::BreakLagCompensation()
{
	Vector velocity = g_LocalPlayer->m_vecVelocity();
	velocity.z = 0;
	float speed = velocity.Length();

	if (speed > 0.f)
	{
		auto distance_per_tick = speed * g_GlobalVars->interval_per_tick;
		int choked_ticks = std::ceilf(65.f / distance_per_tick);
		return std::min<int>(choked_ticks, 14);
	}

	return 1;
}

void CFakelag::Fakelag(CUserCmd* userCMD)
{
	if (!Clientvariables->Miscellaneous.bFakelag || g_InputSystem->IsButtonDown(MOUSE_4))
		return;

	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive())
		return;

	if (Global::bAimbotting && userCMD->buttons & IN_ATTACK)
		return;

	if (Clientvariables->Miscellaneous.bBreakLagComp)
		Global::ChokeAmount = BreakLagCompensation();
	else
	{
		if (!(g_LocalPlayer->m_fFlags() & FL_ONGROUND))
		{
			Global::ChokeAmount = Clientvariables->Miscellaneous.iFakelagJump;
		}
		else if (g_LocalPlayer->m_vecVelocity().Length2D() >= 0.1f && (g_LocalPlayer->m_fFlags() & FL_ONGROUND))
		{
			Global::ChokeAmount = Clientvariables->Miscellaneous.iFakelagMove;
		}
		else
		{
			Global::ChokeAmount = Clientvariables->Miscellaneous.iFakelagStand;
		}
	}
}