//---------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////

int BlowFishDecode(char *SourceName, char *TargetName, char *Key)
{
    unsigned long L = 1, R = 3, RemLong, FileSize, Process_FileSize;
    FILE *Source, *Target;
    DWORD i;

    // Create BlowFish object from blowfish.cpp
    BF_BlowFish((unsigned char*)Key, strlen(Key));

    //Open Files
    if((Source = fopen(SourceName,"rb")) == NULL)
    {
        char TempText[1024];
        sprintf(TempText, "BlowFishDecode: Could not open %s for read.", SourceName);
        Error(TempText);

        if(Source)fclose(Source);
        return 1;
    }

    // Seek to end of file
    fseek(Source,0,SEEK_END);

    // Get byte position (File Size)
    FileSize = ftell(Source) - ExecutableEnd;

    // Calculate how many sets of 8 for encryption loop
    Process_FileSize = (FileSize/sizeof(L))/2;

    // Seek back to the beginning
    fseek(Source,ExecutableEnd+FLAGLENGHT+FILEHEADERLEN,SEEK_SET); // Seek Past Executable

    //Write and read header this allows the coder to check the key
    //before decryption the file

    Process_FileSize--;

    // Read and compare header <nept35>
    fread((char *)&L, sizeof(L), 1, Source);
    fread((char *)&R, sizeof(R), 1, Source);

    //Decrypt header
    BF_Decrypt(&L, &R);

    /* Compare if decrypted correctly,
    If not so tell user wrong key */

    if (L != 15942653 || R != 18753826)
    {
        SetText(StatusText_hWnd,"Wrong Key, Can't Decrypt");
        if(Source)fclose(Source);
        return 3;
    }

    if((Target = fopen(TargetName,"wb")) == NULL)
    {
        char TempText[1024];
        sprintf(TempText, "BlowFishDecode: Could not open %s for write.", TargetName);
        Error(TempText);

        if(Source)fclose(Source);
        if(Target)fclose(Target);
        return 4;
    }

    // Because header does not count suptract one
    Process_FileSize--;

    // Read second header containing Rem
    fread((char *)&RemLong, 4, 1, Source);

    unsigned long CurrentByte;
    long double Current_Size;
    DWORD SourceFileSize;
    DWORD EndOfFile;

    CurrentByte = ftell(Source);
    fseek(Source,0,SEEK_END);
    EndOfFile = ftell(Source);
    fseek(Source,CurrentByte,SEEK_SET);
    SourceFileSize = EndOfFile - CurrentByte;
    Current_Size = 0;

    DWORD PrintCount = 0;

    int ExtraSalt = 1;
    bool ExtraSwap = false;

    // Begin encryption
    for(i=0; i < Process_FileSize; i++)
    {
        Current_Size += 8;
        PrintCount +=8;
        if(PrintCount == 1024)
        {
            StatusValue = (DWORD) ((Current_Size / SourceFileSize) * 100);
            PrintCount = 0;
        }

        if(CancelFlag)
        {
            if(Source)fclose(Source);
            if(Target)fclose(Target);
            return 5;
        }

        // Read 8 bytes from file
        fread((char *)&L, sizeof(L), 1, Source);
        fread((char *)&R, sizeof(R), 1, Source);

        BF_Decrypt(&L, &R);

        if(ExtraSwap)
        {
            L += ExtraSalt;
            R -= ExtraSalt;
            ExtraSwap = false;
        }
        else{
            L -= ExtraSalt;
            R += ExtraSalt;
            ExtraSwap = true;
        }

        // Write 8 bytes to file
        if(i==Process_FileSize-1)
        {
            if (RemLong < 4)
            {
                fwrite((char *)&L, (RemLong), 1, Target);
            }
            else {
                fwrite((char *)&L, sizeof(L), 1, Target);
                fwrite((char *)&R, sizeof(R) - (8-RemLong), 1, Target);
            }
        }
        else {
            fwrite((char *)&L, sizeof(L), 1, Target);
            fwrite((char *)&R, sizeof(R), 1, Target);
        }

        if(ExtraSalt == 1324)
            ExtraSalt = 0;
        ExtraSalt++;
    }

    //close files
    if(Source)fclose(Source);
    if(Target)fclose(Target);

    Sleep(25);
    SendMessage(StatusBar_hWnd, PBM_SETPOS, 100, 0);

    return 0;
}
