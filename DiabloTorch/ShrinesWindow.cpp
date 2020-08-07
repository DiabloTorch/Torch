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

#include "ShrinesWindow.h"
#include "Drawing.h"
#include "Theme.h"
#include "windows.h"

extern HWND hDiablo; // From main.cpp

ShrinesClass Shrines;

#define SHRINE_COUNT 25
Shrine ShrineList[25] = {
	{ DIABLO_TEXT_WHITE,	"ABANDONDED",	"+2 DEXTERITY",												"THE HANDS OF MEN MAY BE GUIDED BY FATE"},
	{ DIABLO_TEXT_WHITE,	"CREEPY",		"+2 STRENGTH",												"STRENGTH IS BOLSTERED BY HEAVENLY FAITH"},
	{ DIABLO_TEXT_WHITE,	"CRYPTIC",		"CASTS NOVA AND FILLS MANA",								"ARCANE POWER BRINGS DESTRUCTION"},
	{ DIABLO_TEXT_WHITE,	"DIVINE",		"FULL LIFE AND MANA AND GIVES 2 POTIONS",					"DRINK AND BE REFRESHED"},
	{ DIABLO_TEXT_WHITE,	"EERIE",		"+2 MAGIC",													"KNOWELDGE AND WISOM AT THE COST OF SELF"},
	{ DIABLO_TEXT_WHITE,	"ELDRITCH",		"ALL POTIONS BECOME REJUVENATION",							"CRIMSON AND AZURE BECOME AS THE SUN"},
	{ DIABLO_TEXT_WHITE,	"ENCHANTED",	"RANDOM SPELL -1 LEVEL, ALL OTHERS +1 LEVEL",				"MAGIC IS NOT ALWAYS WHAT IT SEEMS TO BE"},
	{ DIABLO_TEXT_RED,		"FASCINATING",	"FIREBOLT +2 LEVELS, LOSE 10% MAXIMUM MANA",				"INTENSITY COMES AT THE COST OF WISOM"},
	{ DIABLO_TEXT_WHITE,	"GLIMMERING",	"ALL ITEMS IN INVENTORY IDENTIFIED",						"MYSTERIES ARE REVEALED IN THE LIGHT OF REASON"},
	{ DIABLO_TEXT_WHITE,	"GLOOMY",		"+2 AC ALL ARMOR, -1 WEAPONS MAX DAMAGE",					"THOSE WHO DEFEND SELDOM ATTACK"},
	{ DIABLO_TEXT_WHITE,	"HIDDEN",		"-10 DUR ONE EQUIPPED ITEM, +10 DUR TO OTHERS",				"NEW STRENGTH IS FORGED THROUGH DESTRUCTION"},
	{ DIABLO_TEXT_WHITE,	"HOLY",			"CASTS PHASING",											"WHEREVER YOU GO, THERE YOU ARE"},
	{ DIABLO_TEXT_WHITE,	"MAGICAL",		"CASTS MANA SHIELD",										"WHILE THE SPIRIT IS VIGILANT, THE BODY THRIVES"},
	{ DIABLO_TEXT_WHITE,	"MYSTERIOUS",	"+5 TO RANDOM STAT, -1 TO OTHERS",							"SOME ARE WEAKENED WHILE ONE GROWS STRONG"},
	{ DIABLO_TEXT_RED,		"ORNATE",		"HOLY BOLT +2 LEVELS, LOSE 10% MAXIMUM MANA",				"SALVATION COMES AT THE COST OF WISOM"},
	{ DIABLO_TEXT_WHITE,	"QUIET",		"+2 VITALITY",												"THE ESSENCE OF LIFE FLOWS FROM WITHIN"},
	{ DIABLO_TEXT_WHITE,	"RELIGIOUS",	"REAPIRS ALL ITEMS IN INVENTORY",							"TIME CANNOT DIMINISH THE POWER OF STEEL"},
	{ DIABLO_TEXT_RED,		"SACRED",		"CHARGED BOLT +2 LEVELS, LOSE 10% MAXIMUM MANA",			"ENERGY COMES AT THE COST OF WISDOM"},
	{ DIABLO_TEXT_WHITE,	"SECLUDED",		"COMPLETES AUTOMAP",										"THE WAY IS MADE CLEAR WHEN VIEWED FROM ABOVE"},
	{ DIABLO_TEXT_WHITE,	"SPIRITUAL",	"FILLS EMPTY INVENTORY SLOTS WITH GOLD",					"RICHES ABOUND WHEN LEAST EXPECTED"},
	{ DIABLO_TEXT_WHITE,	"SPOOKY",		"RESTORES OTHER PLAYER'S LIFE AND MANA",					"WHERE AVARICE FAILS, PATIENCE GAINS REWARD"},
	{ DIABLO_TEXT_WHITE,	"STONE",		"RECHARGES ALL STAVES",										"THE POWERS OF MANA REFOCUSED RENEWS"},
	{ DIABLO_TEXT_RED,		"TAINTED",		"OTHER PLAYERS GET +1 TO RANDOM STAT, -1 TO OTHERS",		"THOSE WHO ARE LAST MAY YET BE FIRST"},
	{ DIABLO_TEXT_WHITE,	"THAUMATURGIC",	"ALL CHESTS ON CURRENT LEVEL RESTOCKED",					"WHAT ONCE WAS OPENED NOW IS CLOSED"},
	{ DIABLO_TEXT_WHITE,	"WEIRD",		"+1 MAX DAMAGE TO ALL WEAPONS IN INVENTORY",				"THE SWORD OF JUSTICE IS SWIFT AND SHARP"},
};

#define SHRINE_ROWS		5
#define SHRINE_COLS		5
#define SHRINE_SPACING	3
#define SHRINE_WIDTH	112
#define SHRINE_HEIGHT	16

ShrinesClass::ShrinesClass()
{
	int CalcWidth = (SHRINE_WIDTH * SHRINE_COLS) + (SHRINE_SPACING * (SHRINE_COLS + 1));
	
	SetX((640 - CalcWidth) / 2);
	SetY(25);

	SetWidth(CalcWidth);
	SetHeight(15 + (SHRINE_HEIGHT * (SHRINE_ROWS + 1)) + (SHRINE_SPACING * SHRINE_ROWS) + 80);
	SetTitle("Shrines");
}

ShrinesClass::~ShrinesClass()
{
}

Rect ShrinesClass::GetRectForShrine(int Shrine)
{
	Rect ShrineRect;

	// Set size
	ShrineRect.Width = SHRINE_WIDTH;
	ShrineRect.Height = SHRINE_HEIGHT;

	// Position of first rect
	ShrineRect.x = SHRINE_SPACING;
	ShrineRect.y = 15;

	// Offset for rows / cols
	ShrineRect.x += (Shrine / (25 / SHRINE_COLS)) * (SHRINE_WIDTH + SHRINE_SPACING);
	ShrineRect.y += (Shrine % (25 / SHRINE_ROWS)) * (SHRINE_HEIGHT + SHRINE_SPACING);

	return ShrineRect;
}

void ShrinesClass::Draw()
{
	Window::Draw();

	if (GetWindowState() != Open)
		return;

	// Shrine label
	PrintTextXYCentered(GetClientRect().x + (GetClientRect().Width / 2), GetClientRect().y + 2, DIABLO_TEXT_GOLD, "SHRINES");

	int SelectedShrine = -1;

	// Shrine list
	for (int i = 0; i < SHRINE_COUNT; i++)
	{
		Rect ShrineRect = GetRectForShrine(i);

		ShrineRect.x += GetClientRect().x;
		ShrineRect.y += GetClientRect().y;

		DrawBox(ShrineRect.x, ShrineRect.y, ShrineRect.Width, ShrineRect.Height, DIABLO_COLOR_BLACK);
		DrawFrame(ShrineRect.x, ShrineRect.y, ShrineRect.Width, ShrineRect.Height, DIABLO_COLOR_DARKGOLD);
		PrintTextXYCentered(ShrineRect.x + (ShrineRect.Width / 2), ShrineRect.y + 2, ShrineList[i].TextColor, ShrineList[i].Name);

		// If mouse is in rect
		POINT cursor;
		GetCursorPos(&cursor);
		ScreenToClient(hDiablo, &cursor);

		if (PointInRect(cursor.x, cursor.y, ShrineRect))
		{
			SelectedShrine = i;
		}
	}

	// Effect and message
	Rect LowestRect = GetRectForShrine(SHRINE_COUNT - 1);

	int y = GetClientRect().y + LowestRect.y + LowestRect.Height + SHRINE_SPACING;
	y += 14;

	// Draw message
	PrintTextXYCentered(GetClientRect().x + (GetClientRect().Width / 2), y, DIABLO_TEXT_GOLD, "MESSAGE");
	y += 12 + SHRINE_SPACING;

	if (SelectedShrine == -1)
	{
		// No shrine selected
		DrawBox(GetClientRect().x + SHRINE_SPACING, y, GetClientRect().Width - (SHRINE_SPACING * 2), SHRINE_HEIGHT, DIABLO_COLOR_BLACK);
		PrintTextXYCentered(GetClientRect().x + (GetClientRect().Width / 2), y + 2, DIABLO_TEXT_BLUE, "SELECT A SHRINE");
		DrawTransparentBox(GetClientRect().x + SHRINE_SPACING, y, GetClientRect().Width - (SHRINE_SPACING * 2), SHRINE_HEIGHT, DIABLO_COLOR_BLACK); // Gray out text
	}
	else
	{
		// Shrine selected
		DrawBox(GetClientRect().x + SHRINE_SPACING, y, GetClientRect().Width - (SHRINE_SPACING * 2), SHRINE_HEIGHT, DIABLO_COLOR_BLACK);
		PrintTextXYCentered(GetClientRect().x + (GetClientRect().Width / 2), y + 2, DIABLO_TEXT_BLUE, ShrineList[SelectedShrine].Message);
	}

	DrawFrame(GetClientRect().x + SHRINE_SPACING, y, GetClientRect().Width - (SHRINE_SPACING * 2), SHRINE_HEIGHT, DIABLO_COLOR_DARKGOLD);

	// Draw effect
	y += SHRINE_HEIGHT + SHRINE_SPACING;
	PrintTextXYCentered(GetClientRect().x + (GetClientRect().Width / 2), y, DIABLO_TEXT_GOLD, "Effect");
	y += 12 + SHRINE_SPACING;

	if (SelectedShrine == -1)
	{
		// No shrine selected
		DrawBox(GetClientRect().x + SHRINE_SPACING, y, GetClientRect().Width - (SHRINE_SPACING * 2), SHRINE_HEIGHT, DIABLO_COLOR_BLACK);
		PrintTextXYCentered(GetClientRect().x + (GetClientRect().Width / 2), y + 2, DIABLO_TEXT_BLUE, "SELECT A SHRINE");
		DrawTransparentBox(GetClientRect().x + SHRINE_SPACING, y, GetClientRect().Width - (SHRINE_SPACING * 2), SHRINE_HEIGHT, DIABLO_COLOR_BLACK); // Gray out text
	}
	else
	{
		// Shrine selected
		DrawBox(GetClientRect().x + SHRINE_SPACING, y, GetClientRect().Width - (SHRINE_SPACING * 2), SHRINE_HEIGHT, DIABLO_COLOR_BLACK);
		PrintTextXYCentered(GetClientRect().x + (GetClientRect().Width / 2), y + 2, DIABLO_TEXT_BLUE, ShrineList[SelectedShrine].Effect);
	}

	DrawFrame(GetClientRect().x + SHRINE_SPACING, y, GetClientRect().Width - (SHRINE_SPACING * 2), SHRINE_HEIGHT, DIABLO_COLOR_DARKGOLD);

	/*for (int i = 0; i < ShrineCount; i++)
	{
		// Name
		sprintf(szMessage, "%s", ShrineList[i].Name);
		//PrintTextXYCentered(ShrineCenter, DrawY + 1, ShrineList[i].TextColor, szMessage);
		PrintTextXY(DrawX + 5, DrawY + 1, ShrineList[i].TextColor, szMessage);

		// Effect
		sprintf(szMessage, "%s", ShrineList[i].Effect);
		//PrintTextXYCentered(EffectCenter, DrawY + 2, ShrineList[i].TextColor, szMessage);
		PrintTextXY(DrawX + Divider + 5, DrawY + 1, ShrineList[i].TextColor, szMessage);

		// Line
		DrawLine(GetClientRect().x, DrawY + 14, GetClientRect().x + GetClientRect().Width, DrawY + 14, Theme.GetLineColor());
		DrawY += 12;
	}*/
}