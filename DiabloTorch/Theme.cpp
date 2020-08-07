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

#include "Theme.h"
#include "Drawing.h"
#include "Window.h"
#include <windows.h>
#include <stdio.h>

ThemeClass Theme;

/* Theme Color List */
/*
64		Dark Gray
0x54	White
41		Dark Blue
100		Light Pink
0x75	Neon Green RGB(0, 255, 0)
50		Light Blue
120		Blue-Gray
110		Very Light Rust
105		Light Rust
90		Dark Purple
70		Light Tan (Badlands Dirt Color)
95		Dark Salmon

  /////////
14	Dark Purple
15	Very Dark Purple
16	Dark Brown
17	Very Dark Orange/Rust Brown
18	Dark Rust Brown
19	Rust Brown
20	Light Brown
21	Brown

23	Dark Red



27	Orange





33	Light Yellow

35	Teal Green
36	Teal
// Teals
39	Light Teal
40	Very Very Dark Blue
41	Very Dark Blue
42	Dark Blue


45	Light Blue



49	Bright Blue

51	Lavender
61	Yellow
94	Very Dark Red
98	Dark Red
100	Sammon Pink
102	Light Pink
111	Red
117	Green (RGB(0, 255, 0)
170 Extremely Dark Red
171	Dark Red
172	Rust Red
174	Light Red


*/

int ThemeClass::GetBackgroundColor()
{
/*	static int Current = 0;
	static int LastUpdate = GetTickCount();

	if(GetAsyncKeyState(VK_F3) && GetTickCount() - LastUpdate > 100)
	{
		Current++;
		LastUpdate = GetTickCount();
	}

	char szColor[255];
	sprintf(szColor, "Current Color Index: %i", Current);
	PrintTextXY(5, 5, szColor);

	return Current;*/
	return BackgroundColor[CurrentTheme];
}

ThemeClass::ThemeClass()
{
	CurrentTheme = 0;
	
	SetTheme = 0;
	ThemeTitles[SetTheme] = "Default";
//	BackgroundColor[SetTheme] = DIABLO_COLOR_REDGRAY;
	BackgroundColor[SetTheme] = DIABLO_COLOR_BLACK;
	LineColor[SetTheme] = DIABLO_COLOR_DARKGOLD;
	TitleBarColor[SetTheme] = DIABLO_COLOR_BLACK;
	TitleTextColor[SetTheme] = 3; // Gold
	TextColor[SetTheme] = 0; // White

	SetTheme = 1;
	ThemeTitles[SetTheme] = "Fire";
	BackgroundColor[SetTheme] = DIABLO_COLOR_REDGRAY;
	LineColor[SetTheme] = DIABLO_COLOR_FADEDRED;
	TitleBarColor[SetTheme] = DIABLO_COLOR_RED;
	TitleTextColor[SetTheme] = 3;
	TextColor[SetTheme] = DIABLO_TEXT_RED;

	SetTheme = 2;
	ThemeTitles[SetTheme] = "Ice";
	BackgroundColor[SetTheme] = DIABLO_COLOR_FADEDBLUE;
	LineColor[SetTheme] = DIABLO_COLOR_BLUE;
	TitleBarColor[SetTheme] = DIABLO_COLOR_FADEDBLUE;
	TitleTextColor[SetTheme] = 3;
	TextColor[SetTheme] = DIABLO_TEXT_BLUE;

	SetTheme = 3;
	ThemeTitles[SetTheme] = "Gold";
	BackgroundColor[SetTheme] = DIABLO_COLOR_BLACK;
	LineColor[SetTheme] = DIABLO_COLOR_BRIGHTGOLD;
	TitleBarColor[SetTheme] = DIABLO_COLOR_BLACK;
	TitleTextColor[SetTheme] = DIABLO_TEXT_GOLD;
	TextColor[SetTheme] = DIABLO_TEXT_GOLD;
}

ThemeClass::~ThemeClass()
{ }
