#ifndef NetWorkDLS_LZARI_Data_Compression_Compression
#define NetWorkDLS_LZARI_Data_Compression_Compression
///////////////////////////////////////////////////////////////////////////////////////////

#include "Headers/Compression.H"

int LZARICompress(char *SourceFileName, char *TargetFileName)
{
    char TempText[1024];

    int  i=0, c=0, len=0, r=0, s=0, Last_MatchLength = 0;
    long int PrintCount = 0, TextSize = 0;

    DWORD SourceFileSize = 0;
    long double Current_Size = 0;

    Initialize();

    if((CompressSource = fopen(SourceFileName,"rb")) == NULL)
    {
        sprintf(TempText, "Compress: could not open \"%s\" for read.", SourceFileName);
        Error(StatusDialog_hWnd, TempText);

        if(CompressSource) fclose(CompressSource);
        return 0;
    }

    if((CompressTarget = fopen(TargetFileName,"wb")) == NULL)
    {
        sprintf(TempText, "Compress: could not open \"%s\" for write.", TargetFileName);
        Error(StatusDialog_hWnd, TempText);

        if(CompressSource) fclose(CompressSource);
        if(CompressTarget) fclose(CompressTarget);
        return 0;
    }

    TextSize       = Get_FileSize(SourceFileName);
    SourceFileSize = TextSize;

    if(TextSize == 0)
    {
        Error(StatusDialog_hWnd, "Compress: File size is zero");
        return 0;
    }

    /* output size of text */
    if(fwrite(&TextSize, sizeof(TextSize), 1, CompressTarget) < 1)
    {
        Error(StatusDialog_hWnd, "Compress: File write error.");
        if(CompressSource) fclose(CompressSource);
        if(CompressTarget) fclose(CompressTarget);
        return 0;
    }

    TextSize = 0;

    StartModel();
    InitTree();

    s = 0;
    r = SizeOfRingBuffer - MatchLengthLimit;

    for (i = s; i < r; i++) TextBuffer[i] = '\0';

    for (len = 0; len < MatchLengthLimit && (c = getc(CompressSource)) != EOF; len++)
    {
        TextBuffer[r + len] = c;
        Current_Size++; // Compensate For File Size
        PI.StatusValue = ((Current_Size / SourceFileSize) * 100);
    }

    TextSize = len;

    for (i = 1; i <= MatchLengthLimit; i++) InsertNode(r - i);
    InsertNode(r);

    do
    {
        if (MatchLength > len) MatchLength = len;

        if (MatchLength <= THRESHOLD)
        {
            MatchLength = 1;
            EncodeChar(TextBuffer[r]);
        }
        else{
            EncodeChar(255 - THRESHOLD + MatchLength);
            EncodePosition(MatchPosition - 1);
        }

        Last_MatchLength = MatchLength;

        for (i = 0; i < Last_MatchLength && (c = getc(CompressSource)) != EOF; i++)
        {
            Current_Size++; // Compensate For File Size

            DeleteNode(s);
            TextBuffer[s] = c;
            if (s < MatchLengthLimit - 1) TextBuffer[s + SizeOfRingBuffer] = c;
            s = (s + 1) & (SizeOfRingBuffer - 1);
            r = (r + 1) & (SizeOfRingBuffer - 1);
            InsertNode(r);
        }

        PrintCount++;
        if(PrintCount == 10240)
        {
            PI.StatusValue = ((Current_Size / SourceFileSize) * 100);
            PrintCount = 0;
        }

        while (i++ < Last_MatchLength)
        {
            DeleteNode(s);
            s = (s + 1) & (SizeOfRingBuffer - 1);
            r = (r + 1) & (SizeOfRingBuffer - 1);
            if (--len) InsertNode(r);
        }

        if(PI.CancelFlag == TRUE)
        {
            if(CompressSource)fclose(CompressSource);
            if(CompressTarget)fclose(CompressTarget);
            return 1;
        }
    }
    while(len > 0); // End Of Do Loop

    EncodeEnd();

    if(CompressSource) fclose(CompressSource);
    if(CompressTarget) fclose(CompressTarget);

    Sleep(25);
    SendMessage(StatusBar_hWnd, PBM_SETPOS, 100, 0);

    return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////
#endif

