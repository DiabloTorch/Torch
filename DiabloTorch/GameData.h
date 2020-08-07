/********   ********   ********/
/*         GameData.h         */
/********   ********   ********/
/*          8/7/2020          */
/********   ********   ********/
/*           Tiron            */
/********   ********   ********/
// http://tristr.am/
// https://freshmeat-blog.de.tl/
// https://d1legit.com/

// Class to simplify retrieving game data
// TODO: Move all hardcoded hex addresses from main.cpp, DetailsWindow, and StashWindow into GameData

#ifndef GAMEDATACLASS_H
#define GAMEDATACLASS_H

typedef unsigned char byte;

#define EQUIPSLOT_NONE		7
#define EQUIPSLOT_ONEHAND	1
#define EQUIPSLOT_TWOHAND	2
#define EQUIPSLOT_BODY		3
#define EQUIPSLOT_HEAD		4
#define EQUIPSLOT_RING		5
#define EQUIPSLOT_AMULET	6

#define EQUIPTYPE_WEAPON	1
#define EQUIPTYPE_ARMOR		2
#define EQUIPTYPE_CLICKABLE	3
#define EQUIPTYPE_GOLD		4
#define EQUIPTYPE_USELESS	5

enum CharacterClass : long int
{/* 0,1,..,5=Warrior,Rogue,Mage,Monk,Bard,Barbarian */
	Warrior = 0,
	Rouge,
	Sorceror,
	Monk,
	Bard,
	Barbarian,
};

//#pragma pack(1) // This structure is coped from the Devilution code, and packing at 1 byte makes it incorrect
struct UniqueItemInfo
{
	char* UIName;
	char UIItemId;
	char UIMinLvl;
	char UINumPL;
	int UIValue;
	char UIPower1;
	int UIParam1;
	int UIParam2;
	char UIPower2;
	int UIParam3;
	int UIParam4;
	char UIPower3;
	int UIParam5;
	int UIParam6;
	char UIPower4;
	int UIParam7;
	int UIParam8;
	char UIPower5;
	int UIParam9;
	int UIParam10;
	char UIPower6;
	int UIParam11;
	int UIParam12;
};
//#pragma pack()

// Also don't pack this one
struct MonsterStruct // note: missing field _mAFNum
{
	int _mMTidx;
	int _mmode; /* MON_MODE */
	unsigned char _mgoal;
	int _mgoalvar1;
	int _mgoalvar2;
	int _mgoalvar3;
	int field_18;
	unsigned char _pathcount;
	int _mx;
	int _my;
	int _mfutx;
	int _mfuty;
	int _moldx;
	int _moldy;
	int _mxoff;
	int _myoff;
	int _mxvel;
	int _myvel;
	int _mdir;
	int _menemy;
	unsigned char _menemyx;
	unsigned char _menemyy;
	short falign_52;
	unsigned char* _mAnimData;
	int _mAnimDelay;
	int _mAnimCnt;
	int _mAnimLen;
	int _mAnimFrame;
	int _meflag;
	int _mDelFlag;
	int _mVar1;
	int _mVar2;
	int _mVar3;
	int _mVar4;
	int _mVar5;
	int _mVar6;
	int _mVar7;
	int _mVar8;
	int _mmaxhp;
	int _mhitpoints;
	unsigned char _mAi;
	unsigned char _mint;
	short falign_9A;
	int _mFlags;
	char _msquelch; /* unsigned char */
	int falign_A4;
	int _lastx;
	int _lasty;
	int _mRndSeed;
	int _mAISeed;
	int falign_B8;
	unsigned char _uniqtype;
	unsigned char _uniqtrans;
	char _udeadval;
	char mWhoHit;
	char mLevel;
	unsigned short mExp;
	unsigned char mHit;
	unsigned char mMinDamage;
	unsigned char mMaxDamage;
	unsigned char mHit2;
	unsigned char mMinDamage2;
	unsigned char mMaxDamage2;
	char mArmorClass;
	char falign_CB;
	unsigned short mMagicRes;
	int mtalkmsg;
	unsigned char leader;
	unsigned char leaderflag;
	unsigned char unpackfilesize;
	unsigned char mlid;
	char* mName;
	/*CMonster**/ void* MType;
	/*MonsterData**/ void* MData;
};

#pragma pack(1)
struct Item
{
	unsigned int ItemSeed;
	byte SourceLevel;
	byte SourceCode;
	byte Unknown[2];
	int	ItemType;					// Item Type? Is always 0x0B for gold, and FF FF FF FF for a slot not equipped.
	int x;							// Last X position on the ground.
	int y;							// Last Y position on the ground.
	byte UnknownBytes1[36];			// ?
	byte Identified;				// 00 for not identified, 01 for identified.
	byte Unused1[3];				// Always 0?
	byte Magical;					// 00 for normal, 01 for magic, 02 for unique.
	char Name[64];					// Item's unidentified name.
	char NameIdentified[64];		// Item's actual name.
	byte EquipmentSlot;				// 00 cant equip, 01 for one handed, 02 for 2 handed, 3 body, 4 head, 5 ring, 6 amulet, 7 cant equip
	byte EquipmentType;				// 01 weapons, 02 armor, 03 book/scroll/potion (right-click?), 04 gold, 05 tavern sign
	byte Unused2[1];				// ?
	int Graphic;					// The graphic used for this item. This also determines iventory space used.
	int Cost;						// Item's value if it were purchased (sells for 1/4 this).
	int CostIdentified;				// Item's identified value if it were purchased (sells for 1/4 this).
	int MinimumDamage;
	int MaximumDamage;
	int ArmorClass;				// Item's armor rating.
	int SpecialAbilityFlags;
	int MagicCode;
	int SpellCode;
	int Charges;					// Current staff charges
	int ChargesMax;					// Max staff charges
	int Durability;					// Item's current condition
	int DurabilityMax;				// Item's ideal condition
	int BonusDamagePercent;
	int BonusToHit;
	int BonusArmor;
	int	BonusStrength;
	int BonusMagic;
	int BonusDexterity;
	int BonusVitality;
	int BonusResistFire;
	int BonusResistLightning;
	int BonusResistMagic;
	int BonusMana;
	int BonusLife;
	int BonusDamage;
	int BonusDamageModifier;
	int BonusLightRadius;
	char BonusSpellLevel;
	char BonusManaCost;
	byte Unknown4[2];
	byte UniqueItemId;
	byte Unknown5[2];
	char BonusSpellDuration;
	int BonusFireDamageMinimum;
	int BonusFireDamageMaximum;
	int BonusLightningDamageMinimum;
	int BonusLightningDamageMaximum;
	int BonusArmorPiercing;
	byte BonusPrefix; // This identifies the type of magical prefix on the item
	byte BonusSuffix; // This identifies the type of magical suffix on the item
	byte Unknown6[2];
	int PrefixPriceAddition;
	int PrefixPriceMultiplier;
	int SuffixPriceAddition;
	int SuffixPriceMultiplier;
	byte RequiredStrength;
	byte RequiredMagic;
	byte RequiredDexterity;
	byte Unknown7;
	int Equipable;
	int BaseItemIndex;
	byte Unknown8[4];
};
#pragma pack()

#pragma pack(1)
struct PlayerData
{ 
	long int		CurrentActon;				// Current action
	char			Unknown0004[25];
	char			isActivePlayer;
	char			Padding001E[2];
	long int		Unknown0020[5];				// action related/scroll related
	long int		DungeonLevelOn; //0x686484
	long int		PlayerPosX;					// X position in current level (0,0 is top left of current world, with 100,0 being the top right (it might not be exaclty 100
	long int		PlayerPosY;					// Y position in current level
	long int		PlayerTargetX;				// Next x position (only different than previous if moving)
	long int		PlayerTargetY;				// Next Y position (only different than previous if moving)
	long int		Unknown0040[6];				// various X/Y, maybe til in min...
	long int		AnimationScreenX;			// Some sort of animation counter that works when moving directly across the X axis of the screen (not game X axis)
	long int		AnimationScreenY;			// Some sort of animation counter that works when moving directly across the Y axis of the screen (up and down)
	long int		Unknown0060[2];
	long int		Unknown0070[2];
	long int		Appearance;					// Bitmask value indicating current appearance   Low nibble is items: 01=shield|02=sword|04=bow|05=axe|06=mace|08=staff		High nibble is armor type 00=cloth|10=mail|20=plate
	long int		Unknown007C[3];
	long int		Unknown0088[3];				// last frame number (9B4) / current frame number (1=new) / animation related (9B8)
	long int		Unknown0094[2];
	long int		Unknown009C[2];				// light related
	long int		Unknown00A4[2];				// action related/spell related (scrolls)
	long int		CurrentSpell;
	long int		Unknown00B0;				// action related
	long int		SpellToCast;				// Currently selected spell, -1 = none
	long int		SpellSource;				// Source of current spell: 0=skill|1=book|2=scroll|3=staff|4=staff (no charges)
	long int		Unknown00BC;
	char			Unknown00C0;				// spell related
	char			SpellLevel[0x25];			// Index 0 for current spell
	char			Unused[0x22];
	unsigned char	SpellAvaibleBook[8];		// Bitfield, 36 entries
	unsigned char	SpellAvaibleSkill[8];		// Bitfield, 36 entries
	unsigned char	SpellAvaibleScroll[8];		// Bitfield, 36 entries
	long int		Etheral;					// Immunity
	long int		SpellShortcutIndex[4];		// F5/F6/F7/F8
	char			SpellShortcutEnabled[4];	// F5/F6/F7/F8, enabled = 01
	long int		HaveBow;
	char			CanBlock;
	char			SpellImmune;				// Immunity, flash = 1, <>0 -> not hittable by any spell missiles
	char			LightRadius;				// 10 on start
	char			Invisible;					// 0 -> not selectable as monster-target
	char			Name[0x20];
	CharacterClass	Class;
	long int		StrengthCurrent;
	long int		StrengthBase;
	long int		MagicCurrent;
	long int		MagicBase;
	long int		DexterityCurrent;
	long int		DexterityBase;
	long int		VitalityCurrent;
	long int		VitalityBase;
	long int		StatPointsAvailable;		// bonus points to distribute
	long int		BaseDamage;
	long int		BlockBonus;					// Following life/mana fields are reported in 1/64th life per bit
	long int		LifeBase;					// Current life before items (including +vit items) are factored in
	long int		LifeBaseMax;				// Maximum life before items (including +vit items) are factored in
	long int		LifeCurrent;				// Current life after items are factored in
	long int		LifeCurrentMax;				// Maximum life after items are factored in
	long int		LifeRatio;					// Percentage of life remaining, on a scale of 0-80
	long int		ManaBase;					// Current mana before items (including +mag items) are factored in
	long int		ManaBaseMax;				// Maximum mana before items (including +mag items) are factored in
	long int		ManaCurrent;				// Current mana after items are factored in
	long int		ManaCurrentMax;				// Current mana after items are factored in
	long int		ManaRatio;					// Percentage of mana remaining
	char			CharacterLevel;
	char			Unknown01B9[3];
	long int		Experience;
	long int		Unknown01C0;
	long int		ExperienceNextLevelUp;
	char			Res; // What is this?	This is NOT resist all, resist all adds to all 3 categories instead
	char			ResistMagic;
	char			ResistFire;
	char			ResistLightning;
	long int		Gold;
	long int		Infravision;				// 1 if on
	long int		Unknown01D4[3];
	long int		Unknown01E0;
	long int		Unknown01E4;
	long int		Unknown01E8[2];
	long int		Unknown01F0;
	char			DLvlVisited[0x10];					/* dlvl visited */
	long int		Unknown0204[5];
	long int		AnimationsLoaded;
	long int		Unknown021C;				// animation related 
	long int		Unknown0220[0x57];			// Animations related : anim speeds, number of frames, ... Jarulf seems to know more on it... 
//	Item			ItemEquiped[7];				// 0..6 =  Helm / Amulet / RingLeft / RingRight / Weapon / Shield / Armor
	Item			ItemHead;
	Item			ItemNeck;
	Item			ItemLeftFinger;				// Left meaning left side of the inventory screen, I'm pretty sure that's technically the player's right finger.
	Item			ItemRightFinger;
	Item			ItemLeftHand;
	Item			ItemRightHand;
	Item			ItemChest;
	Item			ItemInventory[0x28];		// Array of items held in inventory
	long int		NumberOfItems;				// Total number of items held in inventory
	char			ItemIndexInInventory[0x28];	// Array of inventory slots and the item currently filled by them (for example, a large axe takes six slots, six of these will be the index of that large axe in ItemInventory
	Item			ItemBelt[8];				// Array of items held on belt
	Item			ItemPointer[1];				// Item held on cursor
	long int		WeaponDamageMin;
	long int		WeaponDamageMax;
	long int		ACFromClass;                /* not Dex/5 */
	long int		EnhancedDamageBonus;
	long int		ToHitBonus;
	long int		ArmorBonus;
	long int		DamageBonus;
	long int		unknown5444;
	long int		unknown5448[2];             /* staff spell bitmap / Staff spells speedbook */
	long int		SpecialAbilityFlags;        /* same as for items */
	long int		DamageModifier;
	char			SpellLevelBonus;
	char			ManaCostBonus;
	long int		SpellDurationBonus;
	char			UnknownBytes[2];
	long int		Bashing;                    /* 0,1,2,3 = none, penetrating, piercing, bashing */
	long int		FireDamageMin;
	long int		FireDamageMax;
	long int		LightningDamageMin;
	long int		LightningDamageMax;
	long int		OilEffectOnCursor;
	char			DungeonType;                /* 0-3 */
	char			Counter[3];                 /* Counting what ?? */
	long int		Unknown547C;
	long int		ReflectCounter;
	long int		Unknown5484[3];
	long int		Dots;
	long int		Unknown5494;
	long int		SomeFlags;                  /* 01=Devastation,04=Peril,08=Jester,10h=Doppelganger */
	long int		Unknown549C[0x0F];
	/* size = 0x54D8 = 21720 */
};
#pragma pack()

struct PlayerStructDiablo // Diablo's internal player struct, from Devilution
{
	int _pmode;
	char walkpath[25];
	unsigned char plractive;
	int destAction;
	int destParam1;
	int destParam2;
	int destParam3;
	int destParam4;
	int plrlevel;
	int WorldX;
	int WorldY;
	int _px;
	int _py;
	int _ptargx;
	int _ptargy;
	int _pownerx;
	int _pownery;
	int _poldx;
	int _poldy;
	int _pxoff;
	int _pyoff;
	int _pxvel;
	int _pyvel;
	int _pdir;
	int _nextdir;
	int _pgfxnum;
	unsigned char* _pAnimData;
	int _pAnimDelay;
	int _pAnimCnt;
	int _pAnimLen;
	int _pAnimFrame;
	int _pAnimWidth;
	int _pAnimWidth2;
	int _peflag;
	int _plid;
	int _pvid;
	int _pSpell;
	char _pSplType;
	char _pSplFrom;
	int _pTSpell;
	int _pTSplType;
	int _pRSpell;
	// enum spell_type
	char _pRSplType;
	int _pSBkSpell;
	char _pSBkSplType;
	char _pSplLvl[64];
	union {
		__int64 _pMemSpells64;
		int _pMemSpells[2];
	};
	union {
		__int64 _pAblSpells64;
		int _pAblSpells[2];
	};
	union {
		__int64 _pScrlSpells64;
		int _pScrlSpells[2];
	};
	int _pSpellFlags;
	int _pSplHotKey[4];
	char _pSplTHotKey[4];
	int _pwtype;
	unsigned char _pBlockFlag;
	unsigned char _pInvincible;
	char _pLightRad;
	unsigned char _pLvlChanging;
	char _pName[32];
	// plr_class enum value.
	// TODO: this could very well be `enum plr_class _pClass`
	// since there are 3 bytes of alingment after this field.
	// it could just be that the compiler optimized away all accesses to
	// the higher bytes by using byte instructions, since all possible values
	// of plr_class fit into one byte.
	char _pClass;
	int _pStrength;
	int _pBaseStr;
	int _pMagic;
	int _pBaseMag;
	int _pDexterity;
	int _pBaseDex;
	int _pVitality;
	int _pBaseVit;
	int _pStatPts;
	int _pDamageMod;
	int _pBaseToBlk;
	int _pHPBase;
	int _pMaxHPBase;
	int _pHitPoints;
	int _pMaxHP;
	int _pHPPer;
	int _pManaBase;
	int _pMaxManaBase;
	int _pMana;
	int _pMaxMana;
	int _pManaPer;
	char _pLevel;
	char _pMaxLvl;
	int _pExperience;
	int _pMaxExp;
	int _pNextExper;
	char _pArmorClass;
	char _pMagResist;
	char _pFireResist;
	char _pLghtResist;
	int _pGold;
	int _pInfraFlag;
	int _pVar1;
	int _pVar2;
	int _pVar3;
	int _pVar4;
	int _pVar5;
	int _pVar6;
	int _pVar7;
	int _pVar8;
	unsigned char _pLvlVisited[17];
	unsigned char _pSLvlVisited[17]; // only 10 used
	int _pGFXLoad;
	unsigned char* _pNAnim[8];
	int _pNFrames;
	int _pNWidth;
	unsigned char* _pWAnim[8];
	int _pWFrames;
	int _pWWidth;
	unsigned char* _pAAnim[8];
	int _pAFrames;
	int _pAWidth;
	int _pAFNum;
	unsigned char* _pLAnim[8];
	unsigned char* _pFAnim[8];
	unsigned char* _pTAnim[8];
	int _pSFrames;
	int _pSWidth;
	int _pSFNum;
	unsigned char* _pHAnim[8];
	int _pHFrames;
	int _pHWidth;
	unsigned char* _pDAnim[8];
	int _pDFrames;
	int _pDWidth;
	unsigned char* _pBAnim[8];
	int _pBFrames;
	int _pBWidth;
	Item InvBody[7];
	Item InvList[40];
	int _pNumInv;
	char InvGrid[40];
	Item SpdList[8];
	Item HoldItem;
	int _pIMinDam;
	int _pIMaxDam;
	int _pIAC;
	int _pIBonusDam;
	int _pIBonusToHit;
	int _pIBonusAC;
	int _pIBonusDamMod;
	union {
		__int64 _pISpells64;
		int _pISpells[2];
	};
	int _pIFlags;
	int _pIGetHit;
	char _pISplLvlAdd;
	char _pISplCost;
	int _pISplDur;
	int _pIEnAc;
	int _pIFMinDam;
	int _pIFMaxDam;
	int _pILMinDam;
	int _pILMaxDam;
	int _pOilType;
	unsigned char pTownWarps;
	unsigned char pDungMsgs;
	unsigned char pLvlLoad;
	unsigned char pBattleNet;
	unsigned char pManaShield;
	char bReserved[3];
	short wReserved[8];
	int pDiabloKillLevel;
	int dwReserved[7];
	unsigned char* _pNData;
	unsigned char* _pWData;
	unsigned char* _pAData;
	unsigned char* _pLData;
	unsigned char* _pFData;
	unsigned char* _pTData;
	unsigned char* _pHData;
	unsigned char* _pDData;
	unsigned char* _pBData;
	int unused_54D4;
};

class GameDataClass
{
private:
public:
	void WritePointerAtAddress(void* Pointer, void* Address);	// This overwrites a Diablo pointer with one of our own, with the original Diablo pointer located at Address
	void WriteByteAtAddress(unsigned char Byte, void* Address); // This overwrites a single Diablo byte
	// Misc player info
	int GetPlayerIndex();								// Returns the number of the current player (0, 1, 2, 3).
	int GetPlayerOffset(int PlayerIndex = -1);			// Returns the offset, in bytes, of the current player's info struct.

	// Player data struct & info
	PlayerData* GetPlayerStruct(int PlayerIndex = -1);	// Returns a reference to a player data structure
	unsigned char* GetPlayerData(int PlayerIndex = -1); // Same address as above, but as a char*
	int GameDataClass::GetPlayerDataSize();

	// Experience and level info
	int GetPlayerLevel();							// Returns the character level of the current player.
	int GetPlayerExperience();						// Returns the total experience earned for the current character.
	int GetExperienceRequiredForLevel(int Level);	// Returns the total experience required to reach a certain level.
	float GetLevelPercent();						// The percentage completed towards the next level.
	float GetLevelChange();							// Returns the percentage of a level that the last change in exp was worth.

	// Equipment info
	Item * Head() { return (Item *)(0x006867C4 + GetPlayerOffset()); };
//	Item * Body() { return (Item *)(0x00687064 + GetPlayerOffset()); };
//	Item * LeftHand() { return (Item *)(0x00686D84 + GetPlayerOffset()); };
//	Item * RightHand() { return (Item *)(0x00686EF4 + GetPlayerOffset()); };
//	Item * LeftFinger();
//	Item * RightFinger();
//	Item * Neck();

	// Inventory info
	/* Notice that the Head item is in line with all inventory items, and can be treated like an array of 47 items. */
	Item * Inventory() { return (Item *)(0x006871D4 + GetPlayerOffset()); };
	signed char * InventoryMap() { return (signed char *)(0x0068AB58 + GetPlayerOffset()); };
	int InventoryCount() { return *(unsigned char *)(0x0068AB54 + GetPlayerOffset()); };

	Item * Belt() { return (Item *)(0x0068AB80 + GetPlayerOffset()); };

	// Ground info
	Item * GroundItems() { return (Item *)(0x00635A28); };			// Array of items on the ground. Can be non-linear.
	int GroundCount() { return *(unsigned char *)(0x006414DC); };	// Number of items on the ground, max of 127.
	int * GroundMap() { return (int *)(0x00634CC8); };				// Map of items on the ground. Linear list of GroundItems indexes that are valid.

	// Level info
	char GetCurrentLevel() { return *(char*)(0x5BB1EE); };			// Returns 0 for town, or any other number for dungeon level (what about special levels?)

	// Camera info
	int CameraOffsetX() { return *(int *)(0x005BDAF8); };
	int CameraOffsetY() { return *(int *)(0x005BDAFC); };

	// Automap and screens info
	bool AutomapActive()	{ return *(bool *)(0x4B7E48); };
	bool InventoryActive()	{ return *(bool *)(0x00634CB8); };			// Left side
	bool SpellbookActive()	{ return *(bool *)(0x004B8968); };			// Left side
	bool CharInfoActive()	{ return *(bool *)(0x004B896C); };			// Right side
	bool QuestLogActive()	{ return *(bool *)(0x0069BD04); };			// Right side
	bool IsChatBoxActive()	{ return *(bool*)(0x004B8840); };			// True when the text entry box is active for chatting
	char* ChatBoxText()		{ return (char *)(0x004B87AC); };			// String containing the current chat box text

	int GetGraphicPixelWidth(int Graphic) { return *((int *)((Graphic + 0x0C) * 4 + 0x479658)); };
	int GetGraphicPixelHeight(int Graphic) { return *((int *)((Graphic + 0x0C) * 4 + 0x479928)); };

		//0041BDBE   833CB5 28994700 >CMP DWORD PTR DS:[ESI*4+479928],54

	// Direct Diablo functions and variables
	void GameFuncStartStore(int Store);
	bool GameFuncItemMinStats(Item* i);
	char* GameFuncPrintItemDetails(Item* i);
	char* GameFuncPrintItemPower(char Power, Item* i);
	char* GameFuncPrintItemMisc(Item* i);
	void GameFuncDrawAutomap();
	void GameFuncDrawAutomapPlr();
	UniqueItemInfo* DataUniqueItemList() { return (UniqueItemInfo*)(0x00004268 + 0x479000); }
	int* DataUniqueItemFlags() { return (int*)(0x001BE2DC + 0x483000); }
	MonsterStruct* DataMonsterList() { return (MonsterStruct*)(0x64D330); } // Array of monsters "monsters"
	int DataMonsterCount() { return *(int*)(0x64D328); } // Number of active monsters "nummonsters"
	int* DataMonsterMap() { return (int*)(0x64D008); } // List of active monster indexes "monsactive"
	char DataMaxPlayers() { return *(char*)(0x679660); } // Number of allowed players in the game "gbMaxPlayers"
	char* DataAutomapExploreArray() { return (char*)(0x00034E6C + 0x483000); } // Array of char[40][40], 0 or 1 for explored
	int* DataLvlSeedTable() { return (int*)(0x000A260C + 0x483000); } // Array of int[17] for level seeds "glSeedTbl" <- These can be used to identify a new game
};

extern GameDataClass GameData;

#endif