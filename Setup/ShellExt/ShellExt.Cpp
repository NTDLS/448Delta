#include "Headers/priv.h"

//
// Initialize GUIDs (should be done only and at-least once per DLL/EXE)
//
#pragma data_seg(".text")
#define INITGUID
#include <initguid.h>
#include <shlguid.h>
#pragma data_seg()

#include "Headers/Resource.H"

//
// Global variables
//
UINT	  g_cRefThisDll = 0;	// Reference count of this DLL.
HINSTANCE g_hmodThisDll = NULL; // Handle to this DLL itself.

#define PriorityKey "Software\\NetWorkDLS\\448Delta\\Priority"
#define EncDlgKey	"Software\\NetWorkDLS\\448Delta\\EncryptDialog"
#define DecDlgKey	"Software\\NetWorkDLS\\448Delta\\DecryptDialog"
#define StatDlgKey	"Software\\NetWorkDLS\\448Delta\\StatusDialog"
#define PathKey 	"Software\\NetWorkDLS\\448Delta\\Path"

#include "Headers/Registry.H"
#include "Headers/ShellExt.H"
#include "Headers/shexinit.H"
#include "Headers/CopyHook.H"
#include "Headers/CTXMenu.H"
#include "Headers/IconHdlr.H"
#include "Headers/PropShet.H"
#include "Headers/Resource.H"

extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		// Extension DLL one-time initialization

		g_hmodThisDll = hInstance;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{

	}

	return 1;	// ok
}

//---------------------------------------------------------------------------
// DllCanUnloadNow
//---------------------------------------------------------------------------

STDAPI DllCanUnloadNow(void)
{
	return (g_cRefThisDll == 0 ? S_OK : S_FALSE);
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppvOut)
{
	*ppvOut = NULL;

	if (IsEqualIID(rclsid, CLSID_ShellExtension))
	{
		CShellExtClassFactory *pcf = new CShellExtClassFactory;

		return pcf->QueryInterface(riid, ppvOut);
	}

	return CLASS_E_CLASSNOTAVAILABLE;
}

CShellExtClassFactory::CShellExtClassFactory()
{
	m_cRef = 0L;

	g_cRefThisDll++;	
}
																
CShellExtClassFactory::~CShellExtClassFactory() 			
{
	g_cRefThisDll--;
}

STDMETHODIMP CShellExtClassFactory::QueryInterface(REFIID riid,
												   LPVOID FAR *ppv)
{
	*ppv = NULL;

	// Any interface on this object is the object pointer

	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IClassFactory))
	{
		*ppv = (LPCLASSFACTORY)this;

		AddRef();

		return NOERROR;
	}

	return E_NOINTERFACE;
}	

STDMETHODIMP_(ULONG) CShellExtClassFactory::AddRef()
{
	return ++m_cRef;
}

STDMETHODIMP_(ULONG) CShellExtClassFactory::Release()
{
	if (--m_cRef)
		return m_cRef;

	delete this;

	return 0L;
}

STDMETHODIMP CShellExtClassFactory::CreateInstance(LPUNKNOWN pUnkOuter,
													  REFIID riid,
													  LPVOID *ppvObj)
{
	*ppvObj = NULL;

	// Shell extensions typically don't support aggregation (inheritance)

	if (pUnkOuter)
		return CLASS_E_NOAGGREGATION;

	// Create the main shell extension object.	The shell will then call
	// QueryInterface with IID_IShellExtInit--this is how shell extensions are
	// initialized.

	LPCSHELLEXT pShellExt = new CShellExt();  //Create the CShellExt object

	if (NULL == pShellExt)
		return E_OUTOFMEMORY;

	return pShellExt->QueryInterface(riid, ppvObj);
}


STDMETHODIMP CShellExtClassFactory::LockServer(BOOL fLock)
{
	return NOERROR;
}

// *********************** CShellExt *************************
CShellExt::CShellExt()
{
	m_cRef = 0L;
	m_pDataObj = NULL;

	g_cRefThisDll++;
}

CShellExt::~CShellExt()
{
	if (m_pDataObj)
		m_pDataObj->Release();

	g_cRefThisDll--;
}

STDMETHODIMP CShellExt::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
	*ppv = NULL;

	if (IsEqualIID(riid, IID_IShellExtInit) || IsEqualIID(riid, IID_IUnknown))
	{
		*ppv = (LPSHELLEXTINIT)this;
	}
	else if (IsEqualIID(riid, IID_IContextMenu))
	{
		*ppv = (LPCONTEXTMENU)this;
	}
	else if (IsEqualIID(riid, IID_IExtractIcon))
	{
		*ppv = (LPEXTRACTICON)this;
	}
	else if (IsEqualIID(riid, IID_IPersistFile))
	{
		*ppv = (LPPERSISTFILE)this;
	}
	else if (IsEqualIID(riid, IID_IShellPropSheetExt))
	{
		*ppv = (LPSHELLPROPSHEETEXT)this;
	}
	else if (IsEqualIID(riid, IID_IShellCopyHook))
	{
		*ppv = (LPCOPYHOOK)this;
	}

	if (*ppv)
	{
		AddRef();

		return NOERROR;
	}

	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CShellExt::AddRef()
{
	return ++m_cRef;
}

STDMETHODIMP_(ULONG) CShellExt::Release()
{
	if (--m_cRef)
		return m_cRef;

	delete this;

	return 0L;
}
