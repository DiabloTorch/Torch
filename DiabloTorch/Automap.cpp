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

#include "Automap.h"
#include "GameData.h"
#include "OptionsWindow.h"
#include "Theme.h"
#include "Drawing.h"
#include "string.h"
#include <Windows.h>

extern bool MapHack; // From main.cpp

void DrawAutomapArrowForMonster(MonsterStruct* Monster, char Color)
{
	// Change the current player number to the desired player
	PlayerData* Player = GameData.GetPlayerStruct();
	PlayerStructDiablo* Internal = (PlayerStructDiablo*)Player;
	//PlayerData SavedPlayer = *Player; // This is done once for all monsters, instead of for each individual monster (struct is HUGE)

	// We've got the struct for the current player, lets change the coords
	Player->PlayerPosX = Monster->_mx;
	Player->PlayerPosY = Monster->_my;
	Internal->_pdir = Monster->_mdir;
	Internal->_pxoff = Monster->_mxoff;
	Internal->_pyoff = Monster->_myoff;
	Internal->_pxvel = Monster->_mxvel;
	Internal->_pyvel = Monster->_myvel;
	Internal->WorldX = Monster->_mx;
	Internal->WorldY = Monster->_my;
	//Internal->_pmode = Monster->_mmode;
	Internal->_pmode = 1;

	// Change arrow render color
	unsigned char DesiredColor = Color;
	unsigned char RestoreColor = 0x99;
	unsigned char* ColorMatrix[8]; // 8 directions
	int ColorMatrixCount = 0;

	ColorMatrix[ColorMatrixCount++] = (unsigned char*)0x00402AD4; // Case 4 (up arrow)
	//ColorMatrix[ColorMatrixCount++] = (unsigned char*)0x00402B30; // Case 5
	ColorMatrix[ColorMatrixCount++] = (unsigned char*)0x00402B2A; // Case 5
	ColorMatrix[ColorMatrixCount++] = (unsigned char*)0x00402B75; // Case 6
	ColorMatrix[ColorMatrixCount++] = (unsigned char*)0x00402BC9; // Case 7
	ColorMatrix[ColorMatrixCount++] = (unsigned char*)0x00402C1A; // Case 0
	ColorMatrix[ColorMatrixCount++] = (unsigned char*)0x00402C6E; // Case 1
	ColorMatrix[ColorMatrixCount++] = (unsigned char*)0x00402CB8; // Case 2
	ColorMatrix[ColorMatrixCount++] = (unsigned char*)0x00402D07; // Case 3

	for (int i = 0; i < ColorMatrixCount; i++)
	{
		unsigned long OldProt = 0;

		/*VirtualProtect((void*)ColorMatrix[i], 1, PAGE_EXECUTE_READWRITE, &OldProt);
		RestoreColor = *ColorMatrix[i];
		*ColorMatrix[i] = DesiredColor;
		VirtualProtect((void*)ColorMatrix[i], 1, OldProt, &OldProt);*/
		RestoreColor = *ColorMatrix[i];
		GameData.WriteByteAtAddress(DesiredColor, ColorMatrix[i]);
	}

	// Re-render map
	GameData.GameFuncDrawAutomapPlr();

	// Restore map colors
	for (int i = 0; i < ColorMatrixCount; i++)
	{
		unsigned long OldProt = 0;

		/*VirtualProtect((void*)ColorMatrix[i], 1, PAGE_EXECUTE_READWRITE, &OldProt);
		*ColorMatrix[i] = RestoreColor;
		VirtualProtect((void*)ColorMatrix[i], 1, OldProt, &OldProt);*/
		GameData.WriteByteAtAddress(RestoreColor, ColorMatrix[i]);
	}

	// Restore player location
	//*Player = SavedPlayer; // See top of func
}

// This draws a blue arrow on the map for the specified player
void DrawAutomapArrowForPlayer(int i)
{
	// Change the current player number to the desired player
	int* PlayerIndex = (int*)0x686444;
	int SavedPlayer = *PlayerIndex;
	*PlayerIndex = i;

	// Change arrow render color
	unsigned char DesiredColor = DIABLO_COLOR_LIGHTBLUE;
	unsigned char RestoreColor = 0x99;
	unsigned char* ColorMatrix[8]; // 8 directions
	int ColorMatrixCount = 0;

	ColorMatrix[ColorMatrixCount++] = (unsigned char*)0x00402AD4; // Case 4 (up arrow)
	//ColorMatrix[ColorMatrixCount++] = (unsigned char*)0x00402B30; // Case 5
	ColorMatrix[ColorMatrixCount++] = (unsigned char*)0x00402B2A; // Case 5
	ColorMatrix[ColorMatrixCount++] = (unsigned char*)0x00402B75; // Case 6
	ColorMatrix[ColorMatrixCount++] = (unsigned char*)0x00402BC9; // Case 7
	ColorMatrix[ColorMatrixCount++] = (unsigned char*)0x00402C1A; // Case 0
	ColorMatrix[ColorMatrixCount++] = (unsigned char*)0x00402C6E; // Case 1
	ColorMatrix[ColorMatrixCount++] = (unsigned char*)0x00402CB8; // Case 2
	ColorMatrix[ColorMatrixCount++] = (unsigned char*)0x00402D07; // Case 3

	for (int i = 0; i < ColorMatrixCount; i++)
	{
		unsigned long OldProt = 0;

		/*VirtualProtect((void*)ColorMatrix[i], 1, PAGE_EXECUTE_READWRITE, &OldProt);
		RestoreColor = *ColorMatrix[i];
		*ColorMatrix[i] = DesiredColor;
		VirtualProtect((void*)ColorMatrix[i], 1, OldProt, &OldProt);*/
		RestoreColor = *ColorMatrix[i];
		GameData.WriteByteAtAddress(DesiredColor, ColorMatrix[i]);
	}

	// Re-render map
	GameData.GameFuncDrawAutomap();

	// Restore map colors
	for (int i = 0; i < ColorMatrixCount; i++)
	{
		unsigned long OldProt = 0;

		/*VirtualProtect((void*)ColorMatrix[i], 1, PAGE_EXECUTE_READWRITE, &OldProt);
		*ColorMatrix[i] = RestoreColor;
		VirtualProtect((void*)ColorMatrix[i], 1, OldProt, &OldProt);*/
		GameData.WriteByteAtAddress(RestoreColor, ColorMatrix[i]);
	}

	// Restore player index
	*PlayerIndex = SavedPlayer;
}

#include <math.h>
// Function to calculate distance 
float distance(int x1, int y1, int x2, int y2)
{
	// Calculating distance 
	return sqrt(pow(x2 - x1, 2) +
		pow(y2 - y1, 2) * 1.0);
}

void Toast(char* str);
char automap[17][40][40];
int automapseed[17];
int automaplocation[4];
int automaplocationtime[4];
void AutomapDrawData()
{
	if (GameData.AutomapActive())
	{
		//char automap[40][40]; // Holds player arrow reveal locations
		char tempmap[40][40]; // Temporarily holds entire game map
		//memset(automap, 0, sizeof(automap));

		// Draw arrows for each player
		if (Options.PlayerMapArrows)
		{
			for (int p = 0; p < 4; p++)
			{
				if (GameData.GetPlayerIndex() != p &&
					GameData.GetPlayerStruct(p)->isActivePlayer)// &&
					//GameData.GetPlayerStruct(p)->DungeonLevelOn == GameData.GetPlayerStruct()->DungeonLevelOn)
				{
					// Draw arrow
					if (GameData.GetPlayerStruct(p)->DungeonLevelOn == GameData.GetPlayerStruct()->DungeonLevelOn)
					{
						DrawAutomapArrowForPlayer(p);
					}

					// Manage map data
					bool bDraw = true;
					int level = GameData.GetPlayerStruct(p)->DungeonLevelOn;
					if (level == 0)
					{ 
						if (automapseed[1] != GameData.DataLvlSeedTable()[1]) // Check level 1
						{
							// Update our seeds
							Toast("Clearing automap data from town");
							memcpy(automapseed, GameData.DataLvlSeedTable(), sizeof(automapseed));
							memset(automap, 0, sizeof(automap));
						}
					}
					else if (automapseed[level] == GameData.DataLvlSeedTable()[level]) // Check that seed hasn't changed
					{
						// Seed is good
						if (automaplocation[p] == level) // Check that they haven't moved levels
						{ 
							if (GetTickCount() < automaplocationtime[p]) // If they have recently moved levels, don't save
							{
								bDraw = false;
							}
						}
						else
						{
							// They have moved levels, lets skip map data for 2.5s so we don't reveal the location of the old stairs in the new level
							automaplocationtime[p] = GetTickCount() + 2500;
							automaplocation[p] = level;
							bDraw = false;
						}
					}
					else
					{
						if (level != 0)
						{
							Toast("Clearing automap data");
							memset(automap, 0, sizeof(automap));
							//automapseed[level] = GameData.DataLvlSeedTable()[level];
							memcpy(automapseed, GameData.DataLvlSeedTable(), sizeof(automapseed));
						}
					}

					// Draw map radius
					for (int i = 0; bDraw && i < 40; i++)
					{
						for (int j = 0; j < 40; j++)
						{
							int xx = (GameData.GetPlayerStruct(p)->PlayerPosX - 16) >> 1;
							int yy = (GameData.GetPlayerStruct(p)->PlayerPosY - 16) >> 1;
							if (distance(i, j, xx, yy) < 5)
							{
								automap[GameData.GetPlayerStruct(p)->DungeonLevelOn][i][j] = 1;
							}
						}
					}
				}
			}

			// Draw arrows for each monster, if maphack is enabled in this build
			// This was used for some debugging purposes, and should not be shipped in public builds
			if (MapHack)
			{
				int Monsters = 0;
				int UniqueMonsters = 0;
				PlayerData* SavedPlayer = new PlayerData;
				*SavedPlayer = *GameData.GetPlayerStruct(); // Save player data (like position)

				for (int i = 0; i < GameData.DataMonsterCount(); i++)
				{
					int* Map = GameData.DataMonsterMap();
					int Index = Map[i];
					MonsterStruct* This = GameData.DataMonsterList();
					This = &This[Index];

					if (This->_uniqtype)
					{
						// Unique monster
						DrawAutomapArrowForMonster(This, DIABLO_COLOR_WHITE);
					}
					else
					{
						// Normal monster
						DrawAutomapArrowForMonster(This, DIABLO_COLOR_BRIGHTRED);
					}
				}

				// Restore player info
				*GameData.GetPlayerStruct() = *SavedPlayer; // See top of func
				delete SavedPlayer;
			}
		}

		// Lets save map data
		memcpy(tempmap, GameData.DataAutomapExploreArray(), sizeof(tempmap));
		
		for (int i = 0; i < 40; i++)
		{
			for (int j = 0; j < 40; j++)
			{
				if (MapHack)
				{
					// Full map
					GameData.DataAutomapExploreArray()[i * 40 + j] = !tempmap[i][j];
				}
				else
				{
					// Player arrow radii only
					GameData.DataAutomapExploreArray()[i * 40 + j] = automap[GameData.GetPlayerStruct()->DungeonLevelOn][i][j];
				}
			}
		}

		// Render blue map
		//SetAutomapRenderColor(DIABLO_COLOR_LIGHTBLUE + 1, DIABLO_COLOR_LIGHTBLUE);
		SetAutomapRenderColor(DIABLO_COLOR_LIGHTBLUE + 48 + 8, DIABLO_COLOR_LIGHTBLUE);
		GameData.GameFuncDrawAutomap();
		SetAutomapRenderColor(DIABLO_COLOR_DARKGOLD, DIABLO_COLOR_BRIGHTGOLD); // Restore original colors

		// Restore map data
		memcpy(GameData.DataAutomapExploreArray(), tempmap, sizeof(tempmap));
	
		// Render one more time to make sure the red arrow is always on top
		GameData.GameFuncDrawAutomap();
	}
}

void SaveAutomapData() // Called while in town
{
	if (GameData.AutomapActive())
	{
		//char automap[40][40]; // Holds player arrow reveal locations
		char tempmap[40][40]; // Temporarily holds entire game map
		//memset(automap, 0, sizeof(automap));

		if (Options.PlayerMapArrows)
		{
			for (int p = 0; p < 4; p++)
			{
				if (GameData.GetPlayerIndex() != p &&
					GameData.GetPlayerStruct(p)->isActivePlayer)// &&
					//GameData.GetPlayerStruct(p)->DungeonLevelOn == GameData.GetPlayerStruct()->DungeonLevelOn)
				{
					// Manage map data
					bool bDraw = true;
					int level = GameData.GetPlayerStruct(p)->DungeonLevelOn;
					if (level == 0)
					{
						if (automapseed[1] != GameData.DataLvlSeedTable()[1]) // Check level 1
						{
							// Update our seeds
							Toast("Clearing automap data from town");
							memcpy(automapseed, GameData.DataLvlSeedTable(), sizeof(automapseed));
							memset(automap, 0, sizeof(automap));
						}
					}
					else if (automapseed[level] == GameData.DataLvlSeedTable()[level]) // Check that seed hasn't changed
					{
						// Seed is good
						if (automaplocation[p] == level) // Check that they haven't moved levels
						{
							if (GetTickCount() < automaplocationtime[p]) // If they have recently moved levels, don't save
							{
								bDraw = false;
							}
						}
						else
						{
							// They have moved levels, lets skip map data for 2.5s so we don't reveal the location of the old stairs in the new level
							automaplocationtime[p] = GetTickCount() + 2500;
							automaplocation[p] = level;
							bDraw = false;
						}
					}
					else
					{
						if (level != 0)
						{
							Toast("Clearing automap data");
							memset(automap, 0, sizeof(automap));
							//automapseed[level] = GameData.DataLvlSeedTable()[level];
							memcpy(automapseed, GameData.DataLvlSeedTable(), sizeof(automapseed));
						}
					}

					// Draw map radius
					for (int i = 0; bDraw && i < 40; i++)
					{
						for (int j = 0; j < 40; j++)
						{
							int xx = (GameData.GetPlayerStruct(p)->PlayerPosX - 16) >> 1;
							int yy = (GameData.GetPlayerStruct(p)->PlayerPosY - 16) >> 1;
							if (distance(i, j, xx, yy) < 5)
							{
								automap[GameData.GetPlayerStruct(p)->DungeonLevelOn][i][j] = 1;
							}
						}
					}
				}
			}
		}
	}
}

void SetAutomapRenderColor(unsigned char DarkColor, unsigned char BrightColor)
{
	// Locations of bright stairs color
/*		004023BD | . 68 90000000    PUSH 90; / Arg3 = 00000090
		004023E5 | . 68 90000000    PUSH 90; / Arg3 = 00000090
		0040240F | . 68 90000000    PUSH 90; / Arg3 = 00000090
		00402440 | . 68 90000000    PUSH 90; / Arg3 = 00000090
		00402547 | . 68 90000000    PUSH 90; / Arg3 = 00000090
		0040255D | . 68 90000000    PUSH 90; / Arg3 = 00000090
		00402570 | . 68 90000000    PUSH 90; / Arg3 = 00000090
		00402585 | . 68 90000000    PUSH 90; / Arg3 = 00000090
		004026BC | . 68 90000000    PUSH 90; / Arg3 = 00000090
		004026D0 | . 68 90000000    PUSH 90; / Arg3 = 00000090
		004026E5 | . 68 90000000    PUSH 90; / Arg3 = 00000090
		004026F8 | . 68 90000000    PUSH 90; / Arg3 = 00000090
		0040282E | . 68 90000000    PUSH 90; / Arg3 = 00000090
		00402844 | . 68 90000000    PUSH 90; / Arg3 = 00000090
		00402857 | . 68 90000000    PUSH 90; / Arg3 = 00000090

		0040291B | .BE 90000000    MOV ESI, 90
		00402920 | . 56             PUSH ESI; / Arg3 = > 00000090
		00402930 | . 56             PUSH ESI; / Arg3
		00402941 | . 56             PUSH ESI; / Arg3
*/

	GameData.WriteByteAtAddress(BrightColor, (void*)(0x004023BD + 1));
	GameData.WriteByteAtAddress(BrightColor, (void*)(0x004023E5 + 1));
	GameData.WriteByteAtAddress(BrightColor, (void*)(0x0040240F + 1));
	GameData.WriteByteAtAddress(BrightColor, (void*)(0x00402440 + 1));
	GameData.WriteByteAtAddress(BrightColor, (void*)(0x00402547 + 1));
	GameData.WriteByteAtAddress(BrightColor, (void*)(0x0040255D + 1));
	GameData.WriteByteAtAddress(BrightColor, (void*)(0x00402570 + 1));
	GameData.WriteByteAtAddress(BrightColor, (void*)(0x00402585 + 1));
	GameData.WriteByteAtAddress(BrightColor, (void*)(0x004026BC + 1));
	GameData.WriteByteAtAddress(BrightColor, (void*)(0x004026D0 + 1));
	GameData.WriteByteAtAddress(BrightColor, (void*)(0x004026E5 + 1));
	GameData.WriteByteAtAddress(BrightColor, (void*)(0x004026F8 + 1));
	GameData.WriteByteAtAddress(BrightColor, (void*)(0x0040282E + 1));
	GameData.WriteByteAtAddress(BrightColor, (void*)(0x00402844 + 1));
	GameData.WriteByteAtAddress(BrightColor, (void*)(0x00402844 + 1));
	GameData.WriteByteAtAddress(BrightColor, (void*)(0x0040291B + 1));

	// Locations of dark walls color
/*		0040224D | .BE C8000000    MOV ESI, 0C8
		00402486 | . 56             PUSH ESI; / Arg3
		004024A9 | . 56             PUSH ESI; / Arg3
		004024B9 | . 56             PUSH ESI; / Arg3
		004024F6 | . 56             PUSH ESI; / Arg3
		00402530 | . 56             PUSH ESI; / Arg3
		004025AB | . 56             PUSH ESI; / Arg3
		004025D9 | . 56             PUSH ESI; / Arg3
		004025FC | . 56             PUSH ESI; / Arg3
		0040260C | . 56             PUSH ESI; / Arg3
		0040261C | . 56             PUSH ESI; / Arg3
		0040263E | . 56             PUSH ESI; / Arg3
		00402665 | . 56             PUSH ESI; / Arg3
		004026A4 | . 56             PUSH ESI; / Arg3
		00402713 | . 56             PUSH ESI; / Arg3
		0040274D | . 56             PUSH ESI; / Arg3
		00402770 | . 56             PUSH ESI; / Arg3
		00402780 | . 56             PUSH ESI; / Arg3
		00402790 | . 56             PUSH ESI; / Arg3
		004027AF | . 56             PUSH ESI; / Arg3
		004027C7 | . 56             PUSH ESI; / Arg3
		00402817 | . 56             PUSH ESI; / Arg3
		004028B8 | . 56             PUSH ESI; / Arg3
		00402903 | . 56             PUSH ESI; / Arg3
*/

	GameData.WriteByteAtAddress(DarkColor, (void*)(0x0040224D + 1));
}