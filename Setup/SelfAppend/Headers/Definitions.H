/*

 Package.H

*/

//--------------------------------------------------------------------

#define BufferSize 512    // Size Of Buffer Used For Copy Processes
#define MegaByte 1048576  // One Megabyte (1,048,576 Bytes)

#define MaxFiles 50000
#define MaxPackFileSize MegaByte * 100

#define TITLECAPTION "448Delta Self-Append"
#define FILEVERSION  "1.0.0.2"
#define APPCOPYRIGHT "CopyRight © 2003 NetWorkDLS. All rights reserved."

#define PriorityKey "Software\\NetWorkDLS\\448Delta\\Priority"
#define EncDlgKey   "Software\\NetWorkDLS\\448Delta\\EncryptDialog"
#define DecDlgKey   "Software\\NetWorkDLS\\448Delta\\DecryptDialog"
#define StatDlgKey  "Software\\NetWorkDLS\\448Delta\\StatusDialog"
#define PathKey     "Software\\NetWorkDLS\\448Delta\\Path"

//--------------------------------------------------------------------
        /* Various Function Prototypes */ // Begin

void Error(char *);
void ClearBuffer(void);
int GetSelfExtractPath();

//---------------------(Full Thread Prototypes)
HANDLE Append_Thread_Handle;
DWORD Append_Thread_ID;
DWORD WINAPI Append_Thread(LPVOID data);

        /* Various Function Prototypes */ // End
//--------------------------------------------------------------------
        /* Various Global Variables */ // Begin

char *SelfExtractPath = NULL;
char *MainInput = NULL;

//---------------------(Global Handles)
HINSTANCE MainInstance;
HWND MainDialog_hWnd;
HWND StatusBar_hWnd;
HWND StatusText_hWnd;

//---------------------(Dialog Prototypes)
BOOL CALLBACK MainDialog(HWND, UINT, WPARAM, LPARAM);

//---------------------(BOOL's Used As Flags)
BOOL CancelFlag = FALSE;
BOOL DoneFlag = FALSE;

        /* Various Global Variables */ // End
//--------------------------------------------------------------------

