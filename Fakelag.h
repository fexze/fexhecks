#pragma once
#include "Structs.hpp"
#include "Options.hpp"

class CFakelag : public Singleton<CFakelag>
{
public:
	int BreakLagCompensation();
	void Fakelag(CUserCmd* userCMD);
};