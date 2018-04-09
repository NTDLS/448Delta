///////////////////////////////////////////////////////////////////////////////////////////

#define FAccess  6   // Check for read and write permission
#define FRead    4   // Check for read permission
#define FWrite   2   // Check for write permission
#define FExecute 1   // Execute (Ignored)
#define FExist   0   // Check for existence of file

//------------------------------

#define OperationComplete "Operation Complete."
#define Canceled          "Canceled."
#define XOk               "Ok"

#define BufferSize        512
#define MaxFileSize       MegaByte * 1000
#define MegaByte          1048576
#define MaxFiles          10240
#define MaxPackFileSize   MegaByte * 100

#define ExecutableEnd     94720

#define TitleCaption      "448Delta Self-Extractor"
#define FileVersion       "1.0.1.3"
#define ENCVERSION        1034 // Version of 448Delta that performed the encryption

#define FILEHEADER        "448Delta|"
#define FILEHEADERLEN     9

#define FLAGLENGHT        sizeof(DWORD)
#define Reserved          ((1 * 448) + ENCVERSION)
#define ENCFlag           ((2 * 448) + ENCVERSION)
#define PACFlag           ((3 * 448) + ENCVERSION)
#define ENCCompress       ((4 * 448) + ENCVERSION)
#define PACCompress       ((5 * 448) + ENCVERSION)

//---------------------(Function Prototypes)

void Error(char *);
int DecryptFile(char *,char *);
int Unpack(char *,char *);
int DecompressDecrypt(char *,char *);
int DecompressUnpack(char *,char *);
int PreCheckFile(char *);
int CopySourceFile(char *, char *);
int UnPackFiles(char *,char *);
int GetDirectory(void);

BOOL FileAccess(char *, short);
BOOL IsDirectory(char *);

char *GetText (HWND);
char *GetTempDirectory(void);

void SetText (HWND, char *);
void CenterWindow (HWND);
void ClearBuffer(void);
void FreeMemory(void);

DWORD Get_FileSize(char *);

//---------------------(Global Handles)

HINSTANCE MainInstance;

HWND MainDialog_hWnd;
HWND StatusDialog_hWnd;

HWND StatusBar_hWnd;
HWND StatusText_hWnd;
HWND OkCancelButton_hWnd;

//---------------------(Global Flags (BOOL))

BOOL CancelFlag = FALSE;
BOOL DoneFlag   = FALSE;

//---------------------(Global Variables)

char SourceFileName[1024];
char *CodedKey;

DWORD StatusValue = 0;
DWORD LastValue   = 0;

//---------------------(Dialog Prototypes)

BOOL CALLBACK StatusDialog(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK MainDialog(HWND, UINT, WPARAM, LPARAM);

//---------------------(Full Thread Prototypes)

HANDLE UnPack_Thread_Handle;
DWORD UnPack_Thread_ID;
DWORD WINAPI UnPack_Thread(LPVOID data);

//---------------------(Full Global Structures)

struct _Package{
    char *FolderNames[MaxFiles+2];
    char *FileNames[MaxFiles+2];
    DWORD FileCount;
    DWORD FolderCount;

    char TargetDir[257];
}; struct _Package Package;

///////////////////////////////////////////////////////////////////////////////////////////

