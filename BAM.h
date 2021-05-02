#pragma once
#include <string>

/**
 * New BAM.h
 * To make easier BAM plugins
 * 
 * Info about how to use it is at end of this file
 */

#define DEFPW "#r160#"
#define DEFPW2 DEFPW"#+ #cfff#"
#define DEFIW "#m350#"

#define BAMEXPORT __declspec(dllexport)
#define BAMIMPORT __declspec(dllimport)
#define ARRAY_ENTRIES(array)	(sizeof(array)/sizeof(array[0]))

namespace BAM {
	typedef void BAM_OnButtonPress_func(int keyCode);
	struct SInternal {
		int _pluginID;
		// BAM functions types
		typedef BAMIMPORT void BAM_MessageBox_func(const char *fmt, ...);
		typedef BAMIMPORT void BAM_MessageBox2_func(const char *txt);
		typedef	BAMEXPORT void BAM_menu_create_func(int pluginID, const char *pname, const char *phelp);
		typedef	BAMEXPORT void BAM_menu_add_info_func(int pluginID, const char *text);
		typedef BAMIMPORT void BAM_menu_add_info3D_func(int pluginID, const char *txt, double *x, double *y, double *z, const char *help );
		typedef	BAMIMPORT void BAM_menu_add_param_func(int PluginID, const char *txt, double *value, double smallStep, double bigStep, const char *help );
		typedef BAMIMPORT void BAM_menu_add_switch_func(int PluginID, const char *txt, int *value, const char *options[], int count, const char *help );
		typedef BAMIMPORT void BAM_menu_add_button_func(int PluginID, const char *txt, BAM_OnButtonPress_func *f);
		typedef	BAMIMPORT void BAM_menu_add_cam_func(int pluginID);
		typedef	BAMEXPORT void BAM_menu_add_3Points_func(int pluginID);
		typedef	BAMIMPORT void BAM_menu_add_TL_func(int pluginID);
		typedef	BAMIMPORT void BAM_menu_add_Reality_func(int pluginID);
		typedef	BAMIMPORT void BAM_menu_add_StartCalibration_func(int pluginID);
		typedef	BAMIMPORT void BAM_menu_add_CalibrationBoard_func(int pluginID);
		typedef BAMIMPORT int  BAM_create_submenu_func(int PluginID);
		typedef BAMIMPORT void BAM_menu_add_submenu_func(int PluginID, const char *txt, int SubMenuID, const char *help);
		typedef BAMIMPORT void BAM_menu_add_back_button_func(int PluginID);
		typedef BAMIMPORT void BAM_get_menu_position_func(int *menuId, int *menuLine);
		typedef BAMIMPORT void BAM_set_submenu_cam_info_func(int submenuId, int camCount);


		typedef BAMIMPORT void BAM_printf_func(int x, int y, double size, bool shadow, const char *fmt, ...);
		typedef BAMIMPORT void BAM_printf2_func(int x, int y, double size, bool shadow, const char *txt);
		typedef BAMIMPORT BYTE *BAM_get_cam_buffer_func(int cam);
		typedef BAMIMPORT void BAM_get_cam_update_completed_func(int cam);

		typedef	BAMIMPORT void BAM_push_raw_XYZ_func(int pluginID, double x, double y, double z);
		typedef	BAMIMPORT void BAM_push_raw_XYZ_2Eyes_func(int PluginID, double x1, double y1, double z1, double x2, double y2, double z2);
		typedef BAMIMPORT void BAM_push_raw_cam_func(double *leftPoints, int leftCount, double *rightPoints, int rightCount);
		typedef	BAMIMPORT void BAM_testing_SetCamXYZ_func(double x, double y, double z);

		typedef BAMIMPORT void BAM_calibrate_func(double *leftPoints, double *rightPoints, int CornersX, int CornersY, int frames);
		typedef BAMIMPORT void BAM_calibrateCams_func(double *leftPoints, double *rightPoints, int CornersX, int CornersY, int frames);
		typedef	BAMIMPORT void BAM_set_CamsParams_func(double (*M1)[3][3], double (*D1)[5], double (*M2)[3][3], double (*D2)[5]);

		typedef	BAMIMPORT double * BAM_get_CalbrationBoard_func();
		typedef BAMIMPORT void BAM_showCheckerboard_func(int status);
		typedef BAMIMPORT double *BAM_get_checerboard_points_func();
		typedef BAMIMPORT void BAM_set_checkerboard_3points_func(double *points);

		typedef BAMIMPORT void BAM_AttachDetour_func(void **ppPointer, void *pDetour);
		typedef BAMIMPORT void BAM_DetachDetour_func(void **ppPointer, void *pDetour);

		typedef BAMIMPORT unsigned int BAM_compileShaders_func(const char *vertexShaderSrc, const char *fragmentShaderSrc);
		typedef BAMIMPORT void *BAM_GetAllFPObjects_func();
		typedef BAMIMPORT void BAM_GetDmdAdditionalInfo_func(void* fpObj, bool* pIsFullColor, float* pBackgroundColor, float* pBaseColor, uint32_t* pSecondTextureId);


		// debug
		BAM_MessageBox_func                *BAM_MessageBox;
		BAM_MessageBox_func                *BAM_hudDebug;
		BAM_MessageBox_func                *BAM_hudDebugLong;

		BAM_MessageBox2_func               *BAM_MessageBox2;
		BAM_MessageBox2_func               *BAM_hudDebug2;
		BAM_MessageBox2_func               *BAM_hudDebugLong2;

		// menu
		BAM_menu_create_func               *BAM_menu_create;
		BAM_menu_add_info_func             *BAM_menu_add_info;
		BAM_menu_add_info3D_func           *BAM_menu_add_info3D;
		BAM_menu_add_cam_func              *BAM_menu_add_cam;
		BAM_menu_add_param_func            *BAM_menu_add_param;
		BAM_menu_add_switch_func           *BAM_menu_add_switch;
		BAM_create_submenu_func            *BAM_create_submenu;
		BAM_menu_add_submenu_func          *BAM_menu_add_submenu;
		BAM_menu_add_back_button_func      *BAM_menu_add_back_button;
		BAM_menu_add_button_func           *BAM_menu_add_button;

		BAM_menu_add_3Points_func          *BAM_menu_add_3Points;
		BAM_menu_add_StartCalibration_func *BAM_menu_add_StartCalibration;
		BAM_menu_add_TL_func               *BAM_menu_add_TL;
		BAM_menu_add_Reality_func          *BAM_menu_add_Reality;
		BAM_menu_add_CalibrationBoard_func *BAM_menu_add_CalibrationBoard;

		BAM_get_menu_position_func         *BAM_get_menu_position;
		BAM_set_submenu_cam_info_func      *BAM_set_submenu_cam_info;

		// hud display
		BAM_printf_func                   *BAM_printf;
		BAM_printf2_func                  *BAM_printf2;

		// cams
		BAM_get_cam_buffer_func           *BAM_get_cam_buffer;
		BAM_get_cam_update_completed_func *BAM_get_cam_update_completed;

		// data to track
		BAM_push_raw_XYZ_func             *BAM_push_raw_XYZ;
		BAM_push_raw_XYZ_2Eyes_func       *BAM_push_raw_XYZ_2Eyes;
		BAM_push_raw_cam_func             *BAM_push_raw_cam;
		BAM_testing_SetCamXYZ_func        *BAM_testing_SetCamXYZ;

		BAM_calibrate_func                *BAM_calibrate;
		BAM_calibrateCams_func            *BAM_calibrateCams;
		BAM_get_CalbrationBoard_func      *BAM_get_CalbrationBoard;
		BAM_set_CamsParams_func           *BAM_set_CamsParams;
		BAM_showCheckerboard_func         *BAM_showCheckerboard;
		BAM_get_checerboard_points_func   *BAM_get_checerboard_points;

		BAM_set_checkerboard_3points_func *BAM_set_checkerboard_3points;

		// detours
		BAM_AttachDetour_func             *BAM_AttachDetour;
		BAM_DetachDetour_func             *BAM_DetachDetour;

		BAM_compileShaders_func           *BAM_compileShaders;
		BAM_GetAllFPObjects_func          *BAM_GetAllFPObjects;
		BAM_GetDmdAdditionalInfo_func     *BAM_GetDmdAdditionalInfo;
	};

	template<typename T>
	struct StHelper {
		static SInternal t;
	};

	template<typename T>
	SInternal StHelper<T>::t = SInternal{};

	inline SInternal & Internal() {
		StHelper<SInternal> st;
		return st.t;
	}

	inline std::string wtoa(std::wstring w)
	{
		std::string result;
		for (wchar_t x : w)
			result += (char)x;
		return result;
	}

	inline void Init (int pluginID, HMODULE bam_module) {
		Internal()._pluginID = pluginID;

		// ========= init BAM functions ==================
		// ::dbg::
		Internal().BAM_MessageBox                = (SInternal::BAM_MessageBox_func *)                GetProcAddress(bam_module, "BAM_MessageBox");
		Internal().BAM_hudDebug                  = (SInternal::BAM_MessageBox_func *)                GetProcAddress(bam_module, "BAM_hudDebug");
		Internal().BAM_hudDebugLong              = (SInternal::BAM_MessageBox_func *)                GetProcAddress(bam_module, "BAM_hudDebugLong");

		Internal().BAM_MessageBox2               = (SInternal::BAM_MessageBox2_func *)               GetProcAddress(bam_module, "BAM_MessageBox2");
		Internal().BAM_hudDebug2                 = (SInternal::BAM_MessageBox2_func *)               GetProcAddress(bam_module, "BAM_hudDebug2");
		Internal().BAM_hudDebugLong2             = (SInternal::BAM_MessageBox2_func *)               GetProcAddress(bam_module, "BAM_hudDebugLong2");

		// ::menu::
		Internal().BAM_menu_create               = (SInternal::BAM_menu_create_func *)               GetProcAddress(bam_module, "BAM_menu_create");
		Internal().BAM_menu_add_info             = (SInternal::BAM_menu_add_info_func *)             GetProcAddress(bam_module, "BAM_menu_add_info");
		Internal().BAM_menu_add_info3D           = (SInternal::BAM_menu_add_info3D_func *)           GetProcAddress(bam_module, "BAM_menu_add_info3D");
		Internal().BAM_menu_add_cam              = (SInternal::BAM_menu_add_cam_func *)              GetProcAddress(bam_module, "BAM_menu_add_cam");
		Internal().BAM_menu_add_param            = (SInternal::BAM_menu_add_param_func *)            GetProcAddress(bam_module, "BAM_menu_add_param");
		Internal().BAM_menu_add_switch           = (SInternal::BAM_menu_add_switch_func *)           GetProcAddress(bam_module, "BAM_menu_add_switch");
		Internal().BAM_menu_add_button           = (SInternal::BAM_menu_add_button_func *)           GetProcAddress(bam_module, "BAM_menu_add_button");

		Internal().BAM_menu_add_3Points          = (SInternal::BAM_menu_add_3Points_func *)          GetProcAddress(bam_module, "BAM_menu_add_3Points");
		Internal().BAM_menu_add_StartCalibration = (SInternal::BAM_menu_add_StartCalibration_func *) GetProcAddress(bam_module, "BAM_menu_add_StartCalibration");
		Internal().BAM_menu_add_TL               = (SInternal::BAM_menu_add_TL_func *)               GetProcAddress(bam_module, "BAM_menu_add_TL");
		Internal().BAM_menu_add_Reality          = (SInternal::BAM_menu_add_Reality_func *)          GetProcAddress(bam_module, "BAM_menu_add_Reality");
		Internal().BAM_menu_add_CalibrationBoard = (SInternal::BAM_menu_add_CalibrationBoard_func *) GetProcAddress(bam_module, "BAM_menu_add_CalibrationBoard");

		Internal().BAM_create_submenu            = (SInternal::BAM_create_submenu_func *)            GetProcAddress(bam_module, "BAM_create_submenu");
		Internal().BAM_menu_add_submenu          = (SInternal::BAM_menu_add_submenu_func *)          GetProcAddress(bam_module, "BAM_menu_add_submenu");
		Internal().BAM_menu_add_back_button      = (SInternal::BAM_menu_add_back_button_func *)      GetProcAddress(bam_module, "BAM_menu_add_back_button");

		Internal().BAM_get_menu_position         = (SInternal::BAM_get_menu_position_func *)         GetProcAddress(bam_module, "BAM_get_menu_position");
		Internal().BAM_set_submenu_cam_info      = (SInternal::BAM_set_submenu_cam_info_func *)      GetProcAddress(bam_module, "BAM_set_submenu_cam_info");

		Internal().BAM_printf                    = (SInternal::BAM_printf_func *)                    GetProcAddress(bam_module, "BAM_printf");
		Internal().BAM_printf2                   = (SInternal::BAM_printf2_func *)                   GetProcAddress(bam_module, "BAM_printf2");

		Internal().BAM_get_cam_buffer            = (SInternal::BAM_get_cam_buffer_func *)            GetProcAddress(bam_module, "BAM_get_cam_buffer");
		Internal().BAM_get_cam_update_completed  = (SInternal::BAM_get_cam_update_completed_func *)  GetProcAddress(bam_module, "BAM_get_cam_update_completed");

		Internal().BAM_push_raw_XYZ              = (SInternal::BAM_push_raw_XYZ_func *)              GetProcAddress(bam_module, "BAM_push_raw_XYZ");
		Internal().BAM_push_raw_XYZ_2Eyes        = (SInternal::BAM_push_raw_XYZ_2Eyes_func *)        GetProcAddress(bam_module, "BAM_push_raw_XYZ_2Eyes");
		Internal().BAM_push_raw_cam              = (SInternal::BAM_push_raw_cam_func *)              GetProcAddress(bam_module, "BAM_push_raw_cam");
		Internal().BAM_testing_SetCamXYZ         = (SInternal::BAM_testing_SetCamXYZ_func *)         GetProcAddress(bam_module, "BAM_testing_SetCamXYZ");

		Internal().BAM_calibrate                 = (SInternal::BAM_calibrate_func *)                 GetProcAddress(bam_module, "BAM_calibrate");
		Internal().BAM_calibrateCams             = (SInternal::BAM_calibrateCams_func *)             GetProcAddress(bam_module, "BAM_calibrateCams");
		Internal().BAM_get_CalbrationBoard       = (SInternal::BAM_get_CalbrationBoard_func *)       GetProcAddress(bam_module, "BAM_get_CalbrationBoard");
		Internal().BAM_set_CamsParams            = (SInternal::BAM_set_CamsParams_func *)            GetProcAddress(bam_module, "BAM_set_CamsParams");
		Internal().BAM_showCheckerboard          = (SInternal::BAM_showCheckerboard_func *)          GetProcAddress(bam_module, "BAM_showCheckerboard");		
		Internal().BAM_get_checerboard_points    = (SInternal::BAM_get_checerboard_points_func *)    GetProcAddress(bam_module, "BAM_get_checerboard_points");

		Internal().BAM_set_checkerboard_3points  = (SInternal::BAM_set_checkerboard_3points_func *)  GetProcAddress(bam_module, "BAM_set_checkerboard_3points");

		Internal().BAM_AttachDetour              = (SInternal::BAM_AttachDetour_func *)              GetProcAddress(bam_module, "BAM_AttachDetour");
		Internal().BAM_DetachDetour              = (SInternal::BAM_DetachDetour_func *)              GetProcAddress(bam_module, "BAM_DetachDetour");

		Internal().BAM_compileShaders            = (SInternal::BAM_compileShaders_func *)            GetProcAddress(bam_module, "BAM_compileShaders");
		Internal().BAM_GetAllFPObjects           = (SInternal::BAM_GetAllFPObjects_func*)            GetProcAddress(bam_module, "BAM_GetAllFPObjects");
		Internal().BAM_GetDmdAdditionalInfo      = (SInternal::BAM_GetDmdAdditionalInfo_func *)      GetProcAddress(bam_module, "BAM_GetDmdAdditionalInfo");
	};

	namespace dbg {
		// Shows MessageBox
		inline void MessageBox(const char *fmt, ...)
		{
			va_list	ap;
			static char txt[2048];

			va_start(ap, fmt);
			vsprintf_s(txt,fmt, ap);
			va_end(ap);

			BAM::Internal().BAM_MessageBox2( txt );
		}

		// Shows debug line (visible in single frame). Use it in hudDisplay
		inline void hudDebug(const char *fmt, ...)
		{
			va_list	ap;
			static char txt[2048];

			va_start(ap, fmt);
			vsprintf_s(txt,fmt, ap);
			va_end(ap);

			BAM::Internal().BAM_hudDebug2( txt );
		}

		// Shows debug line in on screen log (last 30 lines)
		inline void hudDebugLong(const char *fmt, ...)
		{
			va_list	ap;
			static char txt[2048];

			va_start(ap, fmt);
			vsprintf_s(txt,fmt, ap);
			va_end(ap);

			BAM::Internal().BAM_hudDebugLong2( txt );
		}
	};

	namespace menu {
		// add plugin to BAM (menu entry)
		inline void create(const char *pluginName, const char *help) {
			BAM::Internal().BAM_menu_create(BAM::Internal()._pluginID, pluginName, help);
		}

		// default menu entry
		namespace defaults {
			inline void camPosition() {
				BAM::Internal().BAM_menu_add_cam(BAM::Internal()._pluginID);
			}

			inline void set3Points() {
				BAM::Internal().BAM_menu_add_3Points(BAM::Internal()._pluginID);
			}

			inline void startCalibration() {
				BAM::Internal().BAM_menu_add_StartCalibration(BAM::Internal()._pluginID);
			}

			inline void translateAndLighting() {
				BAM::Internal().BAM_menu_add_TL(BAM::Internal()._pluginID);
			}

			inline void reality() {
				BAM::Internal().BAM_menu_add_Reality(BAM::Internal()._pluginID);
			}

			inline void setCalibrationBoard() {
				BAM::Internal().BAM_menu_add_CalibrationBoard(BAM::Internal()._pluginID);
			}
		}

		// line of text in menu
		inline void info(const char *txt) {
			BAM::Internal().BAM_menu_add_info(BAM::Internal()._pluginID, txt);
		}

		// line of text with read only 3 params (like 3D position)
		inline void info3D(const char *txt, double *x, double *y, double *z) {
			BAM::Internal().BAM_menu_add_info3D(BAM::Internal()._pluginID, txt, x, y, z, "");
		}

		// line with param. User can change param value
		inline void paramValue(const char *txt, double *value, double smallStep, double bigStep, const char *help) {
			BAM::Internal().BAM_menu_add_param(BAM::Internal()._pluginID, txt, value, smallStep, bigStep, help);
		}

		// line with param as switch
		inline void paramSwtich(const char *txt, int *value, const char *options[], int count, const char *help) {
			BAM::Internal().BAM_menu_add_switch(BAM::Internal()._pluginID, txt, value, options, count, help);
		}

		// menu option/command
		inline void command(const char *txt, BAM_OnButtonPress_func *f) {
			BAM::Internal().BAM_menu_add_button(BAM::Internal()._pluginID, txt, f);
		}

		// display text, info... Use it in hudDisplay
		inline void printf(int x, int y, double size, bool shadow, const char *fmt, ...) {
			va_list	ap;
			static char txt[2048];

			va_start(ap, fmt);
			vsprintf_s(txt,fmt, ap);
			va_end(ap);

			BAM::Internal().BAM_printf2(x, y, size, shadow, txt);
		}

		inline void showCheckerboard(int status) {
			BAM::Internal().BAM_showCheckerboard(status);
		}

		inline double *getChecerboardPoints() {
			return BAM::Internal().BAM_get_checerboard_points();
		}


//		Internal().BAM_get_menu_position         = (SInternal::BAM_get_menu_position_func *)         GetProcAddress(bam_module, "BAM_get_menu_position");
//		Internal().BAM_set_submenu_cam_info      = (SInternal::BAM_set_submenu_cam_info_func *)      GetProcAddress(bam_module, "BAM_set_submenu_cam_info");

//		Internal::BAM_create_submenu            = (Internal::BAM_create_submenu_func *)            GetProcAddress(bam_module, "BAM_create_submenu");
//		Internal::BAM_menu_add_submenu          = (Internal::BAM_menu_add_submenu_func *)          GetProcAddress(bam_module, "BAM_menu_add_submenu");
//		Internal::BAM_menu_add_back_button      = (Internal::BAM_menu_add_back_button_func *)      GetProcAddress(bam_module, "BAM_menu_add_back_button");

	}

	namespace cam {
		inline BYTE *get_buffer(int cam) {
			return BAM::Internal().BAM_get_cam_buffer(cam);
		}

		inline void update_completed(int cam) {
			BAM::Internal().BAM_get_cam_update_completed(cam);
		}
	}

	namespace push {
		inline void rawXYZ(double x, double y, double z) {
			BAM::Internal().BAM_push_raw_XYZ(BAM::Internal()._pluginID, x, y, z);
		}

		inline void rawXYZ_2Eyes(double x1, double y1, double z1, double x2, double y2, double z2) {
			BAM::Internal().BAM_push_raw_XYZ_2Eyes(BAM::Internal()._pluginID, x1, y1, z1, x2, y2, z2);
		}

		inline void camsPoints(double *leftPoints, int leftCount, double *rightPoints, int rightCount) {
			BAM::Internal().BAM_push_raw_cam(leftPoints, leftCount, rightPoints, rightCount);
		}

		inline void test(double x, double y, double z) {
			BAM::Internal().BAM_testing_SetCamXYZ(x, y, z);
		}

		inline void cherkerboard_3points(double *points) {
			BAM::Internal().BAM_set_checkerboard_3points(points);
		}
	}

	namespace helpers {

		inline const char *buildFilename(const char *fname = NULL, const char *ext = NULL) {
			static char dir[_MAX_PATH+1] = "";
			static char ret[_MAX_PATH+1];
			if (dir[0] == 0) {
				GetCurrentDirectoryA(sizeof(dir), dir);
				strcat_s(dir, _MAX_PATH, "\\");
			}
			strcpy_s(ret, dir);
			if (fname != NULL) {
				strcat_s(ret, _MAX_PATH, fname);
				strcat_s(ret, _MAX_PATH, ext ? ext : ".cfg");
			}
			return ret;
		}

		inline void load(const char *fname, void *cfg, size_t len) {
			// load settings
			const char *strPathName = buildFilename(fname, ".cfg");

			FILE *fp;
			fopen_s(&fp, strPathName, "rb");
			if(fp != NULL) {
				fread(cfg, len, 1, fp);
				fclose(fp);
			}
		}

		inline void save(const char *fname, void *cfg, size_t len) {
			const char *strPathName = buildFilename(fname, ".cfg");

			FILE *fp;
			fopen_s(&fp, strPathName, "wb");
			if(fp != NULL) {
				fwrite(cfg, len, 1, fp);
				fclose(fp);
			}
		}


	}

	namespace detours {
		typedef void *PVOID;
		inline void Attach(PVOID *ppPointer, PVOID pDetour) { BAM::Internal().BAM_AttachDetour(ppPointer, pDetour); }
		inline void Detach(PVOID *ppPointer, PVOID pDetour) { BAM::Internal().BAM_DetachDetour(ppPointer, pDetour); }
		template<typename T>
		void AttachOrDetach(bool doAttach, T& real, T routed)
		{
			if (doAttach)
				Attach(&(PVOID&)real, routed);
			else
				Detach(&(PVOID&)real, routed);
		};
	}

	namespace render {
		inline unsigned int CompileShader(const char *vertexSrc, const char *fragmentSrc) {
			return BAM::Internal().BAM_compileShaders(vertexSrc, fragmentSrc);
		}
	}

	namespace fpObjects {
		template<typename T>
		inline void foreach(T f)
		{
			struct objEntry {
				const wchar_t* name;
				int type;
				void* pUnknown;
			};

			struct allFpObjects {
				int num;
				objEntry* data;
			};

			// new BAM required v332
			if (BAM::Internal().BAM_GetAllFPObjects)
			{
				allFpObjects* all = reinterpret_cast<allFpObjects*>(BAM::Internal().BAM_GetAllFPObjects());
				for (int i = 0; i < all->num; ++i)
				{
					const auto& obj = all->data[i];
					f(wtoa(obj.name), obj.type, obj.pUnknown);
				}
			}
		}

		inline void GetDmdAdditionalInfo(void* pUnknown, bool& outIsDmdFullColor, float(&outBackColor)[3], float(&outBaseColor)[3], uint32_t& outBackTextureId)
		{
			BAM::Internal().BAM_GetDmdAdditionalInfo(pUnknown, &outIsDmdFullColor, &outBackColor[0], &outBaseColor[0], &outBackTextureId);
		}
	}
};

/// <summary>
/// IDs of Future Pinball objects types
/// </summary>
enum {
	TABLE = 1,
	SURFACE = 2,
	FLIPPER = 7,
	TOY = 18,
	AUTOPLUNGER = 19,
	BUMPER = 20,
	DIVERTER = 21,
	EMKICKER = 22,
	KICKER = 23,
	POPUP = 24,
	TARGET = 25,
	DROPTARGET = 26,
	SWINGTARGET = 27,
	TARGETVARI = 28,
	BULB = 29,
	LIGHT = 30,
	SHAPEABLELIGHT = 31,
	GATE = 32,
	SPINNER = 33,
	TRIGGER = 34,
	TRIGGEROPTO = 35,
	SPINNINGDISK = 36,
	WALL = 37,
	FLASHER = 38,
	HUDDMD = 39,
	DISPDMD = 40,
	OVERLAY = 41,
	HUDSEG = 42,
	HUDREEL = 43,
	DISPREEL = 44,
	DISPSEG = 45,
	IMAGELIGHT = 46,
	RUBBER = 47,
};

/*
//#ifndef BAM_PLUGIN_C_FUNCTIONS
//#define BAM_PLUGIN_C_FUNCTIONS
//	BAMEXPORT int BAM_load(HMODULE bam_module) {
//		BAM::onLoad(bam_module);
//	}
//#endif

typedef BAMIMPORT void BAM_MessageBox_func(const char *fmt, ...);
typedef	BAMIMPORT void BAM_menu_create_func(int pluginID, const char *pname, const char *phelp);
typedef	BAMIMPORT void BAM_menu_add_3Points_func(int pluginID);
typedef	BAMIMPORT void BAM_menu_add_info_func(int pluginID, const char *text);
typedef	BAMIMPORT void BAM_push_raw_XYZ_func(int pluginID, double x, double y, double z);
typedef BAMIMPORT void BAM_menu_add_info3D_func(int pluginID, const char *txt, double *x, double *y, double *z, const char *help );

typedef	BAMIMPORT void BAM_menu_add_cam_func(int pluginID);
typedef	BAMIMPORT void BAM_menu_add_TL_func(int pluginID);
typedef	BAMIMPORT void BAM_menu_add_Reality_func(int pluginID);
typedef	BAMIMPORT void BAM_menu_add_param_func(int PluginID, const char *txt, double *value, double smallStep, double bigStep, const char *help );
typedef BAMIMPORT BYTE *BAM_get_cam_buffer_func(int cam);

typedef BAMIMPORT void BAM_menu_add_switch_func(int PluginID, const char *txt, int *value, const char *options[], int count, const char *help );

#define ARRAY_ENTRIES(array)	(sizeof(array)/sizeof(array[0]))
#define PLUGIN_ID_NUM 0x0088 // RaV plugins 0x80 - 0xa0, 0x0101 - Kinect, other - free
*/
extern "C" {
	BAMEXPORT int BAM_load(HMODULE bam_module);
	BAMEXPORT void BAM_unload();
	BAMEXPORT void BAM_hudDisplay();
	BAMEXPORT void BAM_PluginStart();
	BAMEXPORT void BAM_PluginStop();
	BAMEXPORT void BAM_AttachDetours();
	BAMEXPORT void BAM_DetachDetours();
};


// -----------------------------------------------------------------
/********************************************************
dbg::hudDebug(const char *fmt, ...);
dbg::hudDebugLong(const char *fmt, ...);

- x, y, position on BAM hud. left/lower corner [0,0] , top/right corner [1079, 639] 
  it is "virtual resolution" 

- size (in menu i use 2.2f)

- shadow - (then true is drawed "shadow" to letters)


Now about MY#MOD.

In all text printed in hud (in menu, help, or from BAM_printf) i use # as special char to modify displayed text.
To draw # You have to add ##

Now how to use '#'

#cxyz# - change color to (RGB) xxyyzz (x/y/z is hex [0-9a-f] )
#cxxyyzz# - full rgb color

#+ - increase font "25%"

#- - decrease font "25%"

example:   "text#-smalltext#+sameSizeLikeAtBegin#c00f#bluetext"

#l0123# - (small L) - draw text before # and  move text pointer given number "pixels" left (text have constant width)

example "PARAM#l300#VALUE" - will draw "PARAM" as text with Width = 300 and after this VALUE

#m123# - center text, text can be wider than given "value".
example 
BAM_printf(500, 500, 3.0, true, "centeredtext#m0"); 
BAM_printf(500, 400, 3.0, true, "centeredlongtext#m0"); 
BAM_printf(500, 300, 3.0, true, "smal#m0"); 
will draw all text centered

#r123# - move text to right of given "space"

#. - is joust marker to divide text
example
"text#.to right#r300#" ... will draw "text                                  to right"


i added macros:
#define DEFPW "#r160#"
#define DEFIW "#m350#"
 
DEFIW will center "buttons" / "submenus"
DEFPW will move "params names to right"

example:
"#-Processing res:"DEFPW"#+ #cfff#%s", .
is like:
"Processing res: %s"

but text "Processing res:" is moved to right and whole text is 160 "virtual pixels" wide and is drawed 25% smaller font

text value %s is drawed "normal size" and color is set to "white".


**********************************************************************************************/

