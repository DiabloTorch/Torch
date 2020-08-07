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

#include "Taskbar.h"
#include "Theme.h"
#include "Drawing.h"

#include <math.h>
#include <stdio.h>
#include <fstream>
using namespace std;
static ofstream fout("C:\\DrawLog.log");

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef PI
#define PI 3.14
#endif

TaskbarClass Taskbar;

TaskbarClass::TaskbarClass()
{
	TotalWindows = 0;
	
	for(int i = 0; i < TOTAL_WINDOWS; i++)
		WindowList[i] = NULL;

	CurrentState = Closed;

	bMouseDown = false;
	bHidden = false;
	LastMouseY = 0;
	y = 327;
}

TaskbarClass::~TaskbarClass()
{ }

void TaskbarClass::UpdateRects()
{
	MoveSlider.x = 640 - 10;
	MoveSlider.y = y;
	MoveSlider.Width = 10;
	MoveSlider.Height = 16;

	if(CurrentState == Closed)
	{
		OpenButton.x = 0;
		OpenButton.y = y;
		OpenButton.Width = 10;
		OpenButton.Height = 16;
	}
	else if(CurrentState == Open)
	{
		OpenButton.x = 640 - 10 - 10;
		OpenButton.y = y;
		OpenButton.Width = 10;
		OpenButton.Height = 16;
	}
	else
	{
		OpenButton.x = 0;
		OpenButton.y = 0;
		OpenButton.Width = 0;
		OpenButton.Height = 0;
	}
}

void TaskbarClass::Draw()
{
	if (bHidden)
	{
		return;
	}

	for(int i = 0; i < TotalWindows; i++)
	{
		WindowList[i]->Draw();
	}

	UpdateRects();

	DrawTransparentBox(MoveSlider.x, MoveSlider.y, MoveSlider.Width, MoveSlider.Height, Theme.GetBackgroundColor());
	DrawFrame(MoveSlider.x, MoveSlider.y, MoveSlider.Width, MoveSlider.Height, Theme.GetLineColor());

	DrawLine(MoveSlider.x + (MoveSlider.Width >> 1), MoveSlider.y + 2, MoveSlider.x + (MoveSlider.Width >> 1), MoveSlider.y + MoveSlider.Height - 2, Theme.GetLineColor());
	
	DrawTransparentBox(OpenButton.x, OpenButton.y, OpenButton.Width, OpenButton.Height, Theme.GetBackgroundColor());
	DrawFrame(OpenButton.x, OpenButton.y, OpenButton.Width, OpenButton.Height, Theme.GetLineColor());

	static int DrawStatePosition = 0;
	Rect Position;
	Position.x = 0;
	Position.y = y;
	Position.Width = 640 - 10 - 10;
	Position.Height = 16;

	switch(CurrentState)
	{
	case Open:
		{
			DrawTransparentBox(Position.x, Position.y, Position.Width, Position.Height, Theme.GetBackgroundColor());
			DrawFrame(Position.x, Position.y, Position.Width, Position.Height, Theme.GetLineColor());

			Rect First;
			First.x = 0;
			First.y = y;

			for(int i = 0; i < TotalWindows; i++)
			{
				if(WindowList[i]->GetWindowState() != Closed)
				{
					ClickRects[i].x = -1;
					ClickRects[i].y = -1;
					ClickRects[i].Width = 0;
					ClickRects[i].Height = 0;
					continue;
				}
				
				//First.Width = GetStringLength(WindowList[i]->GetTitle()) + 20;
				First.Width = strlen(WindowList[i]->GetTitle()) * 9 + 10;
				First.Height = 16;
				DrawFrame(First.x, First.y, First.Width, First.Height, Theme.GetLineColor());
				
				char szTitle[255];
				sprintf(szTitle, "%s", WindowList[i]->GetTitle());
				PrintTextXYCentered(First.x + First.Width / 2, First.y + 2, Theme.GetTextColor(), szTitle);

				ClickRects[i] = First;
				First.x += First.Width;
			}
		}
		break;

	case Opening:
		{
			int DrawnX = (int)Position.x + Position.Width - (int)((Position.x + Position.Width) * sin(DrawStatePosition * PI / 180));

			if(DrawStatePosition >= 90)
			{
				CurrentState = Open;
				DrawStatePosition = 0;
			}

			DrawStatePosition += WINDOW_ANIMATION_SPEED;
			DrawTransparentBox(Position.x, Position.y, Position.Width - DrawnX, Position.Height, Theme.GetBackgroundColor());
			DrawFrame(Position.x, Position.y, Position.Width - DrawnX, Position.Height, Theme.GetLineColor());
		}
		break;

	case Closed:
		break;

	case Closing:
		{
			int DrawnX = (int)Position.x + Position.Width - (int)((Position.x + Position.Width) * sin((90 - DrawStatePosition) * PI / 180));

			if(DrawStatePosition >= 90)
			{
				CurrentState = Closed;
				DrawStatePosition = 0;
			}

			DrawStatePosition += WINDOW_ANIMATION_SPEED;
			DrawTransparentBox(Position.x, Position.y, Position.Width - DrawnX, Position.Height, Theme.GetBackgroundColor());
			DrawFrame(Position.x, Position.y, Position.Width - DrawnX, Position.Height, Theme.GetLineColor());
		}
		break;
		
	default:
		break;
	}
}

void TaskbarClass::MouseMove(int x, int y)
{
	if (bHidden)
	{
		return;
	}

	if(bMouseDown)
	{
		int ThetaY = y - LastMouseY;

		if((this->y + MoveSlider.Height) + ThetaY >= 343)
		{
			ThetaY = 343 - (this->y + MoveSlider.Height);
			this->y += ThetaY;
			LastMouseY = y;
		}
		else if(this->y + ThetaY < 0)
		{
			this->y = 0;
			LastMouseY = y;
		}
		else
		{
			this->y += ThetaY;
			LastMouseY = y;
		}
	}

	for(int i = 0; i < TotalWindows; i++)
	{
		WindowList[i]->MouseMove(x, y);
	}
}

bool TaskbarClass::MouseButtonDown(int x, int y)
{
	if (bHidden)
	{
		return false;
	}

	UpdateRects();

	Rect Position;
	Position.x = 0;
	Position.y = this->y;
	Position.Width = 640 - 10 - 10;
	Position.Height = 16;
	
	if(PointInRect(x, y, OpenButton) && !bHidden)
	{
		switch(CurrentState)
		{
		case Closed:
			CurrentState = Opening;
			break;
			
		case Open:
			CurrentState = Closing;
			break;

		default:
			break;
		}

		return true;
	}
	else if(PointInRect(x, y, MoveSlider) && !bHidden)
	{
		bMouseDown = true;

		LastMouseY = y;

		return true;
	}
	else if(PointInRect(x, y, Position) && CurrentState == Open && !bHidden)
	{
		for(int i = 0; i < TotalWindows; i++)
		{
			if(PointInRect(x, y, ClickRects[i]))
			{
				WindowList[i]->SetWindowState(Entering);
				return true;
			}
		}

		return false;
	}
	else
	{
		int i = 0;
		for(i = TotalWindows; i > 0; i--)
		{
			if(WindowList[i - 1]->MouseButtonDown(x, y))
			{
				// Swap Window Positions (Z-Ordering)
				Window * Current = WindowList[i - 1];
				WindowList[i - 1] = WindowList[TotalWindows - 1];
				WindowList[TotalWindows - 1] = Current;

				break;
			}
			else
			{ }
		}

		if(i == 0)	// Made it to End, Wasn't Used
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

void TaskbarClass::MouseButtonUp(int x, int y)
{
	if (bHidden)
	{
		return;
	}

	MouseMove(x, y);
	bMouseDown = false;

	for(int i = 0; i < TotalWindows; i++)
	{
		WindowList[i]->MouseButtonUp(x, y);
	}
}

void TaskbarClass::AddWindow(Window * NewWindow)
{
	if(TotalWindows >= TOTAL_WINDOWS || TotalWindows < 0)
		return;

	WindowList[TotalWindows] = NewWindow;
	TotalWindows++;
}