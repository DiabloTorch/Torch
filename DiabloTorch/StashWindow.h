/********   ********   ********/
/*       StashWindow.h        */
/********   ********   ********/
/*          8/7/2020          */
/********   ********   ********/
/*           Tiron            */
/********   ********   ********/
// http://tristr.am/
// https://freshmeat-blog.de.tl/
// https://d1legit.com/

// A psuedo window to provide stashing functionality (previously a standard window)

#ifndef STASH_H
#define STASH_H

#include "Window.h"
#include "GameData.h"

#pragma pack(1)
struct StashFileStruct
{
	unsigned int ItemCount;
	Item Items[1024];
};
#pragma pack()

#define STASH_TABS_ROWS		3
#define STASH_TABS_PER_ROW	10
#define STASH_TABS_TOTAL	(STASH_TABS_ROWS * STASH_TABS_PER_ROW)
#define STASH_GRID_ROWS		9 // Previously 8, to be exactly double inventory size (9 fits 3 large items)
#define STASH_GRID_COLS		10 // Always 10 columns
#define STASH_GRID_CELL_SIZE 28

class StashClass
{
private:
//	Window ThisWindow;
	int Tab;
	int HighlightedItem;

	bool Hovering;
	int HoverX;
	int HoverY;
	Item HoverItem;

	Rect TabBar;
	Rect NotificationBar;
	Rect Grid;

public:
	StashClass();
	~StashClass();

	void MouseMove(int x, int y);
	bool MouseButtonDown(int x, int y);
	void Draw();

	// Item functions
	bool IsInventorySlotClear(int x, int y);
	bool IsStashSlotClear(int x, int y);

	bool InventoryHasSpaceForItem(Item Test);
	bool StashHasSpaceForItem(Item Test);

	void StashItem(int ItemIndex);
	
	void SetTabName(char* Name);
	void SetTabColor(int Color);

	void NextTab();
	void PrevTab();
};

extern StashClass Stash;

#endif