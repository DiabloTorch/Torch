/********   ********   ********/
/*           Window.h         */
/********   ********   ********/
/*          8/7/2020          */
/********   ********   ********/
/*           Tiron            */
/********   ********   ********/
// http://tristr.am/
// https://freshmeat-blog.de.tl/
// https://d1legit.com/


// Base class to manage, draw, and animate a single window. Subclasses need to override the Draw() method to render custom content

#ifndef WINDOW_H
#define WINDOW_H

// 20 gives a nice sliding animation, was suitable in Starcraft but feels somewhat out of place in Diablo (90 gives 1 frame animation time, effectively removing animations)
#define WINDOW_ANIMATION_SPEED 90

enum WindowState
{
	Closed,		// Window Closed -- Do Not Draw
	Closing,	// Window Closing Animation -- Slide Off to Left of Screen
	Entering,	// Window Entering Animation -- Slide On to Current Position (While Collapsed, State = Opening After Entering is Done)
	Leaving,	// Window Leaving (Collapsing Animation) -- State = Closed when Leaving Finishes)
	Collapsed,	// Window Collapsed -- Draw Title Bar Only
	Collapsing,	// Window Collapsing -- Client Area Slides Up
	Open,		// Window Open -- Normal Drawing
	Opening,	// Window Opening -- Client Area Slides Down (Opposite of Collapsing)
};

struct Rect
{
	int x;
	int y;
	int Width;
	int Height;
};

class Window
{
private:
	WindowState DrawState;
	int DrawStatePosition;

	Rect Position;
	Rect TitleBar;
	Rect ClientRect;
	char Title[255];
	
	Rect CloseButton;
	Rect CollapseButton;

	bool IsMouseButtonDown;
	int LastMousePositionX;
	int LastMousePositionY;

	void UpdateRects();

public:
	Window();
	~Window();

	virtual void Draw();

	void SetX(int NewX);
	void SetY(int NewY);
	void SetWidth(int NewWidth);
	void SetHeight(int NewHeight);

	Rect GetPosition();
	Rect GetTitleBar();
	Rect GetClientRect();
	Rect GetCollapseButton();
	Rect GetCloseButton();

	void SetTitle(char * NewTitle);
	char * GetTitle() { return Title; };

	virtual void MouseMove(int x, int y);
	virtual bool MouseButtonDown(int x, int y);
	virtual void MouseButtonUp(int x, int y);

	WindowState GetWindowState() { return DrawState; };
	void SetWindowState(WindowState NewDrawState);// { DrawState = NewDrawState; DrawStatePosition = 0; };
	void LoadWindowState();

	operator Window *() { return (Window *)this; };
};

bool PointInRect(int x, int y, Rect Test);

#endif