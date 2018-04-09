#include <Windows.H>
#include <WindowsX.H>
#include <ShellAPI.H>
#include <Stdio.H>
#include <Stdlib.H>

#include "Headers/NSWFL.H"        // Definitions For NSWFL.Dll

#include "Headers/Definitions.H"
#include "Headers/Functions.H"

#include "Dialogs/MainDialog.H"

/////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR Cmd , int CmdShow)
{
    HANDLE xMutex;

    if( (xMutex = OpenMutex(SYNCHRONIZE, FALSE, GAMMATITLE)) )
    {
        MessageBox (NULL, "You cannot change the priority while 448Delta is running." , TITLECAPTION, 0 + MB_ICONASTERISK + MB_SYSTEMMODAL);
        ReleaseMutex(xMutex);
        CloseHandle(xMutex);
        return 0;
    }

    if( (xMutex = OpenMutex(SYNCHRONIZE, FALSE, TITLECAPTION)) )
    {
        MessageBox (NULL, "Only one instance can be run at a time!" , TITLECAPTION, 0 + MB_ICONASTERISK + MB_SYSTEMMODAL);
        ReleaseMutex(xMutex);
        CloseHandle(xMutex);
        return 0;
    }
    xMutex = CreateMutex(0, 0, TITLECAPTION);

    MainInstance = hInstance;

    DialogBox(MainInstance, MAKEINTRESOURCE(100), 0, MainDialog);

    ReleaseMutex(xMutex);
    CloseHandle(xMutex);

    return 0;
}

/////////////////////////////////////////////////////////////////////

