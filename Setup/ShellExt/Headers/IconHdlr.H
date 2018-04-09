// Modudule: ICONHDLR.CPP
//
// Purpose:  Implements the IExtractIcon and IPersistFile member
//			 functions necessary to support the icon handler portion
//			 of this shell extension.
//

#include "priv.h"
#include "shellext.h"

extern HINSTANCE g_hmodThisDll; // Handle to this DLL itself.

// *********************** IExtractIcon Implementation *************************

STDMETHODIMP CShellExt::GetIconLocation(UINT   uFlags,
										LPSTR  szIconFile,
										UINT   cchMax,
										int   *piIndex,
										UINT  *pwFlags)
{  
	GetModuleFileName(g_hmodThisDll, szIconFile, cchMax);

	*piIndex = (int)GetPrivateProfileInt("IconImage", 
										 "Index", 
										 0, 
										 m_szFileUserClickedOn);

	return S_OK;
}


STDMETHODIMP CShellExt::Extract(LPCSTR pszFile,
								UINT   nIconIndex,
								HICON  *phiconLarge,
								HICON  *phiconSmall,
								UINT   nIconSize)
{
	return S_FALSE;
}


// *********************** IPersistFile Implementation ******************

STDMETHODIMP CShellExt::GetClassID(LPCLSID lpClassID)
{
	return E_FAIL;
}

STDMETHODIMP CShellExt::IsDirty()
{
	return S_FALSE;
}

STDMETHODIMP CShellExt::Load(LPCOLESTR lpszFileName, DWORD grfMode)
{
	WideCharToMultiByte(CP_ACP, 			   // CodePage
						0,					  // dwFlags
						lpszFileName,		   // lpWideCharStr
						-1, 				   // cchWideChar
						m_szFileUserClickedOn, // lpMultiByteStr
						sizeof(m_szFileUserClickedOn),	// cchMultiByte,
						NULL,				   // lpDefaultChar,
						NULL);				   // lpUsedDefaultChar

	return NOERROR;
}

STDMETHODIMP CShellExt::Save(LPCOLESTR lpszFileName, BOOL fRemember)
{
	return E_FAIL;
}

STDMETHODIMP CShellExt::SaveCompleted(LPCOLESTR lpszFileName)
{
	return E_FAIL;
}

STDMETHODIMP CShellExt::GetCurFile(LPOLESTR FAR* lplpszFileName)
{
	return E_FAIL;
}
