/********   ********   ********/
/*      DetailsWindow.h       */
/********   ********   ********/
/*          8/7/2020          */
/********   ********   ********/
/*           Tiron            */
/********   ********   ********/
// http://tristr.am/
// https://freshmeat-blog.de.tl/
// https://d1legit.com/

#ifndef DETAILS_H
#define DETAILS_H

#include "Window.h"

class DetailsClass : public Window
{
private:
//	Window ThisWindow;

public:
	DetailsClass();
	~DetailsClass();

	void Draw();
};

extern DetailsClass MonsterDetails;

#endif