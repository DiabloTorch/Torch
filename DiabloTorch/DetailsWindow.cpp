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
#include "DetailsWindow.h"
#include "Drawing.h"
#include "Theme.h"
#include "GameData.h"

#define TRANSLATE_OFFSET(offset, monster) (offset - (0x0069DD1C - 0x64E3CC) + monster * 0xE4)

DetailsClass MonsterDetails;

DetailsClass::DetailsClass()
{
	SetX(75);
	SetY(150);

	SetWidth(180);
	SetHeight(95);
	SetTitle("Monster Details");
}

DetailsClass::~DetailsClass()
{
}

void DetailsClass::Draw()
{
	Window::Draw();

	if(GetWindowState() != Open)
		return;

	int DrawX = GetClientRect().x;
	int DrawY = GetClientRect().y;
	DrawY += 2;

	int Monster = *(int *)(0x4B8CA8);

	if (Monster == -1)
	{
		char szMessage[255];
		sprintf(szMessage, "Highlight a demon");
		PrintTextXYCentered(DrawX + GetClientRect().Width / 2, DrawY + 5, Theme.GetTextColor(), szMessage);
		return;
	}

	//Monster -= 18;

	// Lets load the first demon as a test
	int* Map = GameData.DataMonsterMap();
	int Index = Map[Monster];
	MonsterStruct* This = GameData.DataMonsterList();
	This = &This[Monster];

	// Monster name
	char szName[255];
	//sprintf(szName, "%s", (char *)(0x004B8978)); // This simply loaded the text that was in the stock text area
	//sprintf(szName, "%s (%i/%i)", This->mName, Clicks % GameData.DataMonsterCount(), GameData.DataMonsterCount());
	sprintf(szName, "%s", This->mName);
	PrintTextXY(DrawX + 5, DrawY, Theme.GetTextColor(), szName);
	
	// Monster ID
	sprintf(szName, "%i", Monster);
	//PrintTextXY(DrawX + GetClientRect().Width - 30, DrawY, Theme.GetTextColor(), szName);

	// Damage
	char szDamage[255];
	//int MinDamage = *(unsigned char *)(TRANSLATE_OFFSET(0x0069DD4D, Monster));
	//int MaxDamage = *(unsigned char *)(TRANSLATE_OFFSET(0x0069DD4E, Monster));
	sprintf(szDamage, "Damage: %i-%i", This->mMinDamage, This->mMaxDamage);
	PrintTextXY(DrawX + 5, DrawY + 14, Theme.GetTextColor(), szDamage);

	// Armor
	char szArmor[255];
	int ArmorClass = *(unsigned short *)(TRANSLATE_OFFSET(0x0069DD52, Monster));
	sprintf(szArmor, "Armor class: %i", This->mArmorClass);
	PrintTextXY(DrawX + 5, DrawY + 14 + 14, Theme.GetTextColor(), szArmor);

	int MonsterType = *(unsigned short *)(TRANSLATE_OFFSET(0x0069DD54, Monster)); // This changes for different monsters 0x72 for lvl1 skeleton, 0x10 for lava demon, etc

	// To hit
	char szHit[255];
	int ToHit = *(unsigned char *)(TRANSLATE_OFFSET(0x0069DD4C, Monster));
	sprintf(szHit, "To hit: %i%%", This->mHit);
	PrintTextXY(DrawX + 5, DrawY + 14 + 14 + 14, Theme.GetTextColor(), szHit);

	int MonsterSubtype = *(unsigned char *)(TRANSLATE_OFFSET(0x0069DD4B, Monster)); // This seems to be the subtype of each monster (0x2 for stone clan and 0x3 for fire clan)

	// Life
	int MonsterOffset = Monster * 0xE4;
	//int MaxLife = *(int *)(0x64E3C8 + MonsterOffset); // Next is 0064FD9C
	//int CurrentLife = *(int *)(0x64E3CC + MonsterOffset);
	int MaxLife = This->_mmaxhp;
	int CurrentLife = This->_mhitpoints;

	char szLife[255];
	sprintf(szLife, "Life: %i / %i", CurrentLife / 64, MaxLife / 64);
	PrintTextXY(DrawX + 5, DrawY + 14 + 14 + 14 + 14, Theme.GetTextColor(), szLife);
	
	DrawBox(DrawX + 5, DrawY + 14 + 14 + 14 + 14 + 14, (GetClientRect().Width - 10) * CurrentLife / (MaxLife?MaxLife:1), 8, DIABLO_COLOR_RED);
	DrawFrame(DrawX + 5, DrawY + 14 + 14 + 14 + 14 + 14, GetClientRect().Width - 10, 8, Theme.GetLineColor());
}