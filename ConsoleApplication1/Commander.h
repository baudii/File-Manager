#pragma once
#include "Panel.h"
#include "Bottom_Panel.h"
#include <stdio.h>


//--------------------------------------------------------------------------------------------------------------
class AsCommander
{
public:
	~AsCommander();

	bool Init();
	void Run();

private:
	bool Draw();
	void Process_Input(INPUT_RECORD input_record);
	void Process_Key_Event(KEY_EVENT_RECORD key_event_record);
	void Switch_Panel(bool isRight);

	bool Can_Run = false;
	bool Can_Draw = false;

	HANDLE Screen_Buffer_Handle = 0;
	HANDLE Std_Output_Handle = 0;
	HANDLE Std_Input_Handle = 0;
	CHAR_INFO* Screen_Buffer = 0;
	CONSOLE_SCREEN_BUFFER_INFO Screen_Buffer_Info{};

	APanel* Current_Panel = 0;
	APanel* Left_Panel = 0;
	APanel* Right_Panel = 0;
	AsBottom_Panel* Bottom_Panel = 0;
};
//--------------------------------------------------------------------------------------------------------------
