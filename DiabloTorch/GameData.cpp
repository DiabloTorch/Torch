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
#include "Windows.h""

GameDataClass GameData;

// This overwrites a Diablo pointer with one of our own, with the original Diablo pointer located at Address
void GameDataClass::WritePointerAtAddress(void* Pointer, void* Address)
{
	unsigned long OldProt = 0;

	VirtualProtect((void*)Address, sizeof(Pointer), PAGE_EXECUTE_READWRITE, &OldProt);
	memcpy((char*)Address, &Pointer, sizeof(Pointer));
	VirtualProtect((void*)Address, sizeof(Pointer), OldProt, &OldProt);
}

// This overwrites a single Diablo byte
void GameDataClass::WriteByteAtAddress(unsigned char Byte, void* Address)
{
	unsigned long OldProt = 0;

	VirtualProtect((void*)Address, sizeof(Byte), PAGE_EXECUTE_READWRITE, &OldProt);
	memcpy((char*)Address, &Byte, sizeof(Byte));
	VirtualProtect((void*)Address, sizeof(Byte), OldProt, &OldProt);
}

int GameDataClass::GetPlayerIndex()
{
	return *(int *)(0x686444);
}

int GameDataClass::GetPlayerOffset(int PlayerIndex/* = -1*/)
{
	return ((PlayerIndex == -1)?GetPlayerIndex():PlayerIndex) * GetPlayerDataSize();
}

PlayerData* GameDataClass::GetPlayerStruct(int PlayerIndex /*= -1*/)
{
	//return (PlayerData*)(0x686588 - 102 + GetPlayerOffset());
	//return (PlayerData*)(0x0068AB80 - 0x4737 - 1 + GetPlayerOffset(PlayerIndex));
	return (PlayerData*)(0x686448 + GetPlayerOffset(PlayerIndex));
}

unsigned char* GameDataClass::GetPlayerData(int PlayerIndex /*= -1*/)
{
	//return (unsigned char*)(0x686588 - 102 + GetPlayerOffset());
	//return (unsigned char*)(0x0068AB80 - 0x4737 - 1 + GetPlayerOffset(PlayerIndex));
	return (unsigned char*)(0x686448 + GetPlayerOffset(PlayerIndex));
}

int GameDataClass::GetPlayerDataSize()
{
	return 0x54D8;
	// return sizeof(PlayerData);
}

int GameDataClass::GetPlayerLevel()
{
	char * CurrentLevel = (char *)(0x00686600 + GetPlayerOffset());
	return *CurrentLevel;
}

int GameDataClass::GetPlayerExperience()
{
	int CurrentExperience = *(int *)(0x686588 + 0x7C + GetPlayerOffset());
	return CurrentExperience;
}

float GameDataClass::GetLevelPercent()
{
	int BeginExp = GetExperienceRequiredForLevel(GetPlayerLevel() - 1);
	int EndExp = GetExperienceRequiredForLevel(GetPlayerLevel());
	
	int NeededExp = EndExp - BeginExp;
	return ((GetPlayerExperience() - BeginExp) / (float)NeededExp) * 100;
}

float GameDataClass::GetLevelChange()
{
	static float ReturnPct = 0;
	static float LastPct = 0;
	static int LastExp = 0;
	static int LastLevel = GetPlayerLevel();

	float ThisPct = GetLevelPercent();
	int ThisExp = GetLevelPercent();
	int ThisLevel = GetPlayerLevel();

	if(ThisExp > LastExp)
	{
		// Experience went up
		ReturnPct = ThisPct - LastPct;
		LastPct = ThisPct;
		LastExp = ThisExp;
	}

	if (ThisLevel != LastLevel)
	{
		// Level changed
		LastLevel = ThisLevel;
		LastExp = ThisExp;
		LastPct = 0.0;
		ReturnPct = 0.0;
	}

	return ReturnPct;
}

int GameDataClass::GetExperienceRequiredForLevel(int Level)
{
/*
	char CurrentLevel = Level;
	int Exp = 0;

	if(CurrentLevel == 0) Exp += 0;
	if(CurrentLevel >= 1) Exp += 2000;
	if(CurrentLevel >= 2) Exp += 2620;
	if(CurrentLevel >= 3) Exp += 3420;
	if(CurrentLevel >= 4) Exp += 4449;
	if(CurrentLevel >= 5) Exp += 5769;
	if(CurrentLevel >= 6) Exp += 7454;
	if(CurrentLevel >= 7) Exp += 9597;
	if(CurrentLevel >= 8) Exp += 12313;
	if(CurrentLevel >= 9) Exp += 15742;
	if(CurrentLevel >= 10) Exp += 20055;
	if(CurrentLevel >= 11) Exp += 25460;
	if(CurrentLevel >= 12) Exp += 32307;
	if(CurrentLevel >= 13) Exp += 40457;
	if(CurrentLevel >= 14) Exp += 49392;
	if(CurrentLevel >= 15) Exp += 82581;
	if(CurrentLevel >= 16) Exp += 110411;
	if(CurrentLevel >= 17) Exp += 147123;
	if(CurrentLevel >= 18) Exp += 195379;
	if(CurrentLevel >= 19) Exp += 258585;
	if(CurrentLevel >= 20) Exp += 341123;
	if(CurrentLevel >= 21) Exp += 448291;
	if(CurrentLevel >= 22) Exp += 587327;
	if(CurrentLevel >= 23) Exp += 766756;
	if(CurrentLevel >= 24) Exp += 997549;
	if(CurrentLevel >= 25) Exp += 1293323;
	if(CurrentLevel >= 26) Exp += 1670973;
	if(CurrentLevel >= 27) Exp += 2151378;
	if(CurrentLevel >= 28) Exp += 2760218;
	if(CurrentLevel >= 29) Exp += 3528939;
	if(CurrentLevel >= 30) Exp += 4495869;
	if(CurrentLevel >= 31) Exp += 5707505;
	if(CurrentLevel >= 32) Exp += 7219994;
	if(CurrentLevel >= 33) Exp += 9100803;
	if(CurrentLevel >= 34) Exp += 11430609;
	if(CurrentLevel >= 35) Exp += 14305407;
	if(CurrentLevel >= 36) Exp += 17838843;
	if(CurrentLevel >= 37) Exp += 22164762;
	if(CurrentLevel >= 38) Exp += 27439976;
	if(CurrentLevel >= 39) Exp += 33847210;
	if(CurrentLevel >= 40) Exp += 41590222;
	if(CurrentLevel >= 41) Exp += 50937022;
	if(CurrentLevel >= 42) Exp += 62143167;
	if(CurrentLevel >= 43) Exp += 75535020;
	if(CurrentLevel >= 44) Exp += 91472909;
	if(CurrentLevel >= 45) Exp += 110362065;
	if(CurrentLevel >= 46) Exp += 132655203;
	if(CurrentLevel >= 47) Exp += 159954605;
	if(CurrentLevel >= 48) Exp += 190228390;
	if(CurrentLevel >= 49) Exp += 227798497;
	if(CurrentLevel >= 50) Exp += 300000000;
	
	return Exp;
*/

	int ExpLvlsTbl[51] =
	{
	  0,
	  2000,
	  4620,
	  8040,
	  12489,
	  18258,
	  25712,
	  35309,
	  47622,
	  63364,
	  83419,
	  108879,
	  141086,
	  181683,
	  231075,
	  313656,
	  424067,
	  571190,
	  766569,
	  1025154,
	  1366227,
	  1814568,
	  2401895,
	  3168651,
	  4166200,
	  5459523,
	  7130496,
	  9281874,
	  12042092,
	  15571031,
	  20066900,
	  25774405,
	  32994399,
	  42095202,
	  53525811,
	  67831218,
	  85670061,
	  107834823,
	  135274799,
	  169122009,
	  210720231,
	  261657253,
	  323800420,
	  399335440,
	  490808349,
	  601170414,
	  733825617,
	  892680222,
	  1082908612,
	  1310707109,
	  1583495809
	};

	return ExpLvlsTbl[Level];
}

// This function opens the store dialog for the specified tonwer store
void GameDataClass::GameFuncStartStore(int Store)
{
	//0x45A2D0: StartStore(char)
	int StartStore = (0x45A2D0);
	//int Store = 5; // 0 - None, 1 - Smith, 5 - Witch, 0xC - Boy, 0xD - BBOY, 0xE - Healer, 0xF - Cain, 0x15 - Tavern, 0x16 - Drunk, 0x17 - Barmaid

	__asm
	{
		PUSHAD
		MOV ECX, Store
		CALL[StartStore]
		POPAD
	}
}

// This function returns true/false if the current player meets the minimum requires for the item
bool GameDataClass::GameFuncItemMinStats(Item* i)
{
	//0x41FBF6: ItemMinStats(PlayerStruct const*, ItemStruct const*)
	int ItemMinStats = (0x41FBD6);
	int PlayerStruct = (int)GameData.GetPlayerStruct();
	int Return = 0;

	__asm
	{
		PUSHAD
		MOV ECX, PlayerStruct
		MOV EDX, i
		CALL[ItemMinStats]
		MOV Return, ECX
		POPAD
	}
}

// Prints item details for a specific prefix or suffix
char* GameDataClass::GameFuncPrintItemDetails(Item* i)
{
	// This function prints all of the item details to the display area, but the last line (requirements) is still left in tempstr when it's done

	//0x4238D4: PrintItemDetails(ItemStruct const*)
	int PrintItemDetails = (0x4238D4);

	__asm
	{
		PUSHAD
		MOV ECX, i
		CALL[PrintItemDetails]
		POPAD
	}

	return (char*)(0x0003584C + 0x483000); // Address of "tempstr" in devilution code
}

// Prints item details for a specific prefix or suffix
char* GameDataClass::GameFuncPrintItemPower(char Power, Item* i)
{
	if (Power != -1)
	{
		//0x422EF4: PrintItemPower(char,ItemStruct const *)
		int PrintItemPower = (0x422EF4);
		int Power32 = Power;

		__asm
		{
			PUSHAD
			MOV ECX, Power32
			MOV EDX, i
			CALL[PrintItemPower]
			POPAD
		}

		return (char*)(0x0003584C + 0x483000); // Address of "tempstr" in devilution code
	}
	else
	{
		return "";
	}
}

// Prints item misc details
char* GameDataClass::GameFuncPrintItemMisc(Item* i)
{
	//0x4237DC: PrintItemMisc(ItemStruct const*)
	int PrintItemMisc = (0x4237DC);

	__asm
	{
		PUSHAD
		MOV ECX, i
		CALL[PrintItemMisc]
		POPAD
	}

	return (char*)(0x0003584C + 0x483000); // Address of "tempstr" in devilution code
}

void GameDataClass::GameFuncDrawAutomap()
{
	// This call directly renders the automap
	int Diablo00401FBD = 0x00401FBD;
	__asm
	{
		//0045317F.E8 39EEFAFF    CALL Diablo.00401FBD
		CALL[Diablo00401FBD]
	}
}

void GameDataClass::GameFuncDrawAutomapPlr()
{
	// This call directly renders the automap
	int DiabloDrawAutomapPlr = 0x4029A8;
	__asm
	{
		CALL[DiabloDrawAutomapPlr]
	}
}