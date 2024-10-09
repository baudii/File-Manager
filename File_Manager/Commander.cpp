#include "Commander.h"
#include <chrono>


//--------------------------------------------------------------------------------------------------------------
AsCommander::~AsCommander()
{
	// Restore the original active screen buffer.
	if (!SetConsoleActiveScreenBuffer(Std_Output_Handle))
	{
		printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
	}
	delete Left_Panel;
	delete Right_Panel;
	delete Bottom_Panel;
	delete Screen_Buffer;
}
//--------------------------------------------------------------------------------------------------------------
bool AsCommander::Init()
{
	int screen_buffer_size;

	wchar_t cur_dir[MAX_PATH];

	GetCurrentDirectoryW(MAX_PATH, cur_dir);


	// Get a handle to the STDOUT screen buffer to copy from and create a new screen buffer to copy to
	Std_Output_Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	Std_Input_Handle = GetStdHandle(STD_INPUT_HANDLE);

	Screen_Buffer_Handle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CONSOLE_TEXTMODE_BUFFER, 0);
	if (Std_Input_Handle == INVALID_HANDLE_VALUE || Std_Output_Handle == INVALID_HANDLE_VALUE || Screen_Buffer_Handle == INVALID_HANDLE_VALUE)
	{
		printf("CreateConsoleScreenBuffer failed - (%d)\n", GetLastError());
		return false;
	}

	// Set the new screen buffer as active screen buffer
	if (!SetConsoleActiveScreenBuffer(Screen_Buffer_Handle))
	{
		printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
		return false;
	}

	// Get new screen buffer info
	if (!GetConsoleScreenBufferInfo(Screen_Buffer_Handle, &Screen_Buffer_Info))
	{
		printf("GetConsoleScreenBufferInfo failed - (%d)\n", GetLastError());
		return false;
	}

	screen_buffer_size = (int)Screen_Buffer_Info.dwSize.X * (int)Screen_Buffer_Info.dwSize.Y;
	Screen_Buffer = new CHAR_INFO[screen_buffer_size];
	memset(Screen_Buffer, 0, screen_buffer_size * sizeof(CHAR_INFO));

	// Setup panels
	Bottom_Panel = new AsBottom_Panel(Screen_Buffer, Screen_Buffer_Info.dwSize.X, 0, Screen_Buffer_Info.dwSize.Y - 1);
	Left_Panel = new APanel(Screen_Buffer, Screen_Buffer_Info.dwSize.X, 0, 0, Screen_Buffer_Info.dwSize.X / 2, Screen_Buffer_Info.dwSize.Y - 2, false, true);
	Right_Panel = new APanel(Screen_Buffer, Screen_Buffer_Info.dwSize.X, Screen_Buffer_Info.dwSize.X / 2, 0, Screen_Buffer_Info.dwSize.X / 2, Screen_Buffer_Info.dwSize.Y - 2, true, false);

	Left_Panel->Get_Files(cur_dir);
	Right_Panel->Get_Files(cur_dir);

	Current_Panel = Left_Panel;

	// Draw once

	return true;
}
//--------------------------------------------------------------------------------------------------------------
void AsCommander::Run()
{
	INPUT_RECORD input_record[128];
	DWORD records_count;

	Can_Run = true;
	Can_Draw = true;

	while (Can_Run)
	{
		if (PeekConsoleInputW(Std_Input_Handle, input_record, 128, &records_count))
		{
			if (ReadConsoleInputW(Std_Input_Handle, input_record, 1, &records_count))
			{
				if (records_count > 0)
				{
					Process_Input(input_record[0]);
				}
			}
		}

		if (Can_Draw)
		{
			if (!Draw())
				return;
			Can_Draw = false;
		}
		Sleep(10);
	}
}
//--------------------------------------------------------------------------------------------------------------
bool AsCommander::Draw()
{
	COORD screen_buffer_pos{};

	Left_Panel->Draw();
	Right_Panel->Draw();
	Bottom_Panel->Draw();

	if (!WriteConsoleOutput(Screen_Buffer_Handle, Screen_Buffer, Screen_Buffer_Info.dwSize, screen_buffer_pos, &Screen_Buffer_Info.srWindow))
	{
		printf("WriteConsoleOutput failed - (%d)\n", GetLastError());
		return false;
	}

	return true;
}
//--------------------------------------------------------------------------------------------------------------
void AsCommander::Process_Input(INPUT_RECORD input_record)
{
	if (input_record.EventType == KEY_EVENT)
	{
		Process_Key_Event(input_record.Event.KeyEvent);
	}
}
//--------------------------------------------------------------------------------------------------------------
void AsCommander::Process_Key_Event(KEY_EVENT_RECORD key_event_record)
{
	if (key_event_record.bKeyDown)
	{
		switch (key_event_record.wVirtualKeyCode)
		{
		case (VK_DOWN):
			Current_Panel->Move_Highlight(false);
			Can_Draw = true;
			break;

		case (VK_UP):
			Current_Panel->Move_Highlight(true);
			Can_Draw = true;
			break;

		case (VK_RETURN):
			Current_Panel->Open_Highlighted_Dir();
			Can_Draw = true;
			break;

		case (VK_F10):
			Can_Run = false;
			break;

		case (VK_RIGHT):
			Switch_Panel(true);
			Can_Draw = true;
			break;

		case (VK_LEFT):
			Switch_Panel(false);
			Can_Draw = true;
			break;
		}
	}
}
//--------------------------------------------------------------------------------------------------------------
void AsCommander::Switch_Panel(bool is_right)
{
	Current_Panel->Panel_Set_Active(false);
	if (is_right)
	{
		Current_Panel = Right_Panel;
	}
	else
	{
		Current_Panel = Left_Panel;
	}
	Current_Panel->Panel_Set_Active(true);
}
//--------------------------------------------------------------------------------------------------------------
