/********   ********   ********/
/*          Hooks.h           */
/********   ********   ********/
/*          8/7/2020          */
/********   ********   ********/
/*           Tiron            */
/********   ********   ********/
// http://tristr.am/
// https://freshmeat-blog.de.tl/
// https://d1legit.com/


/* Hooks.h Includes the Hook Class, Which Handles All Diablo Hooking */

#ifndef HOOKS_H
#define HOOKS_H

//#include "Offsets.h"

class Hook
{
private:
	int HookLocation;
	int HookLength;
	char * HookRevertData;
	void * Callback;

public:
	Hook();
	Hook(int Location, int Length, void * CallbackFunction);

	~Hook();

	void SetHook();		// Sets Up Hook in SC
	void Revert();		// Reverts Changes Back
};

#endif