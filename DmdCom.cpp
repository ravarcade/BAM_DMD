#include "DmdCom.h"
#include "DmdCom_i.c"

HRESULT STDMETHODCALLTYPE DmdCom::Msg(BSTR txt)
{
	MessageBoxW(0, txt, L"DmdCom", 0);

	return S_OK;
}