#include "Bottom_Panel.h"

// ABottom_Panel_Item
//--------------------------------------------------------------------------------------------------------------
ABottom_Panel_Item::ABottom_Panel_Item(const wchar_t* key, const wchar_t* name)
	: Key(key), Name(name)
{
}
//--------------------------------------------------------------------------------------------------------------
void ABottom_Panel_Item::Draw(CHAR_INFO* screen_buffer, unsigned short x_pos, unsigned short y_pos, unsigned short screen_width, unsigned short limit)
{
	// (x_pos + limit + offset) should be <= screen_width
	SText_Data key_text_data(x_pos, y_pos, screen_width, 0x07);
	int offset = Draw_Text(screen_buffer, key_text_data, Key);

	SText_Data name_text_data(x_pos + offset, y_pos, screen_width, 0xb0);
	Draw_Limited_Text(screen_buffer, name_text_data, Name, limit);
}
//--------------------------------------------------------------------------------------------------------------




// AsBottom_Panel
//--------------------------------------------------------------------------------------------------------------
AsBottom_Panel::AsBottom_Panel(CHAR_INFO* screen_buffer, unsigned short screen_width, unsigned short x_pos, unsigned short y_pos)
	: Screen_Buffer(screen_buffer), Screen_Width(screen_width), X_Pos(x_pos), Y_Pos(y_pos)
{
	Items[0] = new ABottom_Panel_Item(L"1", L"Help");
	Items[1] = new ABottom_Panel_Item(L"2", L"UserMenu");
	Items[2] = new ABottom_Panel_Item(L"3", L"View");
	Items[3] = new ABottom_Panel_Item(L"4", L"Edit");
	Items[4] = new ABottom_Panel_Item(L"5", L"Copy");
	Items[5] = new ABottom_Panel_Item(L"6", L"RenMov");
	Items[6] = new ABottom_Panel_Item(L"7", L"MakeDir");
	Items[7] = new ABottom_Panel_Item(L"8", L"Delete");
	Items[8] = new ABottom_Panel_Item(L"9", L"Config");
	Items[9] = new ABottom_Panel_Item(L"10", L"Quit");
}
//--------------------------------------------------------------------------------------------------------------
void AsBottom_Panel::Draw()
{
	int step = Screen_Width / 10;
	int x_pos = X_Pos;
	for (int i = 0; i < 10; i++)
	{
		Items[i]->Draw(Screen_Buffer, x_pos, Y_Pos, Screen_Width, 9);
		x_pos += step;
	}
}
//--------------------------------------------------------------------------------------------------------------
