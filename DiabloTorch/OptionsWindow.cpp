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

#include <stdio.h>

//#include "Offsets.h"
#include "OptionsWindow.h"
#include "Drawing.h"
#include "Theme.h"

#include "Registry.h"

Registry rTransparentWindows("DiabloTorch\\TransparentWindows");
Registry rExpBar("DiabloTorch\\ExpBar");
Registry rPlayerLifeBars("DiabloTorch\\PlayerLifeBars");
Registry rPartyStatus("DiabloTorch\\PartyStatus");
Registry rPlayerMapArrows("DiabloTorch\\PlayerMapArrows");
Registry rClock("DiabloTorch\\Clock");
Registry rLifeBar("DiabloTorch\\LifeBar");
Registry rShowInventoryValues("DiabloTorch\\ShowInventoryValues");
Registry rAutoHighlightItems("DiabloTorch\\AutoHighlightItems");
Registry rLifeText("DiabloTorch\\LiftText");
Registry rDurabilityBars("DiabloTorch\\DurabilityBars");

OptionsClass Options;

void OptionsClass::SaveRegistry()
{
	rTransparentWindows = (int)TransparentWindows;
	rExpBar = (int)ExpBar;
	rPlayerLifeBars = (int)PlayerLifeBars;
	rPartyStatus = (int)PartyStatus;
	rPlayerMapArrows = (int)PlayerMapArrows;
	rClock = (int)Clock;
	rLifeBar = (int)LifeBar;
	rShowInventoryValues = (int)ShowInventoryValues;
	rAutoHighlightItems = (int)AutoHighlightItems;
	rLifeText = (int)LifeText;
	rDurabilityBars = (int)DurabilityBars;
}

OptionsClass::OptionsClass()
{
	SetX(50);
	SetY(100);

	SetWidth(340);
	SetHeight(100);
	SetTitle("Options");

	int Temp;
	
	TransparentWindows = rTransparentWindows.getInt(&Temp);
	if (Temp != KEY_SUCCESS)
	{
		TransparentWindows = true;
	}

	ExpBar = rExpBar.getInt(&Temp);
	if (Temp != KEY_SUCCESS)
	{
		ExpBar = true;
	}

	PlayerLifeBars = rPlayerLifeBars.getInt(&Temp);
	if (Temp != KEY_SUCCESS)
	{
		PlayerLifeBars = true;
	}

	PartyStatus = rPartyStatus.getInt(&Temp);
	if (Temp != KEY_SUCCESS)
	{
		PartyStatus = true;
	}

	PlayerMapArrows = rPlayerMapArrows.getInt(&Temp);
	if (Temp != KEY_SUCCESS)
	{
		PlayerMapArrows = true;
	}

	Clock = rClock.getInt(&Temp);
	if (Temp != KEY_SUCCESS)
	{
		Clock = true;
	}

	LifeBar = rLifeBar.getInt(&Temp);
	if (Temp != KEY_SUCCESS)
	{
		LifeBar = true;
	}

	ShowInventoryValues = rShowInventoryValues.getInt(&Temp);
	if (Temp != KEY_SUCCESS)
	{
		ShowInventoryValues = true;
	}

	AutoHighlightItems = rAutoHighlightItems.getInt(&Temp);
	if (Temp != KEY_SUCCESS)
	{
		AutoHighlightItems = true;
	}

	LifeText = rLifeText.getInt(&Temp);
	if (Temp != KEY_SUCCESS)
	{
		LifeText = 0;
	}

	DurabilityBars = rDurabilityBars.getInt(&Temp);
	if (Temp != KEY_SUCCESS)
	{
		DurabilityBars = false;
	}
}

OptionsClass::~OptionsClass()
{
}

bool OptionsClass::MouseButtonDown(int x, int y)
{
	if (GetWindowState() == Open)
	{
		// Check tabs
		if (PointInRect(x, y, TabBar))
		{
			Tab = (x - TabBar.x) / (TabBar.Width / 4);
			return true;
		}

		// Check options
		switch (Tab)
		{
		case TAB_ITEMS:
			// Inventory values
			if (PointInRect(x, y, Option1))
			{
				ShowInventoryValues = !ShowInventoryValues;
				SaveRegistry();
				return true;
			}

			// Autohighlight
			if (PointInRect(x, y, Option2))
			{
				AutoHighlightItems = !AutoHighlightItems;
				SaveRegistry();
				return true;
			}

			break;

		case TAB_PLAYER:
			// Experience bar
			if (PointInRect(x, y, Option1))
			{
				ExpBar = !ExpBar;
				SaveRegistry();
				return true;
			}
			else if (PointInRect(x, y, Option2))
			{
				PlayerLifeBars = !PlayerLifeBars;
				SaveRegistry();
				return true;
			}
			else if (PointInRect(x, y, Option3))
			{
				PartyStatus = !PartyStatus;
				SaveRegistry();
				return true;
			}
			else if (PointInRect(x, y, Option4))
			{
				PlayerMapArrows = !PlayerMapArrows;
				SaveRegistry();
				return true;
			}
			break;

		case TAB_MONSTERS:
			// Life bar
			if (PointInRect(x, y, Option1))
			{
				LifeBar = !LifeBar;
				SaveRegistry();
				return true;
			}
			break;

		case TAB_MISC:
			// Transparent Windows
			if (PointInRect(x, y, Option1))
			{
				TransparentWindows = !TransparentWindows;
				SaveRegistry();
				return true;
			}

			// Clock
			if (PointInRect(x, y, Option2))
			{
				Clock = !Clock;
				SaveRegistry();
				return true;
			}

			// Life text
			if (PointInRect(x, y, Option3))
			{
				LifeText++;
				if (LifeText == 4)
				{
					LifeText = 0;
				}

				SaveRegistry();
				return true;
			}

			// Durability bars
			if (PointInRect(x, y, Option4))
			{
				DurabilityBars = !DurabilityBars;
				SaveRegistry();
				return true;
			}
			break;
		}
	}

	return Window::MouseButtonDown(x, y);
}

void OptionsClass::UpdateRects()
{
	Rect ClientRect = GetClientRect();

	TabBar.x = ClientRect.x + 5;
	TabBar.y = ClientRect.y + 5;
	TabBar.Width = ClientRect.Width - 10;
	TabBar.Height = 16;

	Option1.x = ClientRect.x + 5;
	Option1.y = TabBar.y + TabBar.Height + 9;
	Option1.Width = 7;
	Option1.Height = 7;

	Option2 = Option1;
	Option2.y += 14;

	Option3 = Option2;
	Option3.y += 14;

	Option4 = Option3;
	Option4.y += 14;
}

void OptionsClass::Draw()
{
	Window::Draw();

	if(GetWindowState() != Open)
		return;

	UpdateRects();

	int DrawX = GetClientRect().x;
	int DrawY = GetClientRect().y;

	// Draw tab bar
	DrawFrame(TabBar.x, TabBar.y, TabBar.Width, TabBar.Height, DIABLO_COLOR_DARKGOLD);
	DrawLine(TabBar.x + TabBar.Width * 1 / 4, TabBar.y, TabBar.x + TabBar.Width * 1 / 4, TabBar.y + TabBar.Height, DIABLO_COLOR_DARKGOLD);
	DrawLine(TabBar.x + TabBar.Width * 2 / 4, TabBar.y, TabBar.x + TabBar.Width * 2 / 4, TabBar.y + TabBar.Height, DIABLO_COLOR_DARKGOLD);
	DrawLine(TabBar.x + TabBar.Width * 3 / 4, TabBar.y, TabBar.x + TabBar.Width * 3 / 4, TabBar.y + TabBar.Height, DIABLO_COLOR_DARKGOLD);

	DrawFrame(TabBar.x + TabBar.Width * Tab / 4, TabBar.y, TabBar.Width / 4, TabBar.Height, DIABLO_COLOR_BRIGHTGOLD);

	PrintTextXYCentered(TabBar.x + TabBar.Width * 1 / 8, TabBar.y + 2, Theme.GetTextColor(), "ITEMS");
	PrintTextXYCentered(TabBar.x + TabBar.Width * 3 / 8, TabBar.y + 2, Theme.GetTextColor(), "PLAYER");
	PrintTextXYCentered(TabBar.x + TabBar.Width * 5 / 8, TabBar.y + 2, Theme.GetTextColor(), "MONSTERS");
	PrintTextXYCentered(TabBar.x + TabBar.Width * 7 / 8, TabBar.y + 2, Theme.GetTextColor(), "MISC");

	// Draw boxes
	char szCheckbox[255];

	switch (Tab)
	{
	case TAB_ITEMS:
		// Inventory values
		sprintf(szCheckbox, "Item values in inventory");
		DrawFrame(Option1.x, Option1.y, Option1.Width, Option1.Height, Theme.GetLineColor());
		PrintTextXY(Option1.x + Option1.Width + 3, Option1.y - 3, Theme.GetTextColor(), szCheckbox);
		if(ShowInventoryValues)
		{
			DrawLine(Option1.x, Option1.y, Option1.x + Option1.Width, Option1.y + Option1.Height, Theme.GetLineColor());
			DrawLine(Option1.x + Option1.Width, Option1.y, Option1.x, Option1.y + Option1.Height, Theme.GetLineColor());
		}

		// Auto highlight
		sprintf(szCheckbox, "Autohighlight dropped items");
		DrawFrame(Option2.x, Option2.y, Option2.Width, Option2.Height, Theme.GetLineColor());
		PrintTextXY(Option2.x + Option2.Width + 3, Option2.y - 3, Theme.GetTextColor(), szCheckbox);
		if(AutoHighlightItems)
		{
			DrawLine(Option2.x, Option2.y, Option2.x + Option2.Width, Option2.y + Option2.Height, Theme.GetLineColor());
			DrawLine(Option2.x + Option2.Width, Option2.y, Option2.x, Option2.y + Option2.Height, Theme.GetLineColor());
		}
		break;

	case TAB_PLAYER:
		// Experience bar
		sprintf(szCheckbox, "Show experience bar");
		DrawFrame(Option1.x, Option1.y, Option1.Width, Option1.Height, Theme.GetLineColor());
		PrintTextXY(Option1.x + Option1.Width + 3, Option1.y - 3, Theme.GetTextColor(), szCheckbox);
		if(ExpBar)
		{
			DrawLine(Option1.x, Option1.y, Option1.x + Option1.Width, Option1.y + Option1.Height, Theme.GetLineColor());
			DrawLine(Option1.x + Option1.Width, Option1.y, Option1.x, Option1.y + Option1.Height, Theme.GetLineColor());
		}

		// Player life bars
		sprintf(szCheckbox, "Show player life bars");
		DrawFrame(Option2.x, Option2.y, Option2.Width, Option2.Height, Theme.GetLineColor());
		PrintTextXY(Option2.x + Option2.Width + 3, Option2.y - 3, Theme.GetTextColor(), szCheckbox);
		if (PlayerLifeBars)
		{
			DrawLine(Option2.x, Option2.y, Option2.x + Option2.Width, Option2.y + Option2.Height, Theme.GetLineColor());
			DrawLine(Option2.x + Option2.Width, Option2.y, Option2.x, Option2.y + Option2.Height, Theme.GetLineColor());
		}

		// Party status
		sprintf(szCheckbox, "Show party status");
		DrawFrame(Option3.x, Option3.y, Option3.Width, Option3.Height, Theme.GetLineColor());
		PrintTextXY(Option3.x + Option3.Width + 3, Option3.y - 3, Theme.GetTextColor(), szCheckbox);
		if (PartyStatus)
		{
			DrawLine(Option3.x, Option3.y, Option3.x + Option3.Width, Option3.y + Option3.Height, Theme.GetLineColor());
			DrawLine(Option3.x + Option3.Width, Option3.y, Option3.x, Option3.y + Option3.Height, Theme.GetLineColor());
		}

		// Player map arrows
		sprintf(szCheckbox, "Show other players on automap");
		DrawFrame(Option4.x, Option4.y, Option4.Width, Option4.Height, Theme.GetLineColor());
		PrintTextXY(Option4.x + Option4.Width + 3, Option4.y - 3, Theme.GetTextColor(), szCheckbox);
		if (PlayerMapArrows)
		{
			DrawLine(Option4.x, Option4.y, Option4.x + Option4.Width, Option4.y + Option4.Height, Theme.GetLineColor());
			DrawLine(Option4.x + Option4.Width, Option4.y, Option4.x, Option4.y + Option4.Height, Theme.GetLineColor());
		}
		break;

	case TAB_MONSTERS:
		// Life bar
		sprintf(szCheckbox, "Show monster life");
		DrawFrame(Option1.x, Option1.y, Option1.Width, Option1.Height, Theme.GetLineColor());
		PrintTextXY(Option1.x + Option1.Width + 3, Option1.y - 3, Theme.GetTextColor(), szCheckbox);
		if(LifeBar)
		{
			DrawLine(Option1.x, Option1.y, Option1.x + Option1.Width, Option1.y + Option1.Height, Theme.GetLineColor());
			DrawLine(Option1.x + Option1.Width, Option1.y, Option1.x, Option1.y + Option1.Height, Theme.GetLineColor());
		}
		break;

	case TAB_MISC:
		// Transparent windows
		sprintf(szCheckbox, "Transparent windows");
		DrawFrame(Option1.x, Option1.y, Option1.Width, Option1.Height, Theme.GetLineColor());
		PrintTextXY(Option1.x + Option1.Width + 3, Option1.y - 3, Theme.GetTextColor(), szCheckbox);
		if(TransparentWindows)
		{
			DrawLine(Option1.x, Option1.y, Option1.x + Option1.Width, Option1.y + Option1.Height, Theme.GetLineColor());
			DrawLine(Option1.x + Option1.Width, Option1.y, Option1.x, Option1.y + Option1.Height, Theme.GetLineColor());
		}

		// Clock
		sprintf(szCheckbox, "Show clock");
		DrawFrame(Option2.x, Option2.y, Option2.Width, Option2.Height, Theme.GetLineColor());
		PrintTextXY(Option2.x + Option2.Width + 3, Option2.y - 3, Theme.GetTextColor(), szCheckbox);
		if(Clock)
		{
			DrawLine(Option2.x, Option2.y, Option2.x + Option2.Width, Option2.y + Option2.Height, Theme.GetLineColor());
			DrawLine(Option2.x + Option2.Width, Option2.y, Option2.x, Option2.y + Option2.Height, Theme.GetLineColor());
		}

		// Life text
		sprintf(szCheckbox, "Show life / mana text");
		DrawFrame(Option3.x, Option3.y, Option3.Width, Option3.Height, Theme.GetLineColor());
		PrintTextXY(Option3.x + Option3.Width + 3, Option3.y - 3, Theme.GetTextColor(), szCheckbox);
		if (LifeText)
		{
			DrawLine(Option3.x, Option3.y, Option3.x + Option3.Width, Option3.y + Option3.Height, Theme.GetLineColor());
			DrawLine(Option3.x + Option3.Width, Option3.y, Option3.x, Option3.y + Option3.Height, Theme.GetLineColor());
		}

		// Durability bars
		sprintf(szCheckbox, "Show durability meter");
		DrawFrame(Option4.x, Option4.y, Option4.Width, Option4.Height, Theme.GetLineColor());
		PrintTextXY(Option4.x + Option4.Width + 3, Option4.y - 3, Theme.GetTextColor(), szCheckbox);
		if (DurabilityBars)
		{
			DrawLine(Option4.x, Option4.y, Option4.x + Option4.Width, Option4.y + Option4.Height, Theme.GetLineColor());
			DrawLine(Option4.x + Option4.Width, Option4.y, Option4.x, Option4.y + Option4.Height, Theme.GetLineColor());
		}
		break;

	default:
		break;
	}
}
