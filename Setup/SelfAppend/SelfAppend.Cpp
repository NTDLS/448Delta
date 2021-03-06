////////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.H>
#include <WindowsX.H>
#include <ShellAPI.H>
#include <ShlOBJ.H>
#include <Stdio.H>
#include <StdLib.H>
#include <IO.H>

////////////////////////////////////////////////////////////////////////////////////////////

#include "Headers/NSWFL.H"		// Definitions For NSWFL.Dll

#include "Headers/Definitions.H"
#include "Headers/Functions.H"

#include "Dialogs/MainDialog.H"

////////////////////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevious, LPSTR Cmd , int CmdShow)
{
	HANDLE xMutex = NULL;

	if( (xMutex = OpenMutex(SYNCHRONIZE, FALSE, TITLECAPTION)) )
	{
		MessageBox (NULL, "Only one instance can be run at a time!", TITLECAPTION, 0 + MB_ICONASTERISK + MB_SYSTEMMODAL);
		ReleaseMutex(xMutex);
		CloseHandle(xMutex);
		return 0;
	}
	xMutex = CreateMutex(0, FALSE, TITLECAPTION);

	MainInstance = hInstance;
	MainInput	 = Cmd;

	DialogBox(MainInstance, MAKEINTRESOURCE(100), 0, MainDialog);

	ReleaseMutex(xMutex);
	CloseHandle(xMutex);

	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////

