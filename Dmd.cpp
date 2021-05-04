#include <windows.h>
#include <memory>
#include "GL/glew.h"
#include "gl/GL.h"

#include "BAM.h"
#include "DmdCom.h"
#include "Dmd.h"
#include "detours.h"

const char* cfgFilename = "BAM_DMD";

CDmd::CDmd(HMODULE hModule, HMODULE bam_module)
	: hModule(hModule)
	, dmdCom(nullptr)
{
	BAM::Init(0x4201, bam_module); // 0x4201 is id of this plugin,  use any value in range 0x4201 - 0x42ff
}

CDmd::~CDmd() = default;

void CDmd::CreateMenu()
{
	static const char* NoYes[] = { "No", "Yes" };

	BAM::menu::create("Dmd Plugin", "DMD Plugin");
	BAM::menu::info("#c777##-Options" DEFIW);

	BAM::menu::paramSwtich("#-Enabled:" DEFPW "#+ #cfff#%s", &cfg.isEnabled, NoYes, ARRAY_ENTRIES(NoYes), "");

	BAM::helpers::load(cfgFilename, &cfg, sizeof(cfg));
}

void CDmd::OnPluginStart()
{
	GLenum nGlewError = glewInit();   // now gl context exist, we can initiate glew
	dmds.clear();

	BAM::fpObjects::foreach([this](const std::string name, int type, void* pUnknown)
		{
			
			if (type == HUDDMD || type == DISPDMD || type == DISPSEG)
			{
				dmds.emplace_back(CFpDmd{name, type, pUnknown});

			}
		});
}

void CDmd::OnPluginStop()
{
	dmdCom.reset();  // destroy COM object
	BAM::helpers::save(cfgFilename, &cfg, sizeof(cfg));
}

void CDmd::OnSwapBuffer(HDC hDC)
{
	static uint64_t lastFrameCounter = -1;
	if (BAM::render::FrameCounter() == lastFrameCounter)
		return;

	lastFrameCounter = BAM::render::FrameCounter();

	// we are sure, that this will be called once per frame
	for (auto &dmd : dmds)
	{
		dmd.refresh();
		dmd.dump();
	}
}

IDispatch* CDmd::GetDmdCom()
{
	// create COM object on request from script
	if (not dmdCom) 
	{
		dmdCom = std::make_unique<DmdCom>(hModule);
	}

	return dmdCom.get(); // get COM object
}


namespace {
	const char* typeAsString(int type) {
		switch (type)
		{
		case DISPDMD: return "DispDMD";
		case HUDDMD: return "HudDMD";
		case DISPSEG: return "DispSEG";
		}
		return "Unknown";
	};

	const char* segTypeAsString(int type) {
		switch (type)
		{
		case CFpDmd::AlphaNumeric: return "AlphaNumeric";
		case CFpDmd::Gottlieb: return "Gottlieb";
		case CFpDmd::Clock: return "Clock";
		}
		return "Unknown";
	};

	const char* segAlignAsString(int type) {
		switch (type)
		{
		case CFpDmd::Left: return "Left";
		case CFpDmd::Center: return "Center";
		case CFpDmd::Right: return "Right";
		}
		return "Unknown";
	};
} // namespace








CFpDmd::CFpDmd(std::string name, int type, void* pUnknown)
	: name(name)
	, type(type)
	, pUnknown(pUnknown)
	, color(tmpColor)
	, isOnBackbox(reinterpret_cast<uint32_t*>(pUnknown)[0x10] == 2)
	, backTextureId(0)
	, isFullColor(false)
	, width(0)
	, height(0)
{
	switch (type)
	{
	case HUDDMD:
		color = reinterpret_cast<float*>(pUnknown) + 0x8b;
		width = reinterpret_cast<uint32_t*>(pUnknown)[0x89];
		height = reinterpret_cast<uint32_t*>(pUnknown)[0x8a];
		break;

	case DISPDMD:
		color = reinterpret_cast<float*>(pUnknown) + 0xce;
		width = reinterpret_cast<uint32_t*>(pUnknown)[0xca];
		height = reinterpret_cast<uint32_t*>(pUnknown)[0xcb];
		break;

	case DISPSEG:
		memset(backColor, 0, sizeof(backColor));
		memset(baseColor, 0, sizeof(backColor));
		segText[sizeof(segText) - 1] = 0;
		segLength = reinterpret_cast<uint32_t*>(pUnknown)[0xa1];
		segRawBufferWithEffects = reinterpret_cast<uint32_t**>(pUnknown)[0x2bd]+0x166;
		segRawBuffer = reinterpret_cast<uint32_t**>(pUnknown)[0x2bc] + 0x166;
	}
	refresh();
}

void CFpDmd::refresh()
{
	isFullColor = false;
	memset(backColor, 0, sizeof(backColor));
	memset(baseColor, 0, sizeof(backColor));
	backTextureId = 0;
	BAM::fpObjects::GetDmdAdditionalInfo(pUnknown, isFullColor, backColor, baseColor, backTextureId);
	switch (type)
	{
	case DISPDMD:
		rawDmdBuffer = reinterpret_cast<uint32_t**>(pUnknown)[0xe8];
		break;

	case HUDDMD:
		rawDmdBuffer = reinterpret_cast<uint32_t**>(pUnknown)[0xa5];
		break;

	case DISPSEG:
	{
		auto pui = reinterpret_cast<uint32_t*>(pUnknown);
		uint32_t col = pui[0xc9];
		tmpColor[0] = (col & 0xff) / 255.0f;
		tmpColor[1] = ((col >> 8) & 0xff) / 255.0f;
		tmpColor[2] = ((col >> 16) & 0xff) / 255.0f;
		color = tmpColor;
		segType = pui[0x9f];
		segAlign = pui[0xa5];
		segCharShapes = pui + 0xe6;
		auto pTxt = reinterpret_cast<char *>(pui + 0x2be);
		strcpy_s(segText, sizeof(segText) - 1, pTxt);
		setTextLength = strlen(segText);
		break;
	}
	}
}

void CFpDmd::dump()
{
	switch (type)
	{
	case DISPDMD:
	case HUDDMD:
		BAM::dbg::hudDebug("%s is %s\n - is on %s\n - color = [%.3f, %.3f, %.3f]\n - backColor = [%.3f, %.3f, %.3f]\n - baseColor = [%.3f, %.3f, %.3f]\n - isFullColor = %s\n - backTextureId = %d, res = %d x %d\n\n",
			name.c_str(),
			typeAsString(type),
			isOnBackbox ? "backbox" : "playfield",
			color[0], color[1], color[2],
			backColor[0], backColor[1], backColor[2],
			baseColor[0], baseColor[1], baseColor[2],
			isFullColor ? "true" : "false",
			backTextureId,
			width, height);
		break;

	case DISPSEG:
		if (false)
		BAM::dbg::hudDebug("%s is %s\n - is on %s\n - color = [%.3f, %.3f, %.3f]\n - segType = %s\n - segAlign = %s\n - text = %s\n",
			name.c_str(),
			typeAsString(type),
			isOnBackbox ? "backbox" : "playfield",
			color[0], color[1], color[2],
			segTypeAsString(segType),
			segAlignAsString(segAlign),
			segText);

		BAM::dbg::hudDebug("%s: ", name.c_str());
		BAM::dbg::hudDebug("[");
		for (int i = 0; i < segLength; ++i)
		{
			if (i!=0) BAM::dbg::hudDebug("|");
			BAM::dbg::hudDebug("%04x", segRawBufferWithEffects[i]);
		}
		BAM::dbg::hudDebug("]\n");
		break;
	}
}
