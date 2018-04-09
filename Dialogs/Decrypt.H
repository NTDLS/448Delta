/////////////////////////////////////////////////////////////////////

BOOL CALLBACK DecryptDialog(HWND xHandle, UINT xMessage, WPARAM wParam, LPARAM lParam)
{
    //--------------------------------------------------------------------------

    static HWND PasswordEdit_hWnd    = NULL;
    static HWND PasswordBitsBar_hWnd = NULL;
    static HWND PasswordBits_hWnd    = NULL;

    static HMENU SystemMenu          = NULL;

    DWORD DWORDRegTemp               = 0;

    //--------------------------------------------------------------------------

    if(xMessage == WM_INITDIALOG) // Received A Initilize Dialog Message
    {
        DecryptDialog_hWnd = xHandle;

        SystemMenu = GetSystemMenu(DecryptDialog_hWnd,FALSE);
        AppendMenu(SystemMenu,MF_SEPARATOR,0,0);
        AppendMenu(SystemMenu,MF_STRING,1000,"About");

        SendMessage(DecryptDialog_hWnd, (UINT)WM_SETTEXT, (WPARAM)0, (LPARAM)gsTitleCaption);
        SendMessage(DecryptDialog_hWnd, WM_SETICON, TRUE, (LPARAM)LoadIcon(MainInstance, MAKEINTRESOURCE(IDI_MAINCON)) );

        PasswordEdit_hWnd    = GetDlgItem(DecryptDialog_hWnd,1000);
        PasswordBitsBar_hWnd = GetDlgItem(DecryptDialog_hWnd,2000);
        PasswordBits_hWnd    = GetDlgItem(DecryptDialog_hWnd,3000);

//      Set_Text(PasswordEdit_hWnd, "123456789");

        SendMessage(PasswordEdit_hWnd, EM_LIMITTEXT, 56, 0 );

        //----------------------------------------

        /* Delete Source File(s) */
        if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE,DecDlgKey,"DeleteSourceOption",DWORDRegTemp))
        {
            if(DWORDRegTemp == 1) // Delete Source File(s)
            {
                CheckDlgButton(DecryptDialog_hWnd, 5000, BST_CHECKED);
                PI.DeleteSourceOption = TRUE;
            }
        }
        else{ // If Key Not Found Create Key And Value
                CreateRegistryKey(HKEY_LOCAL_MACHINE,DecDlgKey);
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, DecDlgKey, "DeleteSourceOption", 0);
                CheckDlgButton(DecryptDialog_hWnd, 5000, BST_CHECKED);
                PI.DeleteSourceOption = FALSE;
        }

        //----------------------------------------

        /* Use Priority Settings */
        if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE,DecDlgKey,"UsePriorityOption",DWORDRegTemp))
        {
            if(DWORDRegTemp == 1) // Use Priority Settings
            {
                CheckDlgButton(DecryptDialog_hWnd, 5002, BST_CHECKED);
                PI.UsePriorityOption = TRUE;
            }
        }
        else{ // If Key Not Found Create Key And Value
                CreateRegistryKey(HKEY_LOCAL_MACHINE,DecDlgKey);
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, DecDlgKey, "UsePriorityOption", 1);
                CheckDlgButton(DecryptDialog_hWnd, 5002, BST_CHECKED);
                PI.UsePriorityOption = FALSE;
        }

        //----------------------------------------

        /* Destroy Temp Files */
        if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE,DecDlgKey,"DestroyTempOption",DWORDRegTemp))
        {
            if(DWORDRegTemp == 1) // Destroy Temp Files
            {
                CheckDlgButton(DecryptDialog_hWnd, 5003, BST_CHECKED);
                PI.DestroyTempOption = TRUE;
            }
        }
        else{ // If Key Not Found Create Key And Value
                CreateRegistryKey(HKEY_LOCAL_MACHINE,DecDlgKey);
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, DecDlgKey, "DestroyTempOption", 1);
                CheckDlgButton(DecryptDialog_hWnd, 5003, BST_CHECKED);
                PI.DestroyTempOption = FALSE;
        }

        //----------------------------------------

        /* Get TopMost Value From Registry */
        if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE,DecDlgKey,"TopMost",DWORDRegTemp))
        {
            if(DWORDRegTemp == 1) // If Window Is Supposed To Be TopMost
            {
                CheckDlgButton(DecryptDialog_hWnd, 5004, BST_CHECKED);
                SetWindowPos(DecryptDialog_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            }
        }
        else{ // If Key Not Found Create Key And Value
                CreateRegistryKey(HKEY_LOCAL_MACHINE,DecDlgKey);
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, DecDlgKey, "TopMost", 1);
                CheckDlgButton(DecryptDialog_hWnd, 5004, BST_CHECKED);
                SetWindowPos(DecryptDialog_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        }

        //----------------------------------------

        CenterWindow(DecryptDialog_hWnd);

        return 0;
    }

    //--------------------------------------------------------------------------

    if(xMessage == 307) // Key Down
    {
        float PasswordLen    = 0;
        float EffeciencyBits = 0;

        char LocalTemp[1024];

        PasswordLen = (float) SendMessage(PasswordEdit_hWnd, WM_GETTEXTLENGTH, 0, 0);

        EffeciencyBits = (PasswordLen / 56) * 100;
        SendMessage(PasswordBitsBar_hWnd, PBM_SETPOS, (int) EffeciencyBits, 0);
        sprintf(LocalTemp," (%.0f Bits)", PasswordLen * 8);
        Set_Text(PasswordBits_hWnd,LocalTemp);
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
       //----------------------------------------

        if(wParam == 5000) // Delete Source File(s)
        {
            if(IsDlgButtonChecked(DecryptDialog_hWnd, 5000))
            {
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, DecDlgKey, "DeleteSourceOption", 1);
                PI.DeleteSourceOption = TRUE;
            }
            else{
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, DecDlgKey, "DeleteSourceOption", 0);
                PI.DeleteSourceOption = FALSE;
            }
        }


        //----------------------------------------

        if(wParam == 5002) // Use Priority Settings
        {
            if(IsDlgButtonChecked(DecryptDialog_hWnd, 5002))
            {
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, DecDlgKey, "UsePriorityOption", 1);
                PI.UsePriorityOption = TRUE;
            }
            else{
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, DecDlgKey, "UsePriorityOption", 0);
                PI.UsePriorityOption = FALSE;
            }
        }

        //----------------------------------------

        if(wParam == 5003) // Destroy Temp Files
        {
            if(IsDlgButtonChecked(DecryptDialog_hWnd, 5003))
            {
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, DecDlgKey, "DestroyTempOption", 1);
                PI.DestroyTempOption = TRUE;
            }
            else{
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, DecDlgKey, "DestroyTempOption", 0);
                PI.DestroyTempOption = FALSE;
            }
        }

        //----------------------------------------

        if(wParam == 5004) // Always On Top (Toggle)
        {
            if(IsDlgButtonChecked(DecryptDialog_hWnd, 5004))
            {
                SetWindowPos(DecryptDialog_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, DecDlgKey, "TopMost", 1);
            }
            else{
                SetWindowPos(DecryptDialog_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, DecDlgKey, "TopMost", 0);
            }
        }

        if(wParam == 100) //- Okay (Dialog Command)
        {
            char Password[56 + 1];

            int Length = Get_TextEx(PasswordEdit_hWnd, Password, 56);

            if(Length <  8)
            {
                MessageBox(DecryptDialog_hWnd, "Your password must be atleast 8 characters long.", gsTitleCaption, MB_TASKMODAL);
                return 0;
            }

            if(!MakeGlobalTargetName(DecryptDialog_hWnd))
                return 0;

            strcpy(PI.Password, Password);
            PI.ReadyToProcess = TRUE;

            EndDialog(DecryptDialog_hWnd,0);
            DestroyWindow(DecryptDialog_hWnd);
            return 0;
        }

        if(wParam == 200) //- Exit (Dialog Command)
        {
            PI.CancelFlag = TRUE;
            EndDialog(DecryptDialog_hWnd,0);
            DestroyWindow(DecryptDialog_hWnd);
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
        EndDialog(DecryptDialog_hWnd,0);
        DestroyWindow(DecryptDialog_hWnd);
        return 0;
    }

    //--------------------------------------------------------------------------

    if(xMessage == WM_DESTROY) // Received Destroy Message
    {
//        PostQuitMessage(0);
        return 0;
    }

    //--------------------------------------------------------------------------

    return 0;
}
//////////////////////////////////////////////////////////////////////

