/********   ********   ********/
/*         Drawing.h          */
/********   ********   ********/
/*          8/7/2020          */
/********   ********   ********/
/*           Tiron            */
/********   ********   ********/
// http://tristr.am/
// https://freshmeat-blog.de.tl/
// https://d1legit.com/


/* Drawing.h Covers All Drawing-Related Structures and Prototypes */

#ifndef DRAWING_H
#define DRAWING_H

#define DIABLO_COLOR_DARKGOLD	0xC8 // This is the color used for walls on the automap
#define DIABLO_COLOR_BRIGHTGOLD	0x90 // Some users have described this as 'beige', and seem to greatly dislike its use. This is the color used for stairs on the automap.

#define DIABLO_COLOR_FADEDRED	0x99
#define DIABLO_COLOR_RED		0xEB
#define DIABLO_COLOR_REDGRAY	0xAE
#define DIABLO_COLOR_BRIGHTRED	0xE8

#define DIABLO_COLOR_FADEDBLUE	0x85
#define DIABLO_COLOR_BLUE		0x84
#define DIABLO_COLOR_LIGHTBLUE	0x80

#define DIABLO_COLOR_WHITE		0xFF
#define DIABLO_COLOR_BLACK		0x00

#define DIABLO_TEXT_WHITE		0x00
#define DIABLO_TEXT_BLUE		0x01
#define DIABLO_TEXT_RED			0x02
#define DIABLO_TEXT_GOLD		0x03

/* Frame / framerate functions */
unsigned int GetFrames();
void CountFrame();

/* Box Functions */
void DrawBox(int x, int y, int Width, int Height, int Color);				// Draws a Filled Box
void DrawTransparentBox(int x, int y, int Width, int Height, int Color);	// Draws a Box with Every Other Pixel Missing (To Look Transparent)
void DrawTransparentBoxAlt(int x, int y, int Width, int Height, int Color);	// Draws transparent box where the filled pixels alternate each frame (this looks a little "wiggly" with Diablos slow 20fps framerate, avoid using)
void DrawFrame(int x, int y, int Width, int Height, int Color);				// Draws a 1-Pixel Frame Around the Specified Box

/* Line Functions */
void DrawOrthoLine(int xFrom, int yFrom, int xTo, int yTo, int Color);		// Fast Line Drawing Code for Lines that Follow Either X/Y Axis
void DrawLine(int xFrom, int yFrom, int xTo, int yTo, int Color);			// Draws a Line Between Any 2 Points

/* Circle Functions */
void DrawRing(int x0, int y0, int Radius, int Color);						// Draws a 1-Pixel Ring of the Specified Radius

/* Misc */
void DrawPixel(int x, int y, int Color);									// Lowest Level Drawing Function, All Above Functions Directly/Indirectly Call This

/* Text */
void PrintTextXY(int x, int y, int Color, char * Text);						// Calls Diablo's PrintText function, DO NOT call outside of code that is running inside Diablo's render loop
void PrintTextXYCentered(int x, int y, int Color, char * Text);				// Calls Diablo's PrintTextCentered function, DO NOT CALL outside of code running inside Diablo's render loop

/* In-Game */
void DrawItem(int x, int y, int Graphic);									// Calls Diablo's DrawItem function using the specified Graphic code (retrieved from item structure)
void DrawHighlightedItem(int x, int y, int Graphic, int Color);				// Calls Diablo's DrawHighlightedItem function

#endif