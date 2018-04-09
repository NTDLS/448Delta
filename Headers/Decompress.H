#ifndef NetWorkDLS_LZARI_Data_Compression_Decompression
#define NetWorkDLS_LZARI_Data_Compression_Decompression
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Headers/Compression.H"

int LZARIDecompress(char *SourceFileName, char *TargetFileName)
{
    char TempText[1024];

    int  i=0, j=0, k=0, r=0, c=0;
    long int PrintCount = 0, TextSize = 0;

    DWORD SourceFileSize = 0;
    long double Current_Size = 0;

    Initialize();

    if((CompressSource = fopen(SourceFileName,"rb")) == NULL)
    {
        sprintf(TempText, "Decompress: could not open \"%s\" for read.", SourceFileName);
        Error(StatusDialog_hWnd, TempText);

        if(CompressSource) fclose(CompressSource);
        return 0;
    }

    if((CompressTarget = fopen(TargetFileName,"wb")) == NULL)
    {
        sprintf(TempText, "Decompress: could not open \"%s\" for write.", TargetFileName);
        Error(StatusDialog_hWnd, TempText);

        if(CompressSource) fclose(CompressSource);
        if(CompressTarget) fclose(CompressTarget);
        return 0;
    }

    /* read size of text */
    if(fread(&TextSize, sizeof(TextSize), 1, CompressSource) < 1)
    {
        Error(StatusDialog_hWnd, "Decompress: File read error.");
        return 0;
    }

    if(TextSize == 0)
    {
        Error(StatusDialog_hWnd, "Decompress: Incorrect image size.");
        return 0;
    }

    SourceFileSize = TextSize;

    StartDecode();
    StartModel();

    for(i = 0; i < SizeOfRingBuffer - MatchLengthLimit; i++) TextBuffer[i] = '\0';

    r = SizeOfRingBuffer - MatchLengthLimit;

    for(Current_Size = 0; Current_Size < TextSize; )
    {
        c = DecodeChar();
        if (c < 256)
        {
            putc(c, CompressTarget);
            TextBuffer[r++] = c;
            r &= (SizeOfRingBuffer - 1);
            Current_Size++;
        }
        else{
            i = (r - DecodePosition() - 1) & (SizeOfRingBuffer - 1);
            j = c - 255 + THRESHOLD;

            for (k = 0; k < j; k++)
            {
                c = TextBuffer[(i + k) & (SizeOfRingBuffer - 1)];
                putc(c, CompressTarget);
                TextBuffer[r++] = c;
                r &= (SizeOfRingBuffer - 1);
                Current_Size++;
            }
        }

        PrintCount++;
        if(PrintCount == 10240)
        {
            PI.StatusValue = ((Current_Size / SourceFileSize) * 100);
            PrintCount = 0;
        }

        if(PI.CancelFlag == TRUE)
        {
            if(CompressSource)fclose(CompressSource);
            if(CompressTarget)fclose(CompressTarget);
            return 1;
        }
    }

    if(CompressSource)fclose(CompressSource);
    if(CompressTarget)fclose(CompressTarget);

    Sleep(25);
    SendMessage(StatusBar_hWnd, PBM_SETPOS, 100, 0);

    return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif

