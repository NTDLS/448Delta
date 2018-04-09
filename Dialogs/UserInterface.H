///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DefaultInfoMessage " - Welcome to 448Delta Encryption\r\n\r\nTo begin encrypting or decrypting, simply select the desired operation from the list on the left. If you need help you can click the help icon, there you will find the answers to common questions and contact information if you need support."

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK UserInterfaceDialog(HWND xHandle, UINT xMessage, WPARAM wParam, LPARAM lParam)
{
    //--------------------------------------------------------------------------

	static HWND FolderIcon_hWnd        = NULL;
	static HWND SingleFileIcon_hWnd    = NULL;
	static HWND EncryptedIcon_hWnd     = NULL;
	static HWND HelpIcon_hWnd          = NULL;
	static HWND BuyOnlineIcon_hWnd     = NULL;

	static HWND FolderText_hWnd        = NULL;
	static HWND SingleFileText_hWnd    = NULL;
	static HWND EncryptedText_hWnd     = NULL;
	static HWND HelpText_hWnd          = NULL;

	static HWND InfoText_hWnd          = NULL;
	static HWND Frame1_hWnd            = NULL;
	static HWND Check1_hWnd            = NULL;

    static bool MouseOverFolder        = false;
    static bool MouseOverSingleFile    = false;
    static bool MouseOverEncrypted     = false;
    static bool MouseOverHelp          = false;
    static bool MouseOverBuyOnline     = false;

    static HFONT InfoTextFontHandle = NULL;
    static HFONT LinksFontHandle    = NULL;

    static HMENU SystemMenu = NULL;

    static DWORD RGBBlack, RGBWhite, RGBBlue, RGBRed;

    static HCURSOR HandCursor, ArrowCursor;

    //--------------------------------------------------------------------------

    if(xMessage == WM_INITDIALOG) // Received A Initilize Dialog Message
    {
        UserInterfaceDialog_hWnd = xHandle;

        SendMessage(xHandle, (UINT)WM_SETTEXT, (WPARAM)0, (LPARAM)gsTitleCaption);
        SendMessage(xHandle, WM_SETICON, TRUE, (LPARAM)LoadIcon(MainInstance, MAKEINTRESOURCE(IDI_MAINCON)) );

        SystemMenu = GetSystemMenu(xHandle, FALSE);
        AppendMenu(SystemMenu,MF_SEPARATOR,0,0);
        AppendMenu(SystemMenu,MF_STRING,1000,"About");

        RGBWhite = RGB(255, 255, 255);
        RGBBlack = RGB(0, 0, 0);
        RGBBlue  = RGB(0, 0, 255);
        RGBRed   = RGB(255, 0, 0);

        InfoTextFontHandle = MakeFont("MS Sans Serif", 11);
        LinksFontHandle = MakeFont("MS Sans Serif", 9);

		HandCursor = LoadCursor(MainInstance, MAKEINTRESOURCE(IDC_HANDCURSOR));
        ArrowCursor = LoadCursor(NULL, IDC_ARROW);

        Frame1_hWnd            = GetDlgItem(xHandle, IDC_FRAME1);
        Check1_hWnd            = GetDlgItem(xHandle, IDC_SAVE_TO_DESKTOP);

        InfoText_hWnd          = GetDlgItem(xHandle, IDC_INFOTEXT);

        FolderIcon_hWnd        = GetDlgItem(xHandle, IDC_FOLDERICON);
        SingleFileIcon_hWnd    = GetDlgItem(xHandle, IDC_SINGLEFILEICON);
        EncryptedIcon_hWnd     = GetDlgItem(xHandle, IDC_ENCRYPTEDICON);
        HelpIcon_hWnd          = GetDlgItem(xHandle, IDC_HELPICON);
        BuyOnlineIcon_hWnd     = GetDlgItem(xHandle, IDC_BUYONLINE);

        FolderText_hWnd        = GetDlgItem(xHandle, IDC_FOLDERTEXT);
        SingleFileText_hWnd    = GetDlgItem(xHandle, IDC_SINGLEFILETEXT);
        EncryptedText_hWnd     = GetDlgItem(xHandle, IDC_ENCRYPTEDTEXT);
        HelpText_hWnd          = GetDlgItem(xHandle, IDC_HELPTEXT);

        SetFont(FolderText_hWnd, LinksFontHandle);
        SetFont(SingleFileText_hWnd, LinksFontHandle);
        SetFont(EncryptedText_hWnd, LinksFontHandle);
        SetFont(HelpText_hWnd, LinksFontHandle);
        SetFont(InfoText_hWnd, InfoTextFontHandle);

        if(!IsRegistered)
        {
            ShowWindow(BuyOnlineIcon_hWnd, SW_SHOW);
        }


        Set_Text(InfoText_hWnd, DefaultInfoMessage);

        if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE, UIDlgKey, "SaveToDesktop", (DWORD &)UID.SaveAllDataToDesktop))
        {
            if(UID.SaveAllDataToDesktop)
            {
                CheckDlgButton(xHandle, IDC_SAVE_TO_DESKTOP, BST_CHECKED);
                UID.SaveAllDataToDesktop = true;
            }
            else{
                CheckDlgButton(xHandle, IDC_SAVE_TO_DESKTOP, BST_UNCHECKED);
                UID.SaveAllDataToDesktop = false;
            }
        }
        else{
            CheckDlgButton(xHandle, IDC_SAVE_TO_DESKTOP, BST_CHECKED);
            UID.SaveAllDataToDesktop = true;
        }

        CenterWindow(xHandle);

        return 0;
    }

    //--------------------------------------------------------------------------

    if(xMessage == WM_CTLCOLORDLG )
        return Set_Color(RGBBlack, RGBWhite, wParam);

    //--------------------------------------------------------------------------

    if(xMessage == WM_CTLCOLORSTATIC)
	{
		if((HANDLE)lParam == FolderText_hWnd)
        {
            if(IsMouseOverHwnd(FolderIcon_hWnd) || IsMouseOverHwnd(FolderText_hWnd))
    			return Set_Color(RGBRed, RGBWhite, wParam);
            else return Set_Color(RGBBlack, RGBWhite, wParam);
        }

		if((HANDLE)lParam == SingleFileText_hWnd)
        {
            if(IsMouseOverHwnd(SingleFileIcon_hWnd) || IsMouseOverHwnd(SingleFileText_hWnd))
    			return Set_Color(RGBRed, RGBWhite, wParam);
            else return Set_Color(RGBBlack, RGBWhite, wParam);
        }

		if((HANDLE)lParam == EncryptedText_hWnd)
        {
            if(IsMouseOverHwnd(EncryptedIcon_hWnd) || IsMouseOverHwnd(EncryptedText_hWnd))
    			return Set_Color(RGBRed, RGBWhite, wParam);
            else return Set_Color(RGBBlack, RGBWhite, wParam);
        }

		if((HANDLE)lParam == HelpText_hWnd)
        {
            if(IsMouseOverHwnd(HelpIcon_hWnd) || IsMouseOverHwnd(HelpText_hWnd))
    			return Set_Color(RGBRed, RGBWhite, wParam);
            else return Set_Color(RGBBlack, RGBWhite, wParam);
        }

        if((HANDLE)lParam == InfoText_hWnd || (HANDLE)lParam == Frame1_hWnd || (HANDLE)lParam == Check1_hWnd)
            return Set_Color(RGBBlue, RGBWhite, wParam);
	}

    //--------------------------------------------------------------------------

	if(IsMouseOverHwnd(FolderIcon_hWnd) || IsMouseOverHwnd(FolderText_hWnd))
	{
        if(xMessage == WM_LBUTTONUP) // Clicked on
        {
            char FolderName[257];
            if(BrowseDirectory(xHandle, "Select the folder that you would like to encrypt.", BIF_RETURNFSANCESTORS, FolderName))
            {
                UID.UserIntent = UIEncryptFolder;
                strcpy(UID.InputName, FolderName);
                UID.DialogHandle = xHandle;
                ProcessFromUserInterface_Thread_Handle = CreateThread(NULL, 0, ProcessFromUserInterface_Thread, (LPVOID)0,0,&ProcessFromUserInterface_Thread_ID);
            }
        }

        if(MouseOverFolder == false)
        {
            Set_Text(InfoText_hWnd, " - Encrypt an Entire Folder\r\n\r\nThis lets you encrypt an entire folder, the files it contains and any folders contained within it as well. Once the folder has been encrypted, all of its contents are safe from anyone who does not have the password.");

            SetClassLong(xHandle, GCL_HCURSOR, (LONG)HandCursor);

            MouseOverFolder = true;

            RECT Rect;
            GetClientRect(FolderText_hWnd, &Rect);
            InvalidateRect(FolderText_hWnd, &Rect, TRUE);
        }
    }
    else{
        if(MouseOverFolder == true)
        {
            Set_Text(InfoText_hWnd, DefaultInfoMessage);

            SetClassLong(xHandle, GCL_HCURSOR, (LONG)ArrowCursor);

            RECT Rect;
            GetClientRect(FolderText_hWnd, &Rect);
            InvalidateRect(FolderText_hWnd, &Rect, TRUE);

            MouseOverFolder = false;
        }
    }

    if(!IsRegistered)
    {
        if(IsMouseOverHwnd(BuyOnlineIcon_hWnd))
        {
            if(!MouseOverBuyOnline)
            {
                MouseOverBuyOnline = true;
                SetClassLong(xHandle, GCL_HCURSOR, (LONG)HandCursor);

                Set_Text(InfoText_hWnd, " - Buy online\r\n\r\nAllows you to enter your registration number or buy one if you have not already done so. By purchasing any of the fine products produced by NetWorkDLS you enable us to provide other products and make free enhancements to previously purchaced products.");
            }

            if(xMessage == WM_LBUTTONUP) // Clicked on
            {
                if(DialogBox(MainInstance, MAKEINTRESOURCE(IDD_REGISTERDIALOG), xHandle, RegisterDialog) == 100)
                {
                    EndDialog(xHandle, 0);
                }
                return TRUE;
            }
        }
        else{
            if(MouseOverBuyOnline == true)
            {
                Set_Text(InfoText_hWnd, DefaultInfoMessage);

                SetClassLong(xHandle, GCL_HCURSOR, (LONG)ArrowCursor);

                RECT Rect;
                GetClientRect(BuyOnlineIcon_hWnd, &Rect);
                InvalidateRect(BuyOnlineIcon_hWnd, &Rect, TRUE);

                MouseOverBuyOnline = false;
            }
        }
    }

	if(IsMouseOverHwnd(SingleFileIcon_hWnd) || IsMouseOverHwnd(SingleFileText_hWnd))
	{
        if(xMessage == WM_LBUTTONUP) // Clicked on
        {
            char FileName[257];

            char *DefaultExtension = NULL;
            char *Filters = "All-Files\0*.*\0\0";

            DWORD Flags      = OFN_EXPLORER + OFN_LONGNAMES + OFN_PATHMUSTEXIST;

            strcpy(FileName, "");

            if(OSFileDialog(xHandle, FileName, DefaultExtension, "Select the file that you want to encrypt.",Filters, Get_CurrentDirectory(), OFD, Flags))
            {
                UID.UserIntent = UIEncryptFile;

                strcpy(UID.InputName, FileName);
                UID.DialogHandle = xHandle;
                ProcessFromUserInterface_Thread_Handle = CreateThread(NULL,0,ProcessFromUserInterface_Thread,(LPVOID)0,0,&ProcessFromUserInterface_Thread_ID);
            }
        }

        if(MouseOverSingleFile == false)
        {
            Set_Text(InfoText_hWnd, " - Encrypt a Single File\r\n\r\nThis lets you encrypt a single file or document. Once the file has been encrypted it is safe from anyone who does not have the password.");

            SetClassLong(xHandle, GCL_HCURSOR, (LONG)HandCursor);

            MouseOverSingleFile = true;

            RECT Rect;
            GetClientRect(SingleFileText_hWnd, &Rect);
            InvalidateRect(SingleFileText_hWnd, &Rect, TRUE);
        }
    }
    else{
        if(MouseOverSingleFile == true)
        {
            Set_Text(InfoText_hWnd, DefaultInfoMessage);

            SetClassLong(xHandle, GCL_HCURSOR, (LONG)ArrowCursor);

            RECT Rect;
            GetClientRect(SingleFileText_hWnd, &Rect);
            InvalidateRect(SingleFileText_hWnd, &Rect, TRUE);

            MouseOverSingleFile = false;
        }
    }

    //--------------------------------------------------------------------------

	if(IsMouseOverHwnd(EncryptedIcon_hWnd) || IsMouseOverHwnd(EncryptedText_hWnd))
	{
        if(xMessage == WM_LBUTTONUP) // Clicked on
        {
            char FileName[257];

            char *DefaultExtension = NULL;
            char *Filters = "All-Encrypted-Data\0*.448;*.448pkg\0Encrypted-Files\0*.448\0Encrypted-Folders\0*.448Pkg\0All-Files\0*.*\0\0";

            DWORD Flags      = OFN_EXPLORER + OFN_LONGNAMES + OFN_PATHMUSTEXIST;

            strcpy(FileName, "");

            if(OSFileDialog(xHandle, FileName, DefaultExtension, "Select the encrypted file or folder that you want to decrypt.",Filters, Get_CurrentDirectory(), OFD, Flags))
            {
                UID.UserIntent = UIEncryptDecrypt;

                strcpy(UID.InputName, FileName);
                UID.DialogHandle = xHandle;
                ProcessFromUserInterface_Thread_Handle = CreateThread(NULL,0,ProcessFromUserInterface_Thread,(LPVOID)0,0,&ProcessFromUserInterface_Thread_ID);
            }
        }

        if(MouseOverEncrypted == false)
        {
            Set_Text(InfoText_hWnd, " - Decrypt a File or Folder\r\n\r\nThis lets you decrypt a file or folder that has been previously encrypted by 448Delta, Supplied that you have the password.");

            SetClassLong(xHandle, GCL_HCURSOR, (LONG)HandCursor);

            MouseOverEncrypted = true;

            RECT Rect;
            GetClientRect(EncryptedText_hWnd, &Rect);
            InvalidateRect(EncryptedText_hWnd, &Rect, TRUE);
        }
    }
    else{
        if(MouseOverEncrypted == true)
        {
            Set_Text(InfoText_hWnd, DefaultInfoMessage);

            SetClassLong(xHandle, GCL_HCURSOR, (LONG)ArrowCursor);

            RECT Rect;
            GetClientRect(EncryptedText_hWnd, &Rect);
            InvalidateRect(EncryptedText_hWnd, &Rect, TRUE);

            MouseOverEncrypted = false;
        }
    }

    //--------------------------------------------------------------------------

	if(IsMouseOverHwnd(HelpIcon_hWnd) || IsMouseOverHwnd(HelpText_hWnd))
	{
        if(xMessage == WM_LBUTTONUP) // Clicked on
        {
            if(!LaunchHelpFile())
                MessageBox(xHandle, "The help file was not found. Please reinstall 448Delta.", gsTitleCaption, 0);
        }

        if(MouseOverHelp == false)
        {
            Set_Text(InfoText_hWnd, " - View Help File\r\n\r\nHere you should find the answer to any problem or question that could arise. Be sure to read and fully understand the NetWorkDLS Software License and Copyright Notice. If you have a question or a problem that is not addressed here, visit the contact page in the help file.");

            SetClassLong(xHandle, GCL_HCURSOR, (LONG)HandCursor);

            MouseOverHelp = true;

            RECT Rect;
            GetClientRect(HelpText_hWnd, &Rect);
            InvalidateRect(HelpText_hWnd, &Rect, TRUE);
        }
    }
    else{
        if(MouseOverHelp == true)
        {
            Set_Text(InfoText_hWnd, DefaultInfoMessage);

            SetClassLong(xHandle, GCL_HCURSOR, (LONG)ArrowCursor);

            RECT Rect;
            GetClientRect(HelpText_hWnd, &Rect);
            InvalidateRect(HelpText_hWnd, &Rect, TRUE);

            MouseOverHelp = false;
        }
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
        if(wParam == IDC_SAVE_TO_DESKTOP)
        {
            if(IsDlgButtonChecked(xHandle, IDC_SAVE_TO_DESKTOP))
            {
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, UIDlgKey, "SaveToDesktop", 1);
                UID.SaveAllDataToDesktop = true;
            }
            else{
                Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE, UIDlgKey, "SaveToDesktop", 0);
                UID.SaveAllDataToDesktop = false;
            }

            return 0;
        }

        if(wParam == IDC_ABOUT) //- About button (Dialog Command)
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

        if(wParam == IDC_EXIT) //- About button (Dialog Command)
        {
            EndDialog(xHandle,0);
            DestroyWindow(xHandle);    
            return 0;
        }

        return 0;
    }

    //--------------------------------------------------------------------------

    if(xMessage == WM_CLOSE)  // Received Close Message
    {
        EndDialog(xHandle,0);
        DestroyWindow(xHandle);
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

    if(xMessage == WM_DESTROY) // Received Destroy Message
    {
//        PostQuitMessage(0);
        return 0;
    }

    //--------------------------------------------------------------------------

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

