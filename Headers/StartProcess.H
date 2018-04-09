// BOOL StartProcess(void);
// DWORD WINAPI StartProcess_Thread(LPVOID xData)

///////////////////////////////////////////////////////////////////////////////////////////

int StartProcess(void)
{
    GetTempFileName(Get_TempDirectory(), "448", 0, PI.TempFile1);
    GetTempFileName(Get_TempDirectory(), "448", 0, PI.TempFile2);

    int Result       = 0;
    float FileSize   = 0;

    /*  PI.PreCheckResult
        0: Error Occured
        1: Directory
        2: Encrypted File
        3: Packed, Encrypted Directory
        4: Compressed, Encrypted File
        5: Packed, Compressed, Encrypted Directory
        6: Non-448Delta File (Unencrypted)
    */

    /*  PI.FileFlagResult
        1: Pack, Encrypt Directory
        2: Pack, Compress, Encrypt Directory
        3: Encrypt File
        4: Compressed, Encrypted File
    */

    Sleep(500); // Try To Give The Status Dialog A Chance To Appear

    //------------------------------------------------------------------------------------

    if( !SetPriority())
        return 0;

    if(PI.PreCheckResult == 1) // Directory
    {
        Result = PackFiles(PI.GlobalSource, PI.TempFile1);
        if(!Result)
        {
            PI.ErrorFlag = TRUE;
            strcpy(PI.AfterText, FINISHEDWE);
            return 0;
        }

        if(PI.CancelFlag == TRUE)
        {
            PI.CompleteFlag = TRUE;
            return 0;
        }

        if(PI.CompressOption == TRUE)
        {
            PI.FileFlagResult = 2;

            FileSize = (float) Get_FileSize(PI.TempFile1);
            sprintf(PI.StatusText,"Compressing: %.3f KB", (float) FileSize / 1024 );

            Result = LZARICompress(PI.TempFile1, PI.TempFile2);
            if(!Result)
            {
                PI.ErrorFlag = TRUE;
                strcpy(PI.AfterText, FINISHEDWE);
                return 0;
            }

            if(PI.CancelFlag == TRUE)
            {
                PI.CompleteFlag = TRUE;
                return 0;
            }

            FileSize = (float) Get_FileSize(PI.TempFile2);
            sprintf(PI.StatusText,"Encrypting: %.3f KB", (float) FileSize / 1024 );

            Result = BlowFishEncode(PI.TempFile2, PI.GlobalTarget, PI.Password);
            if(!Result)
            {
                PI.ErrorFlag = TRUE;
                strcpy(PI.AfterText, FINISHEDWE);
                return 0;
            }

            if(PI.CancelFlag == TRUE)
            {
                PI.CompleteFlag = TRUE;
                return 0;
            }

            PI.CompleteFlag = TRUE;
            strcpy(PI.AfterText, "Data Successfully Packaged!");
            return 1;
        }

        if(PI.CompressOption == FALSE)
        {
            PI.FileFlagResult = 1;

            FileSize = (float) Get_FileSize(PI.TempFile1);
            sprintf(PI.StatusText,"Encrypting: %.3f KB", (float) FileSize / 1024 );

            Result = BlowFishEncode(PI.TempFile1, PI.GlobalTarget, PI.Password);
            if(!Result)
            {
                PI.ErrorFlag = TRUE;
                strcpy(PI.AfterText, FINISHEDWE);
                return 0;
            }

            if(PI.CancelFlag == TRUE)
            {
                PI.CompleteFlag = TRUE;
                return 0;
            }

            PI.CompleteFlag = TRUE;
            strcpy(PI.AfterText, "Data Successfully Packaged!");
            return 1;
        }

        PI.CompleteFlag = TRUE;
        return 0;
    }

    //------------------------------------------------------------------------------------

    if(PI.PreCheckResult == 2) // Encrypted File
    {
        FileSize = (float) Get_FileSize(PI.GlobalSource);
        sprintf(PI.StatusText,"Decrypting: %.3f KB", (float) FileSize / 1024 );
        
        Result = BlowFishDecode(PI.GlobalSource, PI.GlobalTarget, PI.Password);
        if(!Result)
        {
            PI.ErrorFlag = TRUE;
            strcpy(PI.AfterText, FINISHEDWE);
            return 0;
        }

        if(PI.CancelFlag == TRUE)
        {
            PI.CompleteFlag = TRUE;
            return 0;
        }

        if(Result == 2)
        {
            PI.CancelFlag = TRUE;
            PI.ErrorFlag = TRUE;
            PI.CompleteFlag = TRUE;
            strcpy(PI.AfterText, WRONGPWCD);
            return 2;
        }

        PI.CompleteFlag = TRUE;
        strcpy(PI.AfterText, "Data Successfully Decrypted!");
        return 1;
    }

    //------------------------------------------------------------------------------------

    if(PI.PreCheckResult == 3) // Packed, Encrypted Directory
    {
        FileSize = (float) Get_FileSize(PI.GlobalSource);
        sprintf(PI.StatusText,"Decrypting: %.3f KB", (float) FileSize / 1024 );
        
        Result = BlowFishDecode(PI.GlobalSource, PI.TempFile1, PI.Password);
        if(!Result)
        {
            PI.ErrorFlag = TRUE;
            strcpy(PI.AfterText, FINISHEDWE);
            return 0;
        }

        if(PI.CancelFlag == TRUE)
        {
            PI.CompleteFlag = TRUE;
            return 0;
        }

        if(Result == 2)
        {
            PI.CancelFlag = TRUE;
            PI.ErrorFlag = TRUE;
            PI.CompleteFlag = TRUE;
            strcpy(PI.AfterText, WRONGPWCD);
            return 2;
        }

        FileSize = (float) Get_FileSize(PI.TempFile1);
        sprintf(PI.StatusText,"Unpacking: %.3f KB", (float) FileSize / 1024 );
        
        Result = UnPackFiles(PI.TempFile1, PI.GlobalTarget);
        if(!Result)
        {
            PI.ErrorFlag = TRUE;
            strcpy(PI.AfterText, FINISHEDWE);
            return 0;
        }

        if(PI.CancelFlag == TRUE)
        {
            PI.CompleteFlag = TRUE;
            return 0;
        }

        PI.CompleteFlag = TRUE;
        strcpy(PI.AfterText, "Data Successfully Unpackaged!");
        return 1;
    }

    //------------------------------------------------------------------------------------

    if(PI.PreCheckResult == 4) // Compressed, Encrypted File
    {
        FileSize = (float) Get_FileSize(PI.GlobalSource);
        sprintf(PI.StatusText,"Decrypting: %.3f KB", (float) FileSize / 1024 );
        
        Result = BlowFishDecode(PI.GlobalSource, PI.TempFile1, PI.Password);
        if(!Result)
        {
            PI.ErrorFlag = TRUE;
            strcpy(PI.AfterText, FINISHEDWE);
            return 0;
        }

        if(PI.CancelFlag == TRUE)
        {
            PI.CompleteFlag = TRUE;
            return 0;
        }

        if(Result == 2)
        {
            PI.CancelFlag = TRUE;
            PI.ErrorFlag = TRUE;
            PI.CompleteFlag = TRUE;
            strcpy(PI.AfterText, WRONGPWCD);
            return 2;
        }

        FileSize = (float) Get_FileSize(PI.TempFile1);
        sprintf(PI.StatusText,"Decompressing: %.3f KB", (float) FileSize / 1024 );
        
        Result = LZARIDecompress(PI.TempFile1, PI.GlobalTarget);
        if(!Result)
        {
            PI.ErrorFlag = TRUE;
            strcpy(PI.AfterText, FINISHEDWE);
            return 0;
        }

        if(PI.CancelFlag == TRUE)
        {
            PI.CompleteFlag = TRUE;
            return 0;
        }

        PI.CompleteFlag = TRUE;
        strcpy(PI.AfterText, "Data Successfully Decrypted!");
        return 1;
    }

    //------------------------------------------------------------------------------------

    if(PI.PreCheckResult == 5) // Packed, Compressed, Encrypted Directory
    {
        FileSize = (float) Get_FileSize(PI.GlobalSource);
        sprintf(PI.StatusText,"Decrypting: %.3f KB", (float) FileSize / 1024 );

        Result = BlowFishDecode(PI.GlobalSource, PI.TempFile1, PI.Password);
        if(!Result)
        {
            PI.ErrorFlag = TRUE;
            strcpy(PI.AfterText, FINISHEDWE);
            return 0;
        }

        if(PI.CancelFlag == TRUE)
        {
            PI.CompleteFlag = TRUE;
            return 0;
        }

        if(Result == 2)
        {
            PI.CancelFlag = TRUE;
            PI.ErrorFlag = TRUE;
            PI.CompleteFlag = TRUE;
            strcpy(PI.AfterText, WRONGPWCD);
            return 2;
        }

        FileSize = (float) Get_FileSize(PI.TempFile1);
        sprintf(PI.StatusText,"Decompressing: %.3f KB", (float) FileSize / 1024 );
        
        Result = LZARIDecompress(PI.TempFile1, PI.TempFile2);
        if(!Result)
        {
            PI.ErrorFlag = TRUE;
            strcpy(PI.AfterText, FINISHEDWE);
            return 0;
        }

        if(PI.CancelFlag == TRUE)
        {
            PI.CompleteFlag = TRUE;
            return 0;
        }

        FileSize = (float) Get_FileSize(PI.TempFile2);
        sprintf(PI.StatusText,"Unpacking: %.3f KB", (float) FileSize / 1024 );
        
        Result = UnPackFiles(PI.TempFile2, PI.GlobalTarget);
        if(!Result)
        {
            PI.ErrorFlag = TRUE;
            strcpy(PI.AfterText, FINISHEDWE);
            return 0;
        }

        if(PI.CancelFlag == TRUE)
        {
            PI.CompleteFlag = TRUE;
            return 0;
        }

        PI.CompleteFlag = TRUE;
        strcpy(PI.AfterText, "Data Successfully Unpackaged!");
        return 1;
    }

    //------------------------------------------------------------------------------------

    if(PI.PreCheckResult == 6) // Non-448Delta File (Unencrypted)
    {
        if(PI.CompressOption == TRUE)
        {
            PI.FileFlagResult = 4;

            FileSize = (float) Get_FileSize(PI.GlobalSource);
            sprintf(PI.StatusText,"Compressing: %.3f KB", (float) FileSize / 1024 );
            
            Result = LZARICompress(PI.GlobalSource, PI.TempFile1);
            if(!Result)
            {
                PI.ErrorFlag = TRUE;
                strcpy(PI.AfterText, FINISHEDWE);
                return 0;
            }

            if(PI.CancelFlag == TRUE)
            {
                PI.CompleteFlag = TRUE;
                return 0;
            }

            FileSize = (float) Get_FileSize(PI.TempFile1);
            sprintf(PI.StatusText,"Encrypting: %.3f KB", (float) FileSize / 1024 );

            Result = BlowFishEncode(PI.TempFile1, PI.GlobalTarget, PI.Password);
            if(!Result)
            {
                PI.ErrorFlag = TRUE;
                strcpy(PI.AfterText, FINISHEDWE);
                return 0;
            }

            if(PI.CancelFlag == TRUE)
            {
                PI.CompleteFlag = TRUE;
                return 0;
            }

            PI.CompleteFlag = TRUE;
            strcpy(PI.AfterText, "Data Successfully Encrypted!");
            return 1;
        }

        if(PI.CompressOption == FALSE)
        {
            PI.FileFlagResult = 3;

            FileSize = (float) Get_FileSize(PI.GlobalSource);
            sprintf(PI.StatusText,"Encrypting: %.3f KB", (float) FileSize / 1024 );
            
            Result = BlowFishEncode(PI.GlobalSource, PI.GlobalTarget, PI.Password);
            if(!Result)
            {
                PI.ErrorFlag = TRUE;
                strcpy(PI.AfterText, FINISHEDWE);
                return 0;
            }

            if(PI.CancelFlag == TRUE)
            {
                PI.CompleteFlag = TRUE;
                return 0;
            }

            PI.CompleteFlag = TRUE;
            strcpy(PI.AfterText, "Data Successfully Encrypted!");
            return 1;
        }

        PI.CompleteFlag = TRUE;
        return 0;
    }

    //------------------------------------------------------------------------------------

    PI.CompleteFlag = TRUE;
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////

DWORD WINAPI StartProcess_Thread(LPVOID xData)
{
    int StartProcessResult = StartProcess();

    EnableWindow(PI.OKCancelButton_hWnd, FALSE);

    if(PI.DestroyTempOption)
    {
        strcpy(PI.StatusText, "Destroying Temp Files...");
        DestroyTemps();
        Sleep(150);
    }

    if(PI.CancelFlag || PI.ErrorFlag)
    {
        BOOL DeteteTarget = TRUE;

        ClearMem(PI.StatusText, 1024);
        Set_Text(PI.StatusText_hWnd, "Deleting Target...");


        if(StartProcessResult == 2) // The password was incorrect
        {
            DeteteTarget = FALSE;
        }
        else if(PI.ProcededWithOverwrite && PI.CancelFlag)
        {
            if(MessageBox(StatusDialog_hWnd, "The process was canceled while overwriting pre-exesting file(s). Would you like to delete the pre-exesting file(s)?" , gsTitleCaption, MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDNO)
                DeteteTarget = FALSE;
        }
        else if(PI.ProcededWithOverwrite && PI.ErrorFlag)
        {
            if(MessageBox(StatusDialog_hWnd, "The process encountered an error while overwriting pre-exesting file(s). Would you like to delete the pre-exesting file(s)?", gsTitleCaption, MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDNO)
                DeteteTarget = FALSE;
        }

        if(DeteteTarget)
            xDeleteObject(PI.GlobalTarget);

        Sleep(150);
        if(PI.CancelFlag) Set_Text(PI.StatusText_hWnd, "Canceled...");
        Sleep(150);
    }

    if(PI.DeleteSourceOption && !PI.CancelFlag && !PI.ErrorFlag )
    {
        Set_Text(PI.StatusText_hWnd, "Deleting Source File(s)...");
        xDeleteObject(PI.GlobalSource);
        Sleep(150);
    }

    if(PI.AfterText[0] != '\0') Set_Text(PI.StatusText_hWnd, PI.AfterText);

    Sleep(150);

    DeleteFile(PI.TempFile1);
    DeleteFile(PI.TempFile2);

    PI.CompleteFlag = TRUE;
    Set_Text(PI.OKCancelButton_hWnd, "OK");
    EnableWindow(PI.OKCancelButton_hWnd, TRUE);
    KillTimer(StatusDialog_hWnd,0);

    MessageBeep(MB_ICONASTERISK);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////

