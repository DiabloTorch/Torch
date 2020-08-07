/********   ********   ********/
/*         Taskbar.h          */
/********   ********   ********/
/*          8/7/2020          */
/********   ********   ********/
/*           Tiron            */
/********   ********   ********/
// http://tristr.am/
// https://freshmeat-blog.de.tl/
// https://d1legit.com/


// Class to manage and draw other windows

#ifndef TASKBAR_H
#define TASKBAR_H

#define TOTAL_WINDOWS 32

#include "Window.h"

class TaskbarClass
{
public:
	bool bHidden;
private:
	int TotalWindows;
	Window * WindowList[TOTAL_WINDOWS];
	Rect ClickRects[TOTAL_WINDOWS];
	WindowState CurrentState;

	bool bMouseDown;
	int LastMouseY;
	
	int y;
	Rect OpenButton;
	Rect MoveSlider;
	
public:
	TaskbarClass();
	~TaskbarClass();
	
	void Draw();

	void UpdateRects();
	
	void MouseMove(int x, int y);
	bool MouseButtonDown(int x, int y);
	void MouseButtonUp(int x, int y);

	void AddWindow(Window * NewWindow);
};

extern TaskbarClass Taskbar;
#endif