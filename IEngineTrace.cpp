#include "SDK.hpp"
#include "IEngineTrace.hpp"

bool CGameTrace::DidHitWorld() const
{
	return hit_entity == g_EntityList->GetClientEntity(0);
}

bool CGameTrace::DidHitNonWorldEntity() const
{
	return hit_entity != nullptr && !DidHitWorld();
}