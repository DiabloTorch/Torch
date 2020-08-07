/********   ********   ********/
/*      ShrinesWindow.h       */
/********   ********   ********/
/*          8/7/2020          */
/********   ********   ********/
/*           Tiron            */
/********   ********   ********/
// http://tristr.am/
// https://freshmeat-blog.de.tl/
// https://d1legit.com/


// Window to view game shrines and their effects

#ifndef SHRINES_H
#define SHRINES_H

#include "Window.h"
#include "Drawing.h"

struct Shrine
{
	int TextColor;
	char * Name;
	char * Effect;
	char * Message;
};

class ShrinesClass : public Window
{
private:
//	Window ThisWindow;

public:
	ShrinesClass();
	~ShrinesClass();

	void Draw();

private:
	Rect GetRectForShrine(int Shrine);
};

extern ShrinesClass Shrines;

#endif