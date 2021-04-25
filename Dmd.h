#pragma once

class DmdCom;

class CDmd {
private:
	struct Ccfg {
		int isEnabled{ 1 };
	} cfg;

	HMODULE hModule;
	std::unique_ptr<DmdCom> dmdCom;

public:
	CDmd() = delete;
	CDmd(HMODULE hModule, HMODULE bam_module);
	~CDmd();

	void CreateMenu();
	void OnPluginStart();
	void OnPluginStop();
	IDispatch* GetDmdCom();
};