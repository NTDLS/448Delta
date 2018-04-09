///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ApplicationKey   "Software\\NetWorkDLS\\448Delta"
#define DecDlgKey        "Software\\NetWorkDLS\\448Delta\\DecryptDialog"
#define EncDlgKey        "Software\\NetWorkDLS\\448Delta\\EncryptDialog"
#define PathKey          "Software\\NetWorkDLS\\448Delta\\Path"
#define PriorityKey      "Software\\NetWorkDLS\\448Delta\\Priority"
#define ShellFoldersKey  "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"
#define StatDlgKey       "Software\\NetWorkDLS\\448Delta\\StatusDialog"
#define UIDlgKey         "Software\\NetWorkDLS\\448Delta\\UIDialog"

#define BUYONLOINELINK   "http://www.networkdls.com/Pages/Buy/448Delta"

#define TITLECAPTION     "448Delta Encryption (BETA)"
#define APPCOPYRIGHT     "CopyRight © 2003 NetWorkDLS. All rights reserved."
#define MUTEXNAME        "448Delta_Encryption"
#define FILEVERSION      "1.0.3.4"
#define ENCVERSION       1034 // Version of 448Delta that performed the encryption

#define EncFileExt       ".448"
#define EncFolderExt     ".448Pkg"
#define EncExtractExt    ".exe"

#define FINISHEDWE       "Finished with errors!"
#define WRONGPWCD        "Wrong password, Can't decrypt!"

#define FILEHEADER       "448Delta|"
#define FILEHEADERLEN    9

#define CONTACT_INFO     "If you have any further problems. Please contact Tech@NetWorkDLS.com with this error message."

#define FLAGLENGHT       sizeof(DWORD)
#define Reserved         ((1 * 448) + ENCVERSION)
#define ENCFlag          ((2 * 448) + ENCVERSION)
#define PACFlag          ((3 * 448) + ENCVERSION)
#define ENCCompress      ((4 * 448) + ENCVERSION)
#define PACCompress      ((5 * 448) + ENCVERSION)

#define UIEncryptFolder  1
#define UIEncryptFile    2
#define UIEncryptDecrypt 3

#define MegaByte         1048576  // One Megabyte (1,048,576 Bytes)

#define MAXFILESIZE       250 * MegaByte
#define MAXFILES          10240
#define UNREG_MAXFILESIZE 10 * MegaByte
#define UNREG_MAXFILES    10
#define BufferSize        5120

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------(HWND & INSTANCE Definitions)
HINSTANCE MainInstance        = NULL;

HWND DecryptDialog_hWnd       = NULL;
HWND EncryptDialog_hWnd       = NULL;
HWND PasswordDlgParent_hWnd   = NULL;
HWND RegisterDialog_hWnd      = NULL;
HWND StatusBar_hWnd           = NULL;
HWND StatusDialog_hWnd        = NULL;
HWND StatusDlgParent_hWnd     = NULL;
HWND UnregisteredDialog_hWnd  = NULL;
HWND UserInterfaceDialog_hWnd = NULL;

//---------------------(Function Prototypes)
bool CheckRegistration(HWND);
bool InitRegistration(HWND);

BOOL SetPriority(void);
BOOL StartProcess(void);

DWORD WINAPI PasswordDialog_Thread(LPVOID);
DWORD WINAPI ProcessFromUserInterface_Thread(LPVOID);
DWORD WINAPI StartProcess_Thread(LPVOID);

void Alert(char *);
void Error(HWND, char *);

//---------------------(Global Variables)
bool IsRegistered = true;

char gsTitleCaption[50];
char RegHash[255];

DWORD MaxFiles    = 0;
DWORD MaxFileSize = 0;
DWORD PasswordDialog_Thread_ID;
DWORD ProcessFromUserInterface_Thread_ID;
DWORD StartProcess_Thread_ID;

HANDLE PasswordDialog_Thread_Handle;
HANDLE ProcessFromUserInterface_Thread_Handle;
HANDLE StartProcess_Thread_Handle;

//---------------------(Structure Prototypes)
struct _UserInterfaceDialog{
    char InputName[257];
    HWND DialogHandle;
    int UserIntent;
    bool SaveAllDataToDesktop;
}; _UserInterfaceDialog UID;

struct _448ProcessInfo{
    BOOL CancelFlag;
    BOOL CompleteFlag;
    BOOL CompressOption;
    BOOL CreateSelfOption;
    BOOL DeleteSourceOption;
    BOOL DestroyTempOption;
    BOOL ErrorFlag;
    BOOL ProcededWithOverwrite;
    BOOL ReadyToProcess;
    BOOL UsePriorityOption;
    BOOL UseUsersTargetPath;

    char AfterText[1024];
    char GlobalSource[256];
    char GlobalTarget[256];
    char Password[56 + 1];
    char StatusText[1024];
    char TempFile1[256];
    char TempFile2[256];
    char UsersTargetPath[256];

    double LastValue;
    double StatusValue;

    HWND OKCancelButton_hWnd;
    HWND StatusText_hWnd;

    int FileFlagResult;
    int PreCheckResult;

}; _448ProcessInfo PI;

//---------------------(Dialog Prototypes)
BOOL CALLBACK DecryptDialog(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK EncryptDialog(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK RegisterDialog(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK StatusDialog(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK UnregisteredDialog(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK UserInterfaceDialog(HWND, UINT, WPARAM, LPARAM);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

