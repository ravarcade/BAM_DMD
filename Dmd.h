#pragma once
#include <vector>
#include <string>

class DmdCom;
struct CFpDmd
{
	std::string name;
	int type;
	void* pUnknown;
	float* color;
	bool isOnBackbox;
	bool isFullColor;
	float backColor[3];
	float baseColor[3];
	uint32_t backTextureId = 0;
	int width;
	int height;
	uint32_t* rawDmdBuffer;

	float tmpColor[3];

	enum {
		AlphaNumeric = 0,
		Gottlieb,
		Clock
	};

	enum {
		Left = 0,
		Center = 1,
		Right = 2
	};

	int segType;
	int segAlign;
	int segLength;
	uint32_t* segCharShapes;
	char segText[33];
	int setTextLength;
	uint32_t *segRawBuffer;

	CFpDmd(std::string, int, void*);

	void refresh();
	void dump();
};

class CDmd {
private:
	struct Ccfg {
		int isEnabled{ 1 };
	} cfg;

	HMODULE hModule;
	std::unique_ptr<DmdCom> dmdCom;


	std::vector<CFpDmd> dmds;

public:
	CDmd() = delete;
	CDmd(HMODULE hModule, HMODULE bam_module);
	~CDmd();

	void CreateMenu();
	void OnPluginStart();
	void OnPluginStop();
	void OnSwapBuffer(HDC);
	IDispatch* GetDmdCom();
};