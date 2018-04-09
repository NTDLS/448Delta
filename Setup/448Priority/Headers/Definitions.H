
#define TITLECAPTION  "448Delta Priority"
#define GAMMATITLE    "448Delta_Encryption"
#define FILEVERSION   "1.0.0.2"
#define APPCOPYRIGHT  "CopyRight © 2003 NetWorkDLS. All rights reserved."

#define PriorityKey "Software\\NetWorkDLS\\448Delta\\Priority"
#define EncDlgKey   "Software\\NetWorkDLS\\448Delta\\EncryptDialog"
#define DecDlgKey   "Software\\NetWorkDLS\\448Delta\\DecryptDialog"
#define StatDlgKey  "Software\\NetWorkDLS\\448Delta\\StatusDialog"
#define PathKey     "Software\\NetWorkDLS\\448Delta\\Path"

//---------------------(HWND & INSTANCE Definitions)
HINSTANCE MainInstance;         // WinMain (Application) Instance
HWND MainDialog_hWnd;      // MainDialog Instance

//---------------------(Function Prototypes)
int GetValues(void);
int SetValues(void);

void RegError_Get(void);
void RegError_Set(void);

//---------------------(Dialog Prototypes)
BOOL CALLBACK MainDialog(HWND, UINT, WPARAM, LPARAM);

//---------------------(Various Global Variables)

int LastValue_1 = 0;
int LastValue_2 = 0;

DWORD PriorityClass = 0;
DWORD ThreadPriority = 0;
DWORD ThreadBoost = 0;
DWORD ProcessBoost = 0;

//----------------------------------------------------------------

