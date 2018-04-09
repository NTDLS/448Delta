///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HANDLE Registration_Thread_Handle = NULL;
DWORD Registration_Thread_ID = 0;

HWND RegText_hWnd = NULL;
HWND RegProgress_hWnd = NULL;

char gsRegKey[255];

bool IsRegisterThreadRunning = false;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DWORD WINAPI Registration_Thread(LPVOID xData)
{
    IsRegisterThreadRunning = true;

    Set_Text(RegText_hWnd, "Checking registration key.");
    if(ValidateRegKey(CRYPTKEY, 64, gsRegKey, 20, 16))
    {
        Set_Text(RegText_hWnd, "Registration key is valid.");

        char MachineName[255];
        char ThisRegHash[255];

        int RegHashLength = 16;

        DWORD SizeOfRegHash = RegHashLength;

        if(!Get_MachineNameEx(MachineName))
        {
            Error(RegisterDialog_hWnd,"Registration failed due to failure to retrieve uniquities.");
            EnableWindow(GetDlgItem(RegisterDialog_hWnd, IDC_CMDOK), TRUE);
            IsRegisterThreadRunning = false;
            return 0;
        }

        UCase(MachineName);

        HashKey(MachineName, ThisRegHash, strlen(MachineName), RegHashLength);
        ThisRegHash[RegHashLength] = '\0';

        if(!Set_BinaryRegistryValue(HKEY_LOCAL_MACHINE, ApplicationKey, "RegHash", ThisRegHash, SizeOfRegHash))
        {
            Error(RegisterDialog_hWnd, "Registration failed due to failure to set the registration hash. Please re-enter your registration code.");
            EnableWindow(GetDlgItem(RegisterDialog_hWnd, IDC_CMDOK), TRUE);
            IsRegisterThreadRunning = false;
            return 0;
        }

        if(!Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, ApplicationKey, "IsRegistered", 1))
        {
            Error(RegisterDialog_hWnd, "Registration failed due to failure to set the registration flag. Please re-enter your registration code.");
            EnableWindow(GetDlgItem(RegisterDialog_hWnd, IDC_CMDOK), TRUE);
            IsRegisterThreadRunning = false;
            return 0;
        }

        if(CheckRegistration(RegisterDialog_hWnd))
        {
            MessageBox(RegisterDialog_hWnd, "Thank you for registering 448Delta.", gsTitleCaption, 0);
            EndDialog(RegisterDialog_hWnd, 100);
            return 0;
        }

        Error(RegisterDialog_hWnd, "An error occured while processing your VALID registration code.");

        EnableWindow(GetDlgItem(RegisterDialog_hWnd, IDC_CMDOK), TRUE);
        IsRegisterThreadRunning = false;
        return 0;
    }

    Error(RegisterDialog_hWnd, "The registration code you entered is invalid.");

    Set_Text(RegText_hWnd, "448Delta is NOT registered.");


    CloseHandle(Registration_Thread_Handle);
    Registration_Thread_ID = 0;

    EnableWindow(GetDlgItem(RegisterDialog_hWnd, IDC_CMDOK), TRUE);
    IsRegisterThreadRunning = false;
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK RegisterDialog(HWND xHandle, UINT xMessage, WPARAM wParam, LPARAM lParam)
{
    //--------------------------------------------------------------------------

    static HWND RegSeg1_hWnd     = NULL;
    static HWND RegSeg2_hWnd     = NULL;
    static HWND RegSeg3_hWnd     = NULL;
    static HWND RegSeg4_hWnd     = NULL;

    static HMENU SystemMenu = NULL;

    //--------------------------------------------------------------------------

    if(xMessage == WM_INITDIALOG) // Received A Initilize Dialog Message
    {
        RegisterDialog_hWnd = xHandle;

        SystemMenu = GetSystemMenu(xHandle, FALSE);
        AppendMenu(SystemMenu,MF_SEPARATOR, 0, 0);
        AppendMenu(SystemMenu,MF_STRING, 1000, "About");

        SendMessage(xHandle, (UINT)WM_SETTEXT, (WPARAM)0, (LPARAM)gsTitleCaption);
        SendMessage(xHandle, WM_SETICON, TRUE, (LPARAM)LoadIcon(MainInstance, MAKEINTRESOURCE(IDI_MANICON)) );

        RegSeg1_hWnd     = GetDlgItem(xHandle, IDC_REGSEG1);
        RegSeg2_hWnd     = GetDlgItem(xHandle, IDC_REGSEG2);
        RegSeg3_hWnd     = GetDlgItem(xHandle, IDC_REGSEG3);
        RegSeg4_hWnd     = GetDlgItem(xHandle, IDC_REGSEG4);

        RegText_hWnd     = GetDlgItem(xHandle, IDC_REGTEXT);
        RegProgress_hWnd = GetDlgItem(xHandle, IDC_REGPROGRESS);

        SendMessage(RegSeg1_hWnd, EM_LIMITTEXT, 5, 0);
        SendMessage(RegSeg2_hWnd, EM_LIMITTEXT, 5, 0);
        SendMessage(RegSeg3_hWnd, EM_LIMITTEXT, 5, 0);
        SendMessage(RegSeg4_hWnd, EM_LIMITTEXT, 5, 0);

        if(IsRegistered)
        {
            EnableWindow(RegSeg1_hWnd, FALSE);
            EnableWindow(RegSeg2_hWnd, FALSE);
            EnableWindow(RegSeg3_hWnd, FALSE);
            EnableWindow(RegSeg4_hWnd, FALSE);

            EnableWindow(GetDlgItem(RegisterDialog_hWnd, IDC_CMDOK), FALSE);
            EnableWindow(GetDlgItem(RegisterDialog_hWnd, IDC_CMDOK), FALSE);

            Set_Text(RegText_hWnd, "448Delta is already registered.");
        }
        else{

            Set_Text(RegText_hWnd, "448Delta is NOT registered.");
        }

        CenterWindow(xHandle);

        return TRUE;
    }

    //--------------------------------------------------------------------------

	if(xMessage == WM_PAINT)
    {
        HDC ThisHDC;
        PAINTSTRUCT ThisPS;

        ThisHDC = BeginPaint(xHandle, &ThisPS);
        // Any painting should be done here
        EndPaint(xHandle, &ThisPS);
        return TRUE;
    }

    //--------------------------------------------------------------------------

    if(xMessage == WM_COMMAND) // Received A Message
    {
        if(wParam == IDC_CMDOK)
        {
            char RegSeg1[25];
            char RegSeg2[25];
            char RegSeg3[25];
            char RegSeg4[25];

            bool InvalidKey = false;

            if(Get_TextEx(RegSeg1_hWnd, RegSeg1, 5) != 5)
                InvalidKey = true;
            if(Get_TextEx(RegSeg2_hWnd, RegSeg2, 5) != 5)
                InvalidKey = true;
            if(Get_TextEx(RegSeg3_hWnd, RegSeg3, 5) != 5)
                InvalidKey = true;
            if(Get_TextEx(RegSeg4_hWnd, RegSeg4, 5) != 5)
                InvalidKey = true;

            if(InvalidKey)
            {
                MessageBox(xHandle, "You must specifiy a valid registration code.", gsTitleCaption, 0);
                return TRUE;
            }

            sprintf(gsRegKey, "%s%s%s%s", RegSeg1, RegSeg2, RegSeg3, RegSeg4);

            EnableWindow(GetDlgItem(RegisterDialog_hWnd, IDC_CMDOK), FALSE);

            Registration_Thread_Handle = CreateThread(NULL, 0, Registration_Thread, (LPVOID)0, 0, &Registration_Thread_ID);

            return TRUE;
        }

        if(wParam == IDC_CMDBUY)
        {
            ShellExecute(NULL, "open", BUYONLOINELINK, NULL, NULL, SW_SHOWDEFAULT);
            return TRUE;
        }

        if(wParam == IDC_CMDCANCEL)
        {
            if(IsRegisterThreadRunning)
            {
                SuspendThread(Registration_Thread_Handle);
                if(MessageBox(xHandle, "Are you sure you want to cancel the registration process?", gsTitleCaption, MB_YESNO) == IDYES)
                {
                    TerminateThread(Registration_Thread_Handle, 0);
                    EndDialog(xHandle,0);
                    return TRUE;
                }
                ResumeThread(Registration_Thread_Handle);
                return TRUE;
            }

            EndDialog(xHandle,0);
            return TRUE;
        }

        return FALSE;
    }

    //--------------------------------------------------------------------------

    if(xMessage == WM_SYSCOMMAND) // Received A System Menu Message
    {
        if(LOWORD(wParam) == 1000)
        {
            _AboutDialogInfo ADI;

            ADI.DisplayIcon  = LoadIcon(MainInstance, MAKEINTRESOURCE(IDI_MANICON));
            ADI.TitleCaption = TITLECAPTION;
            ADI.FileVersion  = FILEVERSION;
            ADI.BuildDate    = __DATE__;
            ADI.BuildTime    = __TIME__;
            ADI.CopyRight    = APPCOPYRIGHT;
            ADI.OwnerHandle  = xHandle;

            NetLogo(&ADI);
            return TRUE;
        }
        return FALSE;
    }

    //--------------------------------------------------------------------------

    if(xMessage == WM_CLOSE)  // Received Close Message
    {
        PI.CancelFlag = TRUE;
        EndDialog(xHandle, 0);
        return TRUE;
    }

    //--------------------------------------------------------------------------

    if(xMessage == WM_DESTROY) // Received Destroy Message
    {
        //PostQuitMessage(0);
        return FALSE;
    }

    //--------------------------------------------------------------------------

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

