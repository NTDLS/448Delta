////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL DeleteTree(char *);
int xDeleteObject(char *);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int xDeleteObject(char *xObjectName)
{
    PI.StatusValue = 0;

    if(IsDirectory(xObjectName))
    {
        return DeleteTree(xObjectName);
    }
    else{
        SetFileAttributes(xObjectName, FILE_ATTRIBUTE_NORMAL);
        return DeleteFile(xObjectName);
    }

    return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL DeleteTree(char *DeleteRoot)
{
    WIN32_FIND_DATA FindData;
    HANDLE FileHandle;

    int Found = 0;

    static DWORD Deleted = 0;

    static int StatusCount = 123;

    char SearchTemp[257];
    char DeleteTemp[257];

    sprintf(SearchTemp, "%s\\%s", DeleteRoot, "*.*");
    FileHandle = FindFirstFile(SearchTemp, &FindData);

    Found = (int) FileHandle;

    while(Found != 0)
    {
        if( strcmp(FindData.cFileName, ".") && strcmp(FindData.cFileName, "..") )
        {
            sprintf(DeleteTemp, "%s\\%s", DeleteRoot, FindData.cFileName);

            SetFileAttributes(DeleteTemp, FILE_ATTRIBUTE_NORMAL);

            if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                DeleteTree(DeleteTemp);
            else DeleteFile(DeleteTemp);

            if(StatusCount == 123)
            {
                sprintf(PI.StatusText, "Removing: %d Files and Folders", Deleted);
                StatusCount = -1;
            }

            StatusCount++;
            Deleted++;
        }
        Found = FindNextFile(FileHandle, &FindData);
    }

    FindClose(FileHandle);

    RemoveDirectory(DeleteRoot);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

