/////////////////////////////////////////////////////////////////////

#include <Windows.H>
#include <WindowsX.H>
#include <CommCTRL.H>
#include <ShellAPI.H>
#include <Stdio.H>
#include <StdLib.H>
#include <IO.H>

#include "Headers/Definitions.H"
#include "Headers/Decompress.H"
#include "Headers/DeleteObject.H"
#include "Headers/BlowFish.H"
#include "Headers/Interface.H"
#include "Headers/Functions.H"
#include "Headers/UnPackFiles.H"

#include "Dialogs/MainDialog.H"
#include "Dialogs/StatusDialog.H"

/////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevious, LPSTR Cmd , int CmdShow)
{
    HANDLE xMutex = NULL;

    if( (xMutex = OpenMutex(SYNCHRONIZE, FALSE, TitleCaption)) )
    {
        MessageBox (NULL, "Only one instance can be run at a time!" , TitleCaption, 0 + MB_ICONASTERISK + MB_SYSTEMMODAL);
        ReleaseMutex(xMutex);
        CloseHandle(xMutex);
        return 0;
    }
    xMutex = CreateMutex(0, FALSE, TitleCaption);

    MainInstance = hInst;

    GetModuleFileName(NULL, SourceFileName, 1024);

    DWORD ModuleFileSize = Get_FileSize(SourceFileName);

    if(ModuleFileSize == ExecutableEnd)
    {
        MessageBox (NULL, "Package contains no data!" , TitleCaption, 0 + MB_ICONASTERISK + MB_SYSTEMMODAL);
        ReleaseMutex(xMutex);
        CloseHandle(xMutex);
        return 0;
    }

    if(ModuleFileSize < ExecutableEnd)
    {
        MessageBox (NULL, "Package is corrupt!" , TitleCaption, 0 + MB_ICONASTERISK + MB_SYSTEMMODAL);
        ReleaseMutex(xMutex);
        CloseHandle(xMutex);
        return 0;
    }

    char Drive[256];
    char Directory[256];
    char File[256];

    _splitpath(SourceFileName, Drive,Directory, File,NULL);
    _makepath(Package.TargetDir, Drive, Directory, File, NULL);

    DialogBox(MainInstance, MAKEINTRESOURCE(100), 0, MainDialog);

    ReleaseMutex(xMutex);
    CloseHandle(xMutex);

    return 0;
}

/////////////////////////////////////////////////////////////////////

