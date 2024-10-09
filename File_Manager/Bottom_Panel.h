#pragma once
#include "Common.h"
#include "Asm_Tools_Interface.h"

//--------------------------------------------------------------------------------------------------------------
class ABottom_Panel_Item
{
public:
	ABottom_Panel_Item(const wchar_t* key, const wchar_t* name);

	void Draw(CHAR_INFO* screen_buffer, unsigned short x_pos, unsigned short y_pos, unsigned short screen_width, unsigned short limit);

private:
	const wchar_t* Key;
	const wchar_t* Name;
};
//--------------------------------------------------------------------------------------------------------------
class AsBottom_Panel
{
public:
	AsBottom_Panel(CHAR_INFO* screen_buffer, unsigned short screen_width, unsigned short x_pos, unsigned short y_pos);

	void Draw();

private:
	unsigned short X_Pos, Y_Pos;
	unsigned short Screen_Width;
	CHAR_INFO* Screen_Buffer;
	ABottom_Panel_Item* Items[10]{};
};
//--------------------------------------------------------------------------------------------------------------