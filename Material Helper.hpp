#pragma once

#include "SDK.hpp"
#include "Utils.hpp"

class CChams
{
public:
	IMaterial* CreateMaterial(bool ignorez, bool lit, bool wireframe);

private:
	void InitKeyValues(KeyValues* keyValues, std::string name);
	void LoadFromBuffer(KeyValues* keyValues, std::string name_, std::string buffer_);
};

extern CChams* chams;