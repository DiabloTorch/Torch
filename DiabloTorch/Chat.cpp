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

#include "GameData.h"
#include "StashWindow.h"
#include "Theme.h"
#include "Drawing.h"

#include <string.h>
#include <stdio.h>

// In main.cpp
void Toast(char* str);
extern bool MapHack;

#define STARTS_WITH(str1, str2) (strnicmp(str2, str1, strlen(str2)) == 0)

extern char automap[17][40][40];
bool HandleChatCommand()
{
	char* ChatText = GameData.ChatBoxText();
	char* Command = NULL;

	// Process chat commands
	Command = "-help";
	if (STARTS_WITH(ChatText, Command))
	{
		// Show some chat command help
		Toast("-tab name (name) -tab color (color) -adria -gris -wirt -cain -pepin");
		return true;
	}

	Command = "-tab";
	if (STARTS_WITH(ChatText, Command))
	{
		// This is a tab command, check which one
		if (STARTS_WITH(ChatText + strlen(Command), " color "))
		{
			// Setting current tab color
			ChatText += strlen(Command) + strlen(" color ");
			switch (ChatText[0])
			{
			default:
			case 'w':
			case 'W':
				Stash.SetTabColor(DIABLO_TEXT_WHITE);
				break;

			case 'G':
			case 'g':
				Stash.SetTabColor(DIABLO_TEXT_GOLD);
				break;

			case 'R':
			case 'r':
				Stash.SetTabColor(DIABLO_TEXT_RED);
				break;

			case 'B':
			case 'b':
				Stash.SetTabColor(DIABLO_TEXT_BLUE);
				break;
			}

			Toast("Saving tab color...");
		}
		else if (STARTS_WITH(ChatText + strlen(Command), " name "))
		{
			// Setting current tab name
			Stash.SetTabName(ChatText + strlen(Command) + strlen(" name "));
			Toast("Saving tab name...");
		}

		return true;
	}

	Command = "-adria";
	if (STARTS_WITH(ChatText, Command))
	{
		if (GameData.GetCurrentLevel() == 0)
		{
			// Lets auto talk to adria
			GameData.GameFuncStartStore(0x05);
		}
		else
		{
			Toast("Not available in dungeon");
		}

		return true;
	}
	
	Command = "-cain";
	if (STARTS_WITH(ChatText, Command))
	{
		if (GameData.GetCurrentLevel() == 0)
		{
			// Lets auto talk to adria
			GameData.GameFuncStartStore(0x0F);
		}
		else
		{
			Toast("Not available in dungeon");
		}

		return true;
	}

	Command = "-pepin";
	if (STARTS_WITH(ChatText, Command))
	{
		if (GameData.GetCurrentLevel() == 0)
		{
			// Lets auto talk to adria
			GameData.GameFuncStartStore(0x0E);
		}
		else
		{
			Toast("Not available in dungeon");
		}

		return true;
	}

	// Players have expressed concern with instant access to Wirt and Gris for item farming
	// Lets only enable this if this is a maphack build
	if (MapHack)
	{
		Command = "-wirt";
		if (STARTS_WITH(ChatText, Command))
		{
			if (GameData.GetCurrentLevel() == 0)
			{
				// Lets auto talk to adria
				GameData.GameFuncStartStore(0x0C);
			}
			else
			{
				Toast("Not available in dungeon");
			}

			return true;
		}

		Command = "-gris";
		if (STARTS_WITH(ChatText, Command))
		{
			if (GameData.GetCurrentLevel() == 0)
			{
				// Lets auto talk to adria
				GameData.GameFuncStartStore(0x01);
			}
			else
			{
				Toast("Not available in dungeon");
			}

			return true;
		}
	}

#if defined(ENABLE_MAPHACK)
	Command = "--maphack";
	if (STARTS_WITH(ChatText, Command))
	{
		MapHack = !MapHack;

		if (MapHack)
		{
			Toast("Maphack enabled");
		}

		return true;
	}
#endif

	Command = "-clearmap";
	if (STARTS_WITH(ChatText, Command))
	{
		// Show some chat command help
		memset(automap, 0, sizeof(automap));
		return true;
	}


	return false;
}