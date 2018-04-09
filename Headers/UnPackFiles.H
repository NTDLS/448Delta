#ifndef NetWorkDLS_Package_UnPackFile_SourceFile
#define NetWorkDLS_Package_UnPackFile_SourceFile

#include "Package.H"

////////////////////////////////////////////////////////////////////////////////////////////

int UnPackFiles(char *SourceFile, char *DestinationDir)
{
    FILE *LocalTarget = NULL;
    FILE *LocalSource = NULL;

    char LocalBuffer[BufferSize + 1];
    char FolderName[256];
    char StatusText[256];
    char TempText[1024];

    char TargetFileName[1024];

    float xFileCount  = 0;
    float xLocalLoop  = 0;

    DWORD LocalLoop      = 0;
    DWORD SourceFileSize = 0;
    DWORD NameLenTemp    = 0;

    int CurrentBuffer = 0;
    int NeededMemory  = 0;

    DWORD FileSizeLeft = 0;
    DWORD Current_Size = 0;
    DWORD StatusCount  = 0;

    /* Open Source File */
    if((LocalSource = fopen(SourceFile,"rb")) == NULL)
    {
        if(LocalSource)fclose(LocalSource);

        sprintf(TempText, "UnPackFiles: Could not open \"%s\" for read.", SourceFile);
        Error(StatusDialog_hWnd, TempText);

        return 0;
    }

    fread((char *)&Package.FileCount, sizeof(Package.FileCount), 1, LocalSource);
    fread((char *)&Package.FolderCount, sizeof(Package.FolderCount), 1, LocalSource);

    /* Get FileNames ( *FileNames[] )*/
    LocalLoop = 0;
    while(LocalLoop < Package.FileCount)
    {
        fread((char *)&NameLenTemp, sizeof(NameLenTemp), 1, LocalSource);
        Package.FileNames[LocalLoop] = (char *) calloc(NameLenTemp + 1, 1);
        fread(Package.FileNames[LocalLoop], sizeof(char), NameLenTemp, LocalSource);
        LocalLoop++;
    }

    /* Get FolderNames ( *FolderNames[] )*/
    LocalLoop = 0;
    while(LocalLoop < Package.FolderCount)
    {
        fread((char *)&NameLenTemp, sizeof(NameLenTemp), 1, LocalSource);
        Package.FolderNames[LocalLoop] = (char *) calloc(NameLenTemp + 1, 1);
        fread(Package.FolderNames[LocalLoop], sizeof(char), NameLenTemp, LocalSource);
        LocalLoop++;
    }

    /* Create Folders */
    LocalLoop = 0;
    sprintf(StatusText, "Creating %d Folders", Package.FolderCount);
    Set_Text(PI.StatusText_hWnd, StatusText);

    CreateDirectory(DestinationDir,0);

    while(LocalLoop < Package.FolderCount) // Print Folder Names
    {
        sprintf(FolderName,"%s\\%s",DestinationDir,Package.FolderNames[LocalLoop]);
        CreateDirectory(FolderName,0);
        LocalLoop++;
    }

    sprintf(StatusText, "Unpacking: %d Files && %d Folders",Package.FileCount,Package.FolderCount);
    Set_Text(PI.StatusText_hWnd, StatusText);
    SendMessage(StatusBar_hWnd, PBM_SETPOS, 0, 0);

    xFileCount = (float)Package.FileCount;

    // Write Data To Files (Begin)
    LocalLoop = 0;
    while(LocalLoop < Package.FileCount)
    {
        sprintf(TargetFileName, "%s\\%s", DestinationDir, Package.FileNames[LocalLoop]);

        // Set file attributes to normal to avoid errors
        SetFileAttributes(TargetFileName, FILE_ATTRIBUTE_NORMAL);

        /* Open Target File */
        if((LocalTarget = fopen(TargetFileName, "wb")) == NULL)
        {
            if(LocalTarget)fclose(LocalTarget);
            if(LocalSource)fclose(LocalSource);

            sprintf(TempText, "UnPackFiles: Could not open \"%s\" for write.", TargetFileName);
            Error(StatusDialog_hWnd, TempText);
            return 0;
        }

        Current_Size   = 0;
        SourceFileSize = 0;
        FileSizeLeft   = 0;

        fread((char *)&SourceFileSize, sizeof(SourceFileSize), 1, LocalSource);

         // If File Can Be Completed In One Buffer
        if(SourceFileSize < BufferSize)
        {
            fread(LocalBuffer,sizeof(char),(int)SourceFileSize,LocalSource);
            fwrite(LocalBuffer,sizeof(char),(int)SourceFileSize,LocalTarget);
        }
        else{ // Else If File Bigger Than Buffer
            while(Current_Size < SourceFileSize)
            {
                FileSizeLeft = SourceFileSize - Current_Size;

                /* If File Can Be Completed In One Buffer */
                if(FileSizeLeft < BufferSize)
                {
                    fread(LocalBuffer,sizeof(char),FileSizeLeft,LocalSource);
                    fwrite(LocalBuffer,sizeof(char),FileSizeLeft,LocalTarget);
                    Current_Size += FileSizeLeft;
                    break;
                }

                CurrentBuffer = fread(LocalBuffer,sizeof(char),BufferSize,LocalSource);
                fwrite(LocalBuffer,sizeof(char),CurrentBuffer,LocalTarget);
                Current_Size += BufferSize;
            }

        }
        if(LocalTarget)fclose(LocalTarget);

        LocalLoop++;

        StatusCount++;
        if(StatusCount == 5)
        {
            xLocalLoop  = (float)LocalLoop;
            PI.StatusValue = ((xLocalLoop / xFileCount) * 100);
            StatusCount = 0;

            if(PI.CancelFlag == TRUE)
            {
                if(LocalTarget)fclose(LocalTarget);
                if(LocalSource)fclose(LocalSource);
                return 1;
            }

        }
    }
    // Write Data To Files (End)

    FreeMemory();

    if(LocalTarget)fclose(LocalTarget);
    if(LocalSource)fclose(LocalSource);

    Sleep(25);
    SendMessage(StatusBar_hWnd, PBM_SETPOS, 100, 0);

    return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////
#endif

