////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.H>
#include <WindowsX.H>
#include <CommCtrl.H>
#include <ShellAPI.H>
#include <ShlObj.H>
#include <Stdio.H>
#include <Stdlib.H>

#include "Headers/NASCCL.H"
#include "Headers/NSWFL.H"

#include "Resources/Resource.H"

#include "Headers/Definitions.H"
#include "Headers/DeleteObject.H"
#include "Headers/DestroyFile.H"
#include "Headers/PackFiles.H"
#include "Headers/UnPackFiles.H"
#include "Headers/Compress.H"
#include "Headers/Decompress.H"
#include "Headers/BlowFish.H"
#include "Headers/Encode.H"
#include "Headers/Decode.H"
#include "Headers/Functions.H"
#include "Headers/RegCode.H"
#include "Headers/StartProcess.H"

#include "Dialogs/Encrypt.H"
#include "Dialogs/Decrypt.H"
#include "Dialogs/Status.H"
#include "Dialogs/Register.H"
#include "Dialogs/UserInterface.H"
#include "Dialogs/Unregistered.H"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevious, LPSTR CommandLine, int CmdShow)
{
    HANDLE xMutex = CreateMutex(0, 0, MUTEXNAME);

    MainInstance = hInstance;

    if(CommandLine == NULL)
    {
        MessageBox(NULL, "Input file or folder name is NULL.", TITLECAPTION, 0 + MB_ICONASTERISK + MB_SYSTEMMODAL);
        ReleaseMutex(xMutex);
        CloseHandle(xMutex);
        return 0;
    }

    if(!CheckRegistration(GetActiveWindow()))
    {
        DialogBox(MainInstance, MAKEINTRESOURCE(IDD_REGISTERDIALOG), 0, RegisterDialog);
        return 0;
    }

    if(!IsRegistered)
    {
        int DlgResult = DialogBox(MainInstance, MAKEINTRESOURCE(IDD_UNREGISTERED), 0, UnregisteredDialog);
        if(DlgResult == 1)
        {
            DialogBox(MainInstance, MAKEINTRESOURCE(IDD_REGISTERDIALOG), 0, RegisterDialog);
            return 0;
        }
        else if(DlgResult == 2)
        {
            return 0;
        }
    }

    ResetUIDStruct();

    if( strlen(CommandLine) > 0)
    {
        ProcessInput(CommandLine);

        _fcloseall(); // Attempt to close all open files... (If any)
        DeleteFile(PI.TempFile1);
        DeleteFile(PI.TempFile2);

    }
    else DialogBox(MainInstance, MAKEINTRESOURCE(IDD_USERINTERFACE), 0, UserInterfaceDialog);

    ReleaseMutex(xMutex);
    CloseHandle(xMutex);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

