
/////////////////////////////////////////////////////////////////////

BOOL CALLBACK MainDialog(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	static HMENU SystemMenu 	  = NULL;
	static HWND PasswordEdit_hWnd = NULL;

	if(Msg==WM_INITDIALOG)
	{
		MainDialog_hWnd=hDlg;

		SystemMenu = GetSystemMenu(MainDialog_hWnd,FALSE);
		AppendMenu(SystemMenu,MF_SEPARATOR,0,0);
		AppendMenu(SystemMenu,MF_STRING,1000,"www.NetWorkDLS.com");
		AppendMenu(SystemMenu,MF_STRING,1001,"Version");

		SendMessage(MainDialog_hWnd,(UINT)WM_SETTEXT,(WPARAM)0,(LPARAM)TitleCaption);
		SendMessage(MainDialog_hWnd,WM_SETICON,TRUE,(LPARAM)LoadIcon(GetWindowInstance(MainDialog_hWnd),MAKEINTRESOURCE(100)));

		StatusBar_hWnd = GetDlgItem(MainDialog_hWnd,1000);
		StatusText_hWnd = GetDlgItem(MainDialog_hWnd,1500);
		PasswordEdit_hWnd = GetDlgItem(MainDialog_hWnd,2000);

		SendMessage(PasswordEdit_hWnd, EM_LIMITTEXT, 56, 0 );

		CenterWindow(MainDialog_hWnd);
		return 0;
	}

	if(Msg == WM_PAINT)
    {
        HDC ThisHDC;
        PAINTSTRUCT ThisPS;

        ThisHDC = BeginPaint(MainDialog_hWnd, &ThisPS);
        // Any painting should be done here
        EndPaint(MainDialog_hWnd, &ThisPS);
        return 0 ;
    }

	if(Msg == 307) // Key Down
	{
		int PasswordLen = SendMessage(PasswordEdit_hWnd, WM_GETTEXTLENGTH,0,0);

		if(PasswordLen >= 8)EnableWindow(GetDlgItem(MainDialog_hWnd,100),TRUE);
		if(PasswordLen <  8)EnableWindow(GetDlgItem(MainDialog_hWnd,100),FALSE);
		return 0;
	}

	if(Msg==WM_COMMAND)
	{
		if(wParam == 100) //Ok Button
		{
			CodedKey = GetText(PasswordEdit_hWnd);
			ShowWindow(MainDialog_hWnd,FALSE);
			DialogBox(MainInstance, MAKEINTRESOURCE(200), NULL, StatusDialog);
			EndDialog(MainDialog_hWnd,0);
		}

		if(wParam == 101) // Cancel Button
		{
			EndDialog(MainDialog_hWnd,0);
		}

		return 0;
	}

	if(Msg==WM_SYSCOMMAND)
	{
		if(LOWORD(wParam)==1000) //- NetWorkDLS.Com (SysMenu Command)
			ShellExecute(0,"Open","Http://www.NetWorkDLS.com/",NULL,NULL,SW_SHOWNORMAL);

		if(LOWORD(wParam) == 1001) //- Version (SysMenu Command)
		{
			char VersionText[128];
			sprintf(VersionText,"%s, Version %s", TitleCaption, FileVersion);
			MessageBox(MainDialog_hWnd, VersionText, "Application Version", MB_TASKMODAL);
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

