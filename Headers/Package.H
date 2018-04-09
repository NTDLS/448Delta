#ifndef NetWorkDLS_Package_Header
#define NetWorkDLS_Package_Header
///////////////////////////////////////////////////////////////////////////////////////////

//----------(Package Functions)
void FreeMemory(void);

//----------(PackFiles Functions)
int PackFiles(char *, char *);
int Get_FileNames(char *);
int Get_SubFileNames(char *, char *);

//----------(UnPackFiles Functions)
int UnPackFiles(char *, char *);

////////////////////////////////////////////////////////////////////////////////////////////

struct _Package{
    char *FolderNames[MAXFILES+2];
    char *FileNames[MAXFILES+2];
    DWORD FileCount;
    DWORD FolderCount;
}; struct _Package Package;

////////////////////////////////////////////////////////////////////////////////////////////

void FreeMemory(void)
{
    while(Package.FolderCount != -1)
    {
        free(Package.FolderNames[Package.FolderCount]);
        Package.FolderNames[Package.FolderCount] = NULL;
        Package.FolderCount--;
    }

    while(Package.FileCount != -1)
    {
        free(Package.FileNames[Package.FileCount]);
        Package.FileNames[Package.FileCount] = NULL;
        Package.FileCount--;
    }

    Package.FileCount   = 0;
    Package.FolderCount = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////
#endif

