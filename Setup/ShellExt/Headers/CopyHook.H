//  MODULE:   copyhook.cpp
//
//  PURPOSE:   Implements the ICopyHook member functions necessary to support
//             the copy hook portioins of this shell extension.  
//             Copy hook handlers are called each time a folder is copied, moved, 
//             renamed, etc. in the system.  Note that the CopyCallback is NOT
//             called for each file, but only for entire folders.
//

#include "priv.h"
#include "shellext.h"


//
//  FUNCTION: CShellExt::CopyCallback(HWND,
//                                    UINT, 
//                                    UINT, 
//                                    LPCSTR,
//                                    DWORD,
//                                    LPCSTR,
//                                    DWORD)
//
//  PURPOSE: Called by the shell when a folder is being manipulated.
//
//  PARAMETERS:
//    hwnd          - Window handle to use for any UI stuff
//    wFunc         - what operation is being done
//    wFlags        - and flags (FOF_*) set in the initial call 
//                    to the file operation
//    pszSrcFile    - name of the source file
//    dwSrcAttribs  - file attributes of the source file
//    pszDestFile   - name of the destiation file (for move and renames)
//    dwDestAttribs - file attributes of the destination file
//
//  RETURN VALUE:
//
//    IDYES    - allow the operation
//    IDNO     - disallow the operation on this file, but continue with
//               any other operations (eg. batch copy)
//    IDCANCEL - disallow the current operation and cancel any pending
//               operations
//
//  COMMENTS:
//

STDMETHODIMP_(UINT) CShellExt::CopyCallback(HWND hwnd, 
                                            UINT wFunc, 
                                            UINT wFlags, 
                                            LPCSTR pszSrcFile, 
                                            DWORD dwSrcAttribs,
                                            LPCSTR pszDestFile, 
                                            DWORD dwDestAttribs)
{

    MessageBox(NULL, "Copy Call back", "448Shell", MB_OK);

    return IDYES;
}
