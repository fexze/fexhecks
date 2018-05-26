#pragma once
#pragma comment(lib,"urlmon.lib")

class ConfigMgr
{
public:
	void     Reset();
	char*    Handle();
	void     SaveConfig();
	void     LoadConfig();
	bool     CheckConfigs();
	bool     FileExists(std::string file);
	bool     Legit, Rage, AWP, Auto, Scout, Revolver;
	char*    ReadString(char* Cat, char* Name, char* defaultval);
	void     WriteString(char* Cat, char* Name, char* defaultval);
	int      ReadInt(const char* Cat, const char* Name, int defaultval);
	void     WriteInt(const char* Cat, const char* Name, int defaultval);
	void     WriteFloat(const char* Cata, const char* Name, float setVal);
	float    ReadFloat(const char* Cat, const char* Name, float defaultval);

private:
	char Path[255];
	char Path1[255];
	char Path2[255];
	char Path3[255];
	char Path4[255];
	char Path5[255];
	char Picture[255];
};

extern ConfigMgr* ConSys;