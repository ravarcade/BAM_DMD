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
			
			if (type == HUDDMD || type == DISPDMD)
			{
				// print name and color of DMD
				auto pf = reinterpret_cast<float*>(pUnknown);
				float* pColor = pf + (type == HUDDMD ?  0x8b : 0xce);
				dmds.emplace_back(CFpDmd{name, type, pUnknown});
				//BAM::dbg::hudDebugLong("dmd: %s, [r=%.3f, g=%.3f, b=%.3f]\n", name.c_str(), pColor[0], pColor[1], pColor[2]);

			}
		});
}

void CDmd::OnPluginStop()
{
	dmdCom.reset();  // destroy COM object
	BAM::helpers::save(cfgFilename, &cfg, sizeof(cfg));
}

void CDmd::OnSwapBuffer()
{
	for (const auto &dmd : dmds)
	{
		bool isFullColor = false;
		float backColor[3];
		float baseColor[3];
		uint32_t backTextureId = 0;
		BAM::fpObjects::GetDmdAdditionalInfo(dmd.pUnknown, isFullColor, backColor, baseColor, backTextureId);
		BAM::dbg::hudDebug("%s : %s\n  color = [%.3f, %.3f, %.3f]\n  isFullColor = %s\n  backColor = [%.3f, %.3f, %.3f]\n  baseColor = [%.3f, %.3f, %.3f]\n  backTextureId = %d\n\n", 
			dmd.name.c_str(),
			dmd.type == DISPDMD ? "DispDMD" : "HudDMD",
			dmd.pBaseColor[0], dmd.pBaseColor[1], dmd.pBaseColor[2],
			isFullColor ? "True" : "False",
			backColor[0], backColor[1], backColor[2],
			baseColor[0], baseColor[1], baseColor[2],
			backTextureId);
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

CDmd::CFpDmd::CFpDmd(std::string name, int type, void* pUnknown)
	: name(name)
	, type(type)
	, pUnknown(pUnknown)
	, pBaseColor(reinterpret_cast<float*>(pUnknown) + (type == HUDDMD ? 0x8b : 0xce))
{
}
