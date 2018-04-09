///////////////////////////////////////////////////////////////////////////////////////////

BOOL DestroyTemps(void)
{
    FILE *FileHandle1 = NULL;
    FILE *FileHandle2 = NULL;

    DWORD FileSize1  = Get_FileSize(PI.TempFile1);
    DWORD FileSize2  = Get_FileSize(PI.TempFile2);

    float Current   = 0;
    int StatusCount = 0;

    DWORD LocalLoop = 0;

    FileHandle1 = fopen(PI.TempFile1, "wb");
    FileHandle2 = fopen(PI.TempFile2, "wb");

    if(FileHandle1 && (FileSize1 != -1))
    {
        LocalLoop = 0;

        while(LocalLoop < FileSize1)
        {
            if(StatusCount == 10240)
            {
                Current = (float) LocalLoop;
                PI.StatusValue = (Current / FileSize1) * 50;
                StatusCount = 0;
            }
            StatusCount++;

            fputc(0, FileHandle1);
            LocalLoop++;
        }
    }

    if(FileHandle1) fclose(FileHandle1);
    DeleteFile(PI.TempFile1);

    StatusCount = 0;

    if(FileHandle2 && (FileSize2 != -1) )
    {
        LocalLoop = 0;
        while(LocalLoop < FileSize2)
        {
            if(StatusCount == 10240)
            {
                Current = (float) LocalLoop;
                PI.StatusValue = ((Current / FileSize2) * 50) + 50;
                StatusCount = 0;
            }
            StatusCount++;

            fputc(0, FileHandle2);
            LocalLoop++;
        }
    }

    if(FileHandle2) fclose(FileHandle2);
    DeleteFile(PI.TempFile2);

    Sleep(25);
    SendMessage(StatusBar_hWnd, PBM_SETPOS, 100, 0);

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////

