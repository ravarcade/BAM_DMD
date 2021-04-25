#include "DmdCom_h.h"
#include "IDispatchImpl.h"

// ----------------------------------------------------------------------------


class DmdCom :
	public IDispatchImpl<IDmdCom>
{
public:
	DmdCom() = delete;
	DmdCom(HMODULE hModule)
		: IDispatchImpl<IDmdCom>(hModule)
	{}

	virtual ~DmdCom() {};

	HRESULT STDMETHODCALLTYPE Msg(BSTR txt);
};