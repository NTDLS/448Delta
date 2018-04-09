
/////////////////////////////////////////////////////////////////////

BOOL CALLBACK StatusDialog(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	static HMENU SystemMenu 	  = NULL;

	if(Msg==WM_INITDIALOG)
	{
		StatusDialog_hWnd = hDlg;

		SystemMenu = GetSystemMenu(StatusDialog_hWnd,FALSE);
		AppendMenu(SystemMenu,MF_SEPARATOR,0,0);
		AppendMenu(SystemMenu,MF_STRING,1000,"www.NetWorkDLS.com");
		AppendMenu(SystemMenu,MF_STRING,1001,"Version");

		SendMessage(StatusDialog_hWnd,(UINT)WM_SETTEXT,(WPARAM)0,(LPARAM)TitleCaption);
		SendMessage(StatusDialog_hWnd,WM_SETICON,TRUE,(LPARAM)LoadIcon(GetWindowInstance(StatusDialog_hWnd),MAKEINTRESOURCE(100)));

		StatusBar_hWnd = GetDlgItem(StatusDialog_hWnd,1000);
		StatusText_hWnd = GetDlgItem(StatusDialog_hWnd,1500);
		OkCancelButton_hWnd = GetDlgItem(StatusDialog_hWnd,100);

		CenterWindow(StatusDialog_hWnd);

		SetTimer(StatusDialog_hWnd, 0, 10, NULL);

		UnPack_Thread_Handle = CreateThread(NULL,0,UnPack_Thread,(LPVOID)0,0,&UnPack_Thread_ID);

		return 0;
	}

	if(Msg == WM_PAINT)
    {
        HDC ThisHDC;
        PAINTSTRUCT ThisPS;

        ThisHDC = BeginPaint(StatusDialog_hWnd, &ThisPS);
        // Any painting should be done here
        EndPaint(StatusDialog_hWnd, &ThisPS);
        return 0 ;
    }

	if(Msg==WM_COMMAND)
	{
		if(wParam == 100) //Ok / Cancel Button
		{
			if(DoneFlag == TRUE)
				EndDialog(StatusDialog_hWnd,0);

			if(DoneFlag == FALSE)
			{
				EnableWindow(OkCancelButton_hWnd,FALSE);
				CancelFlag = TRUE;
				SetText(StatusText_hWnd,"Please Wait...");
			}
		}
		return 0;
	}

	if(Msg == WM_TIMER)
	{
		/* Update Status Bar */
		if(LastValue != StatusValue)
		{
			LastValue = StatusValue;
			SendMessage(StatusBar_hWnd, PBM_SETPOS, (int) StatusValue, 0);
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
			MessageBox(StatusDialog_hWnd, VersionText, "Application Version", MB_TASKMODAL);
		}
		return 0;
	}

	if(Msg==WM_CLOSE) //Exit (Windows Message)
	{
		if(DoneFlag == TRUE)
			EndDialog(StatusDialog_hWnd,0);

		if(DoneFlag == FALSE)
		{
			EnableWindow(OkCancelButton_hWnd,FALSE);
			CancelFlag = TRUE;
			SetText(StatusText_hWnd,"Please Wait...");
		}
		return 0;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////

