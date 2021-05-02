
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define DRAWDIB_INCLUDE_STRETCHDIB
// Windows Header Files:
#include <windows.h>
#include <Vfw.h>
#include <memory>
#include "Dmd.h"
#include "BAM.h"
#include "detours.h"

std::unique_ptr<CDmd> dmd(nullptr);

#define BAMEXPORT __declspec(dllexport)

HMODULE dll_hModule;

BOOL APIENTRY DllMain( HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	dll_hModule = hModule;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" {
	// called on FP load (before any hooks to opengl is made)
	// It must exist
	// If not exist => dll is not BAM plugin and will be unloaded from RAM
	BAMEXPORT int BAM_load(HMODULE bam_module)
	{
		//BAM::Init(0x4201, bam_module);
		dmd = std::make_unique<CDmd>(dll_hModule, bam_module);
		dmd->CreateMenu();
		return 0;
	}

	// For "Tracking" plugins, it is called when user select plugin (signal for example to create working thread and take resurces like cams)
	// For "Non-Tracking" plugins it is called on game start
	BAMEXPORT void BAM_PluginStart() { dmd->OnPluginStart(); }

	// For "Treacking" plugins, it is called then plugin is "unselected" as mode. It is signal to release resources (like cams, one cam can be used be two or more plugins)
	// For "Non-Tracking" plugins, it is called at end of game
	BAMEXPORT void BAM_PluginStop() { dmd->OnPluginStop(); }

	// Called from script with:
	// Set icom = xBAM.Get("BAM_DMD")
	BAMEXPORT IDispatch *BAM_GetCOM() { return dmd->GetDmdCom(); }

	BAMEXPORT void BAM_AttachDetours() { MakeDetours(true); }
	BAMEXPORT void BAM_DetachDetours() { MakeDetours(false); }
	BAMEXPORT void BAM_swapBuffers() { dmd->OnSwapBuffer(); }
}