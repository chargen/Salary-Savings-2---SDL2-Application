/////////////////////////////////////////////////////////////////////////////
// main.cpp																   
// MaxiJonson															   
// December 15 2016														   
// Test area for SDL					   	
/////////////////////////////////////////////////////////////////////////////

#include "Savings.h"

int main(int argc, char *argv[])
{
	// SDL Components init.
	InitializeDisplay("Salary Savings 2.0", 1280, 800);
	InitializeAudio();

	SndId theme = LoadMusic("Sounds\\theme.mp3");
	PlayMusic(theme, 30);
	// Variables related to selection of options
	Menu_Selection Menu_Select;
	Add_Selection Add_Select;
	Remove_Selection Remove_Select;

	do
	{
		Menu_Select = MainMenu();

		switch (Menu_Select)
		{
		case Menu_Add:
			Add_Select = AddMenu();
			switch (Add_Select)
			{
			case Add_Salary:
				AddSalaryMenu();
				break;
			case Add_Savings:
				AddSavingsMenu();
				break;
			case Add_FirstGoal:
				AddFirstMenu();
				break;
			case Add_SecondGoal:
				AddSecondMenu();
				break;
			case Add_BusPass:
				AddBusMenu();
				break;
			}
			break;
		case Menu_Show:
			Show();
			break;
			
		case Menu_Remove:
			Remove_Select = RemoveMenu();
			switch (Remove_Select)
			{
			case Remove_Salary:
				RemoveSalaryMenu();
				break;
			case Remove_Savings:
				RemoveSavingsMenu();
				break;
			case Remove_FirstGoal:
				RemoveFirstMenu();
				break;
			case Remove_SecondGoal:
				RemoveSecondMenu();
				break;
			case Remove_BusPass:
				RemoveBusMenu();
				break;
			}
			break;
		}

	} while (Menu_Select != Menu_Quit);

	SndShutdown();
	FreeText();
	CloseDisplay();

	return 0;
}