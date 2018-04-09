#include "Package.H"

#ifndef NetWorkDLS_Package_PackFiles_SourceFile
#define NetWorkDLS_Package_PackFiles_SourceFile
////////////////////////////////////////////////////////////////////////////////////////////

int PackFiles(char *SourceDirectory, char *DestinationFile)
{
    char TempText[1024];
    char StatusText[257];

    char LocalBuffer[BufferSize + 1];

    FILE *LocalTarget      = NULL;
    FILE *LocalSource      = NULL;

    float xFileCount       = 0;
    float xLocalLoop       = 0;
    int StatusCount        = 0;

    DWORD LocalLoop        = 0;
    DWORD CurrentBuffer    = 0;
    DWORD SourceFileSize   = 0;

    char SourceFileName[1024];

    if(Get_FileNames(SourceDirectory)) // Get Names Of Folders And Dirs
    {
        PI.CancelFlag = TRUE;
        FreeMemory();
        return 1;
    }

    if(Package.FileCount == 0)
    {
        Alert("The folder that you have selected to encrypt does not contain any files.");
        FreeMemory();
        return 0;
    }

    /* Open Target File For Write */
    if((LocalTarget = fopen(DestinationFile,"wb")) == NULL)
    {
        sprintf(TempText, "PackFiles: Could not open \"%s\" for write.", DestinationFile);
        Error(StatusDialog_hWnd, TempText);

        if(LocalTarget)fclose(LocalTarget);
        FreeMemory();
        return 0;
    }

    fwrite((char *)&Package.FileCount, sizeof(Package.FileCount), 1, LocalTarget);
    fwrite((char *)&Package.FolderCount, sizeof(Package.FolderCount), 1, LocalTarget);

    Set_Text(PI.StatusText_hWnd, "Writing File && Folder Info");

    //Write File Name Leingths & Names (Begin)
    LocalLoop = 0;
    while(LocalLoop < Package.FileCount)
    {
        DWORD NameLen = strlen(Package.FileNames[LocalLoop]);
        fwrite((char *)&NameLen, sizeof(NameLen), 1, LocalTarget);
        fwrite(Package.FileNames[LocalLoop], sizeof(char), strlen(Package.FileNames[LocalLoop]), LocalTarget);
        LocalLoop++;
    }
    //Write File Name Leingths & Names (End)

    //Write Folder Name Leingths & Names (Begin)
    LocalLoop = 0;
    while(LocalLoop < Package.FolderCount)
    {
        DWORD NameLen = strlen(Package.FolderNames[LocalLoop]);
        fwrite((char *)&NameLen, sizeof(NameLen), 1, LocalTarget);
        fwrite(Package.FolderNames[LocalLoop], sizeof(char), strlen(Package.FolderNames[LocalLoop]), LocalTarget);
        LocalLoop++;
    }
    //Write Folder Name Leingths & Names (End)

    sprintf(StatusText,"Packing: %d files && %d folders\n",Package.FileCount,Package.FolderCount);
    Set_Text(PI.StatusText_hWnd, StatusText);

    //Write File Size & Data (Begin)
    LocalLoop = 0;
    while(LocalLoop < Package.FileCount)
    {
        sprintf(SourceFileName,"%s\\%s",SourceDirectory,Package.FileNames[LocalLoop]);

        SourceFileSize = Get_FileSize(SourceFileName);
        if(SourceFileSize == -1)
        {
            sprintf(TempText,"PackFiles: File size error. The file \"%s\" has been locked. 448Delta cannot open locked files.", SourceFileName );
            Alert(TempText);
            FreeMemory();
            return 0;
        }

        if(SourceFileSize > MaxFileSize)
        {
            if(IsRegistered)
                sprintf(TempText, "448Delta cannot pack files larger than %d MB each.", MaxFileSize/1024/1024);
            else sprintf(TempText, "The unregistered version of 448Delta cannot pack files larger than %d MB each.", MaxFileSize/1024/1024);

            Alert(TempText);
            FreeMemory();
            return 0;
        }

        fwrite((char *)&SourceFileSize, sizeof(SourceFileSize), 1, LocalTarget);

        /* Open Target File */
        if((LocalSource = fopen(SourceFileName,"rb")) == NULL)
        {
            sprintf(TempText, "PackFiles: Could not open \"%s\" for read.", SourceFileName);
            Error(StatusDialog_hWnd, TempText);

            if(LocalSource)fclose(LocalSource);
            if(LocalTarget)fclose(LocalTarget);
            FreeMemory();
            return 0;
        }

        /* Write File Data */
        do
        {
            CurrentBuffer = fread(LocalBuffer, sizeof(char), BufferSize, LocalSource);
            fwrite(LocalBuffer, sizeof(char), CurrentBuffer, LocalTarget);
        }
        while(CurrentBuffer == BufferSize);

        LocalLoop++;

        StatusCount++;
        if(StatusCount == 5)
        {
            xFileCount = (float)Package.FileCount;
            xLocalLoop = (float)LocalLoop;
            PI.StatusValue = ((xLocalLoop / xFileCount) * 100);
            StatusCount = 0;

            if(PI.CancelFlag == TRUE)
            {
                if(LocalSource)fclose(LocalSource);
                if(LocalTarget)fclose(LocalTarget);
                FreeMemory();
                return 1;
            }
        }

        if(LocalSource)fclose(LocalSource);
    }
    //Write File Size & Data (End)

    if(LocalSource)fclose(LocalSource);
    if(LocalTarget)fclose(LocalTarget);

    Sleep(25);
    SendMessage(StatusBar_hWnd, PBM_SETPOS, 100, 0);

    FreeMemory();

    return 1;
}

///////////////////////////////////////////////////////////////////////

int Get_FileNames(char *xInputDir)
{
    WIN32_FIND_DATA FindData;
    HANDLE FileHandle;

    int StatCount    = 0;
    int Found        = 0;
    int NeededMemory = 0;

    DWORD LocalLoop = 0;

    char DirTemp[257];
    char StatusText[257];
    char TempText[1024];

    sprintf(DirTemp,"%s\\%s",xInputDir,"*.*");
    FileHandle = FindFirstFile(DirTemp,&FindData);
    Found = (int) FileHandle;

    while (Found != 0)
    {
        if( strcmp(FindData.cFileName,".") && strcmp(FindData.cFileName,"..") ) // If (Not) NULL Or Root
        {
            /* If Found Data Is A Directory */
            if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                NeededMemory = strlen(FindData.cFileName) + 10;
                Package.FolderNames[Package.FolderCount] = (char *) calloc(NeededMemory,1);
                strcpy(Package.FolderNames[Package.FolderCount], FindData.cFileName);
                Package.FolderCount++;
            }
            else{   /* If Found Data Is A File */
                NeededMemory = strlen(FindData.cFileName) + 10;
                Package.FileNames[Package.FileCount] = (char *) calloc(NeededMemory,1);
                strcpy(Package.FileNames[Package.FileCount], FindData.cFileName);
                Package.FileCount++;
            }
        }

        if(PI.CancelFlag == TRUE)
            return 1;

        if(Package.FileCount > MaxFiles || Package.FolderCount > MaxFiles)
        {
            FindClose(FileHandle);

            if(IsRegistered)
                sprintf(TempText, "448Delta can pack a max of %d files and folders",MaxFiles);
            else sprintf(TempText, "The unregistered version of 448Delta can pack a max of %d files and folders",MaxFiles);

            Alert(TempText);
            return 1;
        }

        Found = FindNextFile(FileHandle, &FindData);

        if(StatCount == 47)
        {
            sprintf(StatusText,"Counting: %d files && %d folders.",Package.FileCount,Package.FolderCount);
            Set_Text(PI.StatusText_hWnd, StatusText);
            StatCount = 0;
        }
        StatCount++;
    }

    FindClose(FileHandle);

    if(Package.FolderCount != 0) // If AtLeast One Folder Was Found
    {
        LocalLoop = 0;
        while(LocalLoop < Package.FolderCount)
        {
            if(Get_SubFileNames(xInputDir,Package.FolderNames[LocalLoop]))
            {
                return 1;
            }
            LocalLoop++;
        }
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////

int Get_SubFileNames(char *xInputDir, char *xSubDir)
{
    WIN32_FIND_DATA FindData;
    HANDLE FileHandle;

    int Found        = 0;
    int StatCount    = 0;
    int NeededMemory = 0;

    char DirTemp[257];
    char StatusText[257];
    char TempText[1024];

    BOOL FoundDir = FALSE;

    sprintf(DirTemp, "%s\\%s\\%s", xInputDir, xSubDir,"*.*");
    FileHandle = FindFirstFile(DirTemp, &FindData);
    Found = (int) FileHandle;

    while (Found != 0)
    {
        /* If Found Data Is A Directory */
        if( strcmp(FindData.cFileName,".") && strcmp(FindData.cFileName,"..") ) // If (Not) NULL Or Root
        {
            if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                NeededMemory = strlen(xSubDir) + strlen(FindData.cFileName) + 10;
                Package.FolderNames[Package.FolderCount] = (char *) calloc(NeededMemory,1);
                sprintf(Package.FolderNames[Package.FolderCount], "%s\\%s", xSubDir, FindData.cFileName);
                FoundDir = TRUE;
                Package.FolderCount++;
            }
            else{ /* If Found Data Is A File */
                 NeededMemory = strlen(xSubDir) + strlen(FindData.cFileName) + 10;
                Package.FileNames[Package.FileCount] = (char *) calloc(NeededMemory,1);
                sprintf(Package.FileNames[Package.FileCount], "%s\\%s", xSubDir, FindData.cFileName);
                Package.FileCount++;
            }
        }

        if(PI.CancelFlag == TRUE)
            return 1;

        if(Package.FileCount > MaxFiles || Package.FolderCount > MaxFiles)
        {
            FindClose(FileHandle);

            if(IsRegistered)
                sprintf(TempText, "448Delta can pack a max of %d files and folders",MaxFiles);
            else sprintf(TempText, "The unregistered version of 448Delta can pack a max of %d files and folders",MaxFiles);

            Alert(TempText);
            return 1;
        }

        Found = FindNextFile(FileHandle, &FindData);

        if(StatCount == 48)
        {
            sprintf(StatusText,"Counting: %d Files && %d Folders.",Package.FileCount,Package.FolderCount);
            Set_Text(PI.StatusText_hWnd, StatusText);
            StatCount = 0;
        }
        StatCount++;
    }

    FindClose(FileHandle);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////
#endif

