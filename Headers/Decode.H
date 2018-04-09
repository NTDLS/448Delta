///////////////////////////////////////////////////////////////////////

int BlowFishDecode(char *SourceName,char *TargetName, char *Key)
{
    char TempText[1024];

    unsigned long L = 1, R = 3, RemLong = 0, FileSize = 0;
    FILE *Source = NULL, *Target = NULL;

    double Process_FileSize, DecodeLoop;
    int StatusCount   = 0;

    // Create BlowFish Object
    BF_BlowFish((unsigned char*)Key, strlen(Key));

    FileSize = Get_FileSize(SourceName) - (FLAGLENGHT - FILEHEADERLEN);

    if((Source = fopen(SourceName,"rb")) == NULL)
    {
        sprintf(TempText, "BlowFishDecode: Could not open \"%s\" for read.", SourceName);
        Error(StatusDialog_hWnd, TempText);

        if(Source)fclose(Source);
        return 0;
    }

    // Calculate How Many Sets Of 8 For Encryption Loop
    Process_FileSize = ( FileSize/sizeof(L) ) / 2;

    // Seek Back To The Beginning, Past Flag(s)
    fseek(Source, FILEHEADERLEN + FLAGLENGHT, SEEK_SET);

    // Read And Compare Header <nept35>
    fread((char *)&L, sizeof(L), 1, Source);
    fread((char *)&R, sizeof(R), 1, Source);

    //Decrypt Header
    BF_Decrypt(&L, &R);

    /* Compare If Decrypted Correctly,
    If Not Tell User Wrong Key */

    if (L != 15942653 || R != 18753826)
    {
        if(Source) fclose(Source);
        if(Target) fclose(Target);
        return 2;
    }

    // Set file attributes to normal to avoid errors
    SetFileAttributes(TargetName, FILE_ATTRIBUTE_NORMAL);

    if((Target = fopen(TargetName,"wb")) == NULL)
    {
        sprintf(TempText, "BlowFishDecode: Could not open \"%s\" for write.", TargetName);
        Error(StatusDialog_hWnd, TempText);

        if(Source)fclose(Source);
        if(Target)fclose(Target);
        return 0;
    }

    // Because Header Does Not Count Suptract One
    Process_FileSize--;

    // Read Second Header Containing Rem
    fread((char *)&RemLong, 4, 1, Source);

    int ExtraSalt = 1;
    bool ExtraSwap = false;

    // Begin Encryption / Decryption
    for(DecodeLoop = 0; DecodeLoop < Process_FileSize; DecodeLoop++)
    {
        // Read 8 Bytes From File
        fread((char *)&L, sizeof(L), 1, Source);
        fread((char *)&R, sizeof(R), 1, Source);

        // Call BlowFish Functions
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

        // Write 8 Bytes To File
        if( DecodeLoop == (Process_FileSize - 1) )
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

        StatusCount++;
        if(StatusCount == 10240)
        {
            PI.StatusValue = (DecodeLoop/(Process_FileSize-1)) * 100;
            StatusCount = 0;
        }

        if(PI.CancelFlag == TRUE)
        {
            if(Source)fclose(Source);
            if(Target)fclose(Target);
            return 1;
        }

        if(ExtraSalt == 1324)
        {
            ExtraSalt = 0;
        }

        ExtraSalt++;
    }

    //Close Files
    if(Source)fclose(Source);
    if(Target)fclose(Target);

    Sleep(25);
    SendMessage(StatusBar_hWnd, PBM_SETPOS, 100, 0);

    return 1;
}
