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

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#ifndef PI
#define PI 3.14
#endif
 
//#include "Offsets.h"
#include "Window.h"
#include "Drawing.h"
#include "OptionsWindow.h"
#include "Theme.h"
#include "Registry.h"

Window::Window()
{
	DrawState = Open;
	Position.x = 0;
	Position.y = 0;
	Position.Width = 0;
	Position.Height = 0;
	
	strcpy(Title, "\0\0");
}

Window::~Window()
{ }

void Window::UpdateRects()
{
	ClientRect.x = Position.x;
	ClientRect.y = Position.y + 13;
	ClientRect.Width = Position.Width;
	ClientRect.Height = Position.Height - 13;

	TitleBar.x = Position.x;
	TitleBar.y = Position.y;
	TitleBar.Width = Position.Width;
	TitleBar.Height = 13;

	CloseButton.x = TitleBar.x + TitleBar.Width - 3 - 7;
	CloseButton.y = TitleBar.y + 3;
	CloseButton.Width = 7;
	CloseButton.Height = 7;

	CollapseButton = CloseButton;
	CollapseButton.x = CollapseButton.x - CollapseButton.Width - 3;
}

void Window::Draw()
{
	UpdateRects();

	char szTempTitle[255];

	switch(DrawState)
	{
	case Closed:
		// Still State -- Draw Nothing
		return;
		break;

	case Closing:
		// Animation -- Slide off bottom of Screen
		{
			DrawStatePosition += WINDOW_ANIMATION_SPEED;

			int DrawnY = (int)((360) * sin(DrawStatePosition * PI / 180));

			if(DrawStatePosition >= 90)
			{
				SetWindowState(Closed);
			}

			DrawBox(Position.x, Position.y + DrawnY, Position.Width, 13, Theme.GetTitleBarColor());
			DrawFrame(Position.x, Position.y + DrawnY, Position.Width, 13, Theme.GetLineColor());
			DrawFrame(CloseButton.x, CloseButton.y + DrawnY, CloseButton.Width, CloseButton.Height, Theme.GetLineColor());
			DrawLine(CloseButton.x, CloseButton.y + DrawnY, CloseButton.x + CloseButton.Width, CloseButton.y + DrawnY + CloseButton.Height, Theme.GetLineColor());
			DrawLine(CloseButton.x + CloseButton.Width, CloseButton.y + DrawnY, CloseButton.x, CloseButton.y + CloseButton.Height + DrawnY, Theme.GetLineColor());

			sprintf(szTempTitle, "%s", Title);
			PrintTextXY(Position.x + 3, Position.y + DrawnY, Theme.GetTitleTextColor(), szTempTitle);

			DrawFrame(CollapseButton.x, CollapseButton.y + DrawnY, CollapseButton.Width, CollapseButton.Height, Theme.GetLineColor());
			DrawOrthoLine(CollapseButton.x + 2, CollapseButton.y + CollapseButton.Width - 2 + DrawnY, CollapseButton.x + CollapseButton.Width - 2, CollapseButton.y + DrawnY + CollapseButton.Width - 2, Theme.GetLineColor());
		}
		return;
		break;

	case Entering:
		// Animation -- Slide On from Left of Screen (Next: Opening)
		{
			DrawStatePosition += WINDOW_ANIMATION_SPEED;

			int DrawnX = (int)Position.x + Position.Width - (int)((Position.x + Position.Width) * sin(DrawStatePosition * PI / 180));

			if(DrawStatePosition >= 90)
			{
				DrawState = Opening;
				DrawStatePosition = 0;
			}

			DrawBox(Position.x - DrawnX, Position.y, Position.Width, 13, Theme.GetTitleBarColor());
			DrawFrame(Position.x - DrawnX, Position.y, Position.Width, 13, Theme.GetLineColor());
			DrawFrame(CloseButton.x - DrawnX, CloseButton.y, CloseButton.Width, CloseButton.Height, Theme.GetLineColor());
			DrawLine(CloseButton.x - DrawnX, CloseButton.y, CloseButton.x - DrawnX + CloseButton.Width, CloseButton.y + CloseButton.Height, Theme.GetLineColor());
			DrawLine(CloseButton.x + CloseButton.Width - DrawnX, CloseButton.y, CloseButton.x - DrawnX, CloseButton.y + CloseButton.Height, Theme.GetLineColor());

			sprintf(szTempTitle, "%s", Title);
			PrintTextXY(Position.x  - DrawnX + 3, Position.y, Theme.GetTitleTextColor(), szTempTitle);

			DrawFrame(CollapseButton.x - DrawnX, CollapseButton.y, CollapseButton.Width, CollapseButton.Height, Theme.GetLineColor());
			DrawOrthoLine(CollapseButton.x + 2 - DrawnX, CollapseButton.y + CollapseButton.Width - 2, CollapseButton.x - DrawnX + CollapseButton.Width - 2, CollapseButton.y + CollapseButton.Width - 2, Theme.GetLineColor());
		}
		return;
		break;

	case Leaving:
		// Animation -- Collapsing Animation (Next: Closing)
		{
			DrawStatePosition += WINDOW_ANIMATION_SPEED;

			int DrawnHeight = (int)(Position.Height + 13 - ((Position.Height - 13) * sin(DrawStatePosition * PI / 180) + 13));

			if(DrawStatePosition >= 90)
			{
				DrawState = Closing;
				DrawStatePosition = 0;
			}

			if(Options.TransparentWindows)
				DrawTransparentBox(Position.x, Position.y, Position.Width, DrawnHeight, Theme.GetBackgroundColor());
			else
				DrawBox(Position.x, Position.y, Position.Width, DrawnHeight, Theme.GetBackgroundColor());

			// Title Bar
			DrawBox(Position.x, Position.y, Position.Width, 13, Theme.GetTitleBarColor());

			// Window Frames
			DrawFrame(Position.x, Position.y, Position.Width, DrawnHeight, Theme.GetLineColor());
			DrawFrame(Position.x, Position.y, Position.Width, 13, Theme.GetLineColor());

			// Title Text
			sprintf(szTempTitle, "%s", Title);
			PrintTextXY(Position.x + 3, Position.y, Theme.GetTitleTextColor(), szTempTitle);

			// Buttons
			DrawFrame(CloseButton.x, CloseButton.y, CloseButton.Width, CloseButton.Height, Theme.GetLineColor());
			DrawLine(CloseButton.x, CloseButton.y, CloseButton.x + CloseButton.Width, CloseButton.y + CloseButton.Height, Theme.GetLineColor());
			DrawLine(CloseButton.x + CloseButton.Width, CloseButton.y, CloseButton.x, CloseButton.y + CloseButton.Height, Theme.GetLineColor());

			DrawFrame(CollapseButton.x, CollapseButton.y, CollapseButton.Width, CollapseButton.Height, Theme.GetLineColor());
			DrawOrthoLine(CollapseButton.x + 2, CollapseButton.y + CollapseButton.Width - 2, CollapseButton.x + CollapseButton.Width - 2, CollapseButton.y + CollapseButton.Width - 2, Theme.GetLineColor());
		}
		return;
		break;

	case Collapsed:
		// Still State -- Title Bar Only
		DrawBox(Position.x, Position.y, Position.Width, 13, Theme.GetTitleBarColor());
		DrawFrame(Position.x, Position.y, Position.Width, 13, Theme.GetLineColor());
		DrawFrame(CloseButton.x, CloseButton.y, CloseButton.Width, CloseButton.Height, Theme.GetLineColor());
		DrawLine(CloseButton.x, CloseButton.y, CloseButton.x + CloseButton.Width, CloseButton.y + CloseButton.Height, Theme.GetLineColor());
		DrawLine(CloseButton.x + CloseButton.Width, CloseButton.y, CloseButton.x, CloseButton.y + CloseButton.Height, Theme.GetLineColor());

		sprintf(szTempTitle, "%s", Title);
		PrintTextXY(Position.x + 3, Position.y, Theme.GetTitleTextColor(), szTempTitle);

		DrawFrame(CollapseButton.x, CollapseButton.y, CollapseButton.Width, CollapseButton.Height, Theme.GetLineColor());
		DrawOrthoLine(CollapseButton.x + 2, CollapseButton.y + CollapseButton.Width - 2, CollapseButton.x + CollapseButton.Width - 2, CollapseButton.y + CollapseButton.Width - 2, Theme.GetLineColor());
		return;
		break;

	case Collapsing:
		// Animation -- Bottom of Client Area Slides Up to Title
		{
			DrawStatePosition += WINDOW_ANIMATION_SPEED;
			if(DrawStatePosition >= 90)
				DrawState = Collapsed;

			int DrawnHeight = (int)(Position.Height + 13 - ((Position.Height - 13) * sin(DrawStatePosition * PI / 180) + 13));

			if(Options.TransparentWindows)
				DrawTransparentBox(Position.x, Position.y, Position.Width, DrawnHeight, Theme.GetBackgroundColor());
			else
				DrawBox(Position.x, Position.y, Position.Width, DrawnHeight, Theme.GetBackgroundColor());

			// Title Bar
			DrawBox(Position.x, Position.y, Position.Width, 13, Theme.GetTitleBarColor());

			// Window Frames
			DrawFrame(Position.x, Position.y, Position.Width, DrawnHeight, Theme.GetLineColor());
			DrawFrame(Position.x, Position.y, Position.Width, 13, Theme.GetLineColor());

			// Title Text
			sprintf(szTempTitle, "%s", Title);
			PrintTextXY(Position.x + 3, Position.y, Theme.GetTitleTextColor(), szTempTitle);

			// Buttons
			DrawFrame(CloseButton.x, CloseButton.y, CloseButton.Width, CloseButton.Height, Theme.GetLineColor());
			DrawLine(CloseButton.x, CloseButton.y, CloseButton.x + CloseButton.Width, CloseButton.y + CloseButton.Height, Theme.GetLineColor());
			DrawLine(CloseButton.x + CloseButton.Width, CloseButton.y, CloseButton.x, CloseButton.y + CloseButton.Height, Theme.GetLineColor());

			DrawFrame(CollapseButton.x, CollapseButton.y, CollapseButton.Width, CollapseButton.Height, Theme.GetLineColor());
			DrawOrthoLine(CollapseButton.x + 2, CollapseButton.y + CollapseButton.Width - 2, CollapseButton.x + CollapseButton.Width - 2, CollapseButton.y + CollapseButton.Width - 2, Theme.GetLineColor());
		}
		return;
		break;

	case Opening:
		// Animation -- Bottom of Client Area Slides Down from Top
		{
			DrawStatePosition += WINDOW_ANIMATION_SPEED;
			if(DrawStatePosition >= 90)
				DrawState = Open;

			int DrawnHeight = (int)((Position.Height - 13) * sin(DrawStatePosition * PI / 180) + 13);

			if(Options.TransparentWindows)
				DrawTransparentBox(Position.x, Position.y, Position.Width, DrawnHeight, Theme.GetBackgroundColor());
			else
				DrawBox(Position.x, Position.y, Position.Width, DrawnHeight, Theme.GetBackgroundColor());

			// Title Bar
			DrawBox(Position.x, Position.y, Position.Width, 13, Theme.GetTitleBarColor());

			// Window Frames
			DrawFrame(Position.x, Position.y, Position.Width, DrawnHeight, Theme.GetLineColor());
			DrawFrame(Position.x, Position.y, Position.Width, 13, Theme.GetLineColor());

			// Title Text
			sprintf(szTempTitle, "%s", Title);
			PrintTextXY(Position.x + 3, Position.y, Theme.GetTitleTextColor(), szTempTitle);

			// Buttons
			DrawFrame(CloseButton.x, CloseButton.y, CloseButton.Width, CloseButton.Height, Theme.GetLineColor());
			DrawLine(CloseButton.x, CloseButton.y, CloseButton.x + CloseButton.Width, CloseButton.y + CloseButton.Height, Theme.GetLineColor());
			DrawLine(CloseButton.x + CloseButton.Width, CloseButton.y, CloseButton.x, CloseButton.y + CloseButton.Height, Theme.GetLineColor());

			DrawFrame(CollapseButton.x, CollapseButton.y, CollapseButton.Width, CollapseButton.Height, Theme.GetLineColor());
			DrawOrthoLine(CollapseButton.x + 2, CollapseButton.y + CollapseButton.Width - 2, CollapseButton.x + CollapseButton.Width - 2, CollapseButton.y + CollapseButton.Width - 2, Theme.GetLineColor());
		}
		return;
		break;

	case Open:
		// Still State -- Normal Window Drawing
		// Background Box
		if(Options.TransparentWindows)
			DrawTransparentBox(Position.x, Position.y, Position.Width, Position.Height, Theme.GetBackgroundColor());
		else
			DrawBox(Position.x, Position.y, Position.Width, Position.Height, Theme.GetBackgroundColor());

		// Title Bar
		DrawBox(Position.x, Position.y, Position.Width, 13, Theme.GetTitleBarColor());

		// Window Frames
		DrawFrame(Position.x, Position.y, Position.Width, Position.Height, Theme.GetLineColor());
		DrawFrame(Position.x, Position.y, Position.Width, 13, Theme.GetLineColor());

		// Title Text
		sprintf(szTempTitle, "%s", Title);
		PrintTextXY(Position.x + 3, Position.y, Theme.GetTitleTextColor(), szTempTitle);

		// Buttons
		DrawFrame(CloseButton.x, CloseButton.y, CloseButton.Width, CloseButton.Height, Theme.GetLineColor());
		DrawLine(CloseButton.x, CloseButton.y, CloseButton.x + CloseButton.Width, CloseButton.y + CloseButton.Height, Theme.GetLineColor());
		DrawLine(CloseButton.x + CloseButton.Width, CloseButton.y, CloseButton.x, CloseButton.y + CloseButton.Height, Theme.GetLineColor());

		DrawFrame(CollapseButton.x, CollapseButton.y, CollapseButton.Width, CollapseButton.Height, Theme.GetLineColor());
		DrawOrthoLine(CollapseButton.x + 2, CollapseButton.y + CollapseButton.Width - 2, CollapseButton.x + CollapseButton.Width - 2, CollapseButton.y + CollapseButton.Width - 2, Theme.GetLineColor());
		return;
		break;
	}
}

void Window::SetX(int NewX)
{
	Position.x = NewX;
}

void Window::SetY(int NewY)
{
	Position.y = NewY;
}

void Window::SetWidth(int NewWidth)
{
	Position.Width = NewWidth;
}

void Window::SetHeight(int NewHeight)
{
	Position.Height = NewHeight;
}

Rect Window::GetPosition()
{
	return Position;
}

Rect Window::GetTitleBar()
{
	TitleBar.x = Position.x;
	TitleBar.y = Position.y;
	TitleBar.Width = Position.Width;
	TitleBar.Height = 13;
	return TitleBar;
}

Rect Window::GetClientRect()
{
	ClientRect.x = Position.x;
	ClientRect.y = Position.y + 13;
	ClientRect.Width = Position.Width;
	ClientRect.Height = Position.Height - 13;
	return ClientRect;
}

Rect Window::GetCollapseButton()
{
	UpdateRects();
	return CollapseButton;
}

Rect Window::GetCloseButton()
{
	UpdateRects();
	return CloseButton;
}

void Window::SetTitle(char * NewTitle)
{
	strcpy(Title, NewTitle);
	LoadWindowState();
}

void Window::MouseMove(int x, int y)
{
	if(IsMouseButtonDown)
	{
		int ThetaX = x - LastMousePositionX;
		int ThetaY = y - LastMousePositionY;

		Position.x += ThetaX;
		Position.y += ThetaY;

		LastMousePositionX = x;
		LastMousePositionY = y;
	}
}

bool Window::MouseButtonDown(int x, int y)
{
	// Update TitleBar and ClientRect structres...
	UpdateRects();
	
	if(DrawState != Open && DrawState != Collapsed)
		return false;

	if(PointInRect(x, y, CollapseButton))
	{
		if(DrawState == Open)
		{
			DrawState = Collapsing;
			DrawStatePosition = 0;
		}
		else if(DrawState == Collapsed)
		{
			DrawState = Opening;
			DrawStatePosition = 0;
		}
	}
	else if(PointInRect(x, y, CloseButton))
	{
		if(DrawState == Open)
		{
			DrawState = Leaving;
			DrawStatePosition = 0;
		}
		if(DrawState == Collapsed)
		{
			DrawState = Closing;
			DrawStatePosition = 0;
		}
	}
	else if(PointInRect(x, y, TitleBar))
	{
			IsMouseButtonDown = true;

			LastMousePositionX = x;
			LastMousePositionY = y;
	}
	else
	{
		return false;
	}

	return true;
}

void Window::MouseButtonUp(int x, int y)
{
	MouseMove(x, y);
	IsMouseButtonDown = false;
}

void Window::SetWindowState(WindowState NewDrawState)
{
	DrawState = NewDrawState;
	DrawStatePosition = 0;

	char szKey[255];
	sprintf(szKey, "DiabloTorch\\Windows\\%sWindow", Title);
	Registry rWindow(szKey);

	rWindow = (int)DrawState;
}

void Window::LoadWindowState()
{
	char szKey[255];
	sprintf(szKey, "DiabloTorch\\Windows\\%sWindow", Title);
	Registry rWindow(szKey);

	int Error;
	DrawState = (WindowState)rWindow.getInt(&Error);

	if (Error != KEY_SUCCESS)
	{
		SetWindowState(Open);
	}
}

bool PointInRect(int x, int y, Rect Test)
{
	if(x >= Test.x && x <= Test.x + Test.Width)
	{
		if(y >= Test.y && y <= Test.y + Test.Height)
		{
			return true;
		}
	}

	return false;
}