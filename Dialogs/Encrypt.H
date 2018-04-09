/////////////////////////////////////////////////////////////////////

BOOL CALLBACK EncryptDialog(HWND xHandle, UINT xMessage, WPARAM wParam, LPARAM lParam)
{
    //--------------------------------------------------------------------------

    static HWND PasswordEdit1_hWnd   = NULL;
    static HWND PasswordEdit2_hWnd   = NULL;
    static HWND PasswordBitsBar_hWnd = NULL;
    static HWND PasswordBits_hWnd    = NULL;

    static HMENU SystemMenu          = NULL;

    DWORD DWORDRegTemp               = 0;

    //--------------------------------------------------------------------------

    if(xMessage == WM_INITDIALOG) // Received A Initilize Dialog Message
    {
        EncryptDialog_hWnd = xHandle;

        SystemMenu = GetSystemMenu(EncryptDialog_hWnd,FALSE);
        AppendMenu(SystemMenu,MF_SEPARATOR,0,0);
        AppendMenu(SystemMenu,MF_STRING,1000,"About");

        SendMessage(EncryptDialog_hWnd, (UINT)WM_SETTEXT, (WPARAM)0, (LPARAM)gsTitleCaption);
        SendMessage(EncryptDialog_hWnd, WM_SETICON, TRUE, (LPARAM)LoadIcon(MainInstance, MAKEINTRESOURCE(IDI_MANICON)) );

        PasswordEdit1_hWnd   = GetDlgItem(EncryptDialog_hWnd,1000);
        PasswordEdit2_hWnd   = GetDlgItem(EncryptDialog_hWnd,2000);
        PasswordBitsBar_hWnd = GetDlgItem(EncryptDialog_hWnd,3000);
        PasswordBits_hWnd    = GetDlgItem(EncryptDialog_hWnd,4000);

//      Set_Text(PasswordEdit1_hWnd, "123456789");
//      Set_Text(PasswordEdit2_hWnd, "123456789");

        SendMessage(PasswordEdit1_hWnd, EM_LIMITTEXT, 56, 0 );
        SendMessage(PasswordEdit2_hWnd, EM_LIMITTEXT, 56, 0 );

        //-/////////////////////////////////////////////////////////////////////////////

        /* Compress Data */
        if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE,EncDlgKey,"CompressOption",DWORDRegTemp))
        {
            if(DWORDRegTemp == 1) // Compress Data
            {
                CheckDlgButton(EncryptDialog_hWnd, 5000, BST_CHECKED);
                PI.CompressOption = TRUE;
            }
        }
        else{ // If Key Not Found Create Key And Value
                CreateRegistryKey(HKEY_LOCAL_MACHINE,EncDlgKey);
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, EncDlgKey, "CompressOption", 0);
                CheckDlgButton(EncryptDialog_hWnd, 5000, BST_CHECKED);
                PI.CompressOption = FALSE;
        }

        //----------------------------------------

        /* Delete Source File(s) */
        if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE,EncDlgKey,"DeleteSourceOption",DWORDRegTemp))
        {
            if(DWORDRegTemp == 1) // Delete Source File(s)
            {
                CheckDlgButton(EncryptDialog_hWnd, 5001, BST_CHECKED);
                PI.DeleteSourceOption = TRUE;
            }
        }
        else{ // If Key Not Found Create Key And Value
                CreateRegistryKey(HKEY_LOCAL_MACHINE,EncDlgKey);
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, EncDlgKey, "DeleteSourceOption", 0);
                CheckDlgButton(EncryptDialog_hWnd, 5001, BST_CHECKED);
                PI.DeleteSourceOption = FALSE;
        }

        //----------------------------------------

        /* Create Self Extractor */
        if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE,EncDlgKey,"CreateSelfOption",DWORDRegTemp))
        {
            if(DWORDRegTemp == 1) // Create Self Extractor
            {
                CheckDlgButton(EncryptDialog_hWnd, 5002, BST_CHECKED);
                PI.CreateSelfOption = TRUE;
            }
        }
        else{ // If Key Not Found Create Key And Value
                CreateRegistryKey(HKEY_LOCAL_MACHINE,EncDlgKey);
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, EncDlgKey, "CreateSelfOption", 0);
                CheckDlgButton(EncryptDialog_hWnd, 5002, BST_CHECKED);
                PI.CreateSelfOption = FALSE;
        }

        //----------------------------------------

        /* Use Priority Settings */
        if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE,EncDlgKey,"UsePriorityOption",DWORDRegTemp))
        {
            if(DWORDRegTemp == 1) // Use Priority Settings
            {
                CheckDlgButton(EncryptDialog_hWnd, 5004, BST_CHECKED);
                PI.UsePriorityOption = TRUE;
            }
        }
        else{ // If Key Not Found Create Key And Value
                CreateRegistryKey(HKEY_LOCAL_MACHINE,EncDlgKey);
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, EncDlgKey, "UsePriorityOption", 1);
                CheckDlgButton(EncryptDialog_hWnd, 5004, BST_CHECKED);
                PI.UsePriorityOption = FALSE;
        }

        //----------------------------------------

        /* Destroy Temp Files */
        if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE,EncDlgKey,"DestroyTempOption",DWORDRegTemp))
        {
            if(DWORDRegTemp == 1) // Destroy Temp Files
            {
                CheckDlgButton(EncryptDialog_hWnd, 5005, BST_CHECKED);
                PI.DestroyTempOption = TRUE;
            }
        }
        else{ // If Key Not Found Create Key And Value
                CreateRegistryKey(HKEY_LOCAL_MACHINE,EncDlgKey);
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, EncDlgKey, "DestroyTempOption", 1);
                CheckDlgButton(EncryptDialog_hWnd, 5005, BST_CHECKED);
                PI.DestroyTempOption = FALSE;
        }

        //----------------------------------------

        /* Always On Top */
        if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE,EncDlgKey,"TopMost",DWORDRegTemp))
        {
            if(DWORDRegTemp == 1) // Always On Top
            {
                CheckDlgButton(EncryptDialog_hWnd, 5006, BST_CHECKED);
                SetWindowPos(EncryptDialog_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            }
        }
        else{ // If Key Not Found Create Key And Value
                CreateRegistryKey(HKEY_LOCAL_MACHINE,EncDlgKey);
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, EncDlgKey, "TopMost", 1);
                CheckDlgButton(EncryptDialog_hWnd, 5006, BST_CHECKED);
                SetWindowPos(EncryptDialog_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        }

        //-/////////////////////////////////////////////////////////////////////////////

        CenterWindow(EncryptDialog_hWnd);

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

    if(xMessage == 307) // Key Down
    {
        float PasswordLen1   = 0;
        float PasswordLen2    = 0;
        float EffeciencyBits = 0;

        char LocalTemp[1024];

        PasswordLen1 = (float) SendMessage(PasswordEdit1_hWnd, WM_GETTEXTLENGTH, 0, 0);
        PasswordLen2 = (float) SendMessage(PasswordEdit2_hWnd, WM_GETTEXTLENGTH, 0, 0);

        HWND CurrentFocus = GetFocus();

        if(CurrentFocus == PasswordEdit1_hWnd)
        {
            EffeciencyBits = (PasswordLen1 / 56) * 100;
            SendMessage(PasswordBitsBar_hWnd, PBM_SETPOS, (int) EffeciencyBits, 0);
            sprintf(LocalTemp," (%.0f Bits)", PasswordLen1 * 8);
            Set_Text(PasswordBits_hWnd,LocalTemp);
        }

        if(CurrentFocus == PasswordEdit2_hWnd)
        {
            EffeciencyBits = (PasswordLen2 / 56) * 100;
            SendMessage(PasswordBitsBar_hWnd, PBM_SETPOS, (int) EffeciencyBits, 0);
            sprintf(LocalTemp," (%.0f Bits)", PasswordLen2 * 8);
            Set_Text(PasswordBits_hWnd,LocalTemp);
        }
    }

    //--------------------------------------------------------------------------

    if(xMessage == WM_COMMAND) // Received A Message
    {
        //----------------------------------------

        if(wParam == 5000) // Compress Data
        {
            if(IsDlgButtonChecked(EncryptDialog_hWnd, 5000))
            {
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, EncDlgKey, "CompressOption", 1);
                PI.CompressOption = TRUE;
            }
            else{
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, EncDlgKey, "CompressOption", 0);
                PI.CompressOption = FALSE;
            }
        }

        //----------------------------------------

        if(wParam == 5001) // Delete Source File(s)
        {
            if(IsDlgButtonChecked(EncryptDialog_hWnd, 5001))
            {
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, EncDlgKey, "DeleteSourceOption", 1);
                PI.DeleteSourceOption = TRUE;
            }
            else{
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, EncDlgKey, "DeleteSourceOption", 0);
                PI.DeleteSourceOption = FALSE;
            }
        }

        //----------------------------------------

        if(wParam == 5002) // Create Self Extractor
        {
            if(IsDlgButtonChecked(EncryptDialog_hWnd, 5002))
            {
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, EncDlgKey, "CreateSelfOption", 1);
                PI.CreateSelfOption = TRUE;
            }
            else{
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, EncDlgKey, "CreateSelfOption", 0);
                PI.CreateSelfOption = FALSE;
            }
        }

        //----------------------------------------

        if(wParam == 5004) // Use Priority Settings
        {
            if(IsDlgButtonChecked(EncryptDialog_hWnd, 5004))
            {
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, EncDlgKey, "UsePriorityOption", 1);
                PI.UsePriorityOption = TRUE;
            }
            else{
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, EncDlgKey, "UsePriorityOption", 0);
                PI.UsePriorityOption = FALSE;
            }
        }

        //----------------------------------------

        if(wParam == 5005) // Destroy Temp Files
        {
            if(IsDlgButtonChecked(EncryptDialog_hWnd, 5005))
            {
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, EncDlgKey, "DestroyTempOption", 1);
                PI.DestroyTempOption = TRUE;
            }
            else{
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, EncDlgKey, "DestroyTempOption", 0);
                PI.DestroyTempOption = FALSE;
            }
        }

        //----------------------------------------

        if(wParam == 5006) // Always On Top
        {
            if(IsDlgButtonChecked(EncryptDialog_hWnd, 5006))
            {
                SetWindowPos(EncryptDialog_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, EncDlgKey, "TopMost", 1);
            }
            else{
                SetWindowPos(EncryptDialog_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, EncDlgKey, "TopMost", 0);
            }
        }

        //----------------------------------------

        if(wParam == 100) //- Okay (Dialog Command)
        {
            char Password1[56 + 1];
            char Password2[56 + 1];

            int Length1 = Get_TextEx(PasswordEdit1_hWnd, Password1, 56);
            int Length2 = Get_TextEx(PasswordEdit2_hWnd, Password2, 56);

            if( (Length1 <  8) || (Length2 <  8) )
            {
                MessageBox(EncryptDialog_hWnd, "Your password must be atleast 8 characters long.", gsTitleCaption, MB_TASKMODAL);
                return 0;
            }

            if( strcmp(Password1, Password2) != 0 )
            {
                MessageBox(EncryptDialog_hWnd, "The two passwords do not match.", gsTitleCaption, MB_TASKMODAL);
                return 0;
            }

            if(!MakeGlobalTargetName(EncryptDialog_hWnd))
                return 0;

            strcpy(PI.Password, Password1);
            PI.ReadyToProcess = TRUE;

            EndDialog(EncryptDialog_hWnd,0);
            DestroyWindow(EncryptDialog_hWnd);
            return 0;
        }

        if(wParam == 200) //- Exit (Dialog Command)
        {
            PI.CancelFlag = TRUE;
            EndDialog(EncryptDialog_hWnd,0);
            DestroyWindow(EncryptDialog_hWnd);
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

            ADI.DisplayIcon  = LoadIcon(MainInstance, MAKEINTRESOURCE(IDI_MANICON));
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
        PI.CancelFlag = TRUE;
        EndDialog(EncryptDialog_hWnd,0);
        DestroyWindow(EncryptDialog_hWnd);
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

