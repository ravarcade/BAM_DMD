#include <windows.h>
#include <memory>
#include "GL/glew.h"
#include "gl/GL.h"

#include "BAM.h"
#include "DmdCom.h"
#include "Dmd.h"
#include "detours.h"

#include "winnt.h"
#include <process.h>		// for starting thread

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

const char* cfgFilename = "BAM_DMD";
std::string jsonMessage;
int pipeTimeOut = 10;
namespace {

    void SendJsonMessage(void* dummy)
    {

        LPCWSTR lpszPipename = TEXT("\\\\.\\pipe\\futuredmd");
        HANDLE hPipe;

        hPipe = CreateFile(
            lpszPipename,   // pipe name 
            GENERIC_WRITE,  // write only access 
            0,              // no sharing 
            NULL,           // default security attributes
            OPEN_EXISTING,  // opens existing pipe 
            0,              // default attributes 
            NULL);          // no template file 

        WaitNamedPipe(lpszPipename, pipeTimeOut);

        if (hPipe != INVALID_HANDLE_VALUE)
        {
            int datasize = jsonMessage.size();
            DWORD dwSizeToWrite(datasize); DWORD dwWrittenSize(0);
            while (dwSizeToWrite > 0)
            {
                WriteFile(hPipe, &jsonMessage[datasize - dwSizeToWrite], dwSizeToWrite, &dwWrittenSize, NULL);
                dwSizeToWrite -= dwWrittenSize;
                dwWrittenSize = 0;
            }

            CloseHandle(hPipe);
        }

    }

    void RawImageToBuffer(CFpDmd dmd, byte* rgb_buf) {
        // read raw image into rgb buffer
        int size = dmd.width * dmd.height;
        int idx = 0;

        for (int i = 0; i < size; i++)
        {
            idx = i * 3;
            int red = (dmd.rawDmdBuffer[(0 * size) + i] & 0xFF0000) >> 16; // Use as Alpha and Red

            // using backColor as color if it's not a full color dmd
            if (!dmd.isFullColor) {
                rgb_buf[idx + 0] = (byte)(dmd.backColor[0] * red);
                rgb_buf[idx + 1] = (byte)(dmd.backColor[1] * red);
                rgb_buf[idx + 2] = (byte)(dmd.backColor[2] * red);
            }
            else {
                rgb_buf[idx] = red; // Red
                rgb_buf[idx + 1] = (dmd.rawDmdBuffer[(1 * size) + i] & 0xFF00) >> 8; // Green 
                rgb_buf[idx + 2] = (dmd.rawDmdBuffer[(2 * size) + i] & 0xFF); // Blue
            }
        }
    }

    const char* statusAsString(int status) {
        switch (status)
        {
        case 0: return "Running";
        case 1: return "Started";
        case 2: return "Stopped";
        }
        return "Unknown";
    };

    void PrepareMessage(std::vector<CFpDmd> dmds, int status) {
        // document is the root of a json message
        rapidjson::Document jsonContainer;

        // define the document as an object rather than an array
        jsonContainer.SetObject();

        // must pass an allocator when the object may need to allocate memory
        rapidjson::Document::AllocatorType& allocator = jsonContainer.GetAllocator();

        jsonContainer.AddMember("Status", status, allocator);
        rapidjson::Value statusDescription;
        statusDescription.SetString(statusAsString(status), allocator);
        jsonContainer.AddMember("StatusDescription", statusDescription, allocator);

        if (status == 0) {
			rapidjson::Value dispSegments = rapidjson::Value(rapidjson::kArrayType);
			rapidjson::Value hudSegments = rapidjson::Value(rapidjson::kArrayType);

            for (auto& dmd : dmds)
            {
                if (dmd.type == DISPDMD || dmd.type == HUDDMD) {
                    // create an object to store all DMD info
                    rapidjson::Value jdmd(rapidjson::kObjectType);
                    jdmd.SetObject();

                    jdmd.AddMember("name", dmd.name, allocator);
                    jdmd.AddMember("type", dmd.type, allocator);
                    jdmd.AddMember("isFullColor", dmd.isFullColor, allocator);
                    jdmd.AddMember("isOnBackbox", dmd.isOnBackbox, allocator);
                    jdmd.AddMember("width", dmd.width, allocator);
                    jdmd.AddMember("height", dmd.height, allocator);
                    jdmd.AddMember("color", *dmd.color, allocator);

                    rapidjson::Value backColor(rapidjson::kObjectType); backColor.SetObject();
                    backColor.AddMember("r", dmd.backColor[0], allocator);
                    backColor.AddMember("g", dmd.backColor[1], allocator);
                    backColor.AddMember("b", dmd.backColor[2], allocator);
                    jdmd.AddMember("backColor", backColor, allocator);

                    rapidjson::Value baseColor(rapidjson::kObjectType); baseColor.SetObject();
                    baseColor.AddMember("r", dmd.baseColor[0], allocator);
                    baseColor.AddMember("g", dmd.baseColor[1], allocator);
                    baseColor.AddMember("b", dmd.baseColor[2], allocator);
                    jdmd.AddMember("baseColor", baseColor, allocator);

                    rapidjson::Value tmpColor(rapidjson::kObjectType); tmpColor.SetObject();
                    tmpColor.AddMember("r", dmd.tmpColor[0], allocator);
                    tmpColor.AddMember("g", dmd.tmpColor[1], allocator);
                    tmpColor.AddMember("b", dmd.tmpColor[2], allocator);
                    jdmd.AddMember("tmpColor", tmpColor, allocator);

                    int len = (dmd.width * dmd.height * 3);
                    byte* imgBuffer = new byte[len];
                    RawImageToBuffer(dmd, imgBuffer);
                    rapidjson::Value rgbBufferValue;
                    rgbBufferValue.SetString(reinterpret_cast<char*>(imgBuffer), len, allocator);
                    jdmd.AddMember("rgbBuffer", rgbBufferValue, allocator);
                    delete[] imgBuffer;

                    if (dmd.type == DISPDMD) {
                        jsonContainer.AddMember("DISPDMD", jdmd, allocator);
                    }
                    else {
                        jsonContainer.AddMember("HUDDMD", jdmd, allocator);
                    }

                }
                else if (dmd.type == DISPSEG || dmd.type == HUDSEG) {
                    // create an object to store an individual segment
                    rapidjson::Value segment(rapidjson::kObjectType);
                    segment.SetObject();

                    segment.AddMember("name", dmd.name, allocator);
                    segment.AddMember("type", dmd.type, allocator);
                    segment.AddMember("isFullColor", dmd.isFullColor, allocator);
                    segment.AddMember("isOnBackbox", dmd.isOnBackbox, allocator);

					segment.AddMember("color", *dmd.color, allocator);

					//rapidjson::Value backColor(rapidjson::kObjectType); backColor.SetObject();
					//backColor.AddMember("r", dmd.backColor[0], allocator);
					//backColor.AddMember("g", dmd.backColor[1], allocator);
					//backColor.AddMember("b", dmd.backColor[2], allocator);
					//segment.AddMember("BackColor", backColor, allocator);

					//rapidjson::Value baseColor(rapidjson::kObjectType); baseColor.SetObject();
					//baseColor.AddMember("r", dmd.baseColor[0], allocator);
					//baseColor.AddMember("g", dmd.baseColor[1], allocator);
					//baseColor.AddMember("b", dmd.baseColor[2], allocator);
					//segment.AddMember("BaseColor", baseColor, allocator);

					rapidjson::Value tmpColor(rapidjson::kObjectType); tmpColor.SetObject();
					tmpColor.AddMember("r", dmd.tmpColor[0], allocator);
					tmpColor.AddMember("g", dmd.tmpColor[1], allocator);
					tmpColor.AddMember("b", dmd.tmpColor[2], allocator);
					segment.AddMember("tmpColor", tmpColor, allocator);

                    segment.AddMember("segAlign", dmd.segAlign, allocator);
                    segment.AddMember("segCharShapes", *dmd.segCharShapes, allocator);
                    segment.AddMember("segLength", dmd.segLength, allocator);
                    segment.AddMember("segType", dmd.segType, allocator);
					
					//byte* imgBuffer = new byte[len];
					//RawImageToBuffer(dmd, imgBuffer);
					//segdmd.AddMember("segRawBuffer", dmd.segRawBuffer, allocator);
                    //segdmd.AddMember("segRawBufferWithEffects", dmd.segRawBufferWithEffects, allocator);

                    rapidjson::Value segText;
                    segText.SetString(dmd.segText, allocator);
                    segment.AddMember("segText", segText, allocator);

					if (dmd.type == DISPSEG) {
						dispSegments.PushBack(segment, allocator);
					}
					else {
						hudSegments.PushBack(segment, allocator);
					}


                }
            }

            if (!dispSegments.Empty()) {
                // add display segments to the JSON document
                jsonContainer.AddMember("DISPSEG", dispSegments, allocator);
            }
			if (!hudSegments.Empty()) {
				// add hud segments to the JSON document
				jsonContainer.AddMember("HUDSEG", hudSegments, allocator);
			}
        }

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        jsonContainer.Accept(writer);

        jsonMessage = buffer.GetString();

        rapidjson::Document().Swap(jsonContainer);  // <-- swap with temporary to wipe out memory pool
    }

}


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
			
			if (type == HUDDMD || type == DISPDMD || type == DISPSEG || type == HUDSEG)
			{
				dmds.emplace_back(CFpDmd{name, type, pUnknown});

			}
		});

    //Send status Started
    PrepareMessage(dmds, 1);
    pipeTimeOut = 10;
    _beginthread(SendJsonMessage, 0, NULL);
}

void CDmd::OnPluginStop()
{
	dmdCom.reset();  // destroy COM object
	BAM::helpers::save(cfgFilename, &cfg, sizeof(cfg));

    //Send status Stopped
    PrepareMessage(dmds, 2);
    pipeTimeOut = 500;
    // Send 
    _beginthread(SendJsonMessage, 0, NULL);
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

    PrepareMessage(dmds, 0);
    pipeTimeOut = 10;
    _beginthread(SendJsonMessage, 0, NULL);
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
		case HUDSEG: return "HudSEG";
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
		break;

	case HUDSEG:
		memset(backColor, 0, sizeof(backColor));
		memset(baseColor, 0, sizeof(backColor));
		segText[sizeof(segText) - 1] = 0; 
		segLength = reinterpret_cast<uint32_t*>(pUnknown)[0x72];
		segRawBufferWithEffects = reinterpret_cast<uint32_t**>(pUnknown)[0x23b] + 0x166;
		segRawBuffer = reinterpret_cast<uint32_t**>(pUnknown)[0x23a] + 0x166;
		break;
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
	case HUDSEG:
	{
		auto pui = reinterpret_cast<uint32_t*>(pUnknown);
		uint32_t col = pui[0x89];
		tmpColor[0] = (col & 0xff) / 255.0f;
		tmpColor[1] = ((col >> 8) & 0xff) / 255.0f;
		tmpColor[2] = ((col >> 16) & 0xff) / 255.0f;
		color = tmpColor;
		segType = pui[0x70];
		segAlign = pui[0x76];
		segCharShapes = pui + 0xa6;
		auto pTxt = reinterpret_cast<char*>(pui + 0x23c);
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
	case HUDSEG:
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
