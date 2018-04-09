//////////////////////////////////////////////////////////////////////

void Error(char *Message)
{
    MessageBox(GetActiveWindow(), Message ,TITLECAPTION,MB_ICONHAND + MB_SYSTEMMODAL + MB_TASKMODAL);
}

/////////////////////////////////////////////////////////////////////

DWORD WINAPI Append_Thread(LPVOID xInput)
{
    FILE *LocalSource, *LocalTarget;

    int SentSize = 0;

    char Buffer[BufferSize+1];
    char TargetFileName[257];
    char Path[256];
    char Drive[256];
    char Directory[256];
    char File[256];
    char Extention[256];

    DWORD PrintCount = 0;

    unsigned long int StatusValue = 0;
    long double SourceFileSize = 0;
    long double Current_Size = 0;

    if(strlen(MainInput) < 1)
    {
        Set_Text(StatusText_hWnd, "No input data.");
        Set_Text(GetDlgItem(MainDialog_hWnd,100), "Ok");
        DoneFlag = TRUE;
        return 0;
    }

    _splitpath(MainInput, Drive, Directory, File, Extention);
    _makepath(Path, Drive, Directory, File, NULL);

    sprintf(TargetFileName, "%s.exe", Path);

    if(FileAccess(TargetFileName, FExist))
    {
        if(MessageBox(MainDialog_hWnd, "Target file already exist. Overwrite?", TITLECAPTION, MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDNO)
        {
            Set_Text(StatusText_hWnd, "Canceled");
            Set_Text(GetDlgItem(MainDialog_hWnd,100), "Ok");
            DoneFlag = TRUE;
            return 0;
        }
    }

    if(GetSelfExtractPath())
    {
        Set_Text(GetDlgItem(MainDialog_hWnd,100), "Ok");
        DoneFlag = TRUE;
        return 0;
    }

    // Open Executable Source For Read
    if((LocalSource = fopen(SelfExtractPath,"rb")) == NULL)
    {
        if(LocalSource)fclose(LocalSource);
        Error("Could not open file for read. Error 1.");
        return 0;
    }

    // Open Target (Empty Executable)  For Write
    if((LocalTarget = fopen(TargetFileName,"wb")) == NULL)
    {
        if(LocalTarget)fclose(LocalTarget);
        if(LocalTarget)fclose(LocalTarget);
        Error("Could Not Open File For Write. Error 2.");
        return 0;
    }

    // Write executable to empty file
    do
    {
        Buffer[0] = '\0';
        SentSize = fread(Buffer,sizeof(char),BufferSize,LocalSource);
        fwrite(Buffer,sizeof(char),SentSize,LocalTarget);
    }
    while(SentSize == BufferSize);

    if(LocalSource)fclose(LocalSource);

    // Open 448Delta File For Read
    if((LocalSource = fopen(MainInput,"rb")) == NULL)
    {
        if(LocalSource)fclose(LocalSource);
        if(LocalTarget)fclose(LocalTarget);
        Error("Could not open file for read. Error 3.");
        return 0;
    }

    SentSize = 0;

    fseek(LocalSource, 0L, SEEK_END);
    SourceFileSize = ftell(LocalSource);
    fseek(LocalSource, 0L, SEEK_SET);

    // Write packed and compressed file to new executable
    do
    {
        Buffer[0] = '\0';
        SentSize = fread(Buffer,sizeof(char),BufferSize,LocalSource);
        fwrite(Buffer,sizeof(char),SentSize,LocalTarget);

        if(CancelFlag == TRUE)
        {
            if(LocalSource)fclose(LocalSource);
            if(LocalTarget)fclose(LocalTarget);
            if(DeleteFile(TargetFileName) == FALSE) // If Delete Fails Retry Close Wait 500ms And Retry Delete
            {
                if(LocalSource)fclose(LocalSource);
                if(LocalTarget)fclose(LocalTarget);
                Sleep(500);
                DeleteFile(TargetFileName);
            }
            DoneFlag = TRUE;
            Set_Text(StatusText_hWnd, "Canceled");
            Set_Text(GetDlgItem(MainDialog_hWnd,100), "Ok");
            return 0;
        }

        Current_Size += SentSize;
        PrintCount += SentSize;

        if(PrintCount == 1024)
        {
            StatusValue = (DWORD)((Current_Size / SourceFileSize) * 100);
            SendMessage(StatusBar_hWnd, PBM_SETPOS,StatusValue,0);
            PrintCount = 0;
        }

    }
    while(SentSize == BufferSize);

    // Update StatusBar One More Time
    StatusValue = (DWORD)((Current_Size / SourceFileSize) * 100);
    SendMessage(StatusBar_hWnd, PBM_SETPOS,StatusValue,0);

    DoneFlag = TRUE;
    Set_Text(StatusText_hWnd, "Self-Extractor Created");
    Set_Text(GetDlgItem(MainDialog_hWnd,100), "Ok");
    EnableWindow(GetDlgItem(MainDialog_hWnd,101),TRUE);

    // Close Files
    if(LocalSource)fclose(LocalSource);
    if(LocalTarget)fclose(LocalTarget);
    _fcloseall();

    return 0;
}

/////////////////////////////////////////////////////////////////////

int GetSelfExtractPath()
{
    char GammaPath[1024];
    unsigned long SizeOfGammaPath = sizeof(GammaPath);
    /* Get 448Delta Executable File Name */
    if(Get_StringRegistryValue(HKEY_LOCAL_MACHINE,PathKey,"SelfExtract",GammaPath,SizeOfGammaPath))
    {
        if(FileAccess(GammaPath,0)) // If File Exist
        {
            SelfExtractPath = GammaPath;
            return 0;
        }
        else{ // If File Does Not Exist
            Set_Text(StatusText_hWnd, "GetSelfExtractPath: Execute template not found.");
            return 1;
        }
    }
    else{ // If Registry Error Or Key Not Found
        Set_Text(StatusText_hWnd, "GetSelfExtractPath: Registry key not found.");
        return 1;
    }
}

/////////////////////////////////////////////////////////////////////

