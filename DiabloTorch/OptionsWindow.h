/********   ********   ********/
/*      OptionsWindow.h       */
/********   ********   ********/
/*          8/7/2020          */
/********   ********   ********/
/*           Tiron            */
/********   ********   ********/
// http://tristr.am/
// https://freshmeat-blog.de.tl/
// https://d1legit.com/


// This window controls and toggles features of Torch

#ifndef OPTIONS_H
#define OPTIONS_H

#include "Window.h"

#define TAB_TOTAL		4

#define TAB_ITEMS		0
#define TAB_PLAYER		1
#define TAB_MONSTERS	2
#define TAB_MISC		3
//#define TAB_THEMES		4

class OptionsClass : public Window
{
private:
	int Tab;

public:

	// Items tab
	bool ShowInventoryValues;
	bool AutoHighlightItems;

	// Player tab
	bool ExpBar;
	bool PlayerLifeBars;
	bool PartyStatus;
	bool PlayerMapArrows;

	// Monsters tab
	bool LifeBar;

	// Misc tab
	bool TransparentWindows;
	bool Clock;
	int LifeText;
	bool DurabilityBars;

	// Themes tab
private:
//	Window ThisWindow;

	Rect TabBar;
	Rect Option1;
	Rect Option2;
	Rect Option3;
	Rect Option4;

//	Rect TransparentWindowsRect;
	//bool TransparentWindows;
//	Rect ExpBarRect;
	//bool ExpBar;
//	Rect ClockRect;
	//bool Clock;
//	Rect LifeBarRect;
	//bool LifeBar;

	void UpdateRects();
	void SaveRegistry();

	int NextSound;

public:
	OptionsClass();
	~OptionsClass();

	void Draw();

//	void MouseMove(int x, int y) { ThisWindow.MouseMove(x, y); };
	bool MouseButtonDown(int x, int y);
//	void MouseButtonUp(int x, int y) { ThisWindow.MouseButtonUp(x, y); };

//	bool GetTransparentWindows() { return TransparentWindows; };
//	bool GetExpBar() { return ExpBar; };
};

extern OptionsClass Options;

#endif