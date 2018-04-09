#ifndef _NASCCL_H_
#define _NASCCL_H_
///////////////////////////////////////////////////////////////////////////////////////////

#define ROUNDMUTATION      0x0001
#define SWAPMATRIX         0x0002
#define ROUNDRESULT        0x0004
#define NONREPETITION      0x0010
#define ENSUREOFFSET       0x0020
#define FMATRIXMUTATION    0x0040
#define RMATRIXMUTATION    0x0080
#define CMATRIXMUTATION    0x0100
#define CRYPTFULLDATASET   0x0200
#define RUNNINGKEYPOSITION 0x0400

#define NASCCLDllImport    extern "C" __declspec( dllimport )

///////////////////////////////////////////////////////////////////////////////////////////

struct _NASCCL{
    int SizeOfKey;

    char *Key;

    int Attribute;

    int KeyPosition;

    unsigned char MValue[7][7]; // (288-Bit Matrix) ((6 * 6) * 8) --- (7 = Null)
    unsigned char Matrix[7][7]; // (288-Bit Matrix) ((6 * 6) * 8) --- (7 = Null)
};

///////////////////////////////////////////////////////////////////////////////////////////

NASCCLDllImport bool Encode(struct _NASCCL *, char *, char *, int);
NASCCLDllImport bool Decode(struct _NASCCL *, char *, char *, int);

NASCCLDllImport bool InitializeCryptographySet(struct _NASCCL *, char *, int, int);
NASCCLDllImport bool UninitializeCryptographySet(struct _NASCCL *);

NASCCLDllImport int HashKey(char *, char *, int, int);
NASCCLDllImport int CipherKey(char *, char *, int);

NASCCLDllImport int GetMaxBitStrength(void);
NASCCLDllImport int GetMinBitStrength(void);

NASCCLDllImport int GetMaxDataSize(void);
NASCCLDllImport int GetMinDataSize(void);

NASCCLDllImport int GetMaxKeyLength(void);
NASCCLDllImport int GetMinKeyLength(void);

NASCCLDllImport char *GetAlgorithmName(void);
NASCCLDllImport char *GetAlgorithmVersion(void);

///////////////////////////////////////////////////////////////////////////////////////////
#endif

