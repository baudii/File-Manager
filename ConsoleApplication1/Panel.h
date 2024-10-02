#pragma once
#include "Common.h"
#include "Asm_Tools_Interface.h"

//--------------------------------------------------------------------------------------------------------------
class AFile_Data
{
public:
	AFile_Data(const wchar_t* file_name, unsigned int attribute, unsigned int size_low, unsigned int size_high);

	std::wstring File_Name;
	unsigned int Attribute;
	unsigned long long File_Size;
};
//--------------------------------------------------------------------------------------------------------------
class APanel
{
public:
	APanel(CHAR_INFO* screen_buffer, unsigned short screen_width, unsigned short x_pos, unsigned short y_pos, unsigned short width, unsigned short height, bool is_offset, bool is_panel_active);

	void Draw();
	void Get_Files(std::wstring path);
	void Move_Highlight(bool is_up);
	void Move_Highlight(int pointer);
	void Open_Highlighted_Dir();
	void Panel_Set_Active(bool is_active);

private:
	void Draw_Panels();
	void Draw_Files();
	void Highlight_File();
	void Draw_One_File(AFile_Data* file, int x_offset, int y_offset, unsigned short bg_color);

	CHAR_INFO* Screen_Buffer;
	unsigned short Screen_Width;
	unsigned short X_Pos, Y_Pos;
	unsigned short Highlight_X_Offset, Highlight_Y_Offset;
	unsigned short Width, Height;
	unsigned short Text_Limit_Offset;
	bool Is_Panel_Active;
	std::wstring Current_Path;

	int Max_Visible_Elements;

	int Pointer;

	std::vector<AFile_Data*> Files;
};
//--------------------------------------------------------------------------------------------------------------

