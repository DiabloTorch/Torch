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

#include <stdio.h>

//#include "Offsets.h"
#include "InspectorWindow.h"
#include "Drawing.h"
#include "Theme.h"
#include "GameData.h"
#include <Windows.h>

#define TRANSLATE_OFFSET(offset, monster) (offset - (0x0069DD1C - 0x64E3CC) + monster * 0xE4)

PlayerInspectorClass PlayerInspector;

PlayerInspectorClass::PlayerInspectorClass()
{
	SetX(75);
	SetY(150);

	SetWidth(360);
	SetHeight(320);
	SetTitle("Player Inspector");
}

PlayerInspectorClass::~PlayerInspectorClass()
{
}

void PlayerInspectorClass::Draw()
{
	Window::Draw();

	if (GetWindowState() != Open)
		return;

	int DrawX = GetClientRect().x + 18;
	int DrawY = GetClientRect().y;
	DrawY += 2;

	static int Offset = 0;

	GameData.DataUniqueItemFlags()[47] = 1; // Civerbs

	// Draw color picker
	// Each vertical line is 0x10 colors
	int TotalDataLines = 16;
	int DataLineSize = 16;

	for (int i = 0; i < TotalDataLines; i++)
	{
		for (int j = 0; j < DataLineSize; j++)
		{
			DrawBox(DrawX + (18 * i), DrawY + (18 * j), 16, 16, i * DataLineSize + j);
		}
	}


	/*
	char szName[1024];
	int DataLineSize = 8;
	int TotalDataLines = 24;


	//unsigned char* rawData = GameData.GetPlayerData();
	unsigned char* rawData = (unsigned char*)(0x64D3C4);
	int Monster = *(int *)(0x4B8CA8); // Monster index pointed to by cursor
	rawData = (rawData + (Monster * 0xE4));
	//PlayerData* playerData = (PlayerData*)(((int)GameData.GetPlayerStruct()) + Offset);
	//PlayerData* playerData = (PlayerData*)(GameData.Belt() - 0x4737);

	
	rawData += Offset;
	sprintf(szName, "Offset: %i (0x%08X)", Offset, (int)(rawData));
	PrintTextXY(DrawX + 5, DrawY, DIABLO_TEXT_WHITE, szName);

	if (GetAsyncKeyState(VK_INSERT))
	{
		Offset += 1;// DataLineSize;
	}
	else if (GetAsyncKeyState(VK_DELETE))
	{
		Offset -= 1;// DataLineSize;
	}

	if (GetAsyncKeyState(VK_PRIOR))
	{
		// Page up
		//Offset += DataLineSize * TotalDataLines;
		(*rawData)++;
	}
	else if (GetAsyncKeyState(VK_NEXT))
	{
		// Page down
		//Offset -= DataLineSize * TotalDataLines;
		(*rawData)--;
	}

	int Bytes = 0;
	for (int i = 0; i < TotalDataLines; i++)
	{
		// Line memory address
		sprintf(szName, "0x%08X:\t", (int)(rawData + i * DataLineSize));
		PrintTextXY(DrawX + 5, DrawY + (i + 1) * 14, DIABLO_TEXT_BLUE, szName);
		
		// Line data
		for (int j = 0; j < DataLineSize && Bytes < 0xE4; j++)
		{
			Bytes++;
			sprintf(szName, "%02X", rawData[(i * DataLineSize) + j]);
			PrintTextXY(DrawX + 105 + (j * 28), DrawY + (i + 1) * 14, DIABLO_TEXT_WHITE, szName);
		}
	}
	

	
	int Line = 0;
	*/
	/*
	sprintf(szName, "Offset: %i (0x%08X)", Offset, (int)(playerData));
	PrintTextXY(DrawX + 5, DrawY + (14 * Line++), DIABLO_TEXT_WHITE, szName);

	sprintf(szName, "Name: %s", playerData->Name);
	PrintTextXY(DrawX + 5, DrawY + (14 * Line++), DIABLO_TEXT_WHITE, szName);

	sprintf(szName, "Location: \t%i, %i", playerData->PlayerPosX, playerData->PlayerPosY);
	PrintTextXY(DrawX + 5, DrawY + (14 * Line++), DIABLO_TEXT_WHITE, szName);

	sprintf(szName, "Target:\t%i, %i", playerData->PlayerTargetX, playerData->PlayerTargetY);
	PrintTextXY(DrawX + 5, DrawY + (14 * Line++), DIABLO_TEXT_WHITE, szName);

	sprintf(szName, "Animation X: %i", playerData->AnimationScreenX);
	PrintTextXY(DrawX + 5, DrawY + (14 * Line++), DIABLO_TEXT_WHITE, szName);

	sprintf(szName, "Animation Y: %i", playerData->AnimationScreenY);
	PrintTextXY(DrawX + 5, DrawY + (14 * Line++), DIABLO_TEXT_WHITE, szName);
	
	// Line data
	for (int i = 0; i < 6; i++)
	{
		sprintf(szName, "Unknown0040[%i]: %08X", i, playerData->Unknown0040[i]);
		PrintTextXY(DrawX + 5, DrawY + (14 * Line++), DIABLO_TEXT_WHITE, szName);
	}
	*/

	/*
	sprintf(szName, "Belt address: 0x%08X", GameData.Belt());
	PrintTextXY(DrawX + 5, DrawY, DIABLO_TEXT_WHITE, szName);

	sprintf(szName, "Player struct address: 0x%08X", GameData.GetPlayerStruct());
	PrintTextXY(DrawX + 5, DrawY + 14 * 1, DIABLO_TEXT_WHITE, szName);

	PlayerData temp;
	int BeltAddress = (int)(&temp.ItemBelt[0]);
	int ActionAddress = (int)(&temp.action);
	sprintf(szName, "&belt - &action: %i", BeltAddress - ActionAddress);
	PrintTextXY(DrawX + 5, DrawY + 14 * 2, DIABLO_TEXT_WHITE, szName);

	sprintf(szName, "Adjusted struct address: 0x%08X", (GameData.Belt()) - (BeltAddress - ActionAddress));
	PrintTextXY(DrawX + 5, DrawY + 14 * 3, DIABLO_TEXT_WHITE, szName);
	*/
}