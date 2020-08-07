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

const char szTorchHelp[] =
{
	"$DIABLO TORCH KEYS:|"
	"~:	 Hide Torch menus (tilde key)|"
	"Q:	 Open stash|"
	"A:	 Visit Adria (single player access)|"
	"P:  Toggle party status|"
	"MW: The mouse wheel will now act as up / down arrow keys|"
	"|"
	"$STASH:|"
	"Torch features a full tetris style, drag and drop stash. This "
	"stash is shared between all of your characters, and may only "
	"be accessed while in town. Tabs may be named to help you keep "
	"track of what you are storing, see chat commands or TorchStash.txt.|"
	"|"
	"The mouse scroll wheel can be used to scroll through stash tabs, even while "
	"an item is being held.|"
	"|"
	"$ITEM HIGHLIGHTING:|"
	"Holding down the alt key will highlight all items on the ground.|"
	"|"
	"$Auto-map:|"
	"Auto map has been expanded to include a counter for the number of "
	"monsters active on the current level, how many of those monsters are "
	"unique, and if there are any unique items on the ground. This information "
	"is displayed directly under the level text when the auto map is active.|"
	"|"
	"Auto map will now show other players as a blue arrow.|"
	"|"
	"Auto map can now be scrolled using the mouse wheel. Holding ctrl allows "
	"horizontal scrolling using the mouse wheel as well.|"
	"|"
	"$DURABILITY METER:|"
	"If enabled, the durability meter will show your item durability for each "
	"slot in the bottom right of the screen. Slots with no equipment are not "
	"shown. The order is: right hand (weapon), head, body, left hand (shield). "
	"If a staff is equipped, it's bar will be blue an indicate the charge of "
	"spells remaining. If a weapon or shield is equipped, its bar will be red. "
	" Head or body armors will use a gold bar.|"
	"|"
	"$SHRINE LIST:|"
	"A full shrine list is available in the shrines window. For quick access, "
	"keep the shrines window open and simply hide it with the ~ key.|"
	"|"
	"$CHAT COMMANDS:|"
	"$  -tab name (text)|"
	"    Renames most recently viewed stash tab to (text).|"
	"$  -tab color (color)|"
	"    Changes color for most recently viewed stash tab.|"
	"    Valid options are white, gold, or red.|"
	"$  -adria:|"
	"    Instantly visit adria|"
	"$  -gris:|"
	"    Instantly visit Griswold|"
	"$  -pepin:|"
	"    Instantly visit Pepin|"
	"$  -wirt:|"
	"    Instantly visit wirt|"
	"$  -cain:|"
	"    Instantly visit Cain|"
	"|"
	"$PARTY SCREEN:|"
	"Pushing P will toggle the party status display. The party status will "
	"give info about other players, such as their current life, character level, "
	"and location within the dungeon.|"
	"|"
	"$CONTACT:|"
	"Feel free to contact me at diablotorch@gmail.com with bugs or suggestions.|"
	"|"
	"Most features can be turned on / off in the options window.|"
	"|"
	"Enjoy Torch!|"
	"|"
	"&"
};

const char* GetHelpString()
{
	return &szTorchHelp[0];
}