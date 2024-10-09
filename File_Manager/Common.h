#pragma once
#include <windows.h>
#include <vector>
#include <string>

int Max(int a, int b);
int Min(int a, int b);
//--------------------------------------------------------------------------------------------------------------
struct SPanel_Symbol
{

	SPanel_Symbol(wchar_t main_symbol, wchar_t attributes, wchar_t first_symbol, wchar_t last_symbol);

	wchar_t Main_Symbol;
	unsigned short Attributes;
	wchar_t First_Symbol;
	wchar_t Last_Symbol;
};
//--------------------------------------------------------------------------------------------------------------
struct SLine_Data
{

	SLine_Data(unsigned short x, unsigned short y, unsigned short screen_width, unsigned short len);

	unsigned short X;
	unsigned short Y;
	unsigned short Screen_Width;
	unsigned short Len;
};
//--------------------------------------------------------------------------------------------------------------
struct SFill_Data
{

	SFill_Data(unsigned short x, unsigned short y, unsigned short screen_width, unsigned char width, unsigned char height);

	unsigned short X;
	unsigned short Y;
	unsigned short Screen_Width;
	unsigned char Width, Height;
};
//--------------------------------------------------------------------------------------------------------------
struct SText_Data
{

	SText_Data(unsigned short x, unsigned short y, unsigned short screen_width, unsigned short attributes);

	unsigned short X;
	unsigned short Y;
	unsigned short Screen_Width;
	unsigned short Attributes;
};
//--------------------------------------------------------------------------------------------------------------
