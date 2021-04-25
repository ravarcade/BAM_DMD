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
}

void CDmd::OnPluginStop()
{
	dmdCom.reset();  // destroy COM object
	BAM::helpers::save(cfgFilename, &cfg, sizeof(cfg));
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
