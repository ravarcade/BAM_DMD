#include <windows.h>
#include <memory>
#include "GL/glew.h"
#include "gl/GL.h"
#include "BAM.h"

// two handy macros
#define REAL(x) namespace real { decltype(::x) (*x) = ::x; }
#define MAKEDETOUR(x) BAM::detours::AttachOrDetach(doAttach, real::x, routed::x)

// there are 3 steps to add detour:

// 1. Add line REAL(name_of_function). It will create name copy in real namespace
REAL(glTexImage2D);

// 2. Add in routed namespace your version. You can call real function from real namespace
namespace routed {
	void APIENTRY glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels)
	{
		static int cnt = 0;
		BAM::dbg::hudDebug("in detour: %d\n", ++cnt);
		real::glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
	}
}

// 3. Add line inside MakeDetours: MAKEDETOUR(name_of_function). It will call Attach or Detach if needed to connect routed with real function
void MakeDetours(bool doAttach)
{
	MAKEDETOUR(glTexImage2D);
}




