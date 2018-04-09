/////////////////////////////////////////////////////////////////////

BOOL CALLBACK StatusDialog(HWND xHandle, UINT xMessage, WPARAM wParam, LPARAM lParam)
{
    //--------------------------------------------------------------------------

    static HMENU SystemMenu       = NULL;

    DWORD DWORDRegTemp            = 0;

    //--------------------------------------------------------------------------

    if(xMessage == WM_INITDIALOG) // Received A Initilize Dialog Message
    {
        StatusDialog_hWnd = xHandle;

        SystemMenu = GetSystemMenu(StatusDialog_hWnd,FALSE);
        AppendMenu(SystemMenu,MF_SEPARATOR,0,0);
        AppendMenu(SystemMenu,MF_STRING,1000,"About");

        SendMessage(StatusDialog_hWnd, (UINT)WM_SETTEXT, (WPARAM)0, (LPARAM)gsTitleCaption);
        SendMessage(StatusDialog_hWnd, WM_SETICON, TRUE, (LPARAM)LoadIcon(MainInstance, MAKEINTRESOURCE(IDI_MAINCON)) );

        PI.OKCancelButton_hWnd = GetDlgItem(StatusDialog_hWnd,100);
        StatusBar_hWnd         = GetDlgItem(StatusDialog_hWnd,1000);
        PI.StatusText_hWnd     = GetDlgItem(StatusDialog_hWnd,1500);

        Set_Text(PI.StatusText_hWnd,"Please Wait...");

        SetTimer(StatusDialog_hWnd, 0, 10, NULL);

        SetForegroundWindow(StatusDialog_hWnd);
        CenterWindow(StatusDialog_hWnd);

        /* Get TopMost Value From Registry */
        if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE,StatDlgKey,"TopMost",DWORDRegTemp))
        {
            if(DWORDRegTemp == 1) // If Window Is Supposed To Be TopMost
            {
                CheckDlgButton(StatusDialog_hWnd, 5000, BST_CHECKED);
                SetWindowPos(StatusDialog_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            }
        }
        else{ // If Key Not Found Create Key And Value
                CreateRegistryKey(HKEY_LOCAL_MACHINE,StatDlgKey);
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, StatDlgKey, "TopMost", 1);
                CheckDlgButton(StatusDialog_hWnd, 5000, BST_CHECKED);
                SetWindowPos(StatusDialog_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        }

        StartProcess_Thread_Handle = CreateThread(NULL, 0, StartProcess_Thread, (LPVOID)0, 0, &StartProcess_Thread_ID);

        return 0;
    }

    //--------------------------------------------------------------------------

	if(xMessage == WM_PAINT)
    {
        HDC ThisHDC;
        PAINTSTRUCT ThisPS;

        ThisHDC = BeginPaint(xHandle, &ThisPS);
        // Any painting should be done here
        EndPaint(xHandle, &ThisPS);
        return 0 ;
    }

    //--------------------------------------------------------------------------

    if(xMessage == WM_COMMAND) // Received A Message
    {
        if(wParam == 5000) // Always On Top (Toggle)
        {
            if(IsDlgButtonChecked(StatusDialog_hWnd, 5000))
            {
                SetWindowPos(StatusDialog_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, StatDlgKey, "TopMost", 1);
            }
            else{
                SetWindowPos(StatusDialog_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, StatDlgKey, "TopMost", 0);
            }
        }

        if(wParam == 100) //- Okay (Dialog Command)
        {
            if(PI.CompleteFlag == TRUE) // Acting as a ok button
            {
                EndDialog(StatusDialog_hWnd,0);
                DestroyWindow(StatusDialog_hWnd);
                return 0;
            }

            if(PI.CompleteFlag == FALSE) // Acting as a cancel Button
            {
                SuspendThread(StartProcess_Thread_Handle);
                if(MessageBox(StatusDialog_hWnd, "Are you sure you want to cancel the current process?", gsTitleCaption, MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDYES)
                {
                    Set_Text(PI.StatusText_hWnd,"Please Wait... Canceling.");
                    EnableWindow(PI.OKCancelButton_hWnd, FALSE);
                    PI.CancelFlag = TRUE;
                }
                ResumeThread(StartProcess_Thread_Handle);
                return 0;
            }

            return 0;
        }

        return 0;
    }

    //--------------------------------------------------------------------------

    if(xMessage == WM_SYSCOMMAND) // Received A System Menu Message
    {
        if(LOWORD(wParam) == 1000) //- About (SysMenu Command)
        {
            _AboutDialogInfo ADI;

            ADI.DisplayIcon  = LoadIcon(MainInstance, MAKEINTRESOURCE(IDI_MAINCON));
            ADI.TitleCaption = TITLECAPTION;
            ADI.FileVersion  = FILEVERSION;
            ADI.BuildDate    = __DATE__;
            ADI.BuildTime    = __TIME__;
            ADI.CopyRight    = APPCOPYRIGHT;
            ADI.OwnerHandle  = xHandle;

            NetLogo(&ADI);
            return 0;
        }
        return 0;
    }

    //--------------------------------------------------------------------------

    if(xMessage == WM_CLOSE)  // Received Close Message
    {
        if(PI.CompleteFlag == TRUE) // Acting as a exit
        {
            EndDialog(StatusDialog_hWnd,0);
            DestroyWindow(StatusDialog_hWnd);
            return 0;
        }

        if(PI.CompleteFlag == FALSE) // Acting as a cancel
        {
            SuspendThread(StartProcess_Thread_Handle);
            if(MessageBox(StatusDialog_hWnd, "Are you sure you want to cancel the current process?", gsTitleCaption, MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDYES)
            {
                Set_Text(PI.StatusText_hWnd,"Please Wait... Canceling.");
                EnableWindow(PI.OKCancelButton_hWnd, FALSE);
                PI.CancelFlag = TRUE;
            }
            ResumeThread(StartProcess_Thread_Handle);
            return 0;
        }

        return 0;
    }

    //--------------------------------------------------------------------------

    if(xMessage == WM_DESTROY) // Received Destroy Message
    {
//        PostQuitMessage(0);
        return 0;
    }

    //--------------------------------------------------------------------------

    if(xMessage == WM_TIMER)
    {
        /* Update Status Bar */
        if(PI.LastValue != PI.StatusValue)
        {
            PI.LastValue = PI.StatusValue;
            SendMessage(StatusBar_hWnd, PBM_SETPOS, (int) PI.StatusValue, 0);
        }

        /* Update Status Text */
        if(PI.StatusText[0] != '\0')
        {
            Set_Text(PI.StatusText_hWnd, PI.StatusText);
            ClearMem(PI.StatusText, 1024);
        }

        return 0;
    }

    //--------------------------------------------------------------------------

    return 0;
}

//////////////////////////////////////////////////////////////////////

