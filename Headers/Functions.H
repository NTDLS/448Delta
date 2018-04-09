////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool InitRegistration(HWND xDialog)
{
    if(!Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE, ApplicationKey, "IsRegistered", (DWORD &)IsRegistered))
    {
        Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, ApplicationKey, "IsRegistered", 0);
        IsRegistered = false;
        return true;
    }

    if(IsRegistered)
    {
        DWORD SizeOfRegHash = sizeof(RegHash);

        char MachineName[255];
        char ThisRegHash[255];

        int RegHashLength = 16;

        if(!Get_BinaryRegistryValue(HKEY_LOCAL_MACHINE, ApplicationKey, "RegHash", RegHash, SizeOfRegHash))
        {
            Error(xDialog, "Registration failed due to failure to retrieve the registration hash. Please re-enter your registration code.");
            return false;
        }

        if(!Get_MachineNameEx(MachineName))
        {
            Error(xDialog,"Registration failed due to failure to retrieve uniquities.");
            return false;
        }

        UCase(MachineName);

        HashKey(MachineName, ThisRegHash, strlen(MachineName), RegHashLength);
        ThisRegHash[RegHashLength] = '\0';

        if(memcmp(ThisRegHash, RegHash, RegHashLength) == 0)
        {
            IsRegistered = true;
            return true;
        }
        else{
            IsRegistered = false;
            Error(xDialog, "Registration failed due to uniquity comparison failure. Please re-enter your registration code.");
            return false;
        }
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CheckRegistration(HWND xDialog)
{
    if(!InitRegistration(xDialog))
    {
        Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, ApplicationKey, "IsRegistered", 0);
        return false;
    }

    if(IsRegistered)
    {
        strcpy(gsTitleCaption, TITLECAPTION);
        MaxFileSize = MAXFILESIZE;
        MaxFiles    = MAXFILES;
    }
    else{
        sprintf(gsTitleCaption, "%s %s", TITLECAPTION, "(Unregistered)");
        MaxFileSize = UNREG_MAXFILESIZE;
        MaxFiles    = UNREG_MAXFILES;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool GetDesktopFolderPath(char *Desktop)
{
    DWORD SizeOfBuffer = 256;

    if(Get_StringRegistryValue(HKEY_CURRENT_USER, ShellFoldersKey, "Desktop", Desktop, SizeOfBuffer))
    {
        return true;
    }
    else return false;

    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool LaunchHelpFile(void)
{
    char HelpPath[256];
    DWORD SizeOfBuffer = 256;

    if(Get_StringRegistryValue(HKEY_LOCAL_MACHINE, PathKey, "HelpPath", HelpPath, SizeOfBuffer))
    {
        if(ShellExecute(NULL, "Open", HelpPath, NULL, NULL, SW_SHOWDEFAULT) == NULL)
        {
            return false;
        }
    }
    else return false;

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ResetUIDStruct(void)
{
    memset(UID.InputName, 0, 257);
    UID.DialogHandle = NULL;
    UID.UserIntent = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ResetPIStruct(void)
{
    PI.CompressOption      = FALSE;
    PI.CreateSelfOption    = FALSE;
    PI.DeleteSourceOption  = FALSE;
    PI.DestroyTempOption   = FALSE;
    PI.UsePriorityOption   = FALSE;

    PI.StatusText_hWnd      = NULL;
    PI.OKCancelButton_hWnd  = NULL;

    PI.CancelFlag             = FALSE;
    PI.ErrorFlag              = FALSE;
    PI.CompleteFlag           = FALSE;
    PI.ReadyToProcess         = FALSE;
    PI.ProcededWithOverwrite  = FALSE;
    PI.UseUsersTargetPath     = FALSE;

    PI.StatusValue = 0;
    PI.LastValue   = 0;

    memset(PI.AfterText, 0, 1024);
    memset(PI.StatusText, 0, 1024);
    memset(PI.GlobalTarget, 0, 256);
    memset(PI.GlobalSource, 0, 256);
    memset(PI.UsersTargetPath, 0, 256);
	memset(PI.Password, 0, 56 + 1);

    PI.FileFlagResult = 0;
    PI.PreCheckResult = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////

void Alert(char *xMessage)
{
    MessageBox(StatusDialog_hWnd, xMessage, gsTitleCaption, MB_SYSTEMMODAL);
}

///////////////////////////////////////////////////////////////////////////////////////////

void Error(HWND Handle, char *xMessage)
{
    char ErrorMessage[1024];
    sprintf(ErrorMessage, "%s\r\n\r\n%s", xMessage, CONTACT_INFO);
    MessageBox(Handle, ErrorMessage, gsTitleCaption, MB_SYSTEMMODAL);
}

///////////////////////////////////////////////////////////////////////////////////////////

int PreCheckFile(void)
{
    FILE *FileHandle  = NULL;

    char ErrorTemp[1024];

    char lsHeader[FILEHEADERLEN + 1];

    DWORD FileSize    = 0;
    DWORD LocalFlag   = 0;

    /*
        0: Error Occured
        1: Directory
        2: Encrypted File
        3: Packed, Encrypted Directory
        4: Compressed, Encrypted File
        5: Packed, Compressed, Encrypted Directory
        6: Non-448Delta File (Unencrypted)
    */

    /* Check For File Access */
    if(!FileAccess(PI.GlobalSource, FExist)) // Check Exist
    {
        sprintf(ErrorTemp,"PreCheckFile: \"%s\" does not exist.", PI.GlobalSource);
        Error(StatusDialog_hWnd, ErrorTemp);
        return 0;
    }

    if(IsDirectory(PI.GlobalSource)) // If Input Is A Directory
        return 1;

    if((FileSize = Get_FileSize(PI.GlobalSource)) == -1)
    {
        sprintf(ErrorTemp, "The file \"%s\" has been locked. 448Delta %s cannot open locked files.", PI.GlobalSource ,FILEVERSION);
        Alert(ErrorTemp);
        return 0;
    }

    if(FileSize == 0)
    {
        sprintf(ErrorTemp, "448Delta %s requires that single files contain at least one byte of data.", FILEVERSION);
        Alert(ErrorTemp);
        return 0;
    }

    if((FileHandle = fopen(PI.GlobalSource, "rb")) == NULL)
    {
        sprintf(ErrorTemp, "PreCheckFile: Could not open %s for read.", PI.GlobalSource);
        Error(StatusDialog_hWnd, ErrorTemp);
        return 0;
    }

    fread(lsHeader, FILEHEADERLEN, sizeof(char), FileHandle);
    lsHeader[FILEHEADERLEN] = '\0';

    if( strcmp(lsHeader, FILEHEADER) != 0)
    {
        if(FileHandle) fclose(FileHandle);

        if(FileSize > MaxFileSize )
        {
            sprintf(ErrorTemp, "448Delta %s requires that files be under %d MB", FILEVERSION, MaxFileSize/MegaByte);
            Alert(ErrorTemp);
            return 0;
        }

        return 6; // Not encrypted by 448Delta
    }

    fread(&LocalFlag, FLAGLENGHT, 1, FileHandle);

    if(FileHandle) fclose(FileHandle);

    if(LocalFlag == ENCFlag) // If Encrypted File
        return 2;
    else if(LocalFlag == PACFlag) // If Packed, Encrypted Directory
        return 3;
    else if(LocalFlag == ENCCompress) // If Compressed, Encrypted File
        return 4;
    else if(LocalFlag == PACCompress)// If Packed, Compressed, Encrypted Directory
        return 5;

    Error(StatusDialog_hWnd, "This file was encrypted by 448Delta but its encryption method could not be determined. This is most likely because this file was encrypted by a different version of 448Delta.");

    return 0; // If 448Delta file but type cannot be determined
}

///////////////////////////////////////////////////////////////////////////////////////////

BOOL ProcessInput(char *InputName)
{
    ResetPIStruct();

    strcpy(PI.GlobalSource, InputName);
    PI.PreCheckResult = PreCheckFile();

    if(PI.PreCheckResult == 0)
        return FALSE;

    PasswordDlgParent_hWnd = NULL;
    PasswordDialog_Thread_Handle = CreateThread(NULL,0,PasswordDialog_Thread,(LPVOID)0,0,&PasswordDialog_Thread_ID);

    while(1)
    {
        if(PI.CancelFlag)     return FALSE;
        if(PI.ReadyToProcess) break;
        Sleep(1);
    }

    StatusDlgParent_hWnd = NULL;
    DialogBox(MainInstance, MAKEINTRESOURCE(IDD_STATUSDIALOG), StatusDlgParent_hWnd, StatusDialog);

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////

bool MakeGlobalTargetName(HWND xDialog)
{
    if(PI.PreCheckResult == 1) // Directory
    {
        if(PI.UseUsersTargetPath)
        {
            char Drive[257];
            char Directory[257];
            char FileName[257];
            char Extension[257];
            _splitpath(PI.GlobalSource, Drive, Directory, FileName, Extension);

            if(PI.CreateSelfOption)
                sprintf(PI.GlobalTarget, "%s\\%s%s%s", PI.UsersTargetPath, FileName, Extension, EncExtractExt);
            else sprintf(PI.GlobalTarget, "%s\\%s%s%s", PI.UsersTargetPath, FileName, Extension, EncFolderExt);
        }
        else{
            if(PI.CreateSelfOption)
                sprintf(PI.GlobalTarget, "%s%s", PI.GlobalSource, EncExtractExt);
            else sprintf(PI.GlobalTarget, "%s%s", PI.GlobalSource, EncFolderExt);
        }

        if(FileAccess(PI.GlobalTarget, FExist))
        {
            if(MessageBox(xDialog, "Target file already exist. Overwrite?" , gsTitleCaption, MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDNO)
                return false;

            PI.ProcededWithOverwrite = TRUE;
        }

        return true;
    }

    if( PI.PreCheckResult <= 5 )
    {
        char Drive[257];
        char Directory[257];
        char FileName[257];

        _splitpath(PI.GlobalSource, Drive, Directory, FileName, NULL);

        if(PI.UseUsersTargetPath)
            sprintf(PI.GlobalTarget, "%s\\%s", PI.UsersTargetPath, FileName);
        else _makepath(PI.GlobalTarget, Drive, Directory, FileName, NULL);

        if(FileAccess(PI.GlobalTarget, FExist))
        {
            if(MessageBox(xDialog, "Target file already exist. Overwrite?" , gsTitleCaption, MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDNO)
                return false;

            PI.ProcededWithOverwrite = TRUE;
        }

        return true;
    }

    if(PI.PreCheckResult == 6)
    {
        if(PI.UseUsersTargetPath)
        {
            char Drive[257];
            char Directory[257];
            char FileName[257];
            char Extension[257];
            _splitpath(PI.GlobalSource, Drive, Directory, FileName, Extension);

            if(PI.CreateSelfOption)
                sprintf(PI.GlobalTarget, "%s\\%s%s%s", PI.UsersTargetPath, FileName, Extension, EncExtractExt);
            else sprintf(PI.GlobalTarget, "%s\\%s%s%s", PI.UsersTargetPath, FileName, Extension, EncFileExt);
        }
        else{
            if(PI.CreateSelfOption)
                sprintf(PI.GlobalTarget, "%s%s", PI.GlobalSource, EncExtractExt);
            else sprintf(PI.GlobalTarget, "%s%s", PI.GlobalSource, EncFileExt);
        }

        if(FileAccess(PI.GlobalTarget, FExist))
        {
            if(MessageBox(xDialog, "Target file already exist. Overwrite?" , gsTitleCaption, MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDNO)
                return false;

            PI.ProcededWithOverwrite = TRUE;
        }

        return true;
    }

    Error(StatusDialog_hWnd, "MakeGlobalTargetName: Past acceptable code position.");

    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////

DWORD WINAPI PasswordDialog_Thread(LPVOID xVoid)
{
    char ErrorTemp[1024];

    if(PI.PreCheckResult == 6)
    {
        DWORD FileSize = Get_FileSize(PI.GlobalSource);

        if(FileSize == -1)
        {
            sprintf(ErrorTemp,"PasswordDialog_Thread: File size error. The file \"%s\" has been locked. 448Delta %s cannot open locked files.", PI.GlobalSource ,FILEVERSION);
            Error(StatusDialog_hWnd, ErrorTemp);
            return 0;
        }
    }

    if(PI.PreCheckResult == 1 || PI.PreCheckResult == 6)
    {
        DialogBox(MainInstance, MAKEINTRESOURCE(IDD_ENCRYPTDIALOG), PasswordDlgParent_hWnd, EncryptDialog);
        return TRUE;
    }

    if( PI.PreCheckResult <= 5 )
    {
        DialogBox(MainInstance, MAKEINTRESOURCE(IDD_DECRYPTDIALOG), PasswordDlgParent_hWnd, DecryptDialog);
        return TRUE;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////

BOOL SetPriority(void)
{
    DWORD Priority = 0;
    BOOL HighOrRealTime = FALSE;
    HANDLE CurrentProcessHandle = GetCurrentProcess();
    HANDLE CurrentThreadHandle  = GetCurrentThread();

    DWORD ProcessSET     = 0;
    DWORD ThreadSET      = 0;
    BOOL ThreadBoostSET  = 0;
    BOOL ProcessBoostSET = 0;

    if( !PI.UsePriorityOption ) return TRUE;

    if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"Process",Priority))
    {
        ProcessSET = Priority;
        if(Priority > 2)HighOrRealTime = TRUE;
    }
    else Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"Process",2);

    Priority = 0;

    if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"Thread",Priority))
    {
        ThreadSET = Priority;
        if(Priority > 5)HighOrRealTime = TRUE;
    }
    else Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"Thread",4);

    Priority = 0;

    if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"ThreadBoost",Priority))
    {
        if(Priority == 0)ThreadBoostSET = TRUE; //Disable Boost
        if(Priority == 1)ThreadBoostSET = FALSE; //Enable Boost
    }
    else Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"ThreadBoost",1);

    Priority = 0;

    if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"ProcessBoost",Priority))
    {
        if(Priority == 0)ProcessBoostSET = TRUE; //Disable Boost
        if(Priority == 1)ProcessBoostSET = FALSE; //Enable Boost
    }
    else Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"ProcessBoost",1);

    if(HighOrRealTime == TRUE)
    {
        if(MessageBox(StatusDialog_hWnd,"448Delta is running in high, realtime or time critical mode. \r\nThis severly cripples windows performance. \r\nContinue anyway?",gsTitleCaption,4+256+MB_ICONQUESTION + MB_SYSTEMMODAL) != 6)
        {
            PI.CancelFlag = TRUE;
            PI.CompleteFlag = TRUE;
            return FALSE;
        }
    }

    SetApplicationPriority(CurrentProcessHandle,CurrentThreadHandle,(short)ProcessSET,(short)ThreadSET,ThreadBoostSET,ProcessBoostSET);

    return TRUE; // No Error... Return TRUE
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DWORD WINAPI ProcessFromUserInterface_Thread(LPVOID xVoid)
{
    ResetPIStruct();

    strcpy(PI.GlobalSource, UID.InputName);
    PI.PreCheckResult = PreCheckFile();

    if(PI.PreCheckResult == 0)
        return false;

/* FIXME: Need to implement messages for handling selections that were unexpected (7/25/2003 2:16:17 PM)

    if(PI.PreCheckResult == 1 && UID.UserIntent != UIEncryptFolder)
    {
        // Directory
        if(MessageBox(UID.DialogHandle, "The item that was selected is a folder, but you did not select encrypt a folder from the menu. Would like to encrypt the folder you selected anyway?" , gsTitleCaption, MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDNO)
            return false;
    }
    else if(PI.PreCheckResult == 6  && UID.UserIntent != UIEncryptFile)
    {
        // Non-448Delta File (Unencrypted)
        if(MessageBox(UID.DialogHandle, "The item that was selected is a folder, but you did not select encrypt a folder from the menu. Would like to encrypt the folder you selected anyway?" , gsTitleCaption, MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDNO)
            return false;
    }
    else if((PI.PreCheckResult >= 2 || PI.PreCheckResult <= 5) && UID.UserIntent != UIEncryptDecrypt)
    {
        // Non-448Delta File (Unencrypted)
        if(MessageBox(UID.DialogHandle, "The item that was selected is a folder, but you did not select encrypt a folder from the menu. Would like to encrypt the folder you selected anyway?" , gsTitleCaption, MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDNO)
            return false;
    }

*/
    PI.UseUsersTargetPath = TRUE;

    if(UID.SaveAllDataToDesktop)
    {
        if(!GetDesktopFolderPath(PI.UsersTargetPath))
        {
            Error(StatusDialog_hWnd, "Failed to get the path of the current users desktop.");
            return false;
        }
    }
    else if(!BrowseDirectory(UserInterfaceDialog_hWnd, "Select the folder that you would like the encrypted data to be saved in. Click cancel to save it on your desktop.", BIF_RETURNFSANCESTORS, PI.UsersTargetPath))
    {
        if(!GetDesktopFolderPath(PI.UsersTargetPath))
        {
            Error(StatusDialog_hWnd, "Failed to get the path of the current users desktop.");
            return false;
        }
    }

    PasswordDlgParent_hWnd = UID.DialogHandle;
    PasswordDialog_Thread_Handle = CreateThread(NULL, 0, PasswordDialog_Thread, (LPVOID)0, 0, &PasswordDialog_Thread_ID);

    while(1)
    {
        if(PI.CancelFlag)     return false;
        if(PI.ReadyToProcess) break;
        Sleep(1);
    }

    ShowWindow(UserInterfaceDialog_hWnd, FALSE);
    StatusDlgParent_hWnd = NULL;
    DialogBox(MainInstance, MAKEINTRESOURCE(IDD_STATUSDIALOG), StatusDlgParent_hWnd, StatusDialog);
    ShowWindow(UserInterfaceDialog_hWnd, TRUE);

    SetForegroundWindow(UserInterfaceDialog_hWnd);

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

