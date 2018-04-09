
/////////////////////////////////////////////////////////////////////

BOOL CALLBACK MainDialog(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    static HMENU SystemMenu = 0;

    if(Msg==WM_INITDIALOG)
    {
        MainDialog_hWnd=hDlg;

        SystemMenu = GetSystemMenu(MainDialog_hWnd,FALSE);
        AppendMenu(SystemMenu,MF_SEPARATOR,0,0);
        AppendMenu(SystemMenu,MF_STRING,1000,"About");

        SendMessage(MainDialog_hWnd,(UINT)WM_SETTEXT,(WPARAM)0,(LPARAM)TITLECAPTION);
        SendMessage(MainDialog_hWnd,WM_SETICON,TRUE,(LPARAM)LoadIcon(GetWindowInstance(MainDialog_hWnd),MAKEINTRESOURCE(100)));

        StatusText_hWnd = GetDlgItem(MainDialog_hWnd,1000);
        StatusBar_hWnd = GetDlgItem(MainDialog_hWnd,1500);

        Append_Thread_Handle = CreateThread(NULL,0,Append_Thread,(LPVOID)0,0,&Append_Thread_ID);

        CenterWindow(MainDialog_hWnd);
        return 0;
    }

    if(Msg==WM_COMMAND)
    {
        if(wParam == 100) // Cancel/Ok Button
        {
            if(DoneFlag == FALSE)
                CancelFlag = TRUE;

            if(DoneFlag == TRUE)
                EndDialog(MainDialog_hWnd,0);
        }

        if(wParam == 101) // Delete Source Button
        {
            if(DoneFlag == TRUE)
            {
                if(!DeleteFile(MainInput))
                {
                    Set_Text(StatusText_hWnd, "Error Deleting File.");
                    EnableWindow(GetDlgItem(MainDialog_hWnd,101),FALSE);
                }
                else{
                    Set_Text(StatusText_hWnd, "Process Complete.");
                    EnableWindow(GetDlgItem(MainDialog_hWnd,101),FALSE);
                }
            }
        }

        return 0;
    }

    if(Msg==WM_SYSCOMMAND)
    {
        if(LOWORD(wParam) == 1000)
        {
            _AboutDialogInfo ADI;

            ADI.DisplayIcon  = LoadIcon(MainInstance, MAKEINTRESOURCE(100));
            ADI.TitleCaption = TITLECAPTION;
            ADI.FileVersion  = FILEVERSION;
            ADI.BuildDate    = __DATE__;
            ADI.BuildTime    = __TIME__;
            ADI.CopyRight    = APPCOPYRIGHT;
            ADI.OwnerHandle  = MainDialog_hWnd;

            NetLogo(&ADI);
        }
        return 0;
    }

    if(Msg==WM_CLOSE) //Exit (Windows Message)
    {
        EndDialog(MainDialog_hWnd,0);
        return 0;
    }

    return 0;
}

/////////////////////////////////////////////////////////////////////

