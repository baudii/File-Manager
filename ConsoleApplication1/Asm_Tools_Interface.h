#pragma once
#include "Panel.h"

//--------------------------------------------------------------------------------------------------------------
extern "C" int  Make_Sum(int a, int b);
extern "C" void Draw_Line_Horizontal(CHAR_INFO * screen_buffer, SLine_Data line, SPanel_Symbol symbol_data);
extern "C" void Draw_Line_Vertical(CHAR_INFO * screen_buffer, SLine_Data line, SPanel_Symbol symbol_data);
extern "C" void Fill_Area(CHAR_INFO * screen_buffer, SFill_Data fill_data, SPanel_Symbol symbol_data);
extern "C" void Show_Colors(CHAR_INFO * screen_buffer, SLine_Data line, CHAR_INFO symbol);
extern "C" int  Draw_Text(CHAR_INFO * screen_buffer, SText_Data text_data, const wchar_t* text);
extern "C" void Draw_Limited_Text(CHAR_INFO * screen_buffer, SText_Data text_data, const wchar_t* text, unsigned short limit);
//--------------------------------------------------------------------------------------------------------------