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

#include <windows.h>
#include <WinInet.h>
#include <stdio.h>
#include <fstream>
#include "Hooks.h"
#include "Drawing.h"
using namespace std;

#include "GameData.h"

#include "Taskbar.h"
#include "OptionsWindow.h"
#include "DetailsWindow.h"
#include "StashWindow.h"
#include "ShrinesWindow.h"
#include "InspectorWindow.h"
#include "Help.h"
#include "Automap.h"

#include "Theme.h"

bool HandleChatCommand(); // In Chat.cpp

////////////////////////////////////////
// TODO / Future ideas
// -------------------
// - Put shrine description directly into Diablo text area when a shrine is highlighted, potentially just color text red for bad shrines without full desc
// X Have "show player arrows on automap" also automatcally reveal the map within a radius around other players
// - Add item on ground counter, with magic and uniques separate
// - Add monster counter
// X DONE Add life / mana text above orbs
// X DONE Add welcome screen with contact email
// - Move all hardcoded addresses into GameData.h/.cpp
// X Scroll wheel works in shops and stash (simply send arrow up / down keys, or scroll through tabs)
// X Durability meters
// - Toggle item highlight / smart highlight (alt toggles instead of hold-to-show, or improve detection of when an item enters the scree)
// P *Partial, unique counter done - List of items on ground in current level (maybe a level inspector window that can give info on this? Number of unopened chests, shrines, books, etc)
// X Current level info could be under "Level:x" text on automap for this type of info ^^
// - Mana shield notification
// - Ctrl + click auto stash / unstash (is this even useful with the full drag and drop stash?)
// - Belt potion compression (slide potions to left automatically as they get drank) (possible issue for elitist-legit players)
// - Belt potion stacking(auto fill belt from inventory) (total cheat, probably should skip)
// X Pepin sell mana potion? Hotlink for Adria? (Make it so you don't need to walk to Adria)
// - Auto pick up gold
// - Fix four digit mana / life not showing (do we need this with the actual numbers put over the orbs?)
// - Hotkey / spell key remapping
////////////////////////////////////////
// F1 help text updated to include simple instructions for Torch
// Fixed items getting lost from stash
// Stash tab height increased to 9, can now fit 3 tall items vertically
// 30 stash tabs total
// Fixed exp bar being incorrect for some character levels
// Mouse clicks fixed when windowed
// Mouse scroll wheel works in shops
// Mouse scroll wheel works in stash
// Mouse scroll wheel works to move minimap (ctrl + scroll for horizontal)
// Mouse scroll wheel click for enter (works in shops)
// Monster counter with automap text
// Contact email added
// Item durability meters added to right side
// Chat command for Adria "-adria"
// Stash descriptions are greatly improved
// Improved shrine page
// Automap exploration by other players shown in blue
// Stash tabs can be renamed with "-tab name (x)" chat command
// Stash tabs can be recolored with "-tab color (x)" chat command
// Injector reworked, no longer needs .bat file
// Injector can be launched first, will wait for Diablo to start up
// Any verion of the Torch dll can be used with any version of the injecctor

// Old hook
// 00453177		391D 487E4B00		CMP DWORD PTR DS:[4B7E48],EBX
// New hook
// 0045322A		381D F1B95200		CMP BYTE PTR DS:[52B9F1],BL
#define	DungeonHookLocation		(0x0045322A)
#define	DungeonHookLength		(6)
int		DungeonHookReturn =		(DungeonHookLocation + DungeonHookLength);

//0045FB96   381D F1B95200    CMP BYTE PTR DS:[52B9F1],BL
#define	TownHookLocation		(0x0045FB96)
#define	TownHookLength			(6)
int		TownHookReturn =		(TownHookLocation + TownHookLength);

// We use this for in-dungeon things that need to be covered by the game screens
// 00453177		391D 487E4B00		CMP DWORD PTR DS:[4B7E48],EBX
#define	DungeonLowHookLocation	(0x00453177)
#define	DungeonLowHookLength	(6)
int		DungeonLowHookReturn =	(0x00453184);//(DungeonLowHookLocation + DungeonLowHookLength); // We draw the current player's automap already, skip the Diablo automap so it doesn't cover our stuff.

void DrawSafeDungeon();
void DrawSafeLowDungeon();
void DrawSafeTown();
void DrawDungeonCallback();
void DrawLowDungeonCallback();
void DrawTownCallback();
LRESULT CALLBACK DiabloTorchProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

HWND hDiablo = NULL;
WNDPROC DiabloProc = NULL;

Hook DrawDungeonHook(DungeonHookLocation, DungeonHookLength, &DrawDungeonCallback);
Hook DrawTownHook(TownHookLocation, TownHookLength, &DrawTownCallback);
Hook DrawLowDungeonHook(DungeonLowHookLocation, DungeonLowHookLength, &DrawLowDungeonCallback);

int LastDrawTime = 0;
int LastUpdateTime = 0;
char szToast[255];
int ToastTime = 0;
bool MapHack = false;

void Toast(char* str)
{
	ToastTime = GetTickCount() + 5000;
	strncpy(szToast, str, 255);
}

void UIDrawAutomapArrows()
{
	AutomapDrawData();
}

int GetAutomapTextCount()
{
	int Lines = 0;

	if (GameData.DataMaxPlayers() > 1)
	{
		Lines++; // "GAME: " text

		char* szPlayerDescript = (char*)(0x6771C4);
		if (szPlayerDescript[0])
		{
			Lines++; // "PASSWORD: " text
		}
	}
	
	// Count "LEVEL: " text, we don't really use this in town anyways
	Lines++;

	return Lines;
}

void UIDrawClockAndInfo()
{
	// Draw toast text
	if (GetTickCount() < ToastTime && szToast != NULL)
	{
		PrintTextXYCentered(640 / 2, 36 - 14 - 14, DIABLO_TEXT_GOLD, szToast);
	}
	// Draw clock
	else if (Options.Clock)
	{
		SYSTEMTIME CurrentTime;
		GetLocalTime(&CurrentTime);

		char szTime[255];
		sprintf(szTime, "%i:%02i", (CurrentTime.wHour % 12 == 0) ? 12 : CurrentTime.wHour % 12, CurrentTime.wMinute);
		PrintTextXYCentered(640 / 2, 36 - 14 - 14, DIABLO_TEXT_GOLD, szTime);
	}

	// Draw ground item and monster stats
	if (GameData.AutomapActive() && GameData.GetCurrentLevel() != 0)
	{
		int DrawPosition = 7 + (15 * GetAutomapTextCount());
		char szText[255];

		// Get item stats
		int Items = GameData.GroundCount();
		int Magics = 0;
		int UniqueItems = 0;

		for (int i = 0; i < GameData.GroundCount(); i++)
		{
			Item ThisItem = GameData.GroundItems()[GameData.GroundMap()[i]];
			if (ThisItem.Magical == 2)
			{
				UniqueItems++;
			}
			else if (ThisItem.Magical == 1)
			{
				Magics++;
			}
		}

		// Get monster stats
		int Monsters = 0;
		int UniqueMonsters = 0;

		for (int i = 0; i < GameData.DataMonsterCount(); i++)
		{
			int* Map = GameData.DataMonsterMap();
			int Index = Map[i];
			MonsterStruct* This = GameData.DataMonsterList();
			This = &This[Index];

			//if (This->_mhitpoints > 0)
			if (stricmp(This->mName, "golem") != 0 &&
				(This->_mx != 0 && This->_my != 0))
			{
				Monsters++;

				if (This->_uniqtype)
				{
					UniqueMonsters++;
				}
			}
		}

		// It seems that monster count is really 4 when the level is clear
		// Verified by quests.cpp line 227, where the poison water supply is cleared
		//Monsters -= 4; // These are all "golem", we've checked for them by name above

		//if (Monsters > 0) // Lets show even when 0
		{
			sprintf(szText, "Monsters: %i", Monsters);
			PrintTextXY(8, DrawPosition, DIABLO_TEXT_GOLD, szText);
			DrawPosition += 15;
		}
		if (UniqueMonsters > 0)
		{
			sprintf(szText, "Unique: %i", UniqueMonsters);
			PrintTextXY(8, DrawPosition, DIABLO_TEXT_GOLD, szText);
			DrawPosition += 15;
		}
		if (UniqueItems > 0)
		{
			sprintf(szText, "Unique items: %i", UniqueItems);
			PrintTextXY(8, DrawPosition, DIABLO_TEXT_GOLD, szText);
			DrawPosition += 15;
		}

		// All items
		/*if (Items > 0)
		{
			sprintf(szText, "Items: %i", Items);
			PrintTextXY(8, DrawPosition, DIABLO_TEXT_GOLD, szText);
			DrawPosition += 14;
		}*/
		// Magic items
		/*if (Magics > 0)
		{
			sprintf(szText, "Magic: %i", Magics);
			PrintTextXY(8, DrawPosition, DIABLO_TEXT_GOLD, szText);
			DrawPosition += 14;
		}*/
	}
}

void UIDrawBatteryLevel()
{
	// Battery level
	if (GameData.AutomapActive())
	{
		SYSTEM_POWER_STATUS psBattery;
		GetSystemPowerStatus(&psBattery);
		int Percent = psBattery.BatteryLifePercent;

		if (Percent >= 100)
		{
			// Don't show
		}
		else
		{
			char szPercent[255];
			sprintf(szPercent, "%i%%", Percent);
			PrintTextXY(640 - 40, 36 - 14 - 14, DIABLO_TEXT_GOLD, szPercent);
		}
	}
}

void UIDrawExpBar()
{
	// Draw exp bar
	if (Options.ExpBar)
	{
		DrawTransparentBox(0, 345, 639 * GameData.GetLevelPercent() / 100, 5, DIABLO_COLOR_RED);
		DrawFrame(0, 345, 639, 5, 0xC8);
	}
}

void UIDrawItemsOnGround()
{
	static int LastItemCount = GameData.GroundCount();
	static int LastItemChange = GetTickCount();

	if (LastItemCount < GameData.GroundCount())
	{
		// New item dropped
		LastItemChange = GetTickCount();
	}
	else if (LastItemCount > GameData.GroundCount())
	{
		// An item has been picked up, remove highlighting
		LastItemChange = 0;
	}

	LastItemCount = GameData.GroundCount();

	// Check setting
	if (Options.AutoHighlightItems == false)
	{
		LastItemChange = 0;
	}

	// Item labels
	if (GetAsyncKeyState(VK_MENU) ||
		GetAsyncKeyState(VK_LMENU) ||
		GetAsyncKeyState(VK_RMENU) ||
		GetTickCount() - LastItemChange < 2000)
	{	
		int i = 0;

		if (GetAsyncKeyState(VK_MENU) ||
			GetAsyncKeyState(VK_LMENU) ||
			GetAsyncKeyState(VK_RMENU))
		{ /* If key is held, do nothing and start at index 0 */}
		else
		{
			// An item has dropped, start at 1 index before the end of the item array
			i = GameData.GroundCount() - 1;
		}

		for (; i < GameData.GroundCount(); i++)
		{
			//Item * ThisItem = (Item *)(0x00635A28 + 0x170 * ItemMap[i]);
			Item ThisItem = GameData.GroundItems()[GameData.GroundMap()[i]];

			//int ItemX = *(int *)(0x00635A28 + 0x170 * ItemMap[i] + 0x0C);
			//int ItemY = *(int *)(0x00635A28 + 0x170 * ItemMap[i] + 0x10);

			int CameraX = *(int *)(0x005CF33C);
			int CameraY = *(int *)(0x005CF340);

			int OffsetX = ThisItem.x - CameraX;
			int OffsetY = ThisItem.y - CameraY;

			int ScreenCenterX = 640 / 2;
			int ScreenCenterY = 352 / 2;

			if (GameData.InventoryActive() || GameData.SpellbookActive())
			{
				// Use right half
				ScreenCenterX /= 2;
			}
			else if (GameData.CharInfoActive() || GameData.QuestLogActive())
			{
				// Use left half
				ScreenCenterX /= 2;
				ScreenCenterX *= 3;
			}

			int ItemCenterX = ScreenCenterX + (OffsetX * 32) - (OffsetY * 32);
			int ItemCenterY = ScreenCenterY + (OffsetY * 16) + (OffsetX * 16);

			ItemCenterX += GameData.CameraOffsetX();
			ItemCenterY += GameData.CameraOffsetY();
			//int CharCount = strlen((char *)(0x00635A28 + 0x170 * ItemMap[i] + 0x3D));
			//char IsMagical = *(char *)(0x00635A28 + 0x170 * ItemMap[i] + 0x3C);
			int CharCount = strlen(ThisItem.Name);
			char IsMagical = ThisItem.Magical;
			int Color = 0;

			switch (IsMagical)
			{
			case 0:
				Color = DIABLO_TEXT_WHITE;
				break;

			case 1:
				Color = DIABLO_TEXT_BLUE;
				break;

			case 2:
				Color = DIABLO_TEXT_GOLD;
				break;

			default:
				Color = DIABLO_TEXT_RED;
				break;
			}

			DrawTransparentBox(ItemCenterX - (20 + CharCount * 4), ItemCenterY - 9, 40 + CharCount * 8, 18, DIABLO_COLOR_BLACK);
			DrawFrame(ItemCenterX - (20 + CharCount * 4), ItemCenterY - 9, 40 + CharCount * 8, 18, DIABLO_COLOR_BRIGHTGOLD);
			//PrintTextXYCentered(ItemCenterX, ItemCenterY - 7, (IsMagical)?DIABLO_TEXT_BLUE:DIABLO_TEXT_WHITE, (char *)(0x00635A28 + 0x170 * ItemMap[i] + 0x3D));
			PrintTextXYCentered(ItemCenterX, ItemCenterY - 7, Color, ThisItem.Name);
		}
	}
}

void UIDrawItemValues()
{
	if (Options.ShowInventoryValues == false)
	{
		return;
	}

	// Item value
	if (*(char *)(0x4B8CC0 - 8) != -1 && *(char *)(0x4B8CC0 - 8) < 47)
	{
		int ItemIndex = *(char *)(0x4B8CC0 - 8);
		char szTemp[255];

		Item * ItemMap = GameData.Head(); // The head item is the first in an array of 47 items.
		Item ThisItem = ItemMap[ItemIndex];

		POINT cursor;
		GetCursorPos(&cursor);
		ScreenToClient(hDiablo, &cursor);
		

		if (cursor.x < 640 && cursor.y < 480)
		{
			if (ThisItem.Magical)
			{
				// Magical item
				// Show normal and identified values
				int height = 50;
				int width = 70;
				int startX = cursor.x - width / 2;
				int startY = cursor.y - 10 - height;

				// Draw frame
				//DrawTransparentBox(startX, startY, width, height, DIABLO_COLOR_BLACK);
				DrawBox(startX, startY, width, height, DIABLO_COLOR_BLACK);
				DrawFrame(startX, startY, width, height, Theme.GetLineColor());
				DrawLine(startX, startY + 16, startX + width, startY + 16, Theme.GetLineColor());

				if (1)//GetTickCount() % 4000 > 2000)
				{
					// Draw sell value
					PrintTextXYCentered(cursor.x, startY + 2, DIABLO_TEXT_GOLD, "SELL");

					// Draw value
					sprintf(szTemp, "%i", ThisItem.Cost / 4);
					PrintTextXYCentered(cursor.x, startY + 20, DIABLO_TEXT_WHITE, szTemp);
					sprintf(szTemp, (ThisItem.Identified) ? "%i" : "??", ThisItem.CostIdentified / 4);
					PrintTextXYCentered(cursor.x, startY + 34, DIABLO_TEXT_BLUE, szTemp);
				}
				else
				{
					// Draw buy value
					PrintTextXYCentered(cursor.x, startY + 2, DIABLO_TEXT_GOLD, "BUY");

					// Draw value
					sprintf(szTemp, "%i", ThisItem.Cost);
					PrintTextXYCentered(cursor.x, startY + 20, DIABLO_TEXT_WHITE, szTemp);
					sprintf(szTemp, (ThisItem.Identified) ? "%i" : "??", ThisItem.CostIdentified);
					PrintTextXYCentered(cursor.x, startY + 34, DIABLO_TEXT_BLUE, szTemp);
				}
			}
			else
			{
				// Nomral item
				// Show normal value only
				int height = 50;
				int width = 70;
				int startX = cursor.x - width / 2;
				int startY = cursor.y - 10 - height;

				// Draw frame
				//DrawTransparentBox(startX, startY, width, height, DIABLO_COLOR_BLACK);
				DrawBox(startX, startY, width, height, DIABLO_COLOR_BLACK);
				DrawFrame(startX, startY, width, height, Theme.GetLineColor());
				DrawLine(startX, startY + 16, startX + width, startY + 16, Theme.GetLineColor());

				if (1)//GetTickCount() % 4000 > 2000)
				{
					// Draw sell value
					// Draw title
					PrintTextXYCentered(cursor.x, startY + 2, DIABLO_TEXT_GOLD, "SELL");

					// Draw value
					sprintf(szTemp, "%i", ThisItem.Cost / 4);
					PrintTextXYCentered(cursor.x, startY + 27, DIABLO_TEXT_WHITE, szTemp);
				}
				else
				{
					// Draw buy
					PrintTextXYCentered(cursor.x, startY + 2, DIABLO_TEXT_GOLD, "BUY");

					// Draw value
					sprintf(szTemp, "%i", ThisItem.Cost);
					PrintTextXYCentered(cursor.x, startY + 27, DIABLO_TEXT_WHITE, szTemp);
				}
			}
		}
	}
}

void UIDrawPlayerLifeBars()
{
	if (Options.PlayerLifeBars)
	{
		for (int i = 0; i < 4; i++)
		{
			PlayerData* playerData = GameData.GetPlayerStruct(i);
			PlayerData* currentPlayer = GameData.GetPlayerStruct();
			
			if (GameData.GetPlayerIndex() != i &&	// Don't draw a bar for the current player
				playerData->isActivePlayer &&		// Make sure the player is active
				playerData->DungeonLevelOn == currentPlayer->DungeonLevelOn) // Make sure they're on our level
			//if (playerData->isActivePlayer)
			{
				// Found a player, draw their data

				// Location data relative to current player
				int LocationDeltaX = playerData->PlayerPosX - currentPlayer->PlayerPosX;
				int LocationDeltaY = playerData->PlayerPosY - currentPlayer->PlayerPosY;

				if (currentPlayer->PlayerPosX < currentPlayer->PlayerTargetX &&
					currentPlayer->PlayerPosY != currentPlayer->PlayerTargetY)
				{
					// We are moving horizontally and the X coords are wrong
					LocationDeltaX--;
				}

				if (currentPlayer->PlayerPosX != currentPlayer->PlayerTargetX &&
					currentPlayer->PlayerPosY < currentPlayer->PlayerTargetY)
				{
					// We are moving horizontally and the Y coords are wrong
					LocationDeltaY--;
				}

				if (playerData->PlayerPosX < playerData->PlayerTargetX &&
					playerData->PlayerPosY != playerData->PlayerTargetY)
				{
					// They are moving horizontally and the X coords are wrong
					LocationDeltaX++;
				}

				if (playerData->PlayerPosX != playerData->PlayerTargetX &&
					playerData->PlayerPosY < playerData->PlayerTargetY)
				{
					// They are moving horizontally and the Y coords are wrong
					LocationDeltaY++;
				}


				// Calculate pixel offsets in screen x/y coords
				int PlayerOffsetX = (LocationDeltaX * 32) + (LocationDeltaY * -32) - currentPlayer->AnimationScreenX + playerData->AnimationScreenX;
				int PlayerOffsetY = (LocationDeltaX * 16) + (LocationDeltaY * 16) - currentPlayer->AnimationScreenY + playerData->AnimationScreenY;

				if (GameData.InventoryActive() || GameData.SpellbookActive())
				{
					// Use left half
					PlayerOffsetX -= (640 / 4);
				}
				else if (GameData.CharInfoActive() || GameData.QuestLogActive())
				{
					// Use right half
					PlayerOffsetX += (640 / 4);
				}

				DrawTransparentBox(300 + PlayerOffsetX, 99 + PlayerOffsetY, 40, 5, DIABLO_COLOR_BLACK);
				DrawBox(300 + PlayerOffsetX, 99 + PlayerOffsetY, 40 * playerData->LifeCurrent / playerData->LifeCurrentMax, 5, DIABLO_COLOR_RED);
				//DrawBox(300 + PlayerOffsetX, 96 + PlayerOffsetY + 5, 40 * playerData->ManaCurrent / playerData->ManaCurrentMax, 3, DIABLO_COLOR_FADEDBLUE);
				DrawFrame(300 + PlayerOffsetX, 99 + PlayerOffsetY, 40, 5, DIABLO_COLOR_DARKGOLD);
			}
		}
	}
}

void UIDrawPartyStatus()
{
	// It seems like the game doesn't sync current mana with other players
	// on a routine basis, so things just look broken when we try to do a
	// mana bar. We'll stick with life bars only for now.

	if (Options.PartyStatus)
	{
		int OffsetX = 0;
		int OffsetY = 255;

		char szText[255];

		for (int i = 0; i < 4; i++)
		{
			PlayerData* playerData = GameData.GetPlayerStruct(i);
			PlayerData* currentPlayer = GameData.GetPlayerStruct();

			if (GameData.GetPlayerIndex() != i &&
				playerData->isActivePlayer)
			{
				
				if (playerData == NULL)
				{
					continue;
				}

				// Name
				sprintf(szText, "%s", playerData->Name);
				PrintTextXY(5 + OffsetX, 32 + OffsetY, DIABLO_TEXT_GOLD, szText);

				// Location
				if (playerData->DungeonLevelOn > 0)
				{
					// In dungeon
					sprintf(szText, "Clvl: %2i\tDlvl: %4i", playerData->CharacterLevel, playerData->DungeonLevelOn);
					PrintTextXY(5 + OffsetX, 32 + OffsetY + 14, DIABLO_TEXT_GOLD, szText);
				}
				else
				{
					// In town
					sprintf(szText, "Clvl: %2i\tDlvl:TOWN", playerData->CharacterLevel);
					//sprintf(szText, "EXP: %i", playerData->Experience);
					PrintTextXY(5 + OffsetX, 32 + OffsetY + 14, DIABLO_TEXT_GOLD, szText);
				}

				// Dual life/mana bar
				DrawTransparentBox(5 + OffsetX, 32 + OffsetY + 14 * 2, 155, 6, DIABLO_COLOR_BLACK);
				DrawBox(5 + OffsetX, 32 + OffsetY + 14 * 2, (155 * playerData->LifeCurrent / playerData->LifeCurrentMax), 6, DIABLO_COLOR_RED);
				DrawFrame(5 + OffsetX, 32 + OffsetY + 14 * 2, 155, 6, DIABLO_COLOR_DARKGOLD);
			
				OffsetX += 160;
			}
		}
	}
}

void UIDrawMonsterLife()
{
	// Old test code to put individal bars over each monster
	/*
	// Monster life
	if (Options.LifeBar)
	{
		int Monster = *(int *)(0x4B8CA8);

		if (Monster == -1)
		{
		}
		else
		{
			Monster -= 18;

			int MonsterOffset = Monster * 0xE4;
			int Max = *(int *)(0x64E3C8 + MonsterOffset); // Next is 0064FD9C
			int Current = *(int *)(0x64E3CC + MonsterOffset);

			DrawTransparentBox(120, 26, 400 * Current / (Max ? Max : 1), 15, DIABLO_COLOR_RED);
			DrawFrame(120, 26, 400, 15, DIABLO_COLOR_DARKGOLD);

			char szLife[255];
			sprintf(szLife, "%i / %i", Current / 64, Max / 64);
			PrintTextXYCentered(640 / 2, 27, DIABLO_TEXT_WHITE, szLife);
		}
	}
	*/

	if (Options.LifeBar)
	{
		int Monster = *(int *)(0x4B8CA8); // Monster index pointed to by cursor

		if (Monster == -1)
		{
			// No monster highlighted
		}
		else
		{
			Monster -= 18;

			// Draw monster info
			int MonsterOffset = Monster * 0xE4;
			int MaxLife = *(int *)(0x64E3C8 + MonsterOffset); // Next is 0064FD9C
			int CurrentLife = *(int *)(0x64E3CC + MonsterOffset);


			DrawTransparentBox(120, 26, 400 * CurrentLife / (MaxLife ? MaxLife : 1), 15, DIABLO_COLOR_RED);
			DrawFrame(120, 26, 400, 15, DIABLO_COLOR_DARKGOLD);

			char szLife[255];
			sprintf(szLife, "%i / %i", CurrentLife / 64, MaxLife / 64);
			PrintTextXYCentered(640 / 2, 27, DIABLO_TEXT_WHITE, szLife);
		}
	}
}

void UIDrawWelcome()
{
	static unsigned int LastDrawTime = 0;
	static unsigned int LastWelcomeTime = 0;

	// If Torch hasn't rendered in 25 seconds, we'll assume its a new game and welcome them again
	if (GetTickCount() - LastDrawTime > 25000 ||
		GetTickCount() < 25000)
	{
		LastWelcomeTime = GetTickCount();
	}

	// Update timers
	LastDrawTime = GetTickCount();

	// Lets welcome them back
	if (GetTickCount() - LastWelcomeTime < 5000)
	{
		int ScreenCenterX = 640 / 2;
		int ScreenCenterY = 352 / 2;

		int Height = 77;
		int Width = 400;

		int Left = ScreenCenterX - Width / 2;
		int Top = ScreenCenterY - Height / 2;

		DrawTransparentBox(Left, Top, Width, Height, DIABLO_COLOR_BLACK);
		DrawFrame(Left, Top, Width, Height, DIABLO_COLOR_DARKGOLD);

		static bool StaticTextColor = true;// (rand() % 4) == 0 ? false : true;

		if (StaticTextColor)
		{
			// Static color "TORCH" text
			PrintTextXYCentered(ScreenCenterX, Top + 4, DIABLO_TEXT_RED, "TORCH");
		}
		else
		{
			// Fire color "TORCH" text
			static bool FireColor[5];
			static unsigned int FireUpdate = GetTickCount();

			// Update fire text color
			if (GetTickCount() - FireUpdate > 90) // Update every 90ms
			{
				FireUpdate = GetTickCount();

				for (int i = 0; i < 5; i++)
				{
					FireColor[i] = (rand() % 2);
				}
			}

			// Dynamic fire color "TORCH" text
			PrintTextXYCentered(ScreenCenterX - 24, Top + 4, FireColor[0] ? DIABLO_TEXT_WHITE : DIABLO_TEXT_RED, "T");
			PrintTextXYCentered(ScreenCenterX - 12, Top + 4, FireColor[1] ? DIABLO_TEXT_WHITE : DIABLO_TEXT_RED, "O");
			PrintTextXYCentered(ScreenCenterX, Top + 4, FireColor[2] ? DIABLO_TEXT_WHITE : DIABLO_TEXT_RED, "R");
			PrintTextXYCentered(ScreenCenterX + 12, Top + 4, FireColor[3] ? DIABLO_TEXT_WHITE : DIABLO_TEXT_RED, "C");
			PrintTextXYCentered(ScreenCenterX + 24, Top + 4, FireColor[4] ? DIABLO_TEXT_WHITE : DIABLO_TEXT_RED, "H");
		}

		// Slogan text
		PrintTextXYCentered(ScreenCenterX, Top + 17, DIABLO_TEXT_RED, "LIGHTING YOUR WAY THROUGH HELL");
		PrintTextXY(ScreenCenterX - 140, Top + 17, DIABLO_TEXT_WHITE, "LIGHTING YOUR WAY THROUGH");

		// Contact info
		PrintTextXYCentered(ScreenCenterX, Top + 42, DIABLO_TEXT_BLUE, "DIABLOTORCH@GMAIL.COM");
		PrintTextXYCentered(ScreenCenterX, Top + 42 + 16, DIABLO_TEXT_BLUE, "Press F1 for more help");
	}
}

void UIDrawLifeText()
{
	if (Options.LifeText > 0)
	{
		// Center of orb, needs new hook that is above UI layer
		//PrintTextXYCentered(139, 372, DIABLO_TEXT_WHITE, "196/196");

		// Just above orb
		int Life = GameData.GetPlayerStruct(GameData.GetPlayerIndex())->LifeCurrent;
		int MaxLife = GameData.GetPlayerStruct(GameData.GetPlayerIndex())->LifeCurrentMax;
		char szLife[255];

		int Mana = GameData.GetPlayerStruct(GameData.GetPlayerIndex())->ManaCurrent;
		int ManaMax = GameData.GetPlayerStruct(GameData.GetPlayerIndex())->ManaCurrentMax;
		char szMana[255];

		switch (Options.LifeText)
		{
		default:
		case 1:
			// Style: cur/max
			sprintf(szLife, "%i/%i", Life / 64, MaxLife / 64);
			sprintf(szMana, "%i/%i", Mana / 64, ManaMax / 64);
			break;

		case 2:
			// Style: cur
			sprintf(szLife, "%i", Life / 64);
			sprintf(szMana, "%i", Mana / 64);
			break;

		case 3:
			// Style: %
			sprintf(szLife, "%i%%", Life * 100 / MaxLife);
			sprintf(szMana, "%i%%", Mana * 100 / ManaMax);
			break;
		}
		
		// Since our in-town draw hook is layered above sub-screens (like inventory), lets do some light checking to try and make these less in-the-way
		if (!GameData.CharInfoActive() && !GameData.QuestLogActive())
		{
			PrintTextXYCentered(139, 325, DIABLO_TEXT_RED, szLife);
		}

		if (!GameData.SpellbookActive() && !GameData.InventoryActive())
		{
			PrintTextXYCentered(504, 325, DIABLO_TEXT_BLUE, szMana);
		}
	}
}

void UIDrawDurabilityBars()
{
	if (Options.DurabilityBars)
	{
		PlayerData* Player = GameData.GetPlayerStruct();
		Item* ThisItem = NULL;
		int Color = DIABLO_COLOR_DARKGOLD;
		int Items = 0;
		int Spacing = 1;
		int Width = 1;
		int Height = 12;

		// Right hand (normally shield hand, actually the characters left hand)
		ThisItem = &Player->ItemRightHand;
		Color = DIABLO_COLOR_BRIGHTRED;
		if (ThisItem->ItemType != 0xFFFFFFFF && ThisItem->DurabilityMax != 0)
		{
			// Check for staff
			if (ThisItem->ChargesMax > 0)
			{
				// Draw magic charges
				Color = DIABLO_COLOR_LIGHTBLUE;
				double Durability = (double)ThisItem->Charges / (double)ThisItem->ChargesMax;
				int XOffset = Items * (Width + Spacing);
				DrawOrthoLine(638 - XOffset, 325, 638 - XOffset, 325 - (Height * Durability), Color);
				DrawPixel(638 - XOffset, 325, DIABLO_COLOR_BRIGHTGOLD);
				DrawPixel(638 - XOffset, 325 - Height, DIABLO_COLOR_BRIGHTGOLD);
			}
			else
			{
				// Draw durability
				Color = DIABLO_COLOR_BRIGHTRED;
				double Durability = (double)ThisItem->Durability / (double)ThisItem->DurabilityMax;
				int XOffset = Items * (Width + Spacing);
				DrawOrthoLine(638 - XOffset, 325, 638 - XOffset, 325 - (Height * Durability), Color);
				DrawPixel(638 - XOffset, 325, DIABLO_COLOR_BRIGHTGOLD);
				DrawPixel(638 - XOffset, 325 - Height, DIABLO_COLOR_BRIGHTGOLD);
			}
			Items++;
		}

		// Body
		ThisItem = &Player->ItemChest;
		Color = DIABLO_COLOR_DARKGOLD;
		if (ThisItem->ItemType != 0xFFFFFFFF && ThisItem->DurabilityMax != 0)
		{
			double Durability = (double)ThisItem->Durability / (double)ThisItem->DurabilityMax;
			int XOffset = Items * (Width + Spacing);
			DrawOrthoLine(638 - XOffset, 325, 638 - XOffset, 325 - (Height * Durability), Color);
			DrawPixel(638 - XOffset, 325, DIABLO_COLOR_BRIGHTGOLD);
			DrawPixel(638 - XOffset, 325 - Height, DIABLO_COLOR_BRIGHTGOLD);

			Items++;
		}

		// Head
		ThisItem = &Player->ItemHead;
		Color = DIABLO_COLOR_DARKGOLD;
		if (ThisItem->ItemType != 0xFFFFFFFF && ThisItem->DurabilityMax != 0)
		{
			double Durability = (double)ThisItem->Durability / (double)ThisItem->DurabilityMax;
			int XOffset = Items * (Width + Spacing);
			DrawOrthoLine(638 - XOffset, 325, 638 - XOffset, 325 - (Height * Durability), Color);
			DrawPixel(638 - XOffset, 325, DIABLO_COLOR_BRIGHTGOLD);
			DrawPixel(638 - XOffset, 325 - Height, DIABLO_COLOR_BRIGHTGOLD);

			Items++;
		}

		// Left hand (normally weapon hand)
		ThisItem = &Player->ItemLeftHand;
		Color = DIABLO_COLOR_BRIGHTRED;
		if (ThisItem->ItemType != 0xFFFFFFFF && ThisItem->DurabilityMax != 0)
		{
			// Check for staff
			if (ThisItem->ChargesMax > 0)
			{
				// Draw magic charges
				Color = DIABLO_COLOR_LIGHTBLUE;
				double Durability = (double)ThisItem->Charges / (double)ThisItem->ChargesMax;
				int XOffset = Items * (Width + Spacing);
				DrawOrthoLine(638 - XOffset, 325, 638 - XOffset, 325 - (Height * Durability), Color);
				DrawPixel(638 - XOffset, 325, DIABLO_COLOR_BRIGHTGOLD);
				DrawPixel(638 - XOffset, 325 - Height, DIABLO_COLOR_BRIGHTGOLD);
			}
			else
			{
				// Draw durability
				Color = DIABLO_COLOR_BRIGHTRED;
				double Durability = (double)ThisItem->Durability / (double)ThisItem->DurabilityMax;
				int XOffset = Items * (Width + Spacing);
				DrawOrthoLine(638 - XOffset, 325, 638 - XOffset, 325 - (Height * Durability), Color);
				DrawPixel(638 - XOffset, 325, DIABLO_COLOR_BRIGHTGOLD);
				DrawPixel(638 - XOffset, 325 - Height, DIABLO_COLOR_BRIGHTGOLD);
			}
			Items++;
		}
	}
}

void __declspec(naked) DrawDungeonCallback()
{
	__asm
	{
		//CMP DWORD PTR DS:[004B7E48h], EBX
		CMP BYTE PTR DS:[52B9F1h],BL
		PUSHAD
		PUSHFD
	}

	DrawSafeDungeon();
	
	__asm
	{
		POPFD
		POPAD
		JMP [DungeonHookReturn]
	}
}

void __declspec(naked) DrawLowDungeonCallback()
{
	__asm
	{
		CMP DWORD PTR DS:[004B7E48h], EBX
		PUSHAD
		PUSHFD
	}

	DrawSafeLowDungeon();

	__asm
	{
		POPFD
		POPAD
		JMP[DungeonLowHookReturn]
	}
}

void __declspec(naked) DrawTownCallback()
{
	__asm
	{
		CMP BYTE PTR DS:[52B9F1h],BL
		PUSHAD
		PUSHFD
	}

	DrawSafeTown();
	
	__asm
	{
		POPFD
		POPAD
		JMP [TownHookReturn]
	}
}

void DrawSafeTown()
{
	LastDrawTime = GetTickCount();
	CountFrame();

	SaveAutomapData();
	UIDrawClockAndInfo();
	UIDrawBatteryLevel();
	UIDrawExpBar();
	UIDrawItemsOnGround();
	UIDrawItemValues();
	UIDrawLifeText();
	UIDrawDurabilityBars();

	// These go over the info screens and we don't need them in town
	//UIDrawPlayerLifeBars();
	UIDrawPartyStatus();

	// Draw stash
	Stash.Draw();

	// Draw windows
	Taskbar.Draw();

	// Lets welcome our users
	UIDrawWelcome();
}

void DrawSafeDungeon()
{
	LastDrawTime = GetTickCount();
	CountFrame();

	UIDrawItemsOnGround();
	UIDrawItemValues();
	UIDrawMonsterLife();

	// Draw stash
	Stash.Draw();

	// Draw windows
	Taskbar.Draw();

	// Lets welcome our users
	UIDrawWelcome();
}

void DrawSafeLowDungeon() // This is called after the game area has rendered, but will be covered up by inventory or character screens
{
	UIDrawAutomapArrows();
	UIDrawPlayerLifeBars();
	UIDrawPartyStatus();
	UIDrawClockAndInfo();
	UIDrawBatteryLevel();
	UIDrawExpBar();
	UIDrawLifeText();
	UIDrawDurabilityBars();
}

void UninitHooks()
{
	DrawDungeonHook.Revert();
	DrawLowDungeonHook.Revert();
	DrawTownHook.Revert();
}

void InitHooks()
{
	DrawDungeonHook.SetHook();
	DrawLowDungeonHook.SetHook();
	DrawTownHook.SetHook();
}

void ResetHooks()
{
	Sleep(5000);
	InitHooks();
}

// We need to find a place right before Diablo enters Battle.net to add a hook to this callback.
void __declspec(naked) EnterBattleNetCallback()
{
	//EnterBattleNet();

	//__asm
	//{
	//	ASM_EnterBattleNetHook
	//	JMP EnterBattleNetReturn
	//}
}

void EnterBattleNet()
{
	UninitHooks();

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ResetHooks, NULL, NULL, NULL);

	//__asm
	//{
	//	ASM_EnterBattleNetHook
	//	JMP EnterBattleNetReturn
	//}
}

void InitWndProc()
{
	DiabloProc = (WNDPROC)SetWindowLong(hDiablo, GWL_WNDPROC, (LONG)DiabloTorchProc);
}

void UninitWndProc()
{
	DiabloProc = (WNDPROC)SetWindowLong(hDiablo, GWL_WNDPROC, (LONG)DiabloProc);
}

void InitDiablo()
{
	while(!FindWindow(NULL, "Diablo"))
	{
		Sleep(100);
	}

	Sleep(250);

	hDiablo = FindWindow(NULL, "Diablo");

	if(hDiablo != NULL)
	{
#ifndef _DEBUG
		// Analytics
		// Load a blank file using a bit.ly link so we can use the bit.ly analytics to see how active Torch is
		// This bit.ly link redirects to TorchCount.txt on dropbox, which is simply a blank file.
		// TODO: We could potentially add a version number to this file and notify users when an update is available
		HINTERNET hNet = InternetOpen("DiabloTorch", PRE_CONFIG_INTERNET_ACCESS, NULL, INTERNET_INVALID_PORT_NUMBER, 0);
		HINTERNET hUrlFile = InternetOpenUrl(hNet, "http://bit.ly/13VjeKX", NULL, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_UI, 0);

		// Read the file
		char Buffer[128];
		unsigned long dwBytesRead = 0;
		BOOL bRead = InternetReadFile(hUrlFile, Buffer, 12, &dwBytesRead);
		Buffer[dwBytesRead] = 0;

		// Close URL
		InternetCloseHandle(hUrlFile);
		InternetCloseHandle(hNet);
#endif

		Sleep(250);

		// Other Torch setup
		DiabloProc = (WNDPROC)SetWindowLong(hDiablo, GWL_WNDPROC, (LONG)DiabloTorchProc);
		if(DiabloProc == NULL)
		{
			//MessageBox(NULL, "Failed to Detour Diablo WndProc!", "Error", NULL);
			Beep(1000, 1000);
		}
		else
		{
		}

		Sleep(250);

		InitHooks();

		GameData.WritePointerAtAddress((void*)GetHelpString(), (void*)(0x0041A59B + 1)); // Use our new help string
		//GameData.WriteByteAtAddress(0xAE + 8, (void*)(0x00405FB7 + 1)); // Fix four digit current mana and life
		
		//WriteByteAtAddress(0xAE + 8, (void*)(0x00405F7B + 1)); // Changes X or Y of max life, its no longer visible so not sure

		Taskbar.AddWindow(Options);
		Taskbar.AddWindow(MonsterDetails);
		//Taskbar.AddWindow(Stash); // Old windowed stash, now uses Quest screen
		Taskbar.AddWindow(Shrines);
		//Taskbar.AddWindow(PlayerInspector);
	}
	else
	{
		MessageBox(NULL, "Failed to Find Diablo Window!", "Diablo Torch", NULL);
	}
}

LRESULT CALLBACK DiabloTorchProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	//	if(!InGame())
	if (GetTickCount() - LastDrawTime > 500)
	{
		// Lets not handle anything if it has been more than 500ms since we last drew a frame
		return CallWindowProc(DiabloProc, hWnd, Message, wParam, lParam);
	}

	switch(Message)
	{
	case WM_USER + 123: // Custom message posted by DiabloStats to inform that player data has been saved
		if (wParam == 0)
		{
			LastUpdateTime = GetTickCount();
		}
		return 0;
		break;

	case WM_LBUTTONDOWN:
		{
			int DownX = LOWORD(lParam);
			int DownY = HIWORD(lParam);

			if(Taskbar.MouseButtonDown(DownX, DownY))
			{
				return 0;
			}
			else if (Stash.MouseButtonDown(DownX, DownY))
			{
				return 0;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			short Direction = HIWORD(wParam);
			short Modifiers = LOWORD(wParam);

			if (Direction > 0)
			{
				// Mouse wheel up, to make shops easier
				if (Modifiers & MK_CONTROL)
				{
					// Control is held, lefts send left arrow
					CallWindowProc(DiabloProc, hWnd, WM_KEYDOWN, VK_LEFT, NULL);
					CallWindowProc(DiabloProc, hWnd, WM_KEYUP, VK_LEFT, NULL);
				}
				else
				{
					// Control is NOT held, send up arrow
					CallWindowProc(DiabloProc, hWnd, WM_KEYDOWN, VK_UP, NULL);
					CallWindowProc(DiabloProc, hWnd, WM_KEYUP, VK_UP, NULL);
				}

				// If stash is open, lets scroll through that too
				if (GameData.QuestLogActive())
				{
					Stash.PrevTab();
				}
			}
			else
			{
				// Mouse wheel down, to make shops easier
				if (Modifiers & MK_CONTROL)
				{
					// Control is held, lets send right arrow
					CallWindowProc(DiabloProc, hWnd, WM_KEYDOWN, VK_RIGHT, NULL);
					CallWindowProc(DiabloProc, hWnd, WM_KEYUP, VK_RIGHT, NULL);
				}
				else
				{
					// Control is NOT held, lets down down arrow
					CallWindowProc(DiabloProc, hWnd, WM_KEYDOWN, VK_DOWN, NULL);
					CallWindowProc(DiabloProc, hWnd, WM_KEYUP, VK_DOWN, NULL);
				}

				// If stash is open, lets scroll through that too
				if (GameData.QuestLogActive())
				{
					Stash.NextTab();
				}
			}
		}
		break;

	case WM_MBUTTONDOWN:
		{
			// If in a shop, middle mouse button is "enter"
			// Lets limit this to in town, since I'm too lazy to figure out when a shop is actually open
			if (GameData.GetCurrentLevel() == 0)
			{
				CallWindowProc(DiabloProc, hWnd, WM_KEYDOWN, VK_RETURN, NULL);
				CallWindowProc(DiabloProc, hWnd, WM_KEYUP, VK_RETURN, NULL);
			}

		}
		break;

	case WM_MOUSEMOVE:
		{
			int NewX = LOWORD(lParam);
			int NewY = HIWORD(lParam);

			Taskbar.MouseMove(NewX, NewY);
			Stash.MouseMove(NewX, NewY);
		}
		break;
		
	case WM_LBUTTONUP:
		{
			int UpX = LOWORD(lParam);
			int UpY = HIWORD(lParam);

			Taskbar.MouseButtonUp(UpX, UpY);
		}

	case WM_KEYDOWN:
		{
			unsigned char IsChatting = GameData.IsChatBoxActive();
			if (IsChatting)
			{
				// Don't intercept chat typing

				// Unless its a chat command
				if (wParam == VK_RETURN)
				{
					char* ChatText = GameData.ChatBoxText();

					if (HandleChatCommand())
					{
						// Command was handled, clear the chat text so it doesn't get send to other players
						ChatText[0] = 0;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}
			}

			//if (wParam == '~' || wParam == '`')
			if (wParam == 192)
			{
				Taskbar.bHidden = !Taskbar.bHidden;
				//UninitWndProc();
				//UninitHooks();
				//FreeLibrary(GetModuleHandle("DiabloTorch.dll"));
			}
			else if (wParam == 'P')
			{
				Options.PartyStatus = !Options.PartyStatus;
			}
			else if (wParam == 'A')
			{
				if (GameData.GetCurrentLevel() == 0 && // In town
					GameData.DataMaxPlayers() == 1) // Single player
				{
					// Lets auto talk to adria
					GameData.GameFuncStartStore(0x05);
				}
				else
				{
					Toast("Not available in dungeon");
				}
			}
			break;
		}
	}

	return CallWindowProc(DiabloProc, hWnd, Message, wParam, lParam);
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ReasonForCall, LPVOID lpReserved)
{
	if(ReasonForCall == DLL_PROCESS_ATTACH)
	{
		// Lets beep so the player knows we have been loaded successfully
		Beep(8000, 200);
		Sleep(100);
		Beep(8000, 200);

		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InitDiablo, 0, 0, NULL); 
	}
	else if(ReasonForCall == DLL_PROCESS_DETACH)
	{
	}
	
    return TRUE;
}