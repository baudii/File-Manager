// FILE: Main.cpp
#include "Main.h"
#include <iostream>


int main()
{

	AsCommander commander;

	if (!commander.Init())
	{
		return 1;
	}

	commander.Run();


	return 0;
}