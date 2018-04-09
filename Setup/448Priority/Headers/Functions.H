/////////////////////////////////////////////////////////////////////

void RegError_Set(void)
{
    MessageBox(MainDialog_hWnd, "An Error Has Occured Writing To The Registry Or \r\n There Has Been An Internal Error.", TITLECAPTION, MB_ICONHAND + MB_SYSTEMMODAL);
    exit(0);
}

/////////////////////////////////////////////////////////////////////

void RegError_Get(void)
{
    MessageBox(MainDialog_hWnd, "Either 448Delta Is Not Installed Or \r\n There Has Been An Internal Error.", TITLECAPTION,0 + MB_ICONHAND + MB_SYSTEMMODAL);
    exit(0);
}

/////////////////////////////////////////////////////////////////////

int GetValues(void)
{
    /* Get PriorityClass Value From Registry */
    if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"Process",PriorityClass))
    {
        if(PriorityClass == 1)
        {
            LastValue_1 = 1001;
            CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_CHECKED);
        }
        if(PriorityClass == 2)
        {
            LastValue_1 = 1002;
            CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_CHECKED);
        }
        if(PriorityClass == 3)
        {
            LastValue_1 = 1003;
            CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_CHECKED);
        }
        if(PriorityClass == 4)
        {
            LastValue_1 = 1004;
            CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_CHECKED);
        }
    }
    else RegError_Get();

    /* Get ThreadPriority Value From Registry */
    if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"Thread",ThreadPriority))
    {
        if(ThreadPriority == 1)
        {
            LastValue_2 = 2001;
            CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
        }
        if(ThreadPriority == 2)
        {
            LastValue_2 = 2002;
            CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
        }
        if(ThreadPriority == 3)
        {
            LastValue_2 = 2003;
            CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
        }
        if(ThreadPriority == 4)
        {
            LastValue_2 = 2004;
            CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
        }
        if(ThreadPriority == 5)
        {
            LastValue_2 = 2005;
            CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
        }
        if(ThreadPriority == 6)
        {
            LastValue_2 = 2006;
            CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
        }
        if(ThreadPriority == 7)
        {
            LastValue_2 = 2007;
            CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
        }
    }
    else RegError_Get();

    /* Get ThreadBoost Value From Registry */
    if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"ThreadBoost",ThreadBoost))
    {
        if(ThreadBoost == 0)
            CheckDlgButton(MainDialog_hWnd, 3000, BST_UNCHECKED);
        if(ThreadBoost == 1)
            CheckDlgButton(MainDialog_hWnd, 3000, BST_CHECKED);

    }
    else RegError_Get();

    /* Get ProcessBoost Value From Registry */
    if(Get_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"ProcessBoost",ProcessBoost))
    {
        if(ProcessBoost == 0)
            CheckDlgButton(MainDialog_hWnd, 4000, BST_UNCHECKED);
        if(ProcessBoost == 1)
            CheckDlgButton(MainDialog_hWnd, 4000, BST_CHECKED);

    }
    else RegError_Get();

    return 0;
}

/////////////////////////////////////////////////////////////////////

int SetValues(void)
{
    BOOL HighOrRealTime = FALSE;

    //---------------------------------------------

    if(IsDlgButtonChecked(MainDialog_hWnd,1003))
        HighOrRealTime = TRUE;
    if(IsDlgButtonChecked(MainDialog_hWnd,1004))
        HighOrRealTime = TRUE;
    if(IsDlgButtonChecked(MainDialog_hWnd,2006))
        HighOrRealTime = TRUE;
    if(IsDlgButtonChecked(MainDialog_hWnd,2007))
        HighOrRealTime = TRUE;

    if(HighOrRealTime)
    {
        if(MessageBox(MainDialog_hWnd,"High, RealTime Or TimeCritical Mode Severly Cripples \r\nWindows Performance While Using 448Delta. \r\nSave The Following Changes?",TITLECAPTION,4+256+MB_ICONQUESTION + MB_SYSTEMMODAL) == IDNO)
        {
            return 0;
        }
    }

    //---------------------------------------------

    if(IsDlgButtonChecked(MainDialog_hWnd,1001))
    {
        if(!Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"Process",1))
            RegError_Set();
    }

    if(IsDlgButtonChecked(MainDialog_hWnd,1002))
    {
        if(!Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"Process",2))
            RegError_Set();
    }

    if(IsDlgButtonChecked(MainDialog_hWnd,1003))
    {
        if(!Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"Process",3))
            RegError_Set();
    }

    if(IsDlgButtonChecked(MainDialog_hWnd,1004))
    {
        if(!Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"Process",4))
            RegError_Set();
    }

    //---------------------------------------------

    if(IsDlgButtonChecked(MainDialog_hWnd,2001))
    {
        if(!Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"Thread",1))
            RegError_Set();
    }

    if(IsDlgButtonChecked(MainDialog_hWnd,2002))
    {
        if(!Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"Thread",2))
            RegError_Set();
    }

    if(IsDlgButtonChecked(MainDialog_hWnd,2003))
    {
        if(!Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"Thread",3))
            RegError_Set();
    }

    if(IsDlgButtonChecked(MainDialog_hWnd,2004))
    {
        if(!Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"Thread",4))
            RegError_Set();
    }

    if(IsDlgButtonChecked(MainDialog_hWnd,2005))
    {
        if(!Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"Thread",5))
            RegError_Set();
    }

    if(IsDlgButtonChecked(MainDialog_hWnd,2006))
    {
        if(!Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"Thread",6))
            RegError_Set();
    }

    if(IsDlgButtonChecked(MainDialog_hWnd,2007))
    {
        if(!Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"Thread",7))
            RegError_Set();
    }

    //---------------------------------------------

    if(IsDlgButtonChecked(MainDialog_hWnd,3000))
    {
        if(!Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"ThreadBoost",1))
            RegError_Set();
    }
    else{
        if(!Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"ThreadBoost",0))
            RegError_Set();
    }

    if(IsDlgButtonChecked(MainDialog_hWnd,4000))
    {
        if(!Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"ProcessBoost",1))
            RegError_Set();
    }
    else{
        if(!Set_DWORDRegistryValue(HKEY_LOCAL_MACHINE,PriorityKey,"ProcessBoost",0))
            RegError_Set();
    }

    //---------------------------------------------

    return 1;
}

/////////////////////////////////////////////////////////////////////

