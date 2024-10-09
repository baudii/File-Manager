#include "Common.h"

// SPanelSymbol
//--------------------------------------------------------------------------------------------------------------
SPanel_Symbol::SPanel_Symbol(wchar_t main_symbol, wchar_t attributes, wchar_t first_symbol, wchar_t last_symbol)
	:Main_Symbol(main_symbol), Attributes(attributes), First_Symbol(first_symbol), Last_Symbol(last_symbol)
{
}
//--------------------------------------------------------------------------------------------------------------




// SLine_Data
//--------------------------------------------------------------------------------------------------------------
SLine_Data::SLine_Data(unsigned short x, unsigned short y, unsigned short screen_width, unsigned short len) :
	X(x), Y(y), Screen_Width(screen_width), Len(len)
{
}
//--------------------------------------------------------------------------------------------------------------




// SFill_Data
//--------------------------------------------------------------------------------------------------------------
SFill_Data::SFill_Data(unsigned short x, unsigned short y, unsigned short screen_width, unsigned char width, unsigned char height)
	: X(x), Y(y), Screen_Width(screen_width), Width(width), Height(height)
{
}
//--------------------------------------------------------------------------------------------------------------




// SText_Data
//--------------------------------------------------------------------------------------------------------------
SText_Data::SText_Data(unsigned short x, unsigned short y, unsigned short screen_width, unsigned short attributes)
	: X(x), Y(y), Screen_Width(screen_width), Attributes(attributes)
{
}
//--------------------------------------------------------------------------------------------------------------




// Extention functions
//--------------------------------------------------------------------------------------------------------------
int Max(int a, int b)
{
	if (a > b)
		return a;
	return b;
}
//--------------------------------------------------------------------------------------------------------------
int Min(int a, int b)
{
	if (a < b)
		return a;
	return b;
}
//--------------------------------------------------------------------------------------------------------------