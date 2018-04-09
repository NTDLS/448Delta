///////////////////////////////////////////////////////////////////////

bool WriteSelfExtractCode(FILE *TargetHandle)
{
    FILE *SourceHandle = NULL;

    char LocalBuffer[BufferSize + 1];

    char GammaPath[256];

    int CurrentBuffer = 0;

    unsigned long SizeOfGammaPath = sizeof(GammaPath);

    if(Get_StringRegistryValue(HKEY_LOCAL_MACHINE, PathKey, "SelfExtract", GammaPath, SizeOfGammaPath))
    {
        if(!FileAccess(GammaPath, FExist))
        {
            Error(StatusDialog_hWnd, "WriteSelfExtractCode: Executable template not found.");
            return false;
        }
    }
    else{
        Error(StatusDialog_hWnd, "WriteSelfExtractCode: Executable template location key not found.");
        return false;
    }

    if((SourceHandle = fopen(GammaPath, "rb")) == NULL)
    {
        Error(StatusDialog_hWnd, "WriteSelfExtractCode: Could not open executable template.");
        return false;
    }

    do
    {
        CurrentBuffer = fread(LocalBuffer, sizeof(char), BufferSize, SourceHandle);
        fwrite(LocalBuffer, sizeof(char), CurrentBuffer, TargetHandle);
    }
    while(CurrentBuffer == BufferSize);

    return true;
}

///////////////////////////////////////////////////////////////////////

int BlowFishEncode(char *SourceName,char *TargetName, char *Key)
{
    char TempText[1024];

    unsigned long L = 1, R = 3, RemLong = 0, FileSize = 0;
    FILE *Source = NULL, *Target = NULL;

    double Process_FileSize, EncodeLoop;
    int StatusCount   = 0;

    // Create BlowFish Object
    BF_BlowFish((unsigned char*)Key, strlen(Key));

    FileSize = Get_FileSize(SourceName);

    if (FileSize == 0)
    {
        sprintf(TempText, "BlowFishEncode: \"%s\" contains no data.", SourceName);
        Error(StatusDialog_hWnd, TempText);

        if(Source)fclose(Source);
        return 0;
    }

    /* RemLong Is The Ammount Of Bits Added To The End Of The Encrypted
    File To Make The FileSize mod 8 = 0, These Bits Are Later Removed */
    RemLong = (FileSize % 8);

    //Open Files
    if((Source = fopen(SourceName,"rb")) == NULL)
    {
        sprintf(TempText, "BlowFishEncode: Could not open \"%s\" for read.", SourceName);
        Error(StatusDialog_hWnd, TempText);

        if(Source)fclose(Source);
        return 0;
    }

    // Seek To End Of File
    fseek(Source, 0, SEEK_END);

    // Get byte position (File Size)
    FileSize = ftell(Source);

    // Calculate How Many Sets Of 8 For Encryption Loop
    Process_FileSize = (FileSize/sizeof(L))/2;

    // Seek Back To The Beginning
    fseek(Source, 0, SEEK_SET);

    //Write And Read Header This Allows The Coder To Check The Key
    //Before Decryption The File
    if((Target = fopen(TargetName,"wb")) == NULL)
    {
        sprintf(TempText, "BlowFishEncode: Could not open \"%s\" for write.", TargetName);
        Error(StatusDialog_hWnd, TempText);

        if(Source)fclose(Source);
        if(Target)fclose(Target);
        return 0;
    }

    /*  PI.FileFlagResult
        1: Pack, Encrypt Directory
        2: Pack, Compress, Encrypt Directory
        3: Encrypt File
        4: Compressed, Encrypted File
    */

    if(PI.CreateSelfOption)
    {
        if(!WriteSelfExtractCode(Target))
        {
            return 0;
        }
    }

    fwrite(FILEHEADER, FILEHEADERLEN, sizeof(char), Target);

    DWORD MethodFlag = 0;

    if(PI.FileFlagResult == 1){
        MethodFlag = PACFlag;
    }
    else if(PI.FileFlagResult == 2){
        MethodFlag = PACCompress;
    }
    else if(PI.FileFlagResult == 3){
        MethodFlag = ENCFlag;
    }
    else if(PI.FileFlagResult == 4){
        MethodFlag = ENCCompress;
    }
    else{
        char ErrorTemp[256];
		sprintf(ErrorTemp,"BlowFishEncode: Invalid FileFlagResult: %d.", PI.FileFlagResult);
        Error(StatusDialog_hWnd, ErrorTemp);
        return 0;
    }

    fwrite(&MethodFlag, FLAGLENGHT, sizeof(char), Target);

    //Write Header 84082311328048 128-Bit
    //On Decryption This Is Decrypted Using The Provided Key
    //And Checked Against Value 84082311328048 To See If The
    //Correct Key Was Used

    L = 15942653;
    R = 18753826;

    //Encrypt Header
    BF_Encrypt(&L, &R);

    //Write Header
    fwrite((char *)&L, sizeof(L), 1, Target);
    fwrite((char *)&R, sizeof(R), 1, Target);

    // Write Header Containing The Number Of Bits Added (Rem)
    fwrite((char *)&RemLong, 4, 1, Target);

    Process_FileSize++; // Add 1 To Process_FileSize

    int ExtraSalt = 1;
    bool ExtraSwap = true;

    // Begin Encryption / Decryption
    for(EncodeLoop = 0; EncodeLoop < Process_FileSize; EncodeLoop++)
    {
        // Read 8 Bytes From File
        fread((char *)&L, sizeof(L), 1, Source);
        fread((char *)&R, sizeof(R), 1, Source);

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

        // Call blowfish.cpp Functions
        BF_Encrypt(&L, &R);

        // Write 8 Bytes To File
        fwrite((char *)&L, sizeof(L), 1, Target);
        fwrite((char *)&R, sizeof(R), 1, Target);

        StatusCount++;
        if(StatusCount == 10240)
        {
            PI.StatusValue = (EncodeLoop / (Process_FileSize - 1)) * 100;
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
