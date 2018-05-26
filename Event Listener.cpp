#include "Event Listener.hpp"

player_info_t GetInfo(int Index)
{
	player_info_t Info;
	g_EngineClient->GetPlayerInfo(Index, &Info);
	return Info;
}

CGameEvents* g_pGameEvents;

CGameEvents::WeaponFireListener weapon_fire_listener;
CGameEvents::PlayerHurtListener player_hurt_listener;
CGameEvents::ItemPurchaseListener item_purchase_listener;
CGameEvents::BulletImpactListener bullet_impact_listener;

void CGameEvents::InitializeEventListeners()
{
	g_GameEvents->AddListener(&weapon_fire_listener, "weapon_fire", false);
	g_GameEvents->AddListener(&player_hurt_listener, "player_hurt", false);
	g_GameEvents->AddListener(&item_purchase_listener, "item_purchase", false);
	g_GameEvents->AddListener(&bullet_impact_listener, "bullet_impact", false);
}

void CGameEvents::WeaponFireListener::FireGameEvent(IGameEvent* game_event)
{
	if (!game_event || !g_LocalPlayer)
		return;

	int32_t userid = game_event->GetInt("userid");
	auto entity = C_BasePlayer::GetPlayerByUserId(userid);

	if (entity != g_LocalPlayer)
		return;

	if (!strcmp(game_event->GetName(), "weapon_fire") && entity == g_LocalPlayer && !g_LocalPlayer->m_hActiveWeapon().Get()->IsMiscellaneousWeapon())
	{
		Global::WeaponFire = true;
		Global::LastTimeWeFired = g_GlobalVars->curtime;
	}

	if (Global::WeaponFire)
		Global::ShotsFiredLocally++;
	else
		Global::ShotsFiredLocally = 0;
}

void CGameEvents::PlayerHurtListener::FireGameEvent(IGameEvent* game_event)
{
	if (!game_event || !g_LocalPlayer)
		return;

	int32_t userid = game_event->GetInt("userid");
	auto entity = C_BasePlayer::GetPlayerByUserId(userid);

	if (!entity)
		return;

	auto entity_attacker = g_EntityList->GetClientEntity(g_EngineClient->GetPlayerForUserID(game_event->GetInt("attacker")));

	if (!entity_attacker)
		return;

	if (entity->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
		return;

	player_info_t player_info;

	if (!g_EngineClient->GetPlayerInfo(entity->GetIndex(), &player_info))
		return;

	int attacker_id = game_event->GetInt("attacker");
	int hurt_player_id = game_event->GetInt("userid");
	int iVictim = g_EngineClient->GetPlayerForUserID(game_event->GetInt("userid"));
	int iAttacker = g_EngineClient->GetPlayerForUserID(game_event->GetInt("attacker"));

	if (attacker_id == hurt_player_id)
		return;

	C_BaseEntity* pl = (C_BaseEntity*)g_EntityList->GetClientEntity(g_EngineClient->GetPlayerForUserID(hurt_player_id));

	if (entity_attacker == g_LocalPlayer && hurt_player_id != g_EngineClient->GetLocalPlayer())
	{
		Global::HitMarkerAlpha = 1.f;
		Global::ShotsHitPerEntity[entity->GetIndex()]++;

		if (game_event->GetInt("hitgroup") == 1)
			Global::HeadShottedEntity[pl->EntIndex()] = true;
		else
			Global::HeadShottedEntity[pl->EntIndex()] = false;

		if (Clientvariables->Visuals.bEnable && Clientvariables->Visuals.bHitSound)
		{
			if (Global::HeadShottedEntity[pl->EntIndex()])
			{
				g_EngineClient->ExecuteClientCmd("play buttons\\arena_switch_press_02.wav");
			}
			else
			{
				g_EngineClient->ExecuteClientCmd("play buttons\\arena_switch_press_02.wav");
			}
		}
	}

	if (Clientvariables->Rage.bLagComp)
	{
		int32_t userid = g_EngineClient->GetPlayerForUserID(game_event->GetInt("userid"));
		int32_t attacker = g_EngineClient->GetPlayerForUserID(game_event->GetInt("attacker"));

		/*if (attacker == g_EngineClient->GetLocalPlayer() && userid != g_EngineClient->GetLocalPlayer())
			Visuals::DrawCapsuleOverlay(userid, 0.8f);*/
	}

	if (Clientvariables->Visuals.bEventLogs)
	{
		auto HitgroupToString = [](int hitgroup) -> std::string
		{
			switch (hitgroup)
			{
			case HITGROUP_HEAD:
				return "head";
			case HITGROUP_CHEST:
				return "chest";
			case HITGROUP_STOMACH:
				return "stomach";
			case HITGROUP_LEFTARM:
				return "left arm";
			case HITGROUP_RIGHTARM:
				return "right arm";
			case HITGROUP_LEFTLEG:
				return "left leg";
			case HITGROUP_RIGHTLEG:
				return "right leg";
			default:
				return "body";
			}
		};

		EventInfo info;
		std::stringstream msg;
		auto enemy = game_event->GetInt("userid");
		auto attacker = game_event->GetInt("attacker");
		auto hitgroup = game_event->GetInt("hitgroup");
		auto remaining_health = game_event->GetString("health");
		auto dmg_to_health = game_event->GetString("dmg_health");

		auto enemy_index = g_EngineClient->GetPlayerForUserID(enemy);
		auto attacker_index = g_EngineClient->GetPlayerForUserID(attacker);

		auto pEnemy = C_BasePlayer::GetPlayerByIndex(enemy_index);
		auto pAttacker = C_BasePlayer::GetPlayerByIndex(attacker_index);

		player_info_t enemy_info;
		player_info_t attacker_info;

		if (pEnemy && pAttacker && g_EngineClient->GetPlayerInfo(attacker_index, &attacker_info) && g_EngineClient->GetPlayerInfo(enemy_index, &enemy_info))
		{
			if (attacker_index != g_EngineClient->GetLocalPlayer())
				return;

			info.m_flExpTime = g_GlobalVars->curtime + 4.f;
			std::string szHitgroup = HitgroupToString(hitgroup);
			msg << "Hit " << enemy_info.szName << " in the " << szHitgroup << " for " << dmg_to_health << " dmg " << "(" << remaining_health << " health remaining)";
			info.m_szMessage = msg.str();
			g_Visuals.eventInfo.emplace_back(info);
		}
	}
	else
		if (g_Visuals.eventInfo.size() > 0)
			g_Visuals.eventInfo.clear();

	if (Clientvariables->Rage.bResolver)
	{
		int32_t userid = game_event->GetInt("userid");
		auto player = C_BasePlayer::GetPlayerByUserId(userid);

		if (!player)
			return;

		int32_t idx = player->EntIndex();
		auto &player_recs = Resolver::Get().arr_infos[idx];

		if (!player->IsDormant())
		{
			int32_t local_id = g_EngineClient->GetLocalPlayer();
			int32_t attacker = g_EngineClient->GetPlayerForUserID(game_event->GetInt("attacker"));

			if (attacker == local_id)
			{
				int32_t tickcount = g_GlobalVars->tickcount;
				bool airJump = !(player->m_fFlags() & FL_ONGROUND) && player->m_vecVelocity().Length2D() > 1.0f;

				if (tickHitWall == tickcount)
				{
					player_recs.m_nShotsMissed = originalShotsMissed;
					player_recs.m_nCorrectedFakewalkIdx = originalCorrectedFakewalkIdx;
				}

				if (tickcount != tickHitPlayer)
				{
					tickHitPlayer = tickcount;
					player_recs.m_nShotsMissed = 0;

					if (!airJump)
					{
						if (++player_recs.m_nCorrectedFakewalkIdx > 7)
							player_recs.m_nCorrectedFakewalkIdx = 0;
					}
				}
			}
		}
	}

	const char* szEventName = game_event->GetName();

	if (!szEventName)
		return;

	if (!strcmp(szEventName, "round_start"))
		Global::NewRound = true;
	else
		Global::NewRound = false;
}

void CGameEvents::BulletImpactListener::FireGameEvent(IGameEvent* game_event)
{
	if (!game_event)
		return;

	int iUser = g_EngineClient->GetPlayerForUserID(game_event->GetInt("userid"));
	auto entity = reinterpret_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(iUser));

	if (!entity)
		return;

	if (entity->IsDormant())
		return;

	float x, y, z;
	x = game_event->GetFloat("x");
	y = game_event->GetFloat("y");
	z = game_event->GetFloat("z");
	BulletImpact_t impact(entity, Vector(x, y, z), g_GlobalVars->curtime, iUser == g_EngineClient->GetLocalPlayer() ? Color::Green() : Color::Red());
	g_Visuals.Impacts.push_back(impact);

	if (Clientvariables->Rage.bResolver)
	{
		int32_t userid = g_EngineClient->GetPlayerForUserID(game_event->GetInt("userid"));

		if (userid == g_EngineClient->GetLocalPlayer())
		{
			if (AimRage::Get().prev_aimtarget != NULL)
			{
				auto player = C_BasePlayer::GetPlayerByIndex(AimRage::Get().prev_aimtarget);

				if (!player)
					return;

				int32_t idx = player->EntIndex();
				auto &player_recs = Resolver::Get().arr_infos[idx];

				if (!player->IsDormant())
				{
					int32_t tickcount = g_GlobalVars->tickcount;

					if (tickcount != tickHitWall)
					{
						tickHitWall = tickcount;
						originalShotsMissed = player_recs.m_nShotsMissed;
						originalCorrectedFakewalkIdx = player_recs.m_nCorrectedFakewalkIdx;

						if (tickcount != tickHitPlayer)
						{
							tickHitWall = tickcount;
							++player_recs.m_nShotsMissed;

							if (++player_recs.m_nCorrectedFakewalkIdx > 3)
								player_recs.m_nCorrectedFakewalkIdx = 0;
						}
					}
				}
			}
		}
	}
}

void CGameEvents::ItemPurchaseListener::FireGameEvent(IGameEvent* game_event)
{
	if (!game_event)
		return;

	if (!strcmp(game_event->GetName(), "item_purchase"))
	{
		int nDead = game_event->GetInt("userid");
		int nUserID = game_event->GetInt("attacker");

		if (nUserID == nDead)
			return;

		if (nDead || nUserID)
		{
			EventInfo info;
			std::stringstream msg;

			player_info_t killed_info = GetInfo(g_EngineClient->GetPlayerForUserID(nDead));
			player_info_t killer_info = GetInfo(g_EngineClient->GetPlayerForUserID(nUserID));

			std::string One = killed_info.szName;
			std::string Two = (" bought ");
			std::string Three = game_event->GetString("weapon");
			std::string Four = "\n";

			if (Clientvariables->Visuals.bEnable && Clientvariables->Visuals.bEventLogs)
			{
				info.m_flExpTime = g_GlobalVars->curtime + 4.f;
				msg << One << Two << Three << Four;
				info.m_szMessage = msg.str();
				g_Visuals.eventInfo.emplace_back(info);
			}
			else
				if (g_Visuals.eventInfo.size() > 0)
					g_Visuals.eventInfo.clear();
		}
	}
}