#ifndef _DEFINED_NetWorkDLS_Standard_Windows_Function_Library_Header
#define _DEFINED_NetWorkDLS_Standard_Windows_Function_Library_Header
////////////////////////////////////////////////////////////////////////////////////////////

#define DllImport extern "C" __declspec( dllimport )

//----------( External Open / Save Dialog Definitions )------
#define OFD 1
#define SFD 2

//----------( External File Access Definitions )------
#define FAccess  6   // Check for read and write permission
#define FRead    4   // Check for read permission
#define FWrite   2   // Check for write permission
#define FExecute 1   // Execute (Ignored)
#define FExist   0   // Check for existence of file

//----------( External Key Generation Definitions )------
#define MaxKeySeed       1073741824
#define MinKeySeed       1

#define GKNUMBERS        0x1
#define GKUPPER_AZ       0x2
#define GKLOWER_AZ       0x4
#define GKBROADKEY       0x8
#define GKNONREPETITION  0x16

#define MaxKeyLength     10240
#define MinKeyLength     8

////////////////////////////////////////////////////////////////////////////////////////////

//----------( Global Structures )------
struct _QueuSystem{
    char **Data;
    int *DataSize;
    int UsedItems;
    int MaxItems;
};

struct _AboutDialogInfo{
    char *BuildDate;
    char *BuildTime;
    char *CopyRight;
    char *FileVersion;
    char *TitleCaption;
    HICON DisplayIcon;
    HWND OwnerHandle;
};

//----------( Key Generation Exports )------
DllImport int GenerateSeed(void);
DllImport bool GenerateKey(int, int, int, char *);

//----------( Memory Exports )------
DllImport void *Allocate(int);
DllImport int Free(void *);
DllImport int ClearMem(char *, int);
DllImport int StrCat(char *, const char *, int, int);
DllImport int MemCopy(char *, const char *, int);
DllImport int MemCopyLong(char *, long);
DllImport int MemCopyInt(char *, int);

//----------( Math Exports )------
DllImport int SquareRoot (int);

DllImport int LoByte (unsigned int);
DllImport int HiByte (unsigned int);

DllImport int LongLength(long);
DllImport int IntLength(int);

DllImport int ReverseLong(long);
DllImport int ReverseInt(int);

DllImport DWORD VersionToLong(int, int, int, int);
DllImport bool LongToVersion(DWORD, int *, int *, int *, int *);

//----------( Conversion Exports )------
DllImport void LongCharEx (DWORD, char *);
DllImport void FloatCharEx (float, char *);
DllImport void DoubleCharEx (double, char *);
DllImport void IntCharEx (int, char *);

DllImport char * LongChar (DWORD);
DllImport char * FloatChar (float);
DllImport char * DoubleChar (double);
DllImport char * IntChar (int);

DllImport DWORD CharLong (const char *);
DllImport float CharFloat (const char *);
DllImport double CharDouble (const char *);
DllImport int CharInt (const char *);

//----------( Windows Exports )------
DllImport void TextBoxWrite(HWND, char *);
DllImport void ErrorMessage(HWND, char *);
DllImport void AlertMessage(HWND, char *, char *);

DllImport int Get_TextLength(HWND);
DllImport DWORD Get_Long(HWND);
DllImport int Get_Int(HWND);
DllImport void Set_Long(HWND, DWORD);
DllImport void Set_Int(HWND, int);
DllImport void Set_Text (HWND, char *);
DllImport int Get_TextEx (HWND, char *, int);
DllImport char *Get_Text (HWND);

DllImport void CenterWindow (HWND );

DllImport LRESULT CALLBACK Set_Color(int, int, int);

DllImport int IsMouseOverHwnd(HWND);

DllImport int OSFileDialog (HWND, char *, char *, char *, char *, char *, DWORD, DWORD);
DllImport int BrowseDirectory(HWND, char *, int , char *);

DllImport HFONT MakeFont(char *, int);
DllImport void SetFont(HWND, HFONT);

//----------( String Exports )------
DllImport char * LCase (char *);
DllImport char * UCase (char *);

DllImport int ReverseString(char *, int);
DllImport int CopyString(char *, const char *, int);

DllImport char *StringScan (const char *, int, int);
DllImport int StringScanEx(const char *, int, int, char *);
DllImport int SearchString(const char *, char *);

DllImport int ReplaceStrings (const char *, const char *, const char *, char *);
DllImport int ReplaceCharacter(const char *, char *, const char, const char);

DllImport bool CompareNCString(const char *, const char *);
DllImport bool CompareCSString(const char *, const char *);

DllImport int AppendStrings(const char *, const char *, int, int, char *);
DllImport int GetFlagedValue(const char *, int, int, char *);
DllImport int CompareFlagedString(const char *, const char *);

DllImport int SimpleCipherString(char *, int);

//----------( System Exports )------
DllImport char * Get_Time (void);
DllImport char * Get_LongDate (void);
DllImport char * Get_ShortDate (void);

DllImport int Get_TimeEx (char *);
DllImport int Get_LongDateEx (char *);
DllImport int  Get_ShortDateEx (char *);

DllImport void Get_LocalDateTime(DWORD *, DWORD *);
DllImport void Get_SystemDateTime(DWORD *, DWORD *);

DllImport bool IsDirectory(const char *);
DllImport bool  FileAccess(const char *, short);
DllImport DWORD Get_FileSize(const char *);

DllImport DWORD Get_CurrentDirectoryEx (char *);
DllImport DWORD Get_TempDirectoryEx (char *);
DllImport DWORD Get_WindowsDirectoryEx (char *);
DllImport DWORD Get_SystemDirectoryEx (char *);

DllImport char * Get_CurrentDirectory (void);
DllImport char * Get_TempDirectory (void);
DllImport char * Get_WindowsDirectory (void);
DllImport char * Get_SystemDirectory (void);

DllImport bool IsWinNT(void);

DllImport BOOL ResetWindowsSys(UINT);

DllImport BOOL Get_MachineNameEx (char *);
DllImport BOOL Get_UserNameEx (char *);

DllImport char * Get_MachineName (void);
DllImport char * Get_UserName (void);

DllImport void Get_OsVersionEx (char *);
DllImport char * Get_OsVersion (void);

DllImport void SetApplicationPriority(HANDLE, HANDLE, short, short, BOOL, BOOL);

//----------( Registry Exports )------
DllImport bool DeleteRegistryValue(HKEY, LPCTSTR, LPCTSTR);

DllImport bool CreateRegistryKey(HKEY, LPCTSTR);
DllImport bool DeleteRegistryKey(HKEY, LPCTSTR);

DllImport bool Set_BinaryRegistryValue(HKEY, LPCTSTR, LPCTSTR, PVOID, DWORD);
DllImport bool Get_BinaryRegistryValue(HKEY, LPCTSTR, LPCTSTR, PVOID, DWORD&);

DllImport bool Set_DWORDRegistryValue(HKEY, LPCTSTR, LPCTSTR, DWORD);
DllImport bool Get_DWORDRegistryValue(HKEY, LPCTSTR, LPCTSTR, DWORD&);

DllImport bool Set_StringRegistryValue(HKEY, LPCTSTR, LPCTSTR, LPCTSTR);
DllImport bool Get_StringRegistryValue(HKEY, LPCTSTR, LPCTSTR, LPTSTR, DWORD&);

//----------( Queu System Exports )------
DllImport bool InitilizeQueu(struct _QueuSystem *, int);
DllImport bool FreeQueu(struct _QueuSystem *);
DllImport bool AddQueuItem(struct _QueuSystem *, const char *, int);
DllImport int ReadQueuItem(struct _QueuSystem *, int, char *);
DllImport int GetQueuItem(struct _QueuSystem *, int, char *);
DllImport bool DeleteQueuItem(struct _QueuSystem *, int);

//----------( DateTime Exports )------
DllImport void LongToDate(DWORD, int *, int *, int *);
DllImport void LongToTime(DWORD, int *, int *, int *);

DllImport DWORD DateToLong(int, int, int);
DllImport DWORD TimeToLong(int, int, int);

#ifdef _TIME_T_DEFINED
DllImport void TimetToFileTime(time_t, LPFILETIME);
DllImport bool TimetToSystemTime(time_t, LPSYSTEMTIME);
DllImport bool TimetToLocalTime(time_t, LPSYSTEMTIME);
#endif

//----------( List Box  Exports )------
DllImport int InsertListBoxItem(HWND, const char *, int);
DllImport int InsertListBoxItemEx(HWND, const char *, int, int);
DllImport int ClearListBox(HWND);
DllImport int DeleteListBoxItem(HWND, int);
DllImport int CountListBoxItems(HWND);
DllImport int SelectMultipleListBoxItem(HWND, int, BOOL);
DllImport int SelectListBoxItem(HWND, int);
DllImport int CurrentListBoxItem(HWND);
DllImport char *ListBoxItemText(HWND, int);
DllImport void ListBoxItemTextEx(HWND, int, char *);
DllImport int ListBoxItemLength(HWND, int);
DllImport int HorizontalListBoxExtent(HWND, int);

//----------( NetLogo Control Exports )------
DllImport bool NetLogo(struct _AboutDialogInfo *);
DllImport bool IsNetLogoActive(void);
DllImport void EndNetLogo(void);
DllImport HWND NetLogoHandle(void);

////////////////////////////////////////////////////////////////////////////////////////////
#endif

