#ifndef _REGCODE_CPP
#define _REGCODE_CPP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.H>
#include <CommCtrl.H>
#include <StdIO.H>

#include "../Headers/RegCode.H"
#include "../Headers/NASCCL.H"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern HWND RegText_hWnd;
extern HWND RegProgress_hWnd;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool GenerateRegKey(char *CryptKey, int CryptKeyLen, char *outKey, int OutKeyLen, unsigned long *xOffset)
{
    int WPos = 0;
    int RPos = 0;

    unsigned long Offset = *xOffset;

    char Value = 0;

    while(WPos < OutKeyLen)
    {
        Value = (CryptKey[RPos] + (char)Offset);

        if((Value >= 48 && Value <= 57) || (Value >= 65 && Value <= 90) || (Value >= 97 && Value <= 122))
            outKey[WPos++] = Value;

        Offset = (Offset + CryptKey[RPos]);

        if(RPos == (CryptKeyLen-1))
            RPos = -1;

        RPos++;
    }

    outKey[WPos] = '\0';

    HashKey(outKey, CryptKey, OutKeyLen, CryptKeyLen);

    *xOffset = Offset;

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ValidateRegKey(char *inCryptKey, int CryptKeyLen, char *UserKey, int UserKeyLen, unsigned long OffsetStart)
{
    SendMessage(RegProgress_hWnd, PBM_SETRANGE, 0, MAKELPARAM(0, 10000));

    unsigned long Offset = OffsetStart;

    char NewKey[256];
    char CryptKey[256];

    strcpy(CryptKey, inCryptKey);

    int Keys = 0;
    while(Keys < 10000)
    {
        GenerateRegKey(CryptKey, CryptKeyLen, NewKey, UserKeyLen, &Offset);
        if(_strcmpi(NewKey, UserKey) == 0)
        {
            return true;
        }

        SendMessage(RegProgress_hWnd, PBM_SETPOS, Keys, 0);
        Keys++;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
