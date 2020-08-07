/********   ********   ********/
/*         Source File        */
/********   ********   ********/
/*          8/7/2020          */
/********   ********   ********/
/*           Tiron            */
/********   ********   ********/
// http://tristr.am/
// https://freshmeat-blog.de.tl/
// https://d1legit.com/

/* Hooks.cpp Implements the Hook Class */
/* See Hooks.h for More Info */

#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "Hooks.h"

Hook::Hook()
{
	HookLocation = 0;
	HookLength = 0;
	HookRevertData = NULL;
	Callback = NULL;
}

Hook::Hook(int Location, int Length, void * CallbackFunction)
{
	HookLocation = Location;
	HookLength = Length;

	char * HookData = (char *)(HookLocation);
	HookRevertData = new char[Length];

	memcpy(HookRevertData, HookData, HookLength);

	Callback = CallbackFunction;
}

Hook::~Hook()
{
	if(HookRevertData != NULL)
	{
		delete HookRevertData;
	}
	else
	{ }
}

void JmpPatch(void * pDest, int pSrc, int Nops = 1)
{
	unsigned long OldProt = 0;
	unsigned char Jmp = 0xE9;
	int Address = (DWORD)pDest - (DWORD)pSrc - 5;

	VirtualProtect((LPVOID)pSrc, 5 + Nops, PAGE_EXECUTE_READWRITE, &OldProt);
	
	memcpy((LPVOID)pSrc, &Jmp, 1);
	memcpy((LPVOID)(pSrc + 1), &Address, 4);

	for (int i = 0; i < Nops; ++i)
	{
		*(unsigned char *)((DWORD)pSrc + 5 + i) = 0x90;
	}

	VirtualProtect((LPVOID)pSrc, 5 + Nops, OldProt, &OldProt);
}

void Hook::SetHook()
{
//	JmpPatch(Callback, HookLocation, HookLength - 5);

	unsigned long OldProt = 0;
	unsigned char Jmp = 0xE9;
	int Nops = 5 - HookLength;
	int Address = (int)(Callback) - HookLocation - 5;

	VirtualProtect((void *)HookLocation, HookLength, PAGE_EXECUTE_READWRITE, &OldProt);

	memcpy((void *)HookLocation, &Jmp, 1);
	memcpy((void *)(HookLocation + 1), &Address, 4);

	for (int i = 0; i < Nops; ++i)
	{
		*(unsigned char *)(HookLocation + 5 + i) = 0x90;
	}

	VirtualProtect((void *)HookLocation, HookLength, OldProt, &OldProt);	
}

void Hook::Revert()
{
	unsigned long OldProt = 0;

	VirtualProtect((void *)HookLocation, HookLength, PAGE_EXECUTE_READWRITE, &OldProt);
	memcpy((char *)HookLocation, HookRevertData, HookLength);
	VirtualProtect((void *)HookLocation, HookLength, OldProt, &OldProt);
}