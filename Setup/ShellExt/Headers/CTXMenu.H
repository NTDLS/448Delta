//	MODULE:   ctxmenu.cpp
//
//	PURPOSE:   Implements the IContextMenu member functions necessary to support
//			   the context menu portioins of this shell extension.	Context menu
//			   shell extensions are called when the user right clicks on a file
//			   (of the type registered for the shell extension--see SHELLEXT.REG
//			   for details on the registry entries.  In this sample, the relevant
//			   files are of type .GAK) in the Explorer, or selects the File menu 
//			   item.
//
#include <stdio.H>
#include "priv.h"
#include "shellext.h"


//
//	FUNCTION: CShellExt::QueryContextMenu(HMENU, UINT, UINT, UINT, UINT)
//
//	PURPOSE: Called by the shell just before the context menu is displayed.
//			 This is where you add your specific menu items.
//
//	PARAMETERS:
//	  hMenu 	 - Handle to the context menu
//	  indexMenu  - Index of where to begin inserting menu items
//	  idCmdFirst - Lowest value for new menu ID's
//	  idCmtLast  - Highest value for new menu ID's
//	  uFlags	 - Specifies the context of the menu event
//
//	RETURN VALUE:
//
//
//	COMMENTS:
//

STDMETHODIMP CShellExt::QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
	UINT idCmd = idCmdFirst;

	if ((uFlags & 0x000F) == CMF_NORMAL)
	{
		indexMenu++;
		InsertMenu(hMenu, indexMenu, MF_STRING|MF_BYPOSITION, idCmd++, "Open with 448Delta");

		HBITMAP BitMapResource = LoadBitmap(g_hmodThisDll, MAKEINTRESOURCE(100) );
		SetMenuItemBitmaps(hMenu, indexMenu, MF_BYPOSITION, BitMapResource, BitMapResource);

		return ResultFromShort(idCmd-idCmdFirst);
	}
	/*
	if (uFlags & CMF_VERBSONLY)
	{
		indexMenu++;
		InsertMenu(hMenu, indexMenu++, MF_STRING|MF_BYPOSITION, idCmd++, "Encrypt Shortcut");

		HBITMAP BitMapResource = LoadBitmap(g_hmodThisDll, MAKEINTRESOURCE(100) );
		SetMenuItemBitmaps(hMenu, indexMenu, MF_BYPOSITION, BitMapResource, BitMapResource);

		return ResultFromShort(idCmd-idCmdFirst);
	}
	*/
	if (uFlags & CMF_EXPLORE)
	{
		indexMenu++;
		InsertMenu(hMenu, indexMenu++, MF_STRING|MF_BYPOSITION, idCmd++, "Open with 448Delta");

		HBITMAP BitMapResource = LoadBitmap(g_hmodThisDll, MAKEINTRESOURCE(100) );
		SetMenuItemBitmaps(hMenu, indexMenu, MF_BYPOSITION, BitMapResource, BitMapResource);

		return ResultFromShort(idCmd-idCmdFirst);
	}

	if (uFlags & CMF_DEFAULTONLY)
	{
		return NOERROR;
	}

   return NOERROR;
}

//
//	FUNCTION: CShellExt::InvokeCommand(LPCMINVOKECOMMANDINFO)
//
//	PURPOSE: Called by the shell after the user has selected on of the
//			 menu items that was added in QueryContextMenu().
//
//	PARAMETERS:
//	  lpcmi - Pointer to an CMINVOKECOMMANDINFO structure
//
//	RETURN VALUE:
//
//
//	COMMENTS:
//

STDMETHODIMP CShellExt::InvokeCommand(LPCMINVOKECOMMANDINFO lpcmi)
{
	HRESULT hr = E_INVALIDARG;

	//If HIWORD(lpcmi->lpVerb) then we have been called programmatically
	//and lpVerb is a command that should be invoked.  Otherwise, the shell
	//has called us, and LOWORD(lpcmi->lpVerb) is the menu ID the user has
	//selected.  Actually, it's (menu ID - idCmdFirst) from QueryContextMenu().
	if (!HIWORD(lpcmi->lpVerb))
	{
		UINT idCmd = LOWORD(lpcmi->lpVerb);

		STGMEDIUM medium;
		UINT cbFiles   = 0;
		HRESULT hres   = 0;
		FORMATETC fmte = {CF_HDROP, (DVTARGETDEVICE FAR *)NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};

		if (m_pDataObj)  //Paranoid check, m_pDataObj should have something by now...
		   hres = m_pDataObj->GetData(&fmte, &medium);

		if (SUCCEEDED(hres))
		{
			if (medium.hGlobal)
				cbFiles = DragQueryFile((HDROP)medium.hGlobal, (UINT)-1, 0, 0);

			DragQueryFile((HDROP)medium.hGlobal, 0, m_szFileUserClickedOn, sizeof(m_szFileUserClickedOn));

			if (cbFiles > 1)
			{
				MessageBox(NULL, "448Delta can work with only one item at a time." ,"Execute Error",0 + MB_ICONHAND + MB_SYSTEMMODAL);
				return NOERROR;
			}

			if (cbFiles == 0)
			{
				MessageBox(NULL, "No working file selected." ,"Execute Error",0 + MB_ICONHAND + MB_SYSTEMMODAL);
				return NOERROR;
			}
		}
		else{
			MessageBox(NULL, "Failed to retreive selection name." ,"Execute Error",0 + MB_ICONHAND + MB_SYSTEMMODAL);
			return NOERROR;
		}

		if(idCmd == 0)
		{
			char StringBuffer[1024];
			unsigned long SizeOfStringBuffer = 1024;

			if(!Get_StringRegistryValue(HKEY_LOCAL_MACHINE, PathKey, "448Delta", StringBuffer, SizeOfStringBuffer))
			{
				MessageBox(NULL, "448Delta executable path key not found. \r\nPlease reinstall 448Delta." ,"Execute Error",0 + MB_ICONHAND + MB_SYSTEMMODAL);
				return S_OK;
			}

			HANDLE Result = ShellExecute(lpcmi->hwnd, NULL, StringBuffer, m_szFileUserClickedOn, lpcmi->lpDirectory, lpcmi->nShow);

			if(Result == (VOID *) 0)					  MessageBox(NULL, "The operating system is out of memory or resources."											   ,"Execute Error", MB_ICONHAND + MB_SYSTEMMODAL);
			if(Result == (VOID *) ERROR_FILE_NOT_FOUND)   MessageBox(NULL, "The specified file was not found."																   ,"Execute Error", MB_ICONHAND + MB_SYSTEMMODAL);
			if(Result == (VOID *) ERROR_PATH_NOT_FOUND)   MessageBox(NULL, "The specified path was not found."																   ,"Execute Error", MB_ICONHAND + MB_SYSTEMMODAL);
			if(Result == (VOID *) ERROR_BAD_FORMAT) 	  MessageBox(NULL, "The .EXE file is invalid (non-Win32 .EXE or error in .EXE image)."								   ,"Execute Error", MB_ICONHAND + MB_SYSTEMMODAL);
			if(Result == (VOID *) SE_ERR_ACCESSDENIED)	  MessageBox(NULL, "The operating system denied access to the specified file."										   ,"Execute Error", MB_ICONHAND + MB_SYSTEMMODAL);
			if(Result == (VOID *) SE_ERR_ASSOCINCOMPLETE) MessageBox(NULL, "The filename association is incomplete or invalid." 											   ,"Execute Error", MB_ICONHAND + MB_SYSTEMMODAL);
			if(Result == (VOID *) SE_ERR_DDEBUSY)		  MessageBox(NULL, " The DDE transaction could not be completed because other DDE transactions were being processed."  ,"Execute Error", MB_ICONHAND + MB_SYSTEMMODAL);
			if(Result == (VOID *) SE_ERR_DDEFAIL)		  MessageBox(NULL, "The DDE transaction failed."																	   ,"Execute Error", MB_ICONHAND + MB_SYSTEMMODAL);
			if(Result == (VOID *) SE_ERR_DDETIMEOUT)	  MessageBox(NULL, "The DDE transaction could not be completed because the request timed out."						   ,"Execute Error", MB_ICONHAND + MB_SYSTEMMODAL);
			if(Result == (VOID *) SE_ERR_DLLNOTFOUND)	  MessageBox(NULL, "The specified dynamic-link library was not found."												   ,"Execute Error", MB_ICONHAND + MB_SYSTEMMODAL);
			if(Result == (VOID *) SE_ERR_NOASSOC)		  MessageBox(NULL, "There is no application associated with the given filename extension."							   ,"Execute Error", MB_ICONHAND + MB_SYSTEMMODAL);
			if(Result == (VOID *) SE_ERR_OOM)			  MessageBox(NULL, "There was not enough memory to complete the operation." 										   ,"Execute Error", MB_ICONHAND + MB_SYSTEMMODAL);
			if(Result == (VOID *) SE_ERR_PNF)			  MessageBox(NULL, "The specified path was not found."																   ,"Execute Error", MB_ICONHAND + MB_SYSTEMMODAL);
			if(Result == (VOID *) SE_ERR_SHARE) 		  MessageBox(NULL, "A sharing violation occurred."																	   ,"Execute Error", MB_ICONHAND + MB_SYSTEMMODAL);

			return NOERROR;
		}

	}

	return hr;
}


//
//	FUNCTION: CShellExt::InvokeCommand(LPCMINVOKECOMMANDINFO)
//
//	PURPOSE: Called by the shell after the user has selected on of the
//			 menu items that was added in QueryContextMenu().
//
//	PARAMETERS:
//	  lpcmi - Pointer to an CMINVOKECOMMANDINFO structure
//
//	RETURN VALUE:
//
//
//	COMMENTS:
//

STDMETHODIMP CShellExt::GetCommandString(UINT idCmd, UINT uFlags, UINT FAR *reserved, LPSTR pszName, UINT cchMax)
{
	switch (idCmd)
	{
		case 0:
			if (uFlags & GCS_VERBA)
				lstrcpyn(pszName, "Open this file with 448Delta Encryption.", cchMax);
			else
				lstrcpynW((LPWSTR)pszName, L"Open this file with 448Delta Encryption.", cchMax);

			break;
	}

	return NOERROR;
}



