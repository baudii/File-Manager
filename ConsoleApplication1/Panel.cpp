#include "Panel.h"

// AFile_Data
//--------------------------------------------------------------------------------------------------------------
AFile_Data::AFile_Data(const wchar_t* file_name, unsigned int attribute, unsigned int size_low, unsigned int size_high)
	: File_Name(file_name), Attribute(attribute)
{
	File_Size = ((unsigned long long)size_high << 32) | (unsigned long long)size_low;
}
//--------------------------------------------------------------------------------------------------------------





// APanel
//--------------------------------------------------------------------------------------------------------------
APanel::APanel(CHAR_INFO* screen_buffer, unsigned short screen_width, unsigned short x_pos, unsigned short y_pos, unsigned short width, unsigned short height, bool is_right, bool is_panel_active)
	: Screen_Buffer(screen_buffer), Screen_Width(screen_width), X_Pos(x_pos), Y_Pos(y_pos), Width(width), Height(height), Pointer(0), Is_Panel_Active(is_panel_active)
{
	Max_Visible_Elements = (Height - 5) * 2;
	Highlight_X_Offset = 0;
	Highlight_Y_Offset = 0;
	Text_Limit_Offset = 0;
	if (is_right)
		Text_Limit_Offset = 1;
}
//--------------------------------------------------------------------------------------------------------------
void APanel::Draw()
{
	Draw_Panels();
	Draw_Files();
	Highlight_File();
}
//--------------------------------------------------------------------------------------------------------------
void APanel::Get_Files(std::wstring path)
{
	WIN32_FIND_DATAW file_data{};
	HANDLE file_handle;
	file_handle = FindFirstFileW((path + L"\\*.*").c_str(), &file_data);

	if (!FindNextFileW(file_handle, &file_data))
		return;

	for (auto* file : Files)
		delete file;
	Files.erase(Files.begin(), Files.end());

	AFile_Data* file = new AFile_Data(file_data.cFileName, file_data.dwFileAttributes, file_data.nFileSizeLow, file_data.nFileSizeHigh);
	Files.push_back(file);

	while (FindNextFileW(file_handle, &file_data))
	{
		AFile_Data* file = new AFile_Data(file_data.cFileName, file_data.dwFileAttributes, file_data.nFileSizeLow, file_data.nFileSizeHigh);
		Files.push_back(file);
	}

	if (Files.size() == 0)
	{
		const wchar_t* name = L"..";
		AFile_Data* back = new AFile_Data(name, FILE_ATTRIBUTE_DIRECTORY, 0, 0);
		Files.push_back(back);
		Pointer = 0;
		return;
	}

	Current_Path = path;

	Pointer = 0;
}
//--------------------------------------------------------------------------------------------------------------
void APanel::Move_Highlight(bool is_up)
{
	if (is_up)
	{
		if (Pointer > 0)
			Pointer--;
	}
	else
	{
		if (Pointer + 2 < Max_Visible_Elements - 1 && Pointer < (Files.size() - 1))
			Pointer++;
	}
}
//--------------------------------------------------------------------------------------------------------------
void APanel::Move_Highlight(int pointer)
{
	if (pointer > 0 && pointer < Min(Max_Visible_Elements, Files.size() - 1))
		Pointer = pointer;
}
//--------------------------------------------------------------------------------------------------------------
void APanel::Open_Highlighted_Dir()
{
	AFile_Data* file = Files[Pointer];
	if (file->Attribute & FILE_ATTRIBUTE_DIRECTORY)
		Get_Files(Current_Path + L"\\" + file->File_Name);
}
//--------------------------------------------------------------------------------------------------------------
void APanel::Panel_Set_Active(bool is_active)
{
	Is_Panel_Active = is_active;
}
//--------------------------------------------------------------------------------------------------------------
void APanel::Draw_Panels()
{
	// Fill area
	{
		SFill_Data fill_data(X_Pos, Y_Pos, Screen_Width, Width, Height);
		SPanel_Symbol symbol_data(L' ', 0x1b, 0, 0);
		Fill_Area(Screen_Buffer, fill_data, symbol_data);
	}

	// Vertical line_data left
	{
		SLine_Data line_data(X_Pos, Y_Pos + 1, Screen_Width, Height - 4);
		SPanel_Symbol symbol_data(L'║', 0x1b, L'║', L'║');
		Draw_Line_Vertical(Screen_Buffer, line_data, symbol_data);
	}
	// Vertical line_data right
	{
		SLine_Data line_data(X_Pos + Width - 1, Y_Pos + 1, Screen_Width, Height - 4);
		SPanel_Symbol symbol_data(L'║', 0x1b, L'║', L'║');
		Draw_Line_Vertical(Screen_Buffer, line_data, symbol_data);
	}
	// Horizontal line_data top
	{
		SLine_Data line_data(X_Pos, Y_Pos, Screen_Width, Width - 2);
		SPanel_Symbol symbol_data(L'═', 0x1b, L'╔', L'╗');
		Draw_Line_Horizontal(Screen_Buffer, line_data, symbol_data);
	}
	// Horizontal line_data bottom
	{
		SLine_Data line_data(X_Pos, Y_Pos + Height - 1, Screen_Width, Width - 2);
		SPanel_Symbol symbol_data(L'═', 0x1b, L'╚', L'╝');
		Draw_Line_Horizontal(Screen_Buffer, line_data, symbol_data);
	}
	// Horizontal line_data middle
	{
		SLine_Data line_data(X_Pos, Y_Pos + Height - 3, Screen_Width, Width - 2);
		SPanel_Symbol symbol_data(L'─', 0x1b, L'╟', L'╢');
		Draw_Line_Horizontal(Screen_Buffer, line_data, symbol_data);
	}
	// Vertical line_data middle
	{
		SLine_Data line_data(X_Pos + Width / 2, Y_Pos, Screen_Width, Height - 4);
		SPanel_Symbol symbol_data(L'║', 0x1b, L'╦', L'╨');
		Draw_Line_Vertical(Screen_Buffer, line_data, symbol_data);
	}
}
//--------------------------------------------------------------------------------------------------------------
void APanel::Draw_Files()
{
	int x_offset = 0;
	int y_offset = 0;

	for (auto* file : Files)
	{
		if (Files.size() > 0 && Files[0]->File_Size > 100000)
		{
			return;
		}
		Draw_One_File(file, x_offset, y_offset, 0x10);
		y_offset++;

		if (Y_Pos + y_offset + 2 > Max_Visible_Elements / 2)
		{
			if (x_offset == 0)
			{
				x_offset = Width / 2;
				y_offset = 0;
			}
			else
				break;
		}
	}
}
//--------------------------------------------------------------------------------------------------------------
void APanel::Highlight_File()
{
	if (Files.size() == 0 || Pointer >= Files.size() || Pointer >= Max_Visible_Elements - 1)
		return;

	AFile_Data* file;
	int x_offset;
	int y_offset;

	file = Files[Pointer];

	x_offset = 0;
	y_offset = Pointer;

	if (Pointer + 2 > Max_Visible_Elements / 2)
	{
		if (x_offset == 0)
		{
			x_offset = Width / 2;
			if (Pointer + 3 > Max_Visible_Elements)
			{
				return;
			}
			else
			{
				y_offset = Pointer - (Max_Visible_Elements - 1) / 2;
			}
		}
		else
			return;
	}

	Highlight_X_Offset = x_offset;
	Highlight_Y_Offset = y_offset;
	if (Is_Panel_Active)
		Draw_One_File(file, x_offset, y_offset, 0x80);
	else
		Draw_One_File(file, x_offset, y_offset, 0x90);
}
//--------------------------------------------------------------------------------------------------------------
void APanel::Draw_One_File(AFile_Data* file, int x_offset, int y_offset, unsigned short bg_color)
{
	unsigned short attribute;
	if (file->Attribute & FILE_ATTRIBUTE_DIRECTORY)
		attribute = 0x0f | bg_color;
	else
		attribute = 0x0b | bg_color;
	SText_Data text_data(X_Pos + x_offset + 1, Y_Pos + y_offset + 2, Screen_Width, attribute);
	Draw_Limited_Text(Screen_Buffer, text_data, file->File_Name.c_str(), Width / 2 - 1 - Text_Limit_Offset);
}
//--------------------------------------------------------------------------------------------------------------
