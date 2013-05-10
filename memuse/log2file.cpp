//log2file.cpp

#include "log2file.h"

char	logFileName[MAX_PATH];
TCHAR	logFileNameW[MAX_PATH];

//=================================================================================
//implementation
//=================================================================================

//=================================================================================
// check the actual log file size
// if > 1MB move to bak and start new log file
// return true if new file started
// uses global logFileNameW
//=================================================================================
BOOL checkFileSize(){
	BOOL bRet=FALSE;
	if(wcslen(logFileNameW)==0)
		return bRet;

	TCHAR txtFileName[MAX_PATH+1];
	//make local copy
	wsprintf(txtFileName, L"%s", logFileNameW);

	//#### we maintain two log files, an actual and a bak one
	//get file size
	HANDLE hFile = CreateFile(txtFileName, 
       GENERIC_READ,          // open for writing
       FILE_SHARE_READ,       // share
       NULL,                   // default security
       OPEN_EXISTING,          // open existing
       FILE_ATTRIBUTE_NORMAL,  // normal file
       NULL);                  // no attr. template
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	CloseHandle(hFile);

	if(dwFileSize != 0xFFFFFFFF){ //no error
		if(dwFileSize>0x100000){ //more than 1MB?
			//make a backup
			//delete previous bak
			TCHAR txtFileNameBAK[MAX_PATH];
			wsprintf(txtFileNameBAK, L"%s.bak", txtFileName);
			DeleteFile(txtFileNameBAK);
			//rename old file to .BAK
			MoveFile(txtFileName, txtFileNameBAK);
			bRet=TRUE;
		}
	}
	//copy filename to global char and tchar var
//	wsprintf(logFileNameW, txtFileName);
//	wcstombs(logFileName, logFileNameW, sizeof(logFileNameW)*sizeof(logFileNameW[0]));
	return bRet;
}

//========================== start of file stuff =============================

int initFileNames(TCHAR* _logFileName)
{
	TCHAR txtFileName[MAX_PATH+1];
	wsprintf(txtFileName, L"%s", _logFileName);
	//store the filename to use in char and tchar
	TCHAR logFileNameW[MAX_PATH];

	//#### we maintain two log files, an actual and a bak one
	//get file size
	HANDLE hFile = CreateFile(txtFileName, 
       GENERIC_READ,          // open for writing
       FILE_SHARE_READ,       // share
       NULL,                   // default security
       OPEN_EXISTING,          // open existing
       FILE_ATTRIBUTE_NORMAL,  // normal file
       NULL);                  // no attr. template
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	CloseHandle(hFile);

	if(dwFileSize != 0xFFFFFFFF){ //no error
		if(dwFileSize>0x100000){ //more than 1MB?
			//make a backup
			//delete previous bak
			TCHAR txtFileNameBAK[MAX_PATH];
			wsprintf(txtFileNameBAK, L"%s.bak", txtFileName);
			DeleteFile(txtFileNameBAK);
			//rename old file to .BAK
			MoveFile(txtFileName, txtFileNameBAK);
		}
	}

	//copy filename to global char and tchar var
	wsprintf(logFileNameW, txtFileName);
	wcstombs(logFileName, logFileNameW, sizeof(logFileNameW)*sizeof(logFileNameW[0]));

	FILE	*fp;
	fp = fopen(logFileName, "a+");
	fclose(fp);
	bFirstFileCall=false;
	return 0;
}

int initFileNames()
{
	 // Get name of executable
	TCHAR lpFileName1[MAX_PATH+1];
	GetModuleFileName(NULL, lpFileName1, MAX_PATH); //lpFileName will contain the exe name of this running app!

	//add txt extension
	TCHAR txtFileName[MAX_PATH+1];
	wsprintf(txtFileName, L"%s.log.txt", lpFileName1);
	//store the filename to use in char and tchar
	TCHAR logFileNameW[MAX_PATH];

	//#### we maintain two log files, an actual and a bak one
	//get file size
	HANDLE hFile = CreateFile(txtFileName, 
       GENERIC_READ,          // open for writing
       FILE_SHARE_READ,       // share
       NULL,                   // default security
       OPEN_EXISTING,          // open existing
       FILE_ATTRIBUTE_NORMAL,  // normal file
       NULL);                  // no attr. template
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	CloseHandle(hFile);

	if(dwFileSize != 0xFFFFFFFF){ //no error
		if(dwFileSize>0x100000){ //more than 1MB?
			//make a backup
			//delete previous bak
			TCHAR txtFileNameBAK[MAX_PATH];
			wsprintf(txtFileNameBAK, L"%s.bak", txtFileName);
			DeleteFile(txtFileNameBAK);
			//rename old file to .BAK
			MoveFile(txtFileName, txtFileNameBAK);
		}
	}

	//copy filename to global char and tchar var
	wsprintf(logFileNameW, txtFileName);
	wcstombs(logFileName, logFileNameW, sizeof(logFileNameW)*sizeof(logFileNameW[0]));

	FILE	*fp;
	fp = fopen(logFileName, "a+");
	fclose(fp);
	bFirstFileCall=false;
	return 0;
}

void Add2Log (TCHAR *txt) {
	Add2Log(txt, false);
}

//-----------------------------------------------------------------------
// Add2Log - Add string to the the file
//
void Add2Log (TCHAR *lpszFormat, ...) {
    int nBuf;//, i;
    TCHAR szBuffer[512];

    va_list args;
    va_start(args, lpszFormat);

    nBuf = vswprintf(szBuffer, lpszFormat, args);
	writefile(szBuffer);
    va_end(args);
}

//-----------------------------------------------------------------------
// Add2Log - Add string to the the file
//
void Add2LogWtime (TCHAR *lpszFormat, ...) {
    int nBuf;//, i;
    TCHAR szBuffer[512];

    va_list args;
    va_start(args, lpszFormat);

    nBuf = vswprintf(szBuffer, lpszFormat, args);
	Add2LogWithTime(szBuffer);
    va_end(args);
}
//-----------------------------------------------------------------------
// appendfile - set the global filename to append text
//
int appendfile(char *filename)
{
	//store the filename to use in char and tchar
	sprintf(logFileName, filename);
	mbstowcs(logFileNameW, logFileName, sizeof(logFileName)*sizeof(logFileName[0]));

	FILE	*fp;
	fp = fopen(logFileName, "a+");
	fclose(fp);
	return 0;
}

int newfile(char *filename)
{
	//store the filename to use in char and tchar
	sprintf(logFileName, filename);
	mbstowcs(logFileNameW, logFileName, sizeof(logFileName)*sizeof(logFileName[0]));

	FILE	*fp;
	fp = fopen(logFileName, "w+");
	fclose(fp);
	return 0;
}

int newfile(TCHAR *filename)
{
	//store the filename to use in char and tchar
	TCHAR logFileNameW[MAX_PATH];
	wsprintf(logFileNameW, filename);
	wcstombs(logFileName, logFileNameW, sizeof(logFileNameW)*sizeof(logFileNameW[0]));

	FILE	*fp;
	fp = fopen(logFileName, "w+");
	fclose(fp);
	return 0;
}

int writefile(TCHAR *filetext)
{
#ifdef UseLogging
	/* File Write Function, written by professor chemicalX */
	FILE	*fp;						/* Declare FILE structure */
	TCHAR  szTemp[512];
	char  szTempA[512];

	wsprintf(szTemp, L"%s", filetext);
	wcstombs(szTempA, szTemp, sizeof(szTemp)/sizeof(TCHAR));

	DEBUGMSG(UseLogging, (szTemp));
	
	checkFileSize();
	
	fp = fopen(logFileName, "a+");

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/* First of we open the file supplied by the filename paremeter */

	/*
	 * in the "a+" mode for appending, so if it doesnt exist its created. �
	 * fp = fopen(filename,"w"); // Open using the "w" mode for writing.
	 */
	long	fsize = strlen(szTempA);	/* Declare the long fsize with the length of the filetext */
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/* paremeter witch stores the text or data to right to the file. */
	fwrite(szTempA, 1, fsize, fp);		/* Write File */
	fclose(fp); /* Remember to close the file stream after calling file functions, */

	/* otherwise the file wont be created. */
#endif
	return 0;
}

void Add2LogWithTime(TCHAR *txt)
{
	BOOL bLogTime = TRUE;
	TCHAR str[512];
	TCHAR lpTimeStr[128];
	TCHAR lpDateStr[128];
	LONG res;
	wsprintf(str,L"");
	//Read the system time
	res = GetTimeFormat(LOCALE_SYSTEM_DEFAULT,
							TIME_FORCE24HOURFORMAT,
							NULL,
							L"hh:mm:ss",
							lpTimeStr,
							sizeof (lpTimeStr ) * sizeof(TCHAR));
	if (res == 0)
	{
		wcscpy(lpTimeStr, L"err");
	}

	//Read the system date
	res = GetDateFormat(  LOCALE_SYSTEM_DEFAULT,
						  NULL,
						  NULL,
						  L"dd.MM.yyyy",
						  lpDateStr,
						  sizeof (lpDateStr) * sizeof(TCHAR));
	if (res == 0)
	{
		wcscpy(lpDateStr, L"err");
	}

	if (bLogTime == TRUE)
		wsprintf(str, L"%s %s\t%s", lpDateStr, lpTimeStr , txt);
	else
		wsprintf(str, L"%s", txt);
	writefile(str);

}

void Add2Log(char *txt, BOOL bLogTime = TRUE)
{
	TCHAR  szTemp[512];
	char  szTempA[512];
	wsprintf(szTemp, L"");
	sprintf(szTempA, "");
	sprintf(szTempA, "%s", txt);
	mbstowcs(szTemp, szTempA, 512);//strlen(szTempA)*sizeof(szTempA[0]));

	Add2Log(szTemp, bLogTime);
}
