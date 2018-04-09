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

		GetValues(); // Get Inital Values

		CenterWindow(MainDialog_hWnd);

		return 0;
	}

	if(Msg==WM_COMMAND)
	{
		if(wParam == 1001) // - Priority Class Idle
		{
			CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_UNCHECKED);
			LastValue_1 = 1001;
			CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_CHECKED);
		}
		if(wParam == 1002) // - Priority Class Normal
		{
			CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_UNCHECKED);
			LastValue_1 = 1002;
			CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_CHECKED);
		}
		if(wParam == 1003) // - Priority Class High
		{
			CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_UNCHECKED);
			LastValue_1 = 1003;
			CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_CHECKED);
		}
		if(wParam == 1004) // - Priority Class Real Time
		{
			CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_UNCHECKED);
			LastValue_1 = 1004;
			CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_CHECKED);
		}

		// ------------------------------

		if(wParam == 2001) // - Thread Priority Idle
		{
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_UNCHECKED);
			LastValue_2 = 2001;
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
		}
		if(wParam == 2002) // - Thread Priority Lowest
		{
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_UNCHECKED);
			LastValue_2 = 2002;
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
		}
		if(wParam == 2003) // - Thread Priority Below Normal
		{
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_UNCHECKED);
			LastValue_2 = 2003;
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
		}
		if(wParam == 2004) // - Thread Priority Normal
		{
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_UNCHECKED);
			LastValue_2 = 2004;
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
		}
		if(wParam == 2005) // - Thread Priority Above Normal
		{
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_UNCHECKED);
			LastValue_2 = 2005;
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
		}
		if(wParam == 2006) // - Thread Priority Highest
		{
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_UNCHECKED);
			LastValue_2 = 2006;
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
		}
		if(wParam == 2007) // - Thread Priority Real Time
		{
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_UNCHECKED);
			LastValue_2 = 2007;
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
		}

		if(wParam==100) //- Okay (Dialog Command)
		{
			if(SetValues()) // Set New Values To Registry
			{
				EndDialog(MainDialog_hWnd,0);
			}
		}

		if(wParam==300) //- Exit (Dialog Command)
		{
			EndDialog(MainDialog_hWnd,0);
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

