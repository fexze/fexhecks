#include "Gamehooking.hpp"
#include "Utils.hpp"

#include "Menu.hpp"
#include "Options.hpp"

#include "Math.hpp"

#include "Visuals.hpp"
#include "Miscellaneous.hpp"
#include "PredictionSystem.hpp"
#include "AimRage.hpp"
#include "LagCompensation.hpp"
#include "Resolver.hpp"
#include "AntiAim.hpp"
#include "GrenadePrediction.h"
#include "Skinchanger.hpp"

#include <intrin.h>
#include <experimental/filesystem>

#include "Material Helper.hpp"
#include "Event Listener.hpp"
#include "Fonts.hpp"
#include "Fakelag.h"

int32_t tickHitWall = 0;
int32_t tickHitPlayer = 0;
int32_t originalShotsMissed = 0;
int32_t originalCorrectedFakewalkIdx = 0;

#ifdef INSTANT_DEFUSE_PLANT_EXPLOIT
int32_t nTickBaseShift = 0;
int32_t nSinceUse = 0;
bool bInSendMove = false, bFirstSendMovePack = false;
#endif

namespace Global
{
	bool bVisualAimbotting = false;
	QAngle vecVisualAimbotAngs = QAngle(0.f, 0.f, 0.f);
	int ChokeAmount = 0;
	float flFakewalked = 0.f;
	bool NewRound = false;
	bool WeaponFire = false;
	float LastTimeWeFired = 0;
	int ShotsFiredLocally = 0;
	int ShotsHitPerEntity[65];
	bool HeadShottedEntity[65] = { false };
	float curFov = 0;
	bool bUsingFakeAngles[65];
	float HitMarkerAlpha = 0.f;
	int TicksOnGround = 0;
	int AnimationPitchFix = 0;
	int NextPredictedLBYUpdate = 0;
	int prevChoked = 0;
	bool AAFlip = false;
	char my_documents_folder[MAX_PATH];
	float smt = 0.f;
	QAngle visualAngles = QAngle(0.f, 0.f, 0.f);
	bool bSendPacket = false;
	bool bAimbotting = false;
	CUserCmd* userCMD = nullptr;
	char* szLastFunction = "<No function was called>";
	HMODULE hmDll = nullptr;
	bool bFakewalking = false;
	Vector vecUnpredictedVel = Vector(0, 0, 0);
	float flFakeLatencyAmount = 0.f;
	float flEstFakeLatencyOnServer = 0.f;
	matrix3x4_t traceHitboxbones[128];
	std::array<std::string, 64> resolverModes;
}

void __fastcall Handlers::PaintTraverse_h(void *thisptr, void*, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	static uint32_t HudZoomPanel;

	if (!HudZoomPanel)
		if (!strcmp("HudZoom", g_VGuiPanel->GetName(vguiPanel)))
			HudZoomPanel = vguiPanel;

	if (HudZoomPanel == vguiPanel && Clientvariables->Visuals.bRemoveScope && g_LocalPlayer && g_LocalPlayer->m_hActiveWeapon().Get())
	{
		if (g_LocalPlayer->m_hActiveWeapon().Get()->IsSniper() && g_LocalPlayer->m_bIsScoped())
			return;
	}

	o_PaintTraverse(thisptr, vguiPanel, forceRepaint, allowForce);
	static uint32_t FocusOverlayPanel;

	if (!FocusOverlayPanel)
	{
		const char* szName = g_VGuiPanel->GetName(vguiPanel);

		if (lstrcmpA(szName, "MatSystemTopPanel") == 0)
		{
			FocusOverlayPanel = vguiPanel;

			if (FONTS::ShouldReloadFonts())
				FONTS::InitializeFonts();

			g_EngineClient->ExecuteClientCmd("clear");
			Skinchanger::Get().LoadSkins();
		}
	}

	if (FocusOverlayPanel == vguiPanel)
	{
		if (g_LocalPlayer && Clientvariables->Visuals.bEnable && g_EngineClient->IsInGame() && g_EngineClient->IsConnected())
		{
			g_Visuals.Draw();
			g_Visuals.ClientDraw();

			if (Clientvariables->Visuals.bRemoveScope && (g_LocalPlayer && g_LocalPlayer->m_hActiveWeapon().Get() && g_LocalPlayer->m_hActiveWeapon().Get()->IsSniper() && g_LocalPlayer->m_bIsScoped()))
			{
				int screenX, screenY;
				g_EngineClient->GetScreenSize(screenX, screenY);
				g_VGuiSurface->DrawSetColor(CColor(0, 0, 0, 255));
				g_VGuiSurface->DrawLine(screenX / 2, 0, screenX / 2, screenY);
				g_VGuiSurface->DrawLine(0, screenY / 2, screenX, screenY / 2);
			}

			for (int i = 1; i <= g_EntityList->GetHighestEntityIndex(); i++)
			{
				C_BasePlayer* entity = C_BasePlayer::GetPlayerByIndex(i);

				if (!entity)
					continue;

				if (Clientvariables->Visuals.bDroppedWeapons && entity->IsWeapon())
					g_Visuals.DroppedWeapons((C_BaseCombatWeapon*)entity);

				if (Clientvariables->Visuals.bEnemyThrowables)
					g_Visuals.RenderNadeEsp((C_BaseCombatWeapon*)entity);
			}

			g_Visuals.EventLogs();

			if (Clientvariables->Visuals.bNadePrediction)
				CCSGrenadeHint::Get().Paint();

			if (Clientvariables->Miscellaneous.bDevInformation)
			{
				RENDER::DrawF(0, 705, FONTS::visuals_information_font, false, false, CColor(0, 255, 0, 255), ("Real: " + std::to_string(Global::visualAngles.yaw)));
				RENDER::DrawF(0, 720, FONTS::visuals_information_font, false, false, CColor(255, 255, 0, 255), ("LBY: " + std::to_string(g_LocalPlayer->m_flLowerBodyYawTarget())));
				RENDER::DrawF(0, 740, FONTS::visuals_information_font, false, false, CColor(255, 255, 0, 255), ("Choked Packets: " + std::to_string(g_ClientState->chokedcommands)));
				RENDER::DrawF(0, 755, FONTS::visuals_information_font, false, false, CColor(255, 255, 0, 255), ("Previously Choked Packets: " + std::to_string(Global::prevChoked)));
				RENDER::DrawF(0, 775, FONTS::visuals_information_font, false, false, CColor(255, 0, 255, 255), ("Ticks On Ground: " + std::to_string(Global::TicksOnGround)));
				RENDER::DrawF(0, 795, FONTS::visuals_information_font, false, false, CColor(50, 135, 255, 255), ("Server Time: " + std::to_string(g_GlobalVars->curtime)));
				RENDER::DrawF(0, 815, FONTS::visuals_information_font, false, false, CColor(0, 255, 0, 255), ("Next LBY Update: " + std::to_string(Global::NextPredictedLBYUpdate)));
				RENDER::DrawF(0, 835, FONTS::visuals_information_font, false, false, CColor(255, 0, 0, 255), ("Shots Fired: " + std::to_string(Global::ShotsFiredLocally)));
				RENDER::DrawF(0, 850, FONTS::visuals_information_font, false, false, CColor(255, 0, 0, 255), ("Last Time We Fired: " + std::to_string(Global::LastTimeWeFired)));

				if (Global::WeaponFire)
					RENDER::DrawF(0, 865, FONTS::visuals_information_font, false, false, CColor(255, 0, 0, 255), ("Weapon Fired: Yes"));
				else
					RENDER::DrawF(0, 865, FONTS::visuals_information_font, false, false, CColor(255, 0, 0, 255), ("Weapon Fired: No"));
			}
		}
	}
}

bool __stdcall Handlers::CreateMove_h(float smt, CUserCmd *userCMD)
{
	if (!userCMD->command_number || !g_EngineClient->IsInGame() || !g_LocalPlayer || !g_LocalPlayer->IsAlive())
		return o_CreateMove(g_ClientMode, smt, userCMD);

#ifdef INSTANT_DEFUSE_PLANT_EXPLOIT
	if (g_Options.misc_instant_defuse_plant && (userCMD->buttons & IN_USE))
	{
		if (nSinceUse++ < 3)
			nTickBaseShift = TIME_TO_TICKS(11.f);
	}
	else
		nSinceUse = 0;
#endif

	/*if (g_ClientState->m_NetChannel && !g_pNetChannelHook)
	{
		g_pNetChannelHook->Setup(GetNetChannel1());
		g_pNetChannelHook->Hook(42, Handlers::SendNetMsg_h);
	}*/

	QAngle org_angle = userCMD->viewangles;

	uintptr_t* framePtr;
	__asm mov framePtr, ebp;

	Global::smt = smt;
	Global::userCMD = userCMD;
	Global::bSendPacket = true;
	Global::vecUnpredictedVel = g_LocalPlayer->m_vecVelocity();

	if (Clientvariables->Miscellaneous.bAutoJump)
		Miscellaneous::Get().Bhop(userCMD);

	if (Clientvariables->Miscellaneous.bAutoStrafe)
		Miscellaneous::Get().AutoStrafe(userCMD);

	QAngle wish_angle = userCMD->viewangles;
	userCMD->viewangles = org_angle;

	if (!AimRage::Get().can_fire_weapon && !g_LocalPlayer->m_hActiveWeapon().Get()->IsMiscellaneousWeapon())
		userCMD->buttons &= ~IN_ATTACK;

	PredictionSystem::Get().Start(userCMD, g_LocalPlayer);
	{
		if (Clientvariables->Miscellaneous.bFakelag || g_InputSystem->IsButtonDown(MOUSE_4))
			Global::bSendPacket = false;

		if (g_ClientState->chokedcommands > 13 || (g_ClientState->chokedcommands == Global::ChokeAmount &&
			(Clientvariables->Miscellaneous.bFakelag || g_InputSystem->IsButtonDown(MOUSE_4))))
			Global::bSendPacket = true;

		AntiAim::Get().Fakewalk(userCMD);
		CFakelag::Get().Fakelag(userCMD);
		Miscellaneous::Get().AutoPistol(userCMD);
		AimRage::Get().Work(userCMD);
		AimRage::Get().RemoveRecoil(userCMD);
		Miscellaneous::Get().AntiAim(userCMD);
		AntiAim::Get().LBYBreaker(userCMD);
		Miscellaneous::Get().FixMovement(userCMD, wish_angle);
	}
	PredictionSystem::Get().End(g_LocalPlayer);

	CCSGrenadeHint::Get().Tick(userCMD->buttons);
	CBaseEntityAnimState* Animations = g_LocalPlayer->GetAnimState();

	if (g_LocalPlayer->m_fFlags() & FL_ONGROUND)
		Global::TicksOnGround++;
	else
		Global::TicksOnGround = 0;

	if (Animations->m_bInHitGroundAnimation && Animations->m_flHeadHeightOrOffsetFromHittingGroundAnimation
		&& Global::TicksOnGround > 1 && !g_LocalPlayer->m_hActiveWeapon().Get()->IsMiscellaneousWeapon() && Clientvariables->Rage.iYawType != 2)
		Global::AnimationPitchFix = -10;
	else
		Global::AnimationPitchFix = userCMD->viewangles.pitch;

	if (Clientvariables->Rage.bEnable && Global::bAimbotting && userCMD->buttons & IN_ATTACK)
		*(bool*)(*framePtr - 0x1C) = false;

	*(bool*)(*framePtr - 0x1C) = Global::bSendPacket;

	if (!Global::bSendPacket)
		Global::visualAngles = QAngle(Global::AnimationPitchFix, userCMD->viewangles.yaw, userCMD->viewangles.roll);

	if (g_LocalPlayer->m_hActiveWeapon().Get()->IsMiscellaneousWeapon())
	{
		Global::WeaponFire = false;
		Global::ShotsFiredLocally = 0;
	}

	if ((g_GlobalVars->curtime - Global::LastTimeWeFired) > 3 && g_LocalPlayer->m_hActiveWeapon().Get()->IsSniperRifle())
	{
		Global::WeaponFire = false;
		Global::ShotsFiredLocally = 0;
	}
	else if ((g_GlobalVars->curtime - Global::LastTimeWeFired) > 1 && g_LocalPlayer->m_hActiveWeapon().Get()->IsAutoSniper()
		&& !g_LocalPlayer->m_hActiveWeapon().Get()->IsSniperRifle())
	{
		Global::WeaponFire = false;
		Global::ShotsFiredLocally = 0;
	}
	else if ((g_GlobalVars->curtime - Global::LastTimeWeFired) > 1 && !g_LocalPlayer->m_hActiveWeapon().Get()->IsSniperRifle())
	{
		Global::WeaponFire = false;
		Global::ShotsFiredLocally = 0;
	}

	if (Clientvariables->Miscellaneous.bAntiUntrusted)
	{
		userCMD->forwardmove = Miscellaneous::Get().clamp(userCMD->forwardmove, -450.f, 450.f);
		userCMD->sidemove = Miscellaneous::Get().clamp(userCMD->sidemove, -450.f, 450.f);
		userCMD->upmove = Miscellaneous::Get().clamp(userCMD->upmove, -320.f, 320.f);
		userCMD->viewangles.Clamp();
	}

	if (!Clientvariables->Rage.bSilentAimbot && Global::bVisualAimbotting)
		g_EngineClient->SetViewAngles(Global::vecVisualAimbotAngs);

	if (!o_TempEntities)
	{
		g_pClientStateHook->Setup((uintptr_t*)((uintptr_t)g_ClientState + 0x8));
		g_pClientStateHook->Hook(36, Handlers::TempEntities_h);
		o_TempEntities = g_pClientStateHook->GetOriginal<TempEntities_t>(36);
	}

	if (Global::bSendPacket)
		Global::prevChoked = g_ClientState->chokedcommands;

	return false;
}

void __stdcall Handlers::PlaySound_h(const char *folderIme)
{
	o_PlaySound(g_VGuiSurface, folderIme);

	if (!Clientvariables->Miscellaneous.bAutoAccept)
		return;

	if (!strcmp(folderIme, "!UI/competitive_accept_beep.wav"))
	{
		Utils::IsReady();

		FLASHWINFO flash;
		flash.cbSize = sizeof(FLASHWINFO);
		flash.hwnd = window;
		flash.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
		flash.uCount = 0;
		flash.dwTimeout = 0;
		FlashWindowEx(&flash);
	}
}

HRESULT __stdcall Handlers::EndScene_h(IDirect3DDevice9 *pDevice)
{
	if ((g_EngineClient->IsInGame() && g_EngineClient->IsConnected()) && Clientvariables->Miscellaneous.bRevealRanks && g_InputSystem->IsButtonDown(KEY_TAB))
		Utils::RankRevealAll();

	Menu::Get().Render();
	return o_EndScene(pDevice);
}

HRESULT __stdcall Handlers::Reset_h(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters)
{
	Menu::Get().OnDeviceLost();
	auto hr = o_Reset(pDevice, pPresentationParameters);

	if (hr >= 0)
	{
		Menu::Get().OnDeviceReset();
		FONTS::InitializeFonts();
	}

	return hr;
}

ConVar* r_DrawSpecificStaticProp;

void __stdcall Handlers::FrameStageNotify_h(ClientFrameStage_t stage)
{
	g_LocalPlayer = C_BasePlayer::GetLocalPlayer(true);

	static int OldSky;
	static bool OldNightmode;
	static std::string old_Skyname = "";

	if (!g_LocalPlayer || !g_EngineClient->IsInGame() || !g_EngineClient->IsConnected())
	{
		OldSky = 0;
		old_Skyname = "";
		OldNightmode = false;
		return o_FrameStageNotify(stage);
	}

	QAngle aim_punch_old;
	QAngle view_punch_old;

	QAngle* aim_punch = nullptr;
	QAngle* view_punch = nullptr;

	if (OldNightmode != Clientvariables->Visuals.bNightMode)
	{
		if (!r_DrawSpecificStaticProp)
			r_DrawSpecificStaticProp = g_CVar->FindVar("r_DrawSpecificStaticProp");

		r_DrawSpecificStaticProp->SetValue(0);

		for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
		{
			IMaterial* pMaterial = g_MatSystem->GetMaterial(i);

			if (!pMaterial)
				continue;

			if (strstr(pMaterial->GetTextureGroupName(), "World") || strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
			{
				if (Clientvariables->Visuals.bNightMode)
				{
					if (strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
						pMaterial->ColorModulate(0.3f, 0.3f, 0.3f);
					else
						pMaterial->ColorModulate(0.05f, 0.05f, 0.05f);
				}
				else
					pMaterial->ColorModulate(1.0f, 1.0f, 1.0f);
			}
		}

		OldNightmode = Clientvariables->Visuals.bNightMode;
	}

	if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		if (Clientvariables->Visuals.bSkinChanger)
			Skinchanger::Get().Work();

		Miscellaneous::Get().PunchAngleFix_FSN();
	}

	if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_END)
	{
		// Here we are disabling client sided interpolation.
		for (int i = 1; i <= g_EntityList->GetHighestEntityIndex(); i++)
		{
			C_BasePlayer* player = C_BasePlayer::GetPlayerByIndex(i);

			if (!player)
				continue;

			if (player == g_LocalPlayer)
				continue;

			if (!player->IsAlive())
				continue;

			if (player->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
				continue;

			VarMapping_t* map = player->VarMapping();

			if (map)
			{
				for (int j = 0; j < map->m_nInterpolatedEntries; j++)
				{
					map->m_Entries[j].m_bNeedsToInterpolate = !Clientvariables->Rage.bLagComp;
				}
			}
		}

		// Resolving enemy enitites anti-aim angles.
		if (Clientvariables->Rage.bResolver)
			Resolver::Get().Resolve();
	}

	if (stage == ClientFrameStage_t::FRAME_RENDER_START)
	{
		*(bool*)Offsets::bOverridePostProcessingDisable = Clientvariables->Visuals.bRemoveScope;

		if (g_LocalPlayer->IsAlive())
		{
			static ConVar* default_skyname = g_CVar->FindVar("sv_skyname");
			static int iOldSky = 0;

			if (iOldSky != Clientvariables->Visuals.iSkyBox)
			{
				Utils::LoadNamedSkys(Clientvariables->Visuals.iSkyBox == 0 ? default_skyname->GetString() : SkyBoxes[Clientvariables->Visuals.iSkyBox]);
				iOldSky = Clientvariables->Visuals.iSkyBox;
			}

			// Removal of visual recoil.
			if (Clientvariables->Visuals.bRemoveVisualRecoil)
			{
				aim_punch = &g_LocalPlayer->m_aimPunchAngle();
				view_punch = &g_LocalPlayer->m_viewPunchAngle();

				aim_punch_old = *aim_punch;
				view_punch_old = *view_punch;

				*aim_punch = QAngle(0.f, 0.f, 0.f);
				*view_punch = QAngle(0.f, 0.f, 0.f);
			}

			// Applying anti-aim angles to our thirdperson character model.
			if (*(bool*)((uintptr_t)g_Input + 0xA5))
				g_LocalPlayer->visuals_Angles() = Global::visualAngles;

			if (Clientvariables->Visuals.iSmokeType > 0)
				*(int*)Offsets::smokeCount = 0;
		}

		// Removal of the flashbang effect.
		if (Clientvariables->Visuals.bRemoveFlash && g_LocalPlayer)
			if (g_LocalPlayer->m_flFlashDuration() > 0.f)
				g_LocalPlayer->m_flFlashDuration() = 0.f;
	}

	// Calling original function here so removing visual recoil works properly.
	o_FrameStageNotify(stage);

	if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_END)
	{
		if (Clientvariables->Rage.bResolver)
			Resolver::Get().Log();

		if (Clientvariables->Rage.bLagComp)
			CMBacktracking::Get().FrameUpdatePostEntityThink();
	}

	// Removal of visual recoil.
	if (stage == ClientFrameStage_t::FRAME_RENDER_START)
	{
		if (g_LocalPlayer && g_LocalPlayer->IsAlive())
		{
			if (Clientvariables->Visuals.bRemoveVisualRecoil && (aim_punch && view_punch))
			{
				*aim_punch = aim_punch_old;
				*view_punch = view_punch_old;
			}
		}
	}
}

bool __fastcall Handlers::FireEventClientSide_h(void *thisptr, void*, IGameEvent *gEvent)
{
	if (!gEvent)
		return o_FireEventClientSide(thisptr, gEvent);

	if (strcmp(gEvent->GetName(), "game_newmap") == 0)
	{
		static ConVar* default_skyname = g_CVar->FindVar("sv_skyname");
		Utils::LoadNamedSkys(Clientvariables->Visuals.iSkyBox == 0 ? default_skyname->GetString() : SkyBoxes[Clientvariables->Visuals.iSkyBox]);
	}

	return o_FireEventClientSide(thisptr, gEvent);
}

void __fastcall Handlers::BeginFrame_h(void *thisptr, void*, float ft)
{
	Miscellaneous::Get().NameChanger();
	Miscellaneous::Get().ChatSpamer();
	Miscellaneous::Get().ClanTag();
	g_Visuals.BulletBeams();
	o_BeginFrame(thisptr, ft);
}

void __stdcall Handlers::OverrideView_h(CViewSetup* pSetup)
{
	if (g_EngineClient->IsInGame() && g_EngineClient->IsConnected())
	{
		if (Clientvariables->Visuals.bFov1)
		{
			if (Clientvariables->Visuals.bRemoveZoom && g_LocalPlayer->m_bIsScoped())
				pSetup->fov = Clientvariables->Visuals.iFov1;

			if (!Clientvariables->Visuals.bRemoveZoom && !g_LocalPlayer->m_bIsScoped())
				pSetup->fov = Clientvariables->Visuals.iFov1;

			if (Clientvariables->Visuals.bRemoveZoom && !g_LocalPlayer->m_bIsScoped())
				pSetup->fov = Clientvariables->Visuals.iFov1;
		}

		if (!Clientvariables->Visuals.bFov1 && Clientvariables->Visuals.bRemoveZoom && g_LocalPlayer->m_bIsScoped())
			pSetup->fov = 90;
	}

	Global::curFov = pSetup->fov;
	o_OverrideView(pSetup);

	if (g_EngineClient->IsInGame() && g_EngineClient->IsConnected())
	{
		if (g_LocalPlayer)
		{
			CCSGrenadeHint::Get().View();
			Miscellaneous::Get().ThirdPerson();
		}
	}
}

void Proxies::didSmokeEffect(const CRecvProxyData *pData, void *pStruct, void *pOut)
{
	if (Clientvariables->Visuals.iSmokeType == 0)
		return;

	if (Clientvariables->Visuals.iSmokeType == 1)
		*(bool*)((DWORD)pOut + 0x1) = true;

	std::vector<const char*> wireframesmoke_mats =
	{
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
	};

	if (Clientvariables->Visuals.iSmokeType == 2)
	{
		for (auto smoke_mat : wireframesmoke_mats)
		{
			IMaterial* mat = g_MatSystem->FindMaterial(smoke_mat, TEXTURE_GROUP_OTHER);
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
		}
	}

	o_didSmokeEffect(pData, pStruct, pOut);
}

bool __fastcall Handlers::SetupBones_h(void* ECX, void* EDX, matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
{
	// Supposed to only setupbones tick by tick, instead of frame by frame.
	if (Clientvariables->Rage.bLagComp)
	{
		if (ECX && ((IClientRenderable*)ECX)->GetIClientUnknown())
		{
			IClientNetworkable* pNetworkable = ((IClientRenderable*)ECX)->GetIClientUnknown()->GetClientNetworkable();

			if (pNetworkable && pNetworkable->GetClientClass() && pNetworkable->GetClientClass()->m_ClassID == ClassId::ClassId_CCSPlayer)
			{
				static auto host_timescale = g_CVar->FindVar(("host_timescale"));
				auto player = (C_BasePlayer*)ECX;
				float OldCurTime = g_GlobalVars->curtime;
				float OldRealTime = g_GlobalVars->realtime;
				float OldFrameTime = g_GlobalVars->frametime;
				float OldAbsFrameTime = g_GlobalVars->absoluteframetime;
				float OldAbsFrameTimeStart = g_GlobalVars->absoluteframestarttimestddev;
				float OldInterpAmount = g_GlobalVars->interpolation_amount;
				int OldFrameCount = g_GlobalVars->framecount;
				int OldTickCount = g_GlobalVars->tickcount;

				g_GlobalVars->curtime = player->m_flSimulationTime();
				g_GlobalVars->realtime = player->m_flSimulationTime();
				g_GlobalVars->frametime = g_GlobalVars->interval_per_tick * host_timescale->GetFloat();
				g_GlobalVars->absoluteframetime = g_GlobalVars->interval_per_tick * host_timescale->GetFloat();
				g_GlobalVars->absoluteframestarttimestddev = player->m_flSimulationTime() - g_GlobalVars->interval_per_tick * host_timescale->GetFloat();
				g_GlobalVars->interpolation_amount = 0;
				g_GlobalVars->framecount = TIME_TO_TICKS(player->m_flSimulationTime());
				g_GlobalVars->tickcount = TIME_TO_TICKS(player->m_flSimulationTime());

				*(int*)((int)player + 236) |= 8;
				bool ret_value = o_SetupBones(player, pBoneToWorldOut, nMaxBones, boneMask, g_GlobalVars->curtime);
				*(int*)((int)player + 236) &= ~8;

				g_GlobalVars->curtime = OldCurTime;
				g_GlobalVars->realtime = OldRealTime;
				g_GlobalVars->frametime = OldFrameTime;
				g_GlobalVars->absoluteframetime = OldAbsFrameTime;
				g_GlobalVars->absoluteframestarttimestddev = OldAbsFrameTimeStart;
				g_GlobalVars->interpolation_amount = OldInterpAmount;
				g_GlobalVars->framecount = OldFrameCount;
				g_GlobalVars->tickcount = OldTickCount;
				return ret_value;
			}
		}
	}

	return o_SetupBones(ECX, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
}

void __fastcall Handlers::SceneEnd_h(void* thisptr, void* edx)
{
	if (!g_LocalPlayer || !g_EngineClient->IsInGame() || !g_EngineClient->IsConnected())
		return o_SceneEnd(thisptr);

	o_SceneEnd(thisptr);

	static IMaterial* Ignorez = chams->CreateMaterial(true, true, false);
	static IMaterial* NotIgnorez = chams->CreateMaterial(false, true, false);

	for (int i = 1; i <= 65; i++)
	{
		C_BasePlayer* entity = C_BasePlayer::GetPlayerByIndex(i);

		if (!entity)
			continue;

		if (!g_LocalPlayer)
			continue;

		bool IsLocalPlayer = entity == g_LocalPlayer;
		bool IsTeammate = g_LocalPlayer->m_iTeamNum() == entity->m_iTeamNum() && !IsLocalPlayer;

		if (IsLocalPlayer && !Clientvariables->Visuals.bEnable)
			continue;

		if (IsTeammate)
			continue;

		if (!IsLocalPlayer && entity && Clientvariables->Visuals.bChamsEnemy)
		{
			Ignorez->ColorModulate(Clientvariables->Visuals.cChamsEnemy[0], Clientvariables->Visuals.cChamsEnemy[1], Clientvariables->Visuals.cChamsEnemy[2]);
			g_MdlRender->ForcedMaterialOverride(Ignorez);
			entity->DrawModel(0x1, 255);
			g_MdlRender->ForcedMaterialOverride(nullptr);

			NotIgnorez->ColorModulate(Clientvariables->Visuals.cChamsEnemy[0], Clientvariables->Visuals.cChamsEnemy[1], Clientvariables->Visuals.cChamsEnemy[2]);
			g_MdlRender->ForcedMaterialOverride(NotIgnorez);
			entity->DrawModel(0x1, 255);
			g_MdlRender->ForcedMaterialOverride(nullptr);
		}

		if (IsLocalPlayer && g_LocalPlayer && Clientvariables->Visuals.bChamsLocal)
		{
			NotIgnorez->ColorModulate(Clientvariables->Visuals.cChamsLocal[0], Clientvariables->Visuals.cChamsLocal[1], Clientvariables->Visuals.cChamsLocal[2]);
			g_MdlRender->ForcedMaterialOverride(NotIgnorez);
			entity->DrawModel(0x1, 255);
			g_MdlRender->ForcedMaterialOverride(nullptr);
		}
	}

	for (auto i = 0; i <= g_GlowObjManager->GetSize(); i++)
	{
		auto& glowObject = g_GlowObjManager->m_GlowObjectDefinitions[i];
		auto entity = reinterpret_cast<C_BasePlayer*>(glowObject.m_pEntity);

		if (!entity)
			continue;

		if (glowObject.IsUnused())
			continue;

		bool IsLocalPlayer = entity == g_LocalPlayer;
		bool IsTeammate = g_LocalPlayer->m_iTeamNum() == entity->m_iTeamNum() && !IsLocalPlayer;

		if (IsLocalPlayer && !Clientvariables->Visuals.bEnable)
			continue;

		if (IsTeammate)
			continue;

		auto pClass = entity->GetClientClass()->m_ClassID;

		switch (pClass)
		{
		default:
			glowObject.m_flAlpha = 0.f;
			break;

		case 35:
			if (Clientvariables->Visuals.bGlowEnemy && entity->m_iTeamNum() != g_LocalPlayer->m_iTeamNum())
			{
				if (Clientvariables->Visuals.iGlowStyleEnemy == 0)
				{
					glowObject.m_nGlowStyle = 0;
				}
				else if (Clientvariables->Visuals.iGlowStyleEnemy == 1)
				{
					glowObject.m_nGlowStyle = 1;
				}

				glowObject.m_flAlpha = 1.f;
				glowObject.m_flRed = Clientvariables->Visuals.cGlowEnemy[0];
				glowObject.m_flGreen = Clientvariables->Visuals.cGlowEnemy[1];
				glowObject.m_flBlue = Clientvariables->Visuals.cGlowEnemy[2];
				glowObject.m_bRenderWhenOccluded = true;
				glowObject.m_bRenderWhenUnoccluded = false;
			}

			if (Clientvariables->Visuals.bGlowLocal && entity== g_LocalPlayer)
			{
				if (Clientvariables->Visuals.iGlowStyleLocal == 0)
				{
					glowObject.m_nGlowStyle = 0;
				}
				else if (Clientvariables->Visuals.iGlowStyleLocal == 1)
				{
					glowObject.m_nGlowStyle = 1;
				}

				glowObject.m_flAlpha = 1.f;
				glowObject.m_flRed = Clientvariables->Visuals.cGlowLocal[0];
				glowObject.m_flGreen = Clientvariables->Visuals.cGlowLocal[1];
				glowObject.m_flBlue = Clientvariables->Visuals.cGlowLocal[2];
				glowObject.m_bRenderWhenOccluded = true;
				glowObject.m_bRenderWhenUnoccluded = false;
			}
			break;
		}
	}
}

void __stdcall FireBullets_PostDataUpdate(C_TEFireBullets *thisptr, DataUpdateType_t updateType)
{
	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive())
		return o_FireBullets(thisptr, updateType);

	if (Clientvariables->Rage.bResolver && Clientvariables->Rage.bLagComp && thisptr)
	{
		int iPlayer = thisptr->m_iPlayer + 1;
		if (iPlayer < 64)
		{
			auto player = C_BasePlayer::GetPlayerByIndex(iPlayer);
			
			if (player && player != g_LocalPlayer && !player->IsDormant() && player->m_iTeamNum() != g_LocalPlayer->m_iTeamNum())
			{
				QAngle calcedAngle = Math::CalcAngle(player->GetEyePos(), g_LocalPlayer->GetEyePos());

				player->m_angEyeAngles().pitch = calcedAngle.pitch;
				player->m_angEyeAngles().yaw = calcedAngle.yaw;
				player->m_angEyeAngles().roll = 0.f;

				float
					event_time = g_GlobalVars->tickcount,
					player_time = player->m_flSimulationTime();

				auto lag_records = CMBacktracking::Get().m_LagRecord[iPlayer];

				float shot_time = TICKS_TO_TIME(event_time);
				for (auto& record : lag_records)
				{
					if (record.m_iTickCount <= event_time)
					{
						shot_time = record.m_flSimulationTime + TICKS_TO_TIME(event_time - record.m_iTickCount); // also get choked from this
#ifdef _DEBUG
						g_CVar->ConsoleColorPrintf(Color(0, 255, 0, 255), "Found <<exact>> shot time: %f, ticks choked to get here: %d\n", shot_time, event_time - record.m_iTickCount);
#endif
						break;
					}
#ifdef _DEBUG
					else
						g_CVar->ConsolePrintf("Bad curtime difference, EVENT: %f, RECORD: %f\n", event_time, record.m_iTickCount);
#endif
				}
#ifdef _DEBUG
				g_CVar->ConsolePrintf("CURTIME_TICKOUNT: %f, SIMTIME: %f, CALCED_TIME: %f\n", event_time, player_time, shot_time);
#endif
				// gay
				/*int32_t choked = floorf((TICKS_TO_TIME(event_time) - player_time) / g_GlobalVars->interval_per_tick) + 0.5;
				choked = (choked > 14 ? 14 : choked < 1 ? 0 : choked);
				player->m_vecOrigin() = (lag_records.begin()->m_vecOrigin + (g_GlobalVars->interval_per_tick * lag_records.begin()->m_vecVelocity * choked));*/

				CMBacktracking::Get().SetOverwriteTick(player, calcedAngle, shot_time, 1);
			}
		}
	}

	o_FireBullets(thisptr, updateType);
}

__declspec (naked) void __stdcall Handlers::TEFireBulletsPostDataUpdate_h(DataUpdateType_t updateType)
{
	__asm
	{
		push[esp + 4]
		push ecx
		call FireBullets_PostDataUpdate
		retn 4
	}
}

bool __fastcall Handlers::TempEntities_h(void* ECX, void* EDX, void* msg)
{
	if (!g_LocalPlayer || !g_EngineClient->IsInGame() || !g_EngineClient->IsConnected())
		return o_TempEntities(ECX, msg);

	bool ret = o_TempEntities(ECX, msg);

	auto CL_ParseEventDelta = [](void *RawData, void *pToData, RecvTable *pRecvTable)
	{
		// "RecvTable_DecodeZeros: table '%s' missing a decoder.", look at the function that calls it.
		static uintptr_t CL_ParseEventDeltaF = (uintptr_t)Utils::PatternScan(GetModuleHandle("engine.dll"), ("55 8B EC 83 E4 F8 53 57"));
		__asm
		{
			mov     ecx, RawData
			mov     edx, pToData
			push	pRecvTable
			call    CL_ParseEventDeltaF
			add     esp, 4
		}
	};

	// Filtering events
	if (!Clientvariables->Rage.bLagComp || !g_LocalPlayer->IsAlive())
		return ret;

	CEventInfo *ei = g_ClientState->events;
	CEventInfo *next = NULL;

	if (!ei)
		return ret;

	do
	{
		next = *(CEventInfo**)((uintptr_t)ei + 0x38);

		uint16_t classID = ei->classID - 1;

		auto m_pCreateEventFn = ei->pClientClass->m_pCreateEventFn; // ei->pClientClass->m_pCreateEventFn ptr
		if (!m_pCreateEventFn)
			continue;

		IClientNetworkable *pCE = m_pCreateEventFn();
		if (!pCE)
			continue;

		if (classID == ClassId::ClassId_CTEFireBullets)
		{
			// set fire_delay to zero to send out event so its not here later.
			ei->fire_delay = 0.0f;

			auto pRecvTable = ei->pClientClass->m_pRecvTable;
			void *BasePtr = pCE->GetDataTableBasePtr();

			// Decode data into client event object and use the DTBasePtr to get the netvars
			CL_ParseEventDelta(ei->pData, BasePtr, pRecvTable);

			if (!BasePtr)
				continue;

			// This nigga right HERE just fired a BULLET MANE
			int EntityIndex = *(int*)((uintptr_t)BasePtr + 0x10) + 1;

			auto pEntity = (C_BasePlayer*)g_EntityList->GetClientEntity(EntityIndex);
			if (pEntity && pEntity->GetClientClass() &&  pEntity->GetClientClass()->m_ClassID == ClassId::ClassId_CCSPlayer && pEntity->m_iTeamNum() != g_LocalPlayer->m_iTeamNum())
			{
				QAngle EyeAngles = QAngle(*(float*)((uintptr_t)BasePtr + 0x24), *(float*)((uintptr_t)BasePtr + 0x28), 0.0f),
					CalcedAngle = Math::CalcAngle(pEntity->GetEyePos(), g_LocalPlayer->GetEyePos());

				*(float*)((uintptr_t)BasePtr + 0x24) = CalcedAngle.pitch;
				*(float*)((uintptr_t)BasePtr + 0x28) = CalcedAngle.yaw;
				*(float*)((uintptr_t)BasePtr + 0x2C) = 0;

				float
					event_time = TICKS_TO_TIME(g_GlobalVars->tickcount),
					player_time = pEntity->m_flSimulationTime();

				// Extrapolate tick to hit scouters etc
				auto lag_records = CMBacktracking::Get().m_LagRecord[pEntity->EntIndex()];

				float shot_time = event_time;
				for (auto& record : lag_records)
				{
					if (TICKS_TO_TIME(record.m_iTickCount) <= event_time)
					{
						shot_time = record.m_flSimulationTime + (event_time - TICKS_TO_TIME(record.m_iTickCount)); // also get choked from this
#ifdef _DEBUG
						g_CVar->ConsoleColorPrintf(Color(0, 255, 0, 255), "Found exact shot time: %f, ticks choked to get here: %d\n", shot_time, TIME_TO_TICKS(event_time - TICKS_TO_TIME(record.m_iTickCount)));
#endif
						break;
					}
#ifdef _DEBUG
					else
						g_CVar->ConsolePrintf("Bad curtime difference, EVENT: %f, RECORD: %f\n", event_time, TICKS_TO_TIME(record.m_iTickCount));
#endif
				}
#ifdef _DEBUG
				g_CVar->ConsolePrintf("Calced angs: %f %f, Event angs: %f %f, CURTIME_TICKOUNT: %f, SIMTIME: %f, CALCED_TIME: %f\n", CalcedAngle.pitch, CalcedAngle.yaw, EyeAngles.pitch, EyeAngles.yaw, event_time, player_time, shot_time);
#endif
				if (!lag_records.empty())
				{
					int choked = floorf((event_time - player_time) / g_GlobalVars->interval_per_tick) + 0.5;
					choked = (choked > 14 ? 14 : choked < 1 ? 0 : choked);
					pEntity->m_vecOrigin() = (lag_records.begin()->m_vecOrigin + (g_GlobalVars->interval_per_tick * lag_records.begin()->m_vecVelocity * choked));
				}

				//CMBacktracking::Get().SetOverwriteTick(pEntity, CalcedAngle, shot_time, 1);
			}
		}
		ei = next;
	} while (next != NULL);

	return ret;
}

float __fastcall Handlers::GetViewModelFov_h(void* ECX, void* EDX)
{
	float fov = o_GetViewmodelFov(ECX);
	static auto originalViewModelFov = g_CVar->FindVar("viewmodel_fov");

	if (Clientvariables->Visuals.bFov2)
		return Clientvariables->Visuals.iFov2;
	else
		return originalViewModelFov->GetInt();
}

bool __fastcall Handlers::GetBool_SVCheats_h(PVOID pConVar, int edx)
{
	static DWORD CAM_THINK = (DWORD)Utils::PatternScan(GetModuleHandle("client.dll"), "85 C0 75 30 38 86");

	if (!pConVar)
		return false;

	if (Clientvariables->Visuals.bThirdPerson)
	{
		if ((DWORD)_ReturnAddress() == CAM_THINK)
			return true;
	}

	return o_GetBool(pConVar);
}

void __fastcall Handlers::RunCommand_h(void* ECX, void* EDX, C_BasePlayer* player, CUserCmd* cmd, IMoveHelper* helper)
{
	o_RunCommand(ECX, player, cmd, helper);
	Miscellaneous::Get().PunchAngleFix_RunCommand(player);
}

int __fastcall Handlers::SendDatagram_h(void *ECX, void *EDX, bf_write *data)
{
	INetChannel* net_channel = (INetChannel*)ECX;
	int32_t reliable_state = net_channel->m_nInReliableState;
	int32_t sequencenr = net_channel->m_nInSequenceNr;
	int ret = o_SendDatagram(net_channel, data);
	net_channel->m_nInReliableState = reliable_state;
	net_channel->m_nInSequenceNr = sequencenr;
	return ret;
}

#ifdef INSTANT_DEFUSE_PLANT_EXPLOIT
bool __fastcall Handlers::WriteUsercmdDeltaToBuffer_h(IBaseClientDLL *ECX, void *EDX, int nSlot, bf_write *buf, int from, int to, bool isNewCmd)
{
	//static DWORD WriteUsercmdDeltaToBufferReturn = (DWORD)Utils::PatternScan(GetModuleHandle("engine.dll"), "84 C0 74 04 B0 01 EB 02 32 C0 8B FE 46 3B F3 7E C9 84 C0 0F 84 ? ? ? ?"); //     84 DB 0F 84 ? ? ? ? 8B 8F ? ? ? ? 8B 01 8B 40 1C FF D0

	if (nTickBaseShift <= 0 || (DWORD)_ReturnAddress() != ((DWORD)GetModuleHandleA("engine.dll") + 0xCCCA6))
		return o_WriteUsercmdDeltaToBuffer(ECX, nSlot, buf, from, to, isNewCmd);

	if (from != -1)
		return true;

	// CL_SendMove function
	auto CL_SendMove = []()
	{
		using CL_SendMove_t = void(__fastcall*)(void);
		static CL_SendMove_t CL_SendMoveF = (CL_SendMove_t)Utils::PatternScan(GetModuleHandle("engine.dll"), ("55 8B EC A1 ? ? ? ? 81 EC ? ? ? ? B9 ? ? ? ? 53 8B 98"));

		CL_SendMoveF();
	};

	// WriteUsercmd function
	auto WriteUsercmd = [](bf_write *buf, CUserCmd *in, CUserCmd *out)
	{
		//using WriteUsercmd_t = void(__fastcall*)(bf_write*, CUserCmd*, CUserCmd*);
		static DWORD WriteUsercmdF = (DWORD)Utils::PatternScan(GetModuleHandle("client.dll"), ("55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D"));

		__asm
		{
			mov     ecx, buf
			mov     edx, in
			push	out
			call    WriteUsercmdF
			add     esp, 4
		}
	};

	/*uintptr_t framePtr;
	__asm mov framePtr, ebp;
	auto msg = reinterpret_cast<CCLCMsg_Move_t*>(framePtr + 0xFCC);*/
	int *pNumBackupCommands = (int*)(reinterpret_cast<uintptr_t>(buf) - 0x30);
	int *pNumNewCommands = (int*)(reinterpret_cast<uintptr_t>(buf) - 0x2C);
	auto net_channel = g_ClientState->m_NetChannel;

	int32_t new_commands = *pNumNewCommands;

	if (!bInSendMove)
	{
		if (new_commands <= 0)
			return false;

		bInSendMove = true;
		bFirstSendMovePack = true;
		nTickBaseShift += new_commands;

		while (nTickBaseShift > 0)
		{
			CL_SendMove();
			net_channel->Transmit(false);
			bFirstSendMovePack = false;
		}

		bInSendMove = false;
		return false;
	}

	if (!bFirstSendMovePack)
	{
		int32_t loss = min(nTickBaseShift, 10);

		nTickBaseShift -= loss;
		net_channel->m_nOutSequenceNr += loss;
	}

	int32_t next_cmdnr = g_ClientState->lastoutgoingcommand + g_ClientState->chokedcommands + 1;
	int32_t total_new_commands = min(nTickBaseShift, 62);
	nTickBaseShift -= total_new_commands;

	from = -1;
	*pNumNewCommands = total_new_commands;
	*pNumBackupCommands = 0;

	for (to = next_cmdnr - new_commands + 1; to <= next_cmdnr; to++)
	{
		if (!o_WriteUsercmdDeltaToBuffer(ECX, nSlot, buf, from, to, true))
			return false;

		from = to;
	}

	CUserCmd *last_realCmd = g_Input->GetUserCmd(nSlot, from);
	CUserCmd fromCmd;

	if (last_realCmd)
		fromCmd = *last_realCmd;

	CUserCmd toCmd = fromCmd;
	toCmd.command_number++;
	toCmd.tick_count += 200;

	for (int i = new_commands; i <= total_new_commands; i++)
	{
		WriteUsercmd(buf, &toCmd, &fromCmd);
		fromCmd = toCmd;
		toCmd.command_number++;
		toCmd.tick_count++;
	}

	return true;
}
#endif

int __stdcall Handlers::IsBoxVisible_h(const Vector& mins, const Vector& maxs)
{
	if (!memcmp(_ReturnAddress(), "\x85\xC0\x74\x2D\x83\x7D\x10\x00\x75\x1C", 10))
		return 1;

	return o_IsBoxVisible(mins, maxs);
}

bool __fastcall Handlers::IsHLTV_h(void* ECX, void* EDX)
{
	uintptr_t player;

	__asm
	{
		mov player, edi
	}

	if ((DWORD)_ReturnAddress() != Offsets::reevauluate_anim_lod)
		return o_IsHLTV(ECX);

	if (!player || player == 0x000FFFF)
		return o_IsHLTV(ECX);

	*(int32_t*)(player + 0xA24) = -1;
	*(int32_t*)(player + 0xA2C) = *(int32_t*)(player + 0xA28);
	*(int32_t*)(player + 0xA28) = 0;
	return true;
}

bool __fastcall Handlers::SendNetMsg_h(INetChannel* pNetChan, void* edx, INetMessage& msg, bool bForceReliable, bool bVoice)
{
	o_SendNetMsg = g_pNetChannelHook->GetOriginal<SendNetMsg_t>(42);

	if (msg.GetGroup() == 9)
		bVoice = true;

	return o_SendNetMsg(pNetChan, msg, bForceReliable, bVoice);
}

void __fastcall Handlers::DrawModelExecute_h(void* ecx, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& render_info, matrix3x4_t* matrix)
{
	if (g_EngineClient->IsConnected() && g_EngineClient->IsInGame())
	{
		if (!g_LocalPlayer)
			return;

		if (render_info.entity_index == g_EngineClient->GetLocalPlayer() && g_LocalPlayer &&
			g_Input->m_fCameraInThirdPerson && Clientvariables->Visuals.bChamsLocalScoped && g_LocalPlayer->m_bIsScoped())
			g_RenderView->SetBlend(0.4f);
	}

	o_DrawModelExecute(ecx, context, state, render_info, matrix);
	g_MdlRender->ForcedMaterialOverride(nullptr);
}