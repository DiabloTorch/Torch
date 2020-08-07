/********   ********   ********/
/*      InspectorWindow.h     */
/********   ********   ********/
/*          8/7/2020          */
/********   ********   ********/
/*           Tiron            */
/********   ********   ********/
// http://tristr.am/
// https://freshmeat-blog.de.tl/
// https://d1legit.com/


// This window is used for internal debugging, and should not be present in release builds

#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "Window.h"

class PlayerInspectorClass : public Window
{
private:
	//	Window ThisWindow;

public:
	PlayerInspectorClass();
	~PlayerInspectorClass();

	void Draw();
};

extern PlayerInspectorClass PlayerInspector;

#endif