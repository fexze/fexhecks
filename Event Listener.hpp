#pragma once

#include "SDK.hpp"
#include "Visuals.hpp"
#include "Structs.hpp"
#include "AimRage.hpp"
#include "Resolver.hpp"

#define CREATE_EVENT_LISTENER(class_name)\
class class_name : public IGameEventListener\
{\
public:\
	~class_name() { g_GameEvents->RemoveListener(this); }\
\
	virtual void FireGameEvent(IGameEvent* game_event);\
};\

class CGameEvents
{
public:
	void InitializeEventListeners();
	CREATE_EVENT_LISTENER(WeaponFireListener);
	CREATE_EVENT_LISTENER(PlayerHurtListener);
	CREATE_EVENT_LISTENER(ItemPurchaseListener);
	CREATE_EVENT_LISTENER(BulletImpactListener);
};

extern CGameEvents* g_pGameEvents;