#pragma once
#include <vector>
#include <string>

class DmdCom;

class CDmd {
private:
	struct Ccfg {
		int isEnabled{ 1 };
	} cfg;

	HMODULE hModule;
	std::unique_ptr<DmdCom> dmdCom;

	struct CFpDmd
	{
		std::string name;
		int type;
		void* pUnknown;
		float* pBaseColor;
		CFpDmd(std::string, int, void*);
	};
	std::vector<CFpDmd> dmds;

public:
	CDmd() = delete;
	CDmd(HMODULE hModule, HMODULE bam_module);
	~CDmd();

	void CreateMenu();
	void OnPluginStart();
	void OnPluginStop();
	void OnSwapBuffer();
	IDispatch* GetDmdCom();
};