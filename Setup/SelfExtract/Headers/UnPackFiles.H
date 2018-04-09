/*

 UnPackFiles.H - Functions Used To Unpack Files From A Single File

*/

////////////////////////////////////////////////////////////////////////

int UnPackFiles(char *SourceFile, char *DestinationDir)
{
	FILE *LocalTarget = NULL;
	FILE *LocalSource = NULL;

	char LocalBuffer[BufferSize + 1];

	char StatusString[1024];
	char LocalTemp[1024];

	float xLocalLoop = 0;

	DWORD LocalLoop  = 0;

	int CurrentBuffer = 0;
	int NameLenTemp   = 0;
	int NeededMemory  = 0;

	DWORD SourceFileSize = 0;
	DWORD FileSizeLeft   = 0;
	DWORD Current_Size   = 0;

	/* Open Source File */
	if((LocalSource = fopen(SourceFile,"rb")) == NULL)
	{
		sprintf(LocalTemp, "UnPackFiles: Could not open %s for read.", SourceFile);
		Error(LocalTemp);

		if(LocalSource)fclose(LocalSource);
		return 1;
	}

    fread((char *)&Package.FileCount, sizeof(Package.FileCount), 1, LocalSource);
	if(Package.FileCount < 1)
	{
		Error("UnPackFiles: Package contains no files.");
		return 7;
	}

    fread((char *)&Package.FolderCount, sizeof(Package.FolderCount), 1, LocalSource);

    /* Get FileNames ( *FileNames[] )*/
    LocalLoop = 0;
    while(LocalLoop < Package.FileCount)
    {
        fread((char *)&NameLenTemp, sizeof(NameLenTemp), 1, LocalSource);
        Package.FileNames[LocalLoop] = (char *) calloc(NameLenTemp + 1, 1);
        fread(Package.FileNames[LocalLoop], sizeof(char), NameLenTemp, LocalSource);
        LocalLoop++;
    }

    /* Get FolderNames ( *FolderNames[] )*/
    LocalLoop = 0;
    while(LocalLoop < Package.FolderCount)
    {
        fread((char *)&NameLenTemp, sizeof(NameLenTemp), 1, LocalSource);
        Package.FolderNames[LocalLoop] = (char *) calloc(NameLenTemp + 1, 1);
        fread(Package.FolderNames[LocalLoop], sizeof(char), NameLenTemp, LocalSource);
        LocalLoop++;
    }

	sprintf(StatusString, "Package Contains %d Files && %d Folders \r\n \r\n", Package.FileCount, Package.FolderCount);
	SetText(StatusText_hWnd, StatusString);

	/* Create Folders */
	LocalLoop = 0;

	/* Create Directory (Log It After Log Files Open) */
	CreateDirectory(DestinationDir, 0);

	while(LocalLoop < Package.FolderCount) // Create Folders
	{
		sprintf(LocalTemp,"%s\\%s", DestinationDir, Package.FolderNames[LocalLoop]);
		CreateDirectory(LocalTemp, 0);
		LocalLoop++;
	}

	// Create And Write Data To Files (Begin)
	LocalLoop = 0;
	while(LocalLoop < Package.FileCount)
	{
		sprintf(LocalTemp, "%s\\%s", DestinationDir, Package.FileNames[LocalLoop]);

		Current_Size = 0;
		SourceFileSize = 0;
		FileSizeLeft = 0;
		Current_Size = 0;

		/* Get Size Of File */
        fread((char *)&SourceFileSize, sizeof(SourceFileSize), 1, LocalSource);

		// Set file attributes to normal to avoid errors
		SetFileAttributes(LocalTemp, FILE_ATTRIBUTE_NORMAL);

		/* Open Target File */
		if((LocalTarget = fopen(LocalTemp,"wb")) == NULL)
		{
			sprintf(LocalTemp, "UnPackFiles: Could not open %s for write.", LocalTemp);
			Error(LocalTemp);

			if(LocalTarget) fclose(LocalTarget);
			return 4;
		}

		if(CancelFlag)
		{
			if(LocalTarget) fclose(LocalTarget);
			if(LocalSource) fclose(LocalSource);
			return 5;
		}

		// If File Can Be Copied In One Buffer
		if(SourceFileSize < BufferSize)
		{
			fread(LocalBuffer, sizeof(char), SourceFileSize, LocalSource);
			fwrite(LocalBuffer, sizeof(char), SourceFileSize, LocalTarget);
		}
		else { // Else If File Bigger Than Buffer
			while(Current_Size < SourceFileSize)
			{
				FileSizeLeft = SourceFileSize - Current_Size;

				/* If File Can Be Completed In One Buffer */
				if(FileSizeLeft < BufferSize)
				{
					fread(LocalBuffer, sizeof(char), FileSizeLeft, LocalSource);
					fwrite(LocalBuffer, sizeof(char), FileSizeLeft, LocalTarget);
					Current_Size += FileSizeLeft;
					break;
				}

				CurrentBuffer = fread(LocalBuffer, sizeof(char), BufferSize, LocalSource);
				fwrite(LocalBuffer, sizeof(char), CurrentBuffer, LocalTarget);
				Current_Size += BufferSize;
			}
		}

		if(LocalTarget)fclose(LocalTarget);

		LocalLoop++;
		xLocalLoop++;

		StatusValue = (DWORD) ((xLocalLoop / Package.FileCount) * 100);
	}
	// Create And Write Data To Files (End)

	FreeMemory();

	if(LocalTarget) fclose(LocalTarget);
	if(LocalSource) fclose(LocalSource);

	Sleep(25);
	SendMessage(StatusBar_hWnd, PBM_SETPOS, 100, 0);

	return 0;
}

///////////////////////////////////////////////////////////////////////

