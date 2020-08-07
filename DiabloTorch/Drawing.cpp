/********   ********   ********/
/*         Source File        */
/********   ********   ********/
/*          8/7/2020          */
/********   ********   ********/
/*           Tiron            */
/********   ********   ********/
// http://tristr.am/
// https://freshmeat-blog.de.tl/
// https://d1legit.com/

/* Drawing.cpp Implements All Drawing-Related Functions */

#include "Drawing.h"

#define NULL (0)

unsigned int Frames = 0;

unsigned int GetFrames()
{
	return Frames;
}

void CountFrame()
{
	Frames++;

	if (Frames >= 2000000000)
		Frames = 0;
}

void Swap(int & First, int & Second)
{
	First ^= Second;
	Second ^= First;
	First ^= Second;
}

int Abs(int Value)
{
	if(Value > 0)
		return Value;
	else
		return -Value;
}

float Abs(float Value)
{
	if(Value > 0)
		return Value;
	else
		return -Value;
}

void DrawFrame(int x, int y, int Width, int Height, int Color)
{
	DrawOrthoLine(x, y, x + Width, y, Color);
	DrawOrthoLine(x, y, x, y + Height, Color);
	DrawOrthoLine(x, y + Height, x + Width, y + Height, Color);
	DrawOrthoLine(x + Width, y, x + Width, y + Height, Color);
}

void DrawOrthoLine(int xFrom, int yFrom, int xTo, int yTo, int Color)
{
	if((xFrom != xTo && yFrom != yTo) || (xFrom == xTo && yFrom == yTo))
	{
		// Not Ortho Line, Do Nothing
		return;
	}

	if(xFrom == xTo)
	{
		if(yFrom > yTo)
		{
			Swap(yFrom, yTo);
		}

		for(int i = yFrom; i <= yTo; i++)
		{
			DrawPixel(xFrom, i, Color);
		}
	}
	else if(yFrom == yTo)
	{
		if(xFrom > xTo)
		{
			Swap(xFrom, xTo);
		}

		for(int i = xFrom; i <= xTo; i++)
		{
			DrawPixel(i, yFrom, Color);
		}
	}
}

void DrawTransparentBox(int x, int y, int Width, int Height, int Color)
{
	for(int i = y; i < y + Height; i += 1)
	{
		for(int j = x + ((i - y) % 2); j < x + Width; j += 2)
		{
			DrawPixel(j, i, Color);
		}
	}
}

void DrawTransparentBoxAlt(int x, int y, int Width, int Height, int Color)
{
	int offset = Frames % 2;
	for (int i = y; i < y + Height; i += 1)
	{
		for (int j = offset + x + ((i - y) % 2); j < x + Width; j += 2)
		{
			DrawPixel(j, i, Color);
		}
	}
}
void DrawBox(int x, int y, int Width, int Height, int Color)
{
	for(int i = y; i < y + Height; i++)
	{
		for(int j = x; j < x + Width; j++)
		{
			DrawPixel(j, i, Color);
		}
	}
}

void DrawLine(int xFrom, int yFrom, int xTo, int yTo, int Color)
{
	int DeltaY = yTo - yFrom;
	int DeltaX = xTo - xFrom;

	float temp = (float) 0.5;

	DrawPixel(xFrom, yFrom, Color);

	if(Abs(DeltaX) > Abs(DeltaY))
	{
		float m = (float)DeltaY / (float)DeltaX;

		temp += yFrom;
		DeltaX = (DeltaX < 0) ? -1 : 1;
		m *= DeltaX;
		
		while(xFrom != xTo) 
		{
			xFrom += DeltaX;
			temp += m;
			DrawPixel(xFrom, (int)temp, Color);
		}
	}
	else
	{
		float m = (float)DeltaX / (float)DeltaY;

		temp += xFrom;
		DeltaY = (DeltaY < 0) ? -1 : 1;
		m *= DeltaY;

		while(yFrom != yTo)
		{
			yFrom += DeltaY;
			temp += m;
			DrawPixel((int)temp, yFrom, Color);
		}
	}
}

void DrawRing(int x0, int y0, int Radius, int Color)
{
  int f = 1 - Radius;
  int ddF_x = 1;
  int ddF_y = -2 * Radius;
  int x = 0;
  int y = Radius;
 
  DrawPixel(x0, y0 + Radius, Color);
  DrawPixel(x0, y0 - Radius, Color);
  DrawPixel(x0 + Radius, y0, Color);
  DrawPixel(x0 - Radius, y0, Color);
 
  while(x < y)
  {
    if(f >= 0) 
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;    
    DrawPixel(x0 + x, y0 + y, Color);
    DrawPixel(x0 - x, y0 + y, Color);
    DrawPixel(x0 + x, y0 - y, Color);
    DrawPixel(x0 - x, y0 - y, Color);
    DrawPixel(x0 + y, y0 + x, Color);
    DrawPixel(x0 - y, y0 + x, Color);
    DrawPixel(x0 + y, y0 - x, Color);
    DrawPixel(x0 - y, y0 - x, Color);
  }
}

#define DWORD unsigned long
#define LONG long

struct Surface
{
    DWORD               dwSize;                 // size of the DDSURFACEDESC structure
    DWORD               dwFlags;                // determines what fields are valid
    DWORD               Height;               // height of surface to be created
    DWORD               Width;                // width of input surface
        LONG            lPitch;                 // distance to start of next line (return value only)
        DWORD           dwLinearSize;           // Formless late-allocated optimized surface size
    DWORD               dwBackBufferCount;      // number of back buffers requested
        DWORD           dwMipMapCount;          // number of mip-map levels requested
        DWORD           dwZBufferBitDepth;      // depth of Z buffer requested
        DWORD           dwRefreshRate;          // refresh rate (used when display mode is described)
    DWORD               dwAlphaBitDepth;        // depth of alpha buffer requested
    DWORD               dwReserved;             // reserved
//    LPVOID              PixelData;              // pointer to the associated surface memory
	unsigned char *		PixelData;
};

void DrawPixel(int x, int y, int Color)
{
	if ((x < 0 || x >= 640) ||
		(y < 0 || y >= 352))
	{
		// This pixel is off-screen
		return;
	}

	x += 64;
	y += 160;

	unsigned int EAX = *(unsigned int *)(y * 4 + 0x69BEFC);
	EAX += *(unsigned int *)(0x52A524);
	EAX += x;
	*(unsigned char *)EAX = Color;
}

void PrintTextXY(int x, int y, int Color, char * Text)
{
	y += 12;

	if ((x < 0 || x >= 640) ||
		(y < 0 || y >= 352))
	{
		// This pixel is off-screen
		return;
	}


	int DiabloTextOut = (0x00405681);

	__asm
	{
		PUSHAD
		PUSH Color
		PUSH Text
		MOV ECX, x
		MOV EDX, y
//		CALL DWORD PTR [DiabloTextOut]
		CALL [DiabloTextOut]
		POPAD
	}
}

int count(char * String)
{
	int i = 0;
	while (String[i])
	{
		i++;
	}
	return i;
}

int pixels(char * String)
{
	int width = 0;

	//004060C5   8A80 24944700    |MOV AL,BYTE PTR DS:[EAX+479424]

	for (int i = 0; String[i]; i++)
	{
		width += *(unsigned char *)(0x479424 + String[i]);
	}

	return width;
}
void PrintTextXYCentered(int x, int y, int Color, char * Text)
{
	y += 12;
	if ((x < 0 || x >= 640) ||
		(y < 0 || y >= 352))
	{
		// This pixel is off-screen
		return;
	}


	int DiabloTextOut = (0x00406058);

	int leftX = x - 320;
	int rightX = x + 320;

	__asm
	{
		PUSH Color				// Color code
		PUSH Text				// Text pointer
		PUSH rightX				// Far side of the bounding box
		MOV ECX, leftX			// Near side of the bounding box
		MOV EDX, y				// y position
		CALL [DiabloTextOut]
	}
}

void DrawItem(int x, int y, int Graphic)
{
	if (x < 0 || x > 640 ||
		y < 0 || y > 640)
	{
		// Don't draw off screen
		return;
	}

	x += 64;
	y += 160;

	
	Graphic += 0x0C;
	int ItemSize = *((int *)(Graphic * 4 + 0x479658));
	int ItemHeight = *((int *)(Graphic * 4 + 0x479928));

	// For testing
	//DrawFrame(x - 64, y - ItemHeight - 160, ItemSize, ItemHeight, DIABLO_COLOR_REDGRAY);

	// MOV EBP,DWORD PTR DS:[ESI*4+479658]
	int Internal = *((int *)(0x4B8CB0));

	//0x4162DE: DrawSlabCel(long,long,uchar *,long,long,long,long)
	int DrawItem = (0x004162DE);

	//0x416359: DrawSlabCelP(uchar *,uchar *,long,long,long,long)
	//0x4165BD: DrawSlabCelL(long, long, uchar*, long, long, long, long)
	DrawItem = 0x41685A;
	__asm
	{
		PUSH 8			// -- ?
		PUSH 0			// EBX -- ?
		PUSH ItemSize	// EBP -- Item size?
		PUSH Graphic	// ESI -- Graphic ID + 0x0C
		PUSH Internal	// -- ?	Render stats?
		MOV EDX, y		// 0xDB
		MOV ECX, x		// 0x205
		CALL [DrawItem]
	}
}

void DrawHighlightedItem(int x, int y, int Graphic, int Color)
{
	// Function broken
	return;

	if (x < 0 || x > 640 ||
		y < 0 || y > 640)
	{
		// Don't draw off screen
		return;
	}

	x += 64;
	y += 370;

	Graphic += 0x0C;
	int ItemSize = *((int *)(Graphic * 4 + 0x479658));
	int ItemHeight = *((int *)(Graphic * 4 + 0x479928));

	DrawBox(x - 64, y - 370, ItemSize, ItemHeight, DIABLO_COLOR_BLUE);

	// MOV EBP,DWORD PTR DS:[ESI*4+479658]
	int Internal = *((int *)(0x4B8CB0));

	int DrawHighlightedItem = (0x00416DC6);
	__asm
	{
		PUSH 8			// -- ?
		PUSH 0			// EBX -- ?
		PUSH ItemSize	// EBP -- Item size?
		PUSH Graphic	// ESI -- Graphic ID + 0x0C
		PUSH Internal	// -- ?	Render stats?
		MOV EDX, y		// 0xDB
		MOV ECX, x		// 0x205
		CALL [DrawItem]
	}
}
