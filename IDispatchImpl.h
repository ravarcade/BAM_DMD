#pragma once

/// Helper template class for IDispatch / IUnknown interface implementation.
/// It implements needed methods and behave, so i don't need to copy that all interface methods.
template< class T>
class IDispatchImpl : public T
{
public:
	IDispatchImpl(HMODULE hModule) : m_nRefCount(2) 
	{
		// load type lib for this class from DLL
		wchar_t moduleName[2048];
		GetModuleFileNameW(hModule, moduleName, sizeof(moduleName));
		auto l = lstrlenW(moduleName);
		moduleName[l] = '\\';
		moduleName[l + 1] = '1';
		moduleName[l + 2] = 0;
		ITypeLib* pTypeLib = NULL;
		LoadTypeLib(moduleName, &pTypeLib);
		HRESULT hr = pTypeLib->GetTypeInfoOfGuid(IID_IDmdCom, &m_pTypeInfo);
	}

	virtual ~IDispatchImpl()
	{
		m_pTypeInfo->Release();
	}

	// IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv)
	{
		if (riid == IID_IUnknown)
			*ppv = (IUnknown*)this;
		else if (riid == IID_IDispatch)
			*ppv = (IDispatch*)this;
		else
		{
			*ppv = NULL;
			return E_NOINTERFACE;
		}
		AddRef();
		return S_OK;
	}

	ULONG STDMETHODCALLTYPE AddRef()
	{
		return InterlockedIncrement(&m_nRefCount);
	}

	ULONG STDMETHODCALLTYPE Release()
	{
		long nRefCount = 0;
		nRefCount = InterlockedDecrement(&m_nRefCount);
		if (nRefCount == 0)
			delete this;
		return nRefCount;
	}


	// IDispatch
	HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT* pCountTypeInfo)
	{
		*pCountTypeInfo = 1;
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTypeInfo, LCID lcid, ITypeInfo** ppITypeInfo)
	{
		*ppITypeInfo = NULL;
		if (iTypeInfo != 0)
			return DISP_E_BADINDEX;
		m_pTypeInfo->AddRef();
		*ppITypeInfo = m_pTypeInfo;
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)
	{
		if (riid != IID_NULL)
			return DISP_E_UNKNOWNINTERFACE;

		return DispGetIDsOfNames(m_pTypeInfo, rgszNames, cNames, rgDispId);
	}

	HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr)
	{
		if (riid != IID_NULL)
			return DISP_E_UNKNOWNINTERFACE;

		return DispInvoke(this, m_pTypeInfo, dispIdMember, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
	}

private:
	long m_nRefCount;

protected:
	static ITypeInfo* m_pTypeInfo;
};

template<class T>
ITypeInfo* IDispatchImpl<T>::m_pTypeInfo = nullptr;
