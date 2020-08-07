/********   ********   ********/
/*          Theme.h           */
/********   ********   ********/
/*          8/7/2020          */
/********   ********   ********/
/*           Tiron            */
/********   ********   ********/
// http://tristr.am/
// https://freshmeat-blog.de.tl/
// https://d1legit.com/


// Class to easily theme custom windows with different color schemes (used in previous Starcraft work, not used in Diablo, would require theme window to control)

#ifndef THEME_H
#define THEME_H

class ThemeClass
{
private:
	int CurrentTheme;			// Theme in Use
	int SetTheme;				// Total Themes

	char * ThemeTitles[32];
	int BackgroundColor[32];
	int LineColor[32];
	int TitleBarColor[32];
	char TitleTextColor[32];
	char TextColor[32];

public:
	ThemeClass();
	~ThemeClass();

//	int GetBackgroundColor() { return BackgroundColor[CurrentTheme]; };
	int GetBackgroundColor();
	int GetLineColor() { return LineColor[CurrentTheme]; };
	int GetTitleBarColor() { return TitleBarColor[CurrentTheme]; };
	char GetTitleTextColor() { return TitleTextColor[CurrentTheme]; };
	char GetTextColor() { return TextColor[CurrentTheme]; };
	char * GetThemeName(int TheTheme) { return ThemeTitles[TheTheme]; };

	void SetCurrentTheme(int NewTheme) { CurrentTheme = (NewTheme >= 32 || NewTheme < 0)?0:NewTheme; };
	int GetCurrentTheme() { return CurrentTheme; };
	int GetThemeCount() { return SetTheme + 1; };
};

extern ThemeClass Theme;
#endif