/********   ********   ********/
/*          Registry.h        */
/********   ********   ********/
/*          8/7/2020          */
/********   ********   ********/
/*           Tiron            */
/********   ********   ********/
// http://tristr.am/
// https://freshmeat-blog.de.tl/
// https://d1legit.com/

// Class to ease use of Windows registry for storing Torch options and settings

#ifndef REGISTRY_H	
#define REGISTRY_H

#include <windows.h>
#include <stdio.h>

#define KEY_DOESNT_EXIST	2
#define KEY_BAD_TYPE		1
#define KEY_SUCCESS			0

class Registry
{
private:
	HKEY hKey;
	char szFullKey[255];
	char szKey[255];
	int iCheck;
	int iError;
	char szData[255];

public:
	/* Constructors */
	Registry()
	{
		iError = 0;

		sprintf(szFullKey, "Software\\%s", "UnamedObject");
		sprintf(szKey, "");

		hKey = NULL;
		int iDisp;
		iCheck = RegCreateKeyEx(HKEY_CURRENT_USER, szFullKey, NULL, NULL, NULL, KEY_ALL_ACCESS, NULL, &hKey, (unsigned long *)&iDisp);

		if(check())
		{
		}
		else
		{
			//MessageBox(NULL, "Could Not Open Registry:\n        \"Software\\UnamedObject\"", NULL, MB_OK | MB_ICONERROR);
		}

		close();
	};
	Registry(char szTheKey[])
	{
		iError = 0;

		sprintf(szFullKey, "Software\\%s", szTheKey);
		sprintf(szKey, "");

		iCheck = RegCreateKeyEx(HKEY_CURRENT_USER, szFullKey, NULL, NULL, NULL, KEY_ALL_ACCESS, NULL, &hKey, NULL);
		if(check())
		{
		}
		else
		{
			char szMsg[255];
			sprintf(szMsg, "Could Not Open Registry:\n        \"%s\"", szFullKey);
			//MessageBox(NULL, szMsg, NULL, MB_OK | MB_ICONERROR);
		}

		close();
	};
	~Registry()
	{
		if(check())
			RegCloseKey(hKey);
	};

	/* Function to Save Data */
// Don't Let Anyone Else Call This!
private:
	void open()
	{
		iError = 0;
		iCheck = RegOpenKeyEx(HKEY_CURRENT_USER, szFullKey, 0, KEY_ALL_ACCESS, &hKey);
	};

	void close()
	{
		iCheck = -1;
		RegCloseKey(hKey);
	};
// Default Back to public:
public:

	/* Operators */
	// Allow int = Registry;
	operator int()
	{
		return this->getInt();
	};

	// Allow Registry = int;
	Registry operator =(int iToSave)
	{
		this->saveInt(iToSave);
		return *this;
	};

	// Allow char[] = Registry
	operator char*()
	{
		return this->getChar();
	}

	// Allow Registry = char[]
	Registry operator =(char szNew[])
	{
		this->saveChar(szNew);
		return *this;
	}

	/* Methods */
	bool check()
	{
		if(iCheck == ERROR_SUCCESS)
		{
			return true;
		}
		else
		{
			return false;
		}
	};

	void setKey(char szNewKey[])
	{
		sprintf(szKey, "%s", szNewKey);
	};

	int getInt()
	{
		return getInt(NULL);
	};

	char * getChar()
	{
		return getChar(NULL);
	};

	int getInt(int * pReturn)
	{
		int iNullReturn = 0;

		if(pReturn == NULL)
			pReturn = &iNullReturn;

		int iType = 0;
		int iDataLen = 255;

		open();
		int iReturn = RegQueryValueEx(hKey, szKey, NULL, (unsigned long *)&iType, (unsigned char *)szData, (unsigned long *)&iDataLen);
		close();

		if(iReturn != ERROR_SUCCESS)
		{
			iType = 0;

			char szTemp[255];
			char szDes[255];

			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, iReturn, NULL, szDes, 255, NULL);
			sprintf(szTemp, "Error:\t\t%i\nDescription:\t%s\nKey: %s", iReturn, szDes, szKey);
			//MessageBox(NULL, szTemp, NULL, NULL);
		}

		if(iType == 0)
		{
			*pReturn = KEY_DOESNT_EXIST;
			iError = KEY_DOESNT_EXIST;
			return -iError;
		}
		else if(iType == REG_DWORD)
		{
			*pReturn = KEY_SUCCESS;
			iError = KEY_SUCCESS;
			int * iTemp = (int *)szData;
			return *iTemp;
		}
		else
		{
			*pReturn = KEY_BAD_TYPE;
			iError = KEY_BAD_TYPE;
			return -iError;
		}
	};

	char * getChar(int * pReturn)
	{
		int iNullReturn = 0;

		if(pReturn == NULL)
			pReturn = &iNullReturn;

		int iType = 0;
		int iDataLen = 255;

		open();
		int iReturn = RegQueryValueEx(hKey, szKey, NULL, (unsigned long *)&iType, (unsigned char *)szData, (unsigned long *)&iDataLen);
		close();

		if(iReturn != ERROR_SUCCESS)
		{
			iType = 0;

			char szTemp[255];
			char szDes[255];

			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, iReturn, NULL, szDes, 255, NULL);
			sprintf(szTemp, "Error:\t\t%i\nDescription:\t%s", iReturn, szDes);
			//MessageBox(NULL, szTemp, NULL, NULL);
		}

		if(iType == 0)
		{
			*pReturn = KEY_DOESNT_EXIST;
			return 0;
		}
		else if(iType == REG_SZ)
		{
			*pReturn = KEY_SUCCESS;
			//int * iTemp = (int *)szData;
			char * szTempContainer = new char[255];
			sprintf(szTempContainer, "%s", szData);
			return szTempContainer;
		}
		else
		{
			*pReturn = KEY_BAD_TYPE;
			return "";
		}
	};

	void saveInt(int iSave)
	{
		open();
		RegSetValueEx(hKey, szKey, NULL, REG_DWORD, (unsigned char *)&iSave, sizeof(iSave));
		close();
	};

	void saveChar(char szSave[])
	{
		open();
		RegSetValueEx(hKey, szKey, NULL, REG_SZ, (unsigned char *)szSave, strlen(szSave));
		close();
	};

	char * getFullKey()
	{ return szFullKey; };

	char * getKey()
	{ return szKey; };

	int getError()
	{ return iError; };
};

#endif