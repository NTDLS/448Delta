///////////////////////////////////////////////////////////////////////////////////////////

void Error(char *xMessage)
{
	MessageBox(StatusDialog_hWnd, xMessage, TitleCaption, MB_SYSTEMMODAL);
}

/////////////////////////////////////////////////////////////////////

void SetText (HWND xDialog, char *xText)
{
	SendMessage(xDialog,(UINT)WM_SETTEXT,(WPARAM)0,(LPARAM)xText);
}

/////////////////////////////////////////////////////////////////////

char *GetText (HWND xDialog)
{
	char *ReturnTemp = NULL;
	int Length		 = 0;
	Length = GetWindowTextLength(xDialog) + 1;
	ReturnTemp = (char *) calloc(Length,1);
	GetWindowText(xDialog,ReturnTemp,Length);
	return ReturnTemp;
}

/////////////////////////////////////////////////////////////////////

void CenterWindow (HWND xDialog)
{
	RECT wRect;
	DWORD CordX;
	DWORD CordY;
	GetWindowRect(xDialog,&wRect);
	CordX = (GetSystemMetrics(SM_CXSCREEN) - (wRect.right-wRect.left)) / 2;
	CordY = (GetSystemMetrics(SM_CYSCREEN) - (wRect.bottom - wRect.top + GetSystemMetrics(SM_CYCAPTION))) / 2;
	SetWindowPos(xDialog,NULL,CordX,CordY,0,0,SWP_NOSIZE | SWP_NOZORDER);
}

/////////////////////////////////////////////////////////////////////

int Unpack(char *xInputName,char *xOutputName)
{
	DWORD Result   = 0;

	char DecryptTemp[257];
	sprintf(DecryptTemp,"%s\\%s",GetTempDirectory(),"DecryptTemp.Upd");

	char StatusTemp[1024];
	sprintf(StatusTemp,"Decrypting %d KBytes",Get_FileSize(xInputName)/1024);
	SetText(StatusText_hWnd,StatusTemp);

	Result = BlowFishDecode(xInputName, DecryptTemp, CodedKey);
	
	if(Result == 0)
	{
		Result = UnPackFiles(DecryptTemp,xOutputName);
		if(Result == 0)
		{
			SetText(StatusText_hWnd, OperationComplete);
		}
	}

	SetText(OkCancelButton_hWnd, XOk);

	if(CancelFlag == TRUE)
	{
		xDeleteObject(xOutputName);
		SetText(StatusText_hWnd, Canceled);
		EnableWindow(OkCancelButton_hWnd,TRUE);
	}

	DoneFlag = TRUE;

	return Result;
}

//////////////////////////////////////////////////////////////////////

int DecompressDecrypt(char *xInputName,char *xOutputName)
{
	DWORD Result   = 0;

	char DecryptTemp[257];
	sprintf(DecryptTemp,"%s\\%s",GetTempDirectory(),"DecryptTemp.Upd");
	char CompressTemp[257];
	sprintf(CompressTemp,"%s\\%s",GetTempDirectory(),"CompressTemp.Upd");

	char StatusTemp[1024];
	sprintf(StatusTemp,"Decrypting %d KBytes",Get_FileSize(xInputName)/1024);
	SetText(StatusText_hWnd,StatusTemp);

	Result = BlowFishDecode(xInputName, DecryptTemp, CodedKey);

	if(Result == 0)
	{
		sprintf(StatusTemp,"Decompressing %d KBytes",Get_FileSize(DecryptTemp)/1024);
		SetText(StatusText_hWnd,StatusTemp);
		Result = LZARIDecompress(DecryptTemp,xOutputName);
		if(Result == 0)
		{
			SetText(StatusText_hWnd, OperationComplete);
		}
	}

	SetText(OkCancelButton_hWnd, XOk);
	DoneFlag = TRUE;
	if(CancelFlag == TRUE)
	{
		xDeleteObject(xOutputName);
		SetText(StatusText_hWnd, Canceled);
		EnableWindow(OkCancelButton_hWnd,TRUE);
	}

	return Result;
}

//////////////////////////////////////////////////////////////////////

int DecompressUnpack(char *xInputName,char *xOutputName)
{
	DWORD Result   = 0;

	char DecryptTemp[257];
	sprintf(DecryptTemp,"%s\\%s",GetTempDirectory(),"DecryptTemp.Upd");
	char CompressTemp[257];
	sprintf(CompressTemp,"%s\\%s",GetTempDirectory(),"CompressTemp.Upd");

	char StatusTemp[1024];
	sprintf(StatusTemp,"Decrypting %d KBytes",Get_FileSize(xInputName)/1024);
	SetText(StatusText_hWnd,StatusTemp);

	Result = BlowFishDecode(xInputName, DecryptTemp, CodedKey);

	if(Result == 0)
	{
		sprintf(StatusTemp,"Decompressing %d KBytes",Get_FileSize(DecryptTemp)/1024);
		SetText(StatusText_hWnd,StatusTemp);
		Result = LZARIDecompress(DecryptTemp,CompressTemp);
		if(Result == 0)
		{
			Result = UnPackFiles(CompressTemp,xOutputName);
			if(Result == 0)
			{
				SetText(StatusText_hWnd, OperationComplete);
			}
		}
	}

	SetText(OkCancelButton_hWnd, XOk);
	DoneFlag = TRUE;
	if(CancelFlag == TRUE)
	{
		xDeleteObject(xOutputName);
		SetText(StatusText_hWnd, Canceled);
		EnableWindow(OkCancelButton_hWnd,TRUE);
	}

	return Result;
}

//////////////////////////////////////////////////////////////////////

int DecryptFile(char *xInputName,char *xOutputName)
{
	DWORD Result   = 0;

	char DecryptTemp[257];
	sprintf(DecryptTemp,"%s\\%s",GetTempDirectory(),"DecryptTemp.Upd");
	char CompressTemp[257];
	sprintf(CompressTemp,"%s\\%s",GetTempDirectory(),"CompressTemp.Upd");

	char StatusTemp[1024];
	sprintf(StatusTemp,"Decrypting %d KBytes",Get_FileSize(xInputName)/1024);
	SetText(StatusText_hWnd,StatusTemp);

	Result = BlowFishDecode(xInputName, xOutputName, CodedKey);

	if(Result == 0)
	{
		SetText(StatusText_hWnd, OperationComplete);
	}

	SetText(OkCancelButton_hWnd, XOk);
	DoneFlag = TRUE;
	if(CancelFlag == TRUE)
	{
		xDeleteObject(xOutputName);
		SetText(StatusText_hWnd, Canceled);
		EnableWindow(OkCancelButton_hWnd,TRUE);
	}

	return Result;
}

//////////////////////////////////////////////////////////////////////

DWORD WINAPI UnPack_Thread(LPVOID data)
{
	char CleanSource[257];
	char CompressTemp[257];
	char DecryptTemp[257];

	int FlagResult = 0;
	DWORD Result   = 0;

	sprintf(DecryptTemp,"%s\\%s",GetTempDirectory(),"DecryptTemp.Upd");
	sprintf(CleanSource,"%s\\%s",GetTempDirectory(),"CleanSource.Upd");
	sprintf(CompressTemp,"%s\\%s",GetTempDirectory(),"CompressTemp.Upd");

	char *TargetDestination = Package.TargetDir;

	if(FileAccess(TargetDestination,FExist))
	{
		if(MessageBox(MainDialog_hWnd, "Target File Already Exist. Overwrite?" , TitleCaption, 0 + MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDNO)
		{
			SetText(StatusText_hWnd, Canceled);
			SetText(OkCancelButton_hWnd, XOk);
			DoneFlag = TRUE;
			return 0;
		}
	}

	FlagResult = PreCheckFile(SourceFileName);

	if(FlagResult == -1)
		Result = 1;
	else if(FlagResult == 1)
		Result = DecryptFile(SourceFileName, TargetDestination);
	else if(FlagResult == 2)
		Result = Unpack(SourceFileName, TargetDestination);
	else if(FlagResult == 3)
		Result = DecompressDecrypt(SourceFileName, TargetDestination);
	else if(FlagResult == 4)
		Result = DecompressUnpack(SourceFileName, TargetDestination);
	else if(FlagResult == 5){
		Error("Package is corrupt.");
		Result = 2;
	}

	SetText(OkCancelButton_hWnd, XOk);
	DoneFlag = TRUE;

	if(CancelFlag == TRUE)
	{
		SetText(StatusText_hWnd, Canceled);
		EnableWindow(OkCancelButton_hWnd,TRUE);
	}

	unlink(DecryptTemp);
	unlink(CleanSource);
	unlink(CompressTemp);

	return Result;
}

/////////////////////////////////////////////////////////////////////

void FreeMemory(void)
{
	while(Package.FolderCount != -1)
	{
		free(Package.FolderNames[Package.FolderCount]);
		Package.FolderNames[Package.FolderCount] = NULL;
		Package.FolderCount--;
	}

	while(Package.FileCount != -1)
	{
		free(Package.FileNames[Package.FileCount]);
		Package.FileNames[Package.FileCount] = NULL;
		Package.FileCount--;
	}
}

//////////////////////////////////////////////////////////////////////

char *GetTempDirectory(void)
{
	char *Name;
	Name = (char *) calloc(257,1);
	GetTempPath(256,Name);
	return Name;
}

//////////////////////////////////////////////////////////////////////

int PreCheckFile(char *LocalFileName)
{
    FILE *FileHandle  = NULL;

    char lsHeader[FILEHEADERLEN + 1];

    DWORD LocalFlag   = 0;

	/* Check for file access */
	if(!FileAccess(LocalFileName,FExist)) // Check exist
	{
		char ErrorTemp[1024];
		sprintf(ErrorTemp,"PreCheckFile: %s does not exist.", LocalFileName);
		Error(ErrorTemp);

		return -1;
	}

	/* Open Source File */
	if((FileHandle = fopen(LocalFileName, "rb")) == NULL)
	{
		char ErrorTemp[1024];
		sprintf(ErrorTemp,"PreCheckFile: Could not open %s for read.", LocalFileName);
		Error(ErrorTemp);
		return -1;
	}

	fseek(FileHandle, ExecutableEnd, SEEK_SET); // Seek Past Executable

    fread(lsHeader, FILEHEADERLEN, sizeof(char), FileHandle);
    lsHeader[FILEHEADERLEN] = '\0';

    if( strcmp(lsHeader, FILEHEADER) != 0)
    {
        if(FileHandle) fclose(FileHandle);
        return 5; // Not encrypted by 448Delta
    }

	/* Read file flag ( If Any ) */
    fread((char *)&LocalFlag, FLAGLENGHT, 1, FileHandle);

	if(FileHandle) fclose(FileHandle);

	if(LocalFlag == ENCFlag) // If File Encryted
		return 1;
	else if(LocalFlag == PACFlag) // If File Packed
		return 2;
	else if(LocalFlag == ENCCompress) // If File Compressed Encryted
		return 3;
	else if(LocalFlag == PACCompress) // If File Compressed Packed
		return 4;

    Error("This file was encrypted by 448Delta but its encryption method could not be determined. This is most likely because this file was encrypted by a different version of 448Delta.");

	return -1; // If file not encrypted
}

//////////////////////////////////////////////////////////////////////

DWORD Get_FileSize(char *File2Size)
{
	HANDLE Source;
	DWORD size;
	Source = CreateFile(File2Size,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
	size = GetFileSize(Source,NULL);
	if(size == HFILE_ERROR) return -1;
	CloseHandle(Source);
	return size;
}

//////////////////////////////////////////////////////////////////////

BOOL FileAccess(char *FileName, short Mode)
{
	if(access(FileName, Mode) == 0)
		return TRUE;
	return FALSE;
}

//////////////////////////////////////////////////////////////////////

BOOL IsDirectory(char *xRequest)
{
	int Attrib;
	Attrib = GetFileAttributes(xRequest);
	if(Attrib != 0xFFFFFFFF)
	{
		if(Attrib & FILE_ATTRIBUTE_DIRECTORY)
			return TRUE;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////

