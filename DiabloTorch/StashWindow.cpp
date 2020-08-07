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

#include <Windows.h>
#include <stdio.h>
#include <fstream>
using namespace std;

//#include "Offsets.h"
#include "StashWindow.h"
#include "Drawing.h"
#include "Theme.h"
#include "GameData.h"

StashFileStruct StashFile;
StashClass Stash;

int StashTabColors[STASH_TABS_TOTAL] = { DIABLO_TEXT_WHITE, };
char* StashTabNames[STASH_TABS_TOTAL] = { 0, };

extern HWND hDiablo; // From main.cpp

void LoadStashNames()
{
	// Load Diablo.exe location
	char szDiablo[255];
	int Length = GetModuleFileName(NULL, szDiablo, 255);

	if (Length < 255 && Length > strlen("Diablo.exe"))
	{
		szDiablo[Length - strlen("Diablo.exe")] = 0;

		// Success, lets load our data
		for (int i = 0; i < STASH_TABS_TOTAL; i++)
		{
			char szStash[512];
			sprintf(szStash, "%s%s%s", szDiablo, "TorchStash", ".txt");

			ifstream fin(szStash);

			if (fin.is_open())
			{
				char szLine[255];

				while (fin.getline(szLine, 255))
				{
					bool bFoundEquals = false;
					char szVar[255] = { 0, };
					char szVal[255] = { 0, };

					if (szLine[0] == ';')
					{
						// Allow comments
						continue;
					}

					for (int c = 0; c < strlen(szLine); c++)
					{
						if (!bFoundEquals)
						{
							// Have not yet found equal sign
							if (szLine[c] == '=')
							{
								bFoundEquals = true;
							}
							else
							{
								szVar[strlen(szVar)] = szLine[c];
							}
						}
						else
						{
							// Have found equal sign, everything else is the value
							szVal[strlen(szVal)] = szLine[c];
						}
					}

					// Parse value
					char szName[32];
					char szColor[32];
					sprintf(szName, "Name%i", i);
					sprintf(szColor, "Color%i", i);

					if (stricmp(szVar, szName) == 0)
					{
						if (StashTabNames[i] != 0)
						{
							delete StashTabNames[i];
							StashTabNames[i] = 0;
						}

						StashTabNames[i] = new char[strlen(szVal) + 1];
						strcpy(StashTabNames[i], szVal);
					}
					else if (stricmp(szVar, szColor) == 0)
					{
						switch (szVal[0])
						{
						case 'G':
						case 'g':
							StashTabColors[i] = DIABLO_TEXT_GOLD;
							break;

						default:
						case 'W':
						case 'w':
							StashTabColors[i] = DIABLO_TEXT_WHITE;
							break;

						case 'R':
						case 'r':
							StashTabColors[i] = DIABLO_TEXT_RED;
							break;

						case 'B':
						case 'b':
							StashTabColors[i] = DIABLO_TEXT_BLUE;
							break;
						}
					}
				}
			}
		}
	}
}

void SaveStashNames()
{
	// Load Diablo.exe location
	char szDiablo[255];
	int Length = GetModuleFileName(NULL, szDiablo, 255);

	if (Length < 255 && Length > strlen("Diablo.exe"))
	{
		szDiablo[Length - strlen("Diablo.exe")] = 0;

		char szStash[512];
		sprintf(szStash, "%s%s%s", szDiablo, "TorchStash", ".txt");

		ofstream fout(szStash);

		if (fout.is_open())
		{
			// Success, lets load our data
			for (int i = 0; i < STASH_TABS_TOTAL; i++)
			{
				char szName[32];
				char szColor[32];

				char tabColor = 0;
				switch (StashTabColors[i])
				{
				default:
				case DIABLO_TEXT_WHITE:
					tabColor = 'W';
					break;

				case DIABLO_TEXT_GOLD:
					tabColor = 'G';
					break;

				case DIABLO_TEXT_RED:
					tabColor = 'R';
					break;

				case DIABLO_TEXT_BLUE:
					tabColor = 'B';
					break;
				}

				if (StashTabNames[i] != NULL)
				{
					sprintf(szName, "Name%i=%s\n", i, StashTabNames[i]);
					sprintf(szColor, "Color%i=%c\n", i, tabColor);

					fout.write(szName, strlen(szName));
					fout.write(szColor, strlen(szColor));
				}
			}
		}
	}
}

void LoadStash(int StashNum = 0)
{
	// Make sure stash names are up to date
	LoadStashNames();

	// Loads stash from file

	// Load Diablo.exe location
	char szDiablo[255];
	int Length = GetModuleFileName(NULL, szDiablo, 255);

	// Clear the stash first, in case we fail
	StashFile.ItemCount = 0;

	if (Length < 255)
	{
		szDiablo[Length - strlen("Diablo.exe")] = 0;

		char szStash[512];
		sprintf(szStash, "%s%s%i%s", szDiablo, "TorchStash", StashNum, ".tsf");

		ifstream fin(szStash, std::ifstream::binary);

		if (fin.is_open())
		{
			fin.read((char *)&StashFile.ItemCount, sizeof(StashFile.ItemCount));

			if (fin)
			{
				// Continue reading items
				for (int i = 0; i < StashFile.ItemCount; i++)
				{
					fin.read((char *)StashFile.Items, sizeof(Item) * StashFile.ItemCount);

					if (fin)
					{
						// Done!
					}
					else
					{
						// ERROR!
					}
				}
			}
			else
			{
				// Failed
			}

			fin.close();
		}
		else
		{
			// failed to open file
		}
	}
	else
	{
		// Faiure
	}
}

void SaveStash(int StashNum = 0)
{
	// Saves stash to file

	// Load Diablo.exe location
	char szDiablo[255];
	int Length = GetModuleFileName(NULL, szDiablo, 255);

	if (Length < 255)
	{
		szDiablo[Length - strlen("Diablo.exe")] = 0;

		char szStash[512];
		sprintf(szStash, "%s%s%i%s", szDiablo, "TorchStash", StashNum, ".tsf");

		ofstream fout(szStash, std::ofstream::binary);

		if (fout.is_open())
		{
			fout.write((char *)&StashFile.ItemCount, sizeof(StashFile.ItemCount));
			fout.write((char *)StashFile.Items, sizeof(Item) * StashFile.ItemCount);
			fout.close();
		}
		else
		{
			// failed to open file
		}
	}
	else
	{
		// Faiure
	}
}

void StashClass::SetTabName(char* Name)
{
	if (StashTabNames[Tab] != 0)
	{
		delete StashTabNames[Tab];
		StashTabNames[Tab] = 0;
	}

	StashTabNames[Tab] = new char[strlen(Name) + 1];
	strcpy(StashTabNames[Tab], Name);

	SaveStashNames();
}

void StashClass::SetTabColor(int Color)
{
	StashTabColors[Tab] = Color;

	SaveStashNames();
}

bool StashClass::IsInventorySlotClear(int x, int y)
{
	if (x >= 10 || y >= 4)
	{
		// Out of bounds
		return false;
	}

	if (GameData.InventoryMap()[x + y * 10] == 0)
	{
		return true;
	}
	
	return false;
}

bool StashClass::IsStashSlotClear(int x, int y)
{
	if (x >= STASH_GRID_COLS || y >= STASH_GRID_ROWS)
	{
		// Out of bounds
		return false;
	}

	for (int i = 0; i < StashFile.ItemCount; i++)
	{
		int Width = GameData.GetGraphicPixelWidth(StashFile.Items[i].Graphic) / 28;
		int Height = GameData.GetGraphicPixelHeight(StashFile.Items[i].Graphic) / 28;

		int ItemX = (StashFile.Items[i].x % (STASH_GRID_COLS - (Width - 1)));
		int ItemY = (StashFile.Items[i].y % (STASH_GRID_ROWS - (Height - 1)));

		if (x >= ItemX && x < ItemX + Width &&
			y >= ItemY && y < ItemY + Height)
		{
			return false;
		}
	}

	// No overlapping items found
	return true;
}

bool StashClass::InventoryHasSpaceForItem(Item Test)
{
	int Width = GameData.GetGraphicPixelWidth(Test.Graphic) / 28;
	int Height = GameData.GetGraphicPixelHeight(Test.Graphic) / 28;

	// Inventory is stored with Y = 0 being the top row. Items are achored by their bottom left slot.
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			bool HasSpace = true;
			// Test for full size item
			for (int i = 0; i < Width; i++)
			{
				for (int j = 0; j < Height; j++)
				{
					if (IsInventorySlotClear(x + i, y + j) == false)
					{
						HasSpace = false;
					}
				}
			}

			if (HasSpace)
			{
				return true;
			}
		}
	}

	return false;
}

bool StashClass::StashHasSpaceForItem(Item Test)
{
	// Stash is stored with Y = 0 being the BOTTOM row.
	int Width = GameData.GetGraphicPixelWidth(Test.Graphic) / 28;
	int Height = GameData.GetGraphicPixelHeight(Test.Graphic) / 28;

	// Inventory is stored with Y = 0 being the top row. Items are achored by their bottom left slot.
	for (int x = 0; x < STASH_GRID_COLS; x++)
	{
		for (int y = 0; y < STASH_GRID_ROWS; y++)
		{
			bool HasSpace = true;
			// Test for full size item
			for (int i = 0; i < Width; i++)
			{
				for (int j = 0; j < Height; j++)
				{
					if (IsStashSlotClear(x + i, y + j) == false)
					{
						HasSpace = false;
					}
				}
			}

			if (HasSpace)
			{
				return true;
			}
		}
	}

	return false;
}

void StashClass::StashItem(int ItemIndex)
{
	// Check item is in inventory
	if (ItemIndex != -1 && ItemIndex < 47)
	{
		// Check that the current stash tab has room
		if (StashHasSpaceForItem(GameData.Head()[ItemIndex]) == false)
		{
			// Stash full
			return;
		}

		// Item is in inventory
		int Player = GameData.GetPlayerIndex();
		Item * ItemMap = GameData.Head(); // The head item is the first in an array of 47 items.
		Item ThisItem = ItemMap[ItemIndex];


		// Add to stash
		StashFile.Items[StashFile.ItemCount] = ThisItem;

		// Setup X/Y coords
		int Width = (GameData.GetGraphicPixelWidth(ThisItem.Graphic)) / 28;
		int Height = (GameData.GetGraphicPixelHeight(ThisItem.Graphic)) / 28;

		for (int x = 0; x < STASH_GRID_COLS; x++)
		{
			for (int y = 0; y < STASH_GRID_ROWS; y++)
			{
				bool HasSpace = true;
				// Test for full size item
				for (int i = 0; i < Width; i++)
				{
					for (int j = 0; j < Height; j++)
					{
						if (IsStashSlotClear(x + i, y + j) == false)
						{
							HasSpace = false;
						}
					}
				}

				if (HasSpace)
				{
					StashFile.Items[StashFile.ItemCount].x = x;
					StashFile.Items[StashFile.ItemCount].y = y;
					goto Finished;
				}
			}
		}

		Finished:
		StashFile.ItemCount++;
		SaveStash(Tab);

		// Remove from inventory list
		if (ItemIndex < 7)
		{
			// Was equipped
			ItemMap[ItemIndex].ItemType = -1;
		}
		else
		{
			// Was in inventory
			for (int i = ItemIndex - 7; i < GameData.InventoryCount() - 1; i++)
			{
				GameData.Inventory()[i] = GameData.Inventory()[i + 1];
				GameData.Inventory()[i + 1].ItemType = -1;
			}
			//memset((void*)&GameData.Inventory()[ItemIndex - 7], 0, sizeof(Item));
			///GameData.Inventory()[ItemIndex - 7].ItemType = -1;
			char * InvCnt = (char *)(0x0068AB54 + GameData.GetPlayerOffset());
			*InvCnt = (*InvCnt) - 1;
			
			// Remove from inventory grid
			for (int i = 0; i < 40; i++)
			{
				if (GameData.InventoryMap()[i] == ItemIndex - 7 + 1 || GameData.InventoryMap()[i] == -(ItemIndex - 7 + 1))
				{
					GameData.InventoryMap()[i] = 0;
				}
				else if (GameData.InventoryMap()[i] > ItemIndex - 7 + 1)
				{
					GameData.InventoryMap()[i]--;
				}
				else if (GameData.InventoryMap()[i] < -(ItemIndex - 7 + 1))
				{
					GameData.InventoryMap()[i]++;
				}
			}
		}
	}
}

StashClass::StashClass()
{
	Tab = 0;
	LoadStash(Tab);
	HighlightedItem = -1;

	// Layout rects
	TabBar.x = 19;
	TabBar.y = 23;
	TabBar.Width = STASH_GRID_COLS * STASH_GRID_CELL_SIZE; // 10 (cols) * 28	(280 total width)
	TabBar.Height = 58; // 3 (rows) * 28 (84 total height)

	NotificationBar.x = 19;
	NotificationBar.y = TabBar.y + TabBar.Height;
	NotificationBar.Width = STASH_GRID_COLS * STASH_GRID_CELL_SIZE;
	NotificationBar.Height = 0;

	Grid.x = 19;
	Grid.y = NotificationBar.y + NotificationBar.Height;
	Grid.Height = STASH_GRID_ROWS * STASH_GRID_CELL_SIZE; // 8 * 28 (224 total height)
	Grid.Width = STASH_GRID_COLS * STASH_GRID_CELL_SIZE; // (cols) * 28)
}

StashClass::~StashClass()
{
}

void StashClass::Draw()
{
	// Quest log
	if (GameData.QuestLogActive())
	{	
		// Draw red box (this works)
		/*
		int MakeRed = (int)(0x0041B871);
		__asm
		{
			MOV EDI,0x0DB
			PUSH 0x38
			PUSH 0x38
			MOV EDX,EDI
			MOV ECX,0x205
			CALL [MakeRed]
		}
		*/

		// Tetris grid
		for (int i = 0; i < STASH_GRID_COLS; i++)
		{
			for (int j = 0; j < STASH_GRID_ROWS; j++)
			{
				if (IsStashSlotClear(i, (STASH_GRID_ROWS - 1) - j))
				{
					DrawBox(Grid.x + i * STASH_GRID_CELL_SIZE, Grid.y + j * STASH_GRID_CELL_SIZE, STASH_GRID_CELL_SIZE, STASH_GRID_CELL_SIZE, DIABLO_COLOR_BLACK);
				}
				else
				{
					DrawBox(Grid.x + i * STASH_GRID_CELL_SIZE, Grid.y + j * STASH_GRID_CELL_SIZE, STASH_GRID_CELL_SIZE, STASH_GRID_CELL_SIZE, DIABLO_COLOR_REDGRAY);
				}

				DrawFrame(Grid.x + i * STASH_GRID_CELL_SIZE, Grid.y + j * STASH_GRID_CELL_SIZE, STASH_GRID_CELL_SIZE, STASH_GRID_CELL_SIZE, DIABLO_COLOR_DARKGOLD);
			}
		}

		// Holding item on cursor
		if (*(char *)(0x004B8CD0) != 1)
		{
			// Copy cursor item
			memcpy(&HoverItem, (void *)(0x0068B700 + GameData.GetPlayerOffset()), sizeof(Item));

			POINT Cursor;
			GetCursorPos(&Cursor);
			ScreenToClient(hDiablo, &Cursor);

			//Cursor.x -= GameData.GetGraphicPixelWidth(HoverItem.Graphic);
			Cursor.x += STASH_GRID_CELL_SIZE / 2;
			Cursor.y += GameData.GetGraphicPixelHeight(HoverItem.Graphic) - STASH_GRID_CELL_SIZE / 2;

			int MouseX = (Cursor.x - Grid.x) / STASH_GRID_CELL_SIZE;
			int MouseY = 0 + ((Cursor.y - Grid.y) / STASH_GRID_CELL_SIZE);

			if (MouseX >= 0 && MouseX < STASH_GRID_COLS &&
				MouseY >= 0 && MouseY < STASH_GRID_ROWS)
			{
				Hovering = true;
				memcpy(&HoverItem, (void *)(0x0068B700 + GameData.GetPlayerOffset()), sizeof(Item));
				
				int Width = GameData.GetGraphicPixelWidth(HoverItem.Graphic) / STASH_GRID_CELL_SIZE;
				int Height = GameData.GetGraphicPixelHeight(HoverItem.Graphic) / STASH_GRID_CELL_SIZE;
				
				DrawFrame(Grid.x + MouseX * STASH_GRID_CELL_SIZE, Grid.y + (1 + MouseY - Height) * STASH_GRID_CELL_SIZE, STASH_GRID_CELL_SIZE * Width, STASH_GRID_CELL_SIZE * Height, DIABLO_COLOR_BRIGHTGOLD);

				HoverX = MouseX;
				HoverY = (STASH_GRID_ROWS - 1) - MouseY;
				
			}
		}
		else
		{
			Hovering = false;
		}

		// Tabs
		for (int i = 0; i < STASH_TABS_PER_ROW; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				char szTemp[255];
				sprintf(szTemp, "%i", 1 + i + j * STASH_TABS_PER_ROW);

				POINT Cursor;
				GetCursorPos(&Cursor);
				ScreenToClient(hDiablo, &Cursor);

				bool IsCurrentTab = (i + j * STASH_TABS_PER_ROW) == Tab;
				int TabHeight = TabBar.Height / STASH_TABS_ROWS;
				int TabWidth = TabBar.Width / STASH_TABS_PER_ROW; //28 * 2;
				Rect TabRect;
				TabRect.x = TabBar.x + i * TabWidth;
				TabRect.y = TabBar.y + j * TabHeight;
				TabRect.Width = TabWidth;
				TabRect.Height = TabHeight;

				// Draw background
				DrawBox(TabRect.x, TabRect.y, TabRect.Width, TabRect.Height, DIABLO_COLOR_BLACK);
				
				// Draw accent color
				if (PointInRect(Cursor.x, Cursor.y, TabRect))
				{
					// Mouse over tab, draw highlight
					DrawTransparentBox(TabBar.x + i * TabWidth, TabBar.y + j * TabHeight, TabWidth, TabHeight, DIABLO_COLOR_FADEDBLUE);
					DrawFrame(TabRect.x, TabRect.y, TabRect.Width, TabRect.Height, DIABLO_COLOR_DARKGOLD);
					PrintTextXYCentered(TabRect.x + TabWidth / 2, TabRect.y + 3, IsCurrentTab ? DIABLO_TEXT_GOLD : DIABLO_TEXT_BLUE, szTemp);

					// Tab name
					if (StashTabNames[i + j * STASH_TABS_PER_ROW] != 0)
					{
						DrawTransparentBox(TabBar.x, TabBar.y - TabHeight - 2, TabWidth * 10, TabHeight, DIABLO_COLOR_BLACK);
						PrintTextXYCentered(TabBar.x + TabBar.Width / 2, TabBar.y - TabHeight + 1, StashTabColors[i + j * STASH_TABS_PER_ROW], StashTabNames[i + j * STASH_TABS_PER_ROW]);
					}

					HighlightedItem = -1;
				}
				else
				{
					// Draw normally
					DrawTransparentBox(TabBar.x + i * TabWidth, TabBar.y + j * TabHeight, TabWidth, TabHeight, DIABLO_COLOR_REDGRAY);
					DrawFrame(TabRect.x, TabRect.y, TabRect.Width, TabRect.Height, DIABLO_COLOR_DARKGOLD);
					PrintTextXYCentered(TabRect.x + TabWidth / 2, TabRect.y + 3, IsCurrentTab ? DIABLO_TEXT_GOLD : DIABLO_TEXT_WHITE, szTemp);
				}
			}
		}

		// Tab highlight (the lower edge is covered when done above)
		for (int i = 0; i < STASH_TABS_PER_ROW; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (Tab == i + j * STASH_TABS_PER_ROW)
				{
					int TabHeight = TabBar.Height / STASH_TABS_ROWS;
					int TabWidth = TabBar.Width / STASH_TABS_PER_ROW; //28 * 2;
					DrawFrame(TabBar.x + i * TabWidth, TabBar.y + j * TabHeight, TabWidth, TabHeight, DIABLO_COLOR_BRIGHTGOLD);
					break;
				}
			}
		}

		// Items
		for (int i = 0; i < StashFile.ItemCount; i++)
		{
			// X = Grid.x + i * 28
			// Y = Grid.y + j * 28

			int Width = GameData.GetGraphicPixelWidth(StashFile.Items[i].Graphic) / 28; // Convert from pixel size to inventory size
			int Height = GameData.GetGraphicPixelHeight(StashFile.Items[i].Graphic) / 28; // Convert from pixel size to inventory size

			int ItemXPosition = StashFile.Items[i].x % (STASH_GRID_COLS - (Width - 1));
			int ItemYPosition = STASH_GRID_ROWS - (StashFile.Items[i].y % (STASH_GRID_ROWS - (Height - 1)));

			DrawItem(Grid.x + (StashFile.Items[i].x % (STASH_GRID_COLS - (Width - 1))) * STASH_GRID_CELL_SIZE, Grid.y + (STASH_GRID_ROWS - (StashFile.Items[i].y % (STASH_GRID_ROWS - (Height - 1)))) * STASH_GRID_CELL_SIZE, StashFile.Items[i].Graphic);
		}

		// Intructions
		DrawBox(NotificationBar.x + 1, NotificationBar.y + 1, NotificationBar.Width - 2, NotificationBar.Height - 2, DIABLO_COLOR_BLACK);
		//DrawFrame(NotificationBar.x, NotificationBar.y, NotificationBar.Width, NotificationBar.Height, DIABLO_COLOR_DARKGOLD);

		// Check current level
		//if (*(char *)(0x5BB1EE) != 0)
		if (GameData.GetCurrentLevel() != 0)
		{
			DrawTransparentBox(Grid.x, Grid.y, Grid.Width, Grid.Height, DIABLO_COLOR_BLACK);
			PrintTextXYCentered(NotificationBar.x + NotificationBar.Width / 2, NotificationBar.y + 66, DIABLO_TEXT_RED, "NOT AVAILABLE OUTSIDE TOWN");
			return;
		}

//////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// ////////
//////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// ////////
//////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// ////////
//////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// ////////
//////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// ////////
//////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// ////////
//////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// ////////
//////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// //////// ////////

		Rect ItemDetails;
		ItemDetails.x = 640 / 2 + 25;
		ItemDetails.y = 33;
		ItemDetails.Width = 640 / 2 - 50;
		ItemDetails.Height = 300;

		if (HighlightedItem != -1)
		{
			char szTextLines[16][64] = { 0, };
			int TextLines = 0;

			DrawBox(ItemDetails.x, ItemDetails.y, ItemDetails.Width, ItemDetails.Height, DIABLO_COLOR_BLACK);
			DrawFrame(ItemDetails.x, ItemDetails.y, ItemDetails.Width, ItemDetails.Height, DIABLO_COLOR_DARKGOLD);

			Item ThisItem = StashFile.Items[HighlightedItem];
			int Color = DIABLO_TEXT_WHITE;
			char szTemp[255];

			//GameData.GameFuncPrintItemDetails(&ThisItem);

			int ItemSize = *((int*)((ThisItem.Graphic + 0x0C) * 4 + 0x479658));
			DrawItem(ItemDetails.x + ItemDetails.Width / 2 - ItemSize / 2, ItemDetails.y + ItemDetails.Height - ItemDetails.Height / 8 - 5, ThisItem.Graphic);

			// Determine text color
			if (ThisItem.Magical == 01)
			{
				Color = DIABLO_TEXT_BLUE;
			}
			else if (ThisItem.Magical == 02)
			{
				Color = DIABLO_TEXT_GOLD;
			}

			// Add name
			sprintf(szTextLines[TextLines++], "%s", (ThisItem.Identified && ThisItem.Magical) ? ThisItem.NameIdentified : ThisItem.Name);

			// If weapon add damage
			if (ThisItem.EquipmentType == EQUIPTYPE_WEAPON)
			{
				if (ThisItem.DurabilityMax == 255)
					sprintf(szTextLines[TextLines++], "damage: %i-%i  Indestructible", ThisItem.MinimumDamage, ThisItem.MaximumDamage);
				else
					sprintf(szTextLines[TextLines++], "damage: %i-%i  Dur: %i/%i", ThisItem.MinimumDamage, ThisItem.MaximumDamage, ThisItem.Durability, ThisItem.DurabilityMax);
			}

			// If armor add armor
			if (ThisItem.EquipmentType == EQUIPTYPE_ARMOR)
			{
				if (ThisItem.DurabilityMax == 255)
					sprintf(szTextLines[TextLines++], "armor: %i  Indestructible", ThisItem.ArmorClass);
				else
					sprintf(szTextLines[TextLines++], "armor: %i  Dur: %i/%i", ThisItem.ArmorClass, ThisItem.Durability, ThisItem.DurabilityMax);
			}

			// If staff add charges
			//if (v1->_iMiscId == IMISC_STAFF && v1->_iMaxCharges)
			if (ThisItem.ChargesMax)
			{
				sprintf(szTextLines[TextLines++], "Charges: %i/%i", ThisItem.Charges, ThisItem.ChargesMax);
			}

			// If magical, add magical powers
			if (ThisItem.Magical == 1 && ThisItem.Identified)
			{
				// Prefix
				if (ThisItem.BonusPrefix != 255)
				{
					sprintf(szTextLines[TextLines++], "%s", GameData.GameFuncPrintItemPower(ThisItem.BonusPrefix, &ThisItem));
				}
				// Suffix
				if (ThisItem.BonusSuffix != 255)
				{
					sprintf(szTextLines[TextLines++], "%s", GameData.GameFuncPrintItemPower(ThisItem.BonusSuffix, &ThisItem));
				}
			}

			//else
			if ((ThisItem.Magical == 1 || ThisItem.Magical == 2) && !ThisItem.Identified)
			{
				sprintf(szTextLines[TextLines++], "%s", "Not Identified");
			}

			// If unique, add tag
			if (ThisItem.Magical == 2)
			{
				sprintf(szTextLines[TextLines++], "%s", "Unique Item");
			}

			// Item misc
			if (ThisItem.EquipmentType == EQUIPTYPE_CLICKABLE || ThisItem.EquipmentType == EQUIPTYPE_GOLD)
			{
				// This seems to just usually repeat something that was already listed, lets just remove it
				//sprintf(szTextLines[TextLines++], "%s", GameData.GameFuncPrintItemMisc(&ThisItem));
			}

			// Gold
			if (ThisItem.EquipmentType == EQUIPTYPE_GOLD)
			{
				sprintf(szTextLines[TextLines++], "%i pieces", ThisItem.Cost);
			}

			// Requirements
			if ((unsigned char)ThisItem.RequiredMagic + ThisItem.RequiredDexterity + ThisItem.RequiredStrength)
			{
				sprintf(szTextLines[TextLines], "%s", "Required:");

				if (ThisItem.RequiredStrength)
					sprintf(szTextLines[TextLines], "%s %i Str", szTextLines[TextLines], ThisItem.RequiredStrength);
				if (ThisItem.RequiredMagic)
					sprintf(szTextLines[TextLines], "%s %i Mag", szTextLines[TextLines], ThisItem.RequiredMagic);
				if (ThisItem.RequiredDexterity)
					sprintf(szTextLines[TextLines], "%s %i Dex", szTextLines[TextLines], ThisItem.RequiredDexterity);

				TextLines++;
			}

			// If unique, add all unique powers
			if (ThisItem.Magical == 2 && ThisItem.Identified)
			{
				if (ThisItem.UniqueItemId >= 0 && ThisItem.UniqueItemId < 91)
				{
					sprintf(szTextLines[TextLines++], "%s", " ");
					UniqueItemInfo* uif = GameData.DataUniqueItemList();// [ThisItem.UniqueItemId] ;
					uif = &uif[ThisItem.UniqueItemId];

					// Power 1
					sprintf(szTextLines[TextLines++], "%s", GameData.GameFuncPrintItemPower(uif->UIPower1, &ThisItem));

					// Power 2
					if (uif->UINumPL > 1)
						sprintf(szTextLines[TextLines++], "%s", GameData.GameFuncPrintItemPower(uif->UIPower2, &ThisItem));

					// Power 3
					if (uif->UINumPL > 2)
						sprintf(szTextLines[TextLines++], "%s", GameData.GameFuncPrintItemPower(uif->UIPower3, &ThisItem));

					// Power 4
					if (uif->UINumPL > 3)
						sprintf(szTextLines[TextLines++], "%s", GameData.GameFuncPrintItemPower(uif->UIPower4, &ThisItem));

					// Power 5
					if (uif->UINumPL > 4)
						sprintf(szTextLines[TextLines++], "%s", GameData.GameFuncPrintItemPower(uif->UIPower5, &ThisItem));

					// Power 6
					if (uif->UINumPL > 5)
						sprintf(szTextLines[TextLines++], "%s", GameData.GameFuncPrintItemPower(uif->UIPower6, &ThisItem));
				}
			}

			// Draw all item info
			for (int tl = 0; tl < TextLines; tl++)
			{
				PrintTextXYCentered(ItemDetails.x + ItemDetails.Width / 2, ItemDetails.y + 4 + (14 * tl), Color, szTextLines[tl]);
			}
		}
	}
}

void StashClass::MouseMove(int x, int y)
{
	if (GameData.QuestLogActive() == false ||
		*(char *)(0x004B8CD0) != 1)
	{
		// Stash isn't open or item is on cursor.
		HighlightedItem = -1;
		return;
	}
	
	// Check that its in our stash
	if (x > Grid.x && x < Grid.x + Grid.Width &&
		y > Grid.y && y < Grid.y + Grid.Height)
	{
		x -= Grid.x;
		y -= Grid.y;

		int MouseX = x / STASH_GRID_CELL_SIZE;
		int MouseY = STASH_GRID_ROWS - (y / STASH_GRID_CELL_SIZE);

		HighlightedItem = -1;
		for (int i = 0; i < StashFile.ItemCount; i++)
		{

			int Width = GameData.GetGraphicPixelWidth(StashFile.Items[i].Graphic) / 28; // Convert from pixel size to inventory size
			int Height = GameData.GetGraphicPixelHeight(StashFile.Items[i].Graphic) / 28; // Convert from pixel size to inventory size

			int ItemX = (StashFile.Items[i].x % (STASH_GRID_COLS - (Width - 1)));
			int ItemY = 1 + (StashFile.Items[i].y % (STASH_GRID_ROWS - (Height - 1)));

			if (MouseX >= ItemX && MouseX < ItemX + Width &&
				MouseY >= ItemY && MouseY < ItemY + Height)
			{
				HighlightedItem = i;
			}
		}
	}
	else
	{
		HighlightedItem = -1;
	}
}

bool StashClass::MouseButtonDown(int x, int y)
{
	if (GameData.QuestLogActive() == false)
	{
		// Not open
		return false;
	}
	
	int ClickedItem = -1;

	if (PointInRect(x, y, TabBar))
	{
		int TabWidth = TabBar.Width / STASH_TABS_PER_ROW; //28 * 2;
		int NewTab = ((x - TabBar.x) / TabWidth) + ((y - TabBar.y) / (TabBar.Height / 3) * STASH_TABS_PER_ROW);
		if (NewTab < (STASH_TABS_ROWS * STASH_TABS_PER_ROW))
		{
			Tab = NewTab;
			LoadStash(Tab);
			return true;
		}
	}

	// Check that its in our stash
	if (PointInRect(x + STASH_GRID_CELL_SIZE / 2, y, Grid) || PointInRect(x, y, Grid)) // Give it half a square to the left also
	{
		int MouseX = (x - Grid.x) / STASH_GRID_CELL_SIZE;
		int MouseY = (STASH_GRID_ROWS - 1) - ((y - Grid.y) / STASH_GRID_CELL_SIZE);

		if (MouseX < 0)
		{
			MouseX = 0;
		}

		ClickedItem = -1;
		for (int i = 0; i < StashFile.ItemCount; i++)
		{

			int Width = GameData.GetGraphicPixelWidth(StashFile.Items[i].Graphic) / 28; // Convert from pixel size to inventory size
			int Height = GameData.GetGraphicPixelHeight(StashFile.Items[i].Graphic) / 28; // Convert from pixel size to inventory size

			int ItemX = (StashFile.Items[i].x % (STASH_GRID_COLS - (Width - 1)));
			int ItemY = (StashFile.Items[i].y % (STASH_GRID_ROWS - (Height - 1)));

			if (MouseX >= ItemX && MouseX < ItemX + Width &&
				MouseY >= ItemY && MouseY < ItemY + Height)
			{
				ClickedItem = i;
			}
		}
	}
	else
	{
		ClickedItem = -1;
		return false;
	}

	// If not in town
	if (*(char *)(0x5BB1EE) != 0)
	{
		if (x < 640 / 2 && y < 352)
		{
			// Its in the Quest Log, so stop the click.
			return true;
		}
		else
		{
			return false;
		}
	}

	if (*(char *)(0x004B8CD0) != 1 &&
		Hovering == true &&
		(PointInRect(x + STASH_GRID_CELL_SIZE / 2, y, Grid) || PointInRect(x, y, Grid))) // Lets check normally and x+half, in case they are a little off the left side (the cursor is the very to left of the item graphic, which isn't where the user expects)
	{
		// Holding an item and clicked in the stash, drop it there.



		// Item is on cursor
		bool IsClear = true;
		int Width = GameData.GetGraphicPixelWidth(HoverItem.Graphic) / 28; // Convert from pixel size to inventory size
		int Height = GameData.GetGraphicPixelHeight(HoverItem.Graphic) / 28; // Convert from pixel size to inventory size

		for (int i = 0; i < Width; i++)
		{
			for (int j = 0; j < Height; j++)
			{
				if (IsStashSlotClear(HoverX + i, HoverY + j) == false)
				{
					IsClear = false;
					break;
				}
			}
		}

		//if (IsStashSlotClear(MouseX, MouseY))
		if (IsClear && HoverX >= 0 && HoverX < STASH_GRID_COLS && HoverY >= 0 && HoverY < STASH_GRID_ROWS)
		{
			int ClearCursor = 0x40746B;
			__asm
			{
				MOV EAX,1
				MOV ECX,1
				CALL [ClearCursor]
			}

			// Add new item to our stash
			//int MouseX = (x - 19) / 28;
			//int MouseY = 8 - ((y - 23) / 28);
			StashFile.Items[StashFile.ItemCount] = HoverItem;
			StashFile.Items[StashFile.ItemCount].x = HoverX;
			StashFile.Items[StashFile.ItemCount].y = HoverY;
			StashFile.ItemCount++;
			SaveStash(Tab);
			Hovering = false;
			return true;
		}
		else
		{
			return true;
		}
	}
	else if (ClickedItem == -1)
	{
		// Do nothing
	}
	else
	{
		// Clicked an item in the stash, pick it up.



		// This belt slot stealing method works well for 2 reaons.

		// 1: This slot in the belt gets filled at least once for all players,
		// which leaves "item residue" on the belt. This ensures no invalid items
		// are picked up.

		// 2: The belt is always visible, and as such can always be clicked. This
		// is an easy way to get an item onto the cursor without weeding through
		// Diablo's internal code.

		/*if (InventoryHasSpaceForItem(StashFile.Items[ClickedItem]) == false)
		{
			// No room
			return true;
		}*/

		// Save current belt
		Item SavedBelt[8];
		memcpy(SavedBelt, GameData.Belt(), sizeof(SavedBelt));

		// Make sure slot 1 has something in it
		GameData.Belt()[0].ItemType = 0;

		// Save cursor pos
		POINT cursor;
		GetCursorPos(&cursor);
		ScreenToClient(hDiablo, &cursor);

		// Overwrite belt graphic
		GameData.Belt()[0].Graphic = StashFile.Items[ClickedItem].Graphic;

		// Click there! (Potion slot 1)
		SendMessage(hDiablo, WM_LBUTTONDOWN, NULL, (LPARAM)0x017400DC);

		// Restore belt
		memcpy(GameData.Belt(), SavedBelt, sizeof(SavedBelt));

		// Restore cursor
		SetCursorPos(cursor.x, cursor.y);

		// Overwrite cursor item with stash item
		Item * CursorItem = (Item *)(0x0068B700 + GameData.GetPlayerOffset());
		*CursorItem = StashFile.Items[ClickedItem];

		// Check that the cursor is now holding an item
		if (*(char*)(0x004B8CD0) != 1)
		{
			// Cursor is holding an item
			// Remove it from our stash
			for (int i = ClickedItem; i < StashFile.ItemCount; i++)
			{
				StashFile.Items[i] = StashFile.Items[i + 1];
			}
			StashFile.ItemCount--;
			SaveStash(Tab);
		}
		return true;
	}

	if (x < 640 / 2 && y < 352)
	{
		// Its in the Quest Log, so stop the click.
		return true;
	}

	return false;
}

void StashClass::NextTab()
{
	Tab++;

	if (Tab >= STASH_TABS_TOTAL)
		Tab = STASH_TABS_TOTAL - 1;

	LoadStash(Tab);
}

void StashClass::PrevTab()
{
	Tab--;

	if (Tab < 0)
		Tab = 0;

	LoadStash(Tab);
}