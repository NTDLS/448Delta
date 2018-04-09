///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK UnregisteredDialog(HWND xHandle, UINT xMessage, WPARAM wParam, LPARAM lParam)
{
    //--------------------------------------------------------------------------

    static HMENU SystemMenu = NULL;
    static DWORD RGBBlack, RGBWhite, RGBBlue, RGBRed;

    //--------------------------------------------------------------------------

    if(xMessage == WM_INITDIALOG) // Received A Initilize Dialog Message
    {
        UnregisteredDialog_hWnd = xHandle;

        SystemMenu = GetSystemMenu(xHandle, FALSE);
        AppendMenu(SystemMenu,MF_SEPARATOR, 0, 0);
        AppendMenu(SystemMenu,MF_STRING, 1000, "About");

        RGBWhite = RGB(255, 255, 255);
        RGBBlack = RGB(0, 0, 0);
        RGBBlue  = RGB(0, 0, 255);
        RGBRed   = RGB(255, 0, 0);

        SendMessage(xHandle, (UINT)WM_SETTEXT, (WPARAM)0, (LPARAM)gsTitleCaption);
        SendMessage(xHandle, WM_SETICON, TRUE, (LPARAM)LoadIcon(MainInstance, MAKEINTRESOURCE(IDI_MANICON)) );

        CenterWindow(xHandle);

        return TRUE;
    }

    //--------------------------------------------------------------------------

    if(xMessage == WM_CTLCOLORDLG || xMessage == WM_CTLCOLORSTATIC)
        return Set_Color(RGBBlack, RGBWhite, wParam);

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
        if(wParam == IDC_CMDTRIAL)
        {
            EndDialog(xHandle, 0);
            return TRUE;
        }

        if(wParam == IDC_CMDREGNOW)
        {
            EndDialog(xHandle, 1);
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

