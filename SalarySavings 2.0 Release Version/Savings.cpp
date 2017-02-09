/////////////////////////////////////////////////////////////////////////////
// Savings.cpp															   
// MaxiJonson															   
// December 22 2016														   
// Contains all elements that make the base program work				   	
/////////////////////////////////////////////////////////////////////////////
#include "Savings.h"




Menu_Selection MainMenu()
{
#pragma region VARIABLES
	ImageId Background = LoadBmp("Images\\background.bmp");
	ImageId Add = LoadBmp("Images\\add.bmp");
	ImageId aAdd = LoadBmp("Images\\aAdd.bmp");
	Area AddHitbox;
	AddHitbox.start.x = 488;
	AddHitbox.start.y = 154;
	AddHitbox.end.x = 761;
	AddHitbox.end.y = 263;
	ImageId Show = LoadBmp("Images\\show.bmp");
	ImageId aShow = LoadBmp("Images\\aShow.bmp");
	Area ShowHitbox;
	ShowHitbox.start.x = 463;
	ShowHitbox.start.y = 290;
	ShowHitbox.end.x = 797;
	ShowHitbox.end.y = 362;
	ImageId Remove = LoadBmp("Images\\remove.bmp");
	ImageId aRemove = LoadBmp("Images\\aRemove.bmp");
	Area RemoveHitbox;
	RemoveHitbox.start.x = 408;
	RemoveHitbox.start.y = 413;
	RemoveHitbox.end.x = 854;
	RemoveHitbox.end.y = 488;
	ImageId Quit = LoadBmp("Images\\quit.bmp");
	ImageId aQuit = LoadBmp("Images\\aQuit.bmp");
	Area QuitHitbox;
	QuitHitbox.start.x = 496;
	QuitHitbox.start.y = 537;
	QuitHitbox.end.x = 762;
	QuitHitbox.end.y = 618;
	SndId navigation = LoadEffect("Sounds\\sndNavigation.wav");
	SndId confirm = LoadEffect("Sounds\\sndSelect.wav");

	Menu_Selection choice = NA;
	int cursor = Menu_Add;
	int soundEvent = NA; // This will be used to prevent the navigation sound from playing constantly when hovering on a choice
	KEvent keyPress = KEvent_None;
	Mouse mouseEvent;
	mouseEvent.position.x = 0;
	mouseEvent.position.y = 0;
#pragma endregion

	do
	{
		Wait(10);
#pragma region SHOW MENU
		ShowImage(Background, 0, 0);
		// Shows the hovered action depending on the cursor (Make sure the order of the cases matches the enum for better clarity and prevent errors)
		switch (cursor)
		{
		case Menu_Add:
			ShowImage(aAdd, 235, 125);
			ShowImage(Show, 235, 250);
			ShowImage(Remove, 235, 375);
			ShowImage(Quit, 235, 500);
			break;
		case Menu_Show:
			ShowImage(Add, 235, 125);
			ShowImage(aShow, 235, 250);
			ShowImage(Remove, 235, 375);
			ShowImage(Quit, 235, 500);
			break;
		case Menu_Remove:
			ShowImage(Add, 235, 125);
			ShowImage(Show, 235, 250);
			ShowImage(aRemove, 235, 375);
			ShowImage(Quit, 235, 500);
			break;
		case Menu_Quit:
			ShowImage(Add, 235, 125);
			ShowImage(Show, 235, 250);
			ShowImage(Remove, 235, 375);
			ShowImage(aQuit, 235, 500);
			break;
		}
		Refresh();
#pragma endregion

		// Read Input
		PollMouseAndKeyboard(mouseEvent, keyPress);

#pragma region INPUT ACTION
		switch (keyPress)
		{
		case KEvent_Up: // Going Up
			if (cursor > Menu_Add)
			{
				--cursor;
			}
			PlayEffect(navigation);
			break;
		case KEvent_Down: // Going Down
			if (cursor < Menu_Quit)
			{
				++cursor;
			}
			PlayEffect(navigation);
			break;
		case KEvent_Space:
		case KEvent_Return:
			// We convert the cursor (int) into the choice (Menu_Selection)
			// We can't just do choice = cursor since they aren't the same type
			switch (cursor)
			{
			case Menu_Add:
				choice = Menu_Add;
				break;

			case Menu_Show:
				choice = Menu_Show;
				break;

			case Menu_Remove:
				choice = Menu_Remove;
				break;

			case Menu_Quit:
				choice = Menu_Quit;
				break;
			}
			PlayEffect(confirm);
			break;
		case KEvent_Esc:
			choice = Menu_Quit;
			PlayEffect(confirm);
			break;
		}

		// Here we check if the cursor is in an option area
		if (inArea(mouseEvent, AddHitbox))
		{
			cursor = Menu_Add;
			if (soundEvent != cursor)
			{
				PlayEffect(navigation);
			}
		}
		else if (inArea(mouseEvent, ShowHitbox))
		{
			cursor = Menu_Show;
			if (soundEvent != cursor)
			{
				PlayEffect(navigation);
			}
		}
		else if (inArea(mouseEvent, RemoveHitbox))
		{
			cursor = Menu_Remove;
			if (soundEvent != cursor)
			{
				PlayEffect(navigation);
			}
		}
		else if (inArea(mouseEvent, QuitHitbox))
		{
			cursor = Menu_Quit;
			if (soundEvent != cursor)
			{
				PlayEffect(navigation);
			}
		}

		soundEvent = cursor;

		if ((mouseEvent.button == MEvent_Left) &&
			(inArea(mouseEvent, AddHitbox) ||
				inArea(mouseEvent, ShowHitbox) ||
				inArea(mouseEvent, RemoveHitbox) ||
				inArea(mouseEvent, QuitHitbox))) // If the user clicks an option in it's hitbox, we assign that option to the choice
		{
			PlayEffect(confirm);
			switch (cursor)
			{
			case Menu_Add:
				choice = Menu_Add;
				break;

			case Menu_Show:
				choice = Menu_Show;
				break;

			case Menu_Remove:
				choice = Menu_Remove;
				break;

			case Menu_Quit:
				choice = Menu_Quit;
				break;
			}
		}
#pragma endregion


	} while (choice == NA);
	FreeImages();
	FreeText();
	return choice;
}



Add_Selection AddMenu()
{
#pragma region VARIABLES
	// Variables
	Element Savings;
	Element FirstGoal;
	Element SecondGoal;
	Element Bus;
	Add_Selection choice = Add_NA;
	int cursor = Add_Salary;
	KEvent keyPress = KEvent_None;
	Mouse mouseEvent;
	SndId navigation = LoadEffect("Sounds\\sndNavigation.wav");
	SndId confirm = LoadEffect("Sounds\\sndSelect.wav");
	int soundEvent = Add_NA;
	// Read the savings file and place the elements into the 'Element' type variables
	string file = "Ressources\\savings.sav";
	ifstream Read(file);
	if (!Read)
	{
		string errTitle = "Couldn't open .sav file";
		string errMsg = "Unable to find file: " + file;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, errTitle.c_str(), errMsg.c_str(), NULL);
		exit(1);
	}
	Read >>
		Savings.name >> Savings.amount >>
		FirstGoal.name >> FirstGoal.amount >>
		SecondGoal.name >> SecondGoal.amount >>
		Bus.name >> Bus.amount;
	Read.close();
	// Images
	//FontId font = LoadFont("font.ttf", 100);
	ImageId Background = LoadBmp("Images\\background.bmp");
	ImageId Quit = LoadBmp("Images\\quit.bmp");
	Area QuitHB;
	QuitHB.start.x = 497;
	QuitHB.start.y = 690;
	QuitHB.end.x = 761;
	QuitHB.end.y = 760;
	ImageId aQuit = LoadBmp("Images\\aQuit.bmp");
	ImageId btn_Salary = LoadBmp("Images\\salary.bmp");
	Area SalaryHB;
	SalaryHB.start.x = 388;
	SalaryHB.start.y = 58;
	SalaryHB.end.x = 873;
	SalaryHB.end.y = 143;
	ImageId btn_aSalary = LoadBmp("Images\\aSalary.bmp");
	AlphaId active = LoadAlphabet("Alphabets\\active");
	AlphaId inactive = LoadAlphabet("Alphabets\\inactive");
	Area SavingsHB;
	SavingsHB.start.x = 328;
	SavingsHB.start.y = 154;
	SavingsHB.end.x = 914;
	SavingsHB.end.y = 242;
	Area First;
	First.start.x = 328;
	First.start.y = 285;
	First.end.x = 914;
	First.end.y = 373;
	Area Second;
	Second.start.x = 328;
	Second.start.y = 410;
	Second.end.x = 914;
	Second.end.y = 493;
	Area BusP;
	BusP.start.x = 328;
	BusP.start.y = 564;
	BusP.end.x = 914;
	BusP.end.y = 646;
#pragma endregion
	do
	{
		Wait(10);
#pragma region SHOW MENU
		ShowImage(Background, 0, 0);
		// Shows the hovered action depending on the cursor (Make sure the order of the cases matches the enum for better clarity and prevent errors)
		switch (cursor)
		{
		case Add_Salary:
			ShowImage(btn_aSalary, 235, 25);
			ShowAlpha(Savings.name, inactive, 330, 150, -5);
			ShowAlpha(FirstGoal.name, inactive, 330, 275, -5);
			ShowAlpha(SecondGoal.name, inactive, 330, 400, -5);
			ShowAlpha(Bus.name, inactive, 330, 550, -5);
			ShowImage(Quit, 235, 650);
			break;
		case Add_Savings:
			ShowImage(btn_Salary, 235, 25);
			ShowAlpha(Savings.name, active, 330, 150, -5);
			ShowAlpha(FirstGoal.name, inactive, 330, 275, -5);
			ShowAlpha(SecondGoal.name, inactive, 330, 400, -5);
			ShowAlpha(Bus.name, inactive, 330, 550, -5);
			ShowImage(Quit, 235, 650);
			break;
		case Add_FirstGoal:
			ShowImage(btn_Salary, 235, 25);
			ShowAlpha(Savings.name, inactive, 330, 150, -5);
			ShowAlpha(FirstGoal.name, active, 330, 275, -5);
			ShowAlpha(SecondGoal.name, inactive, 330, 400, -5);
			ShowAlpha(Bus.name, inactive, 330, 550, -5);
			ShowImage(Quit, 235, 650);
			break;
		case Add_SecondGoal:
			ShowImage(btn_Salary, 235, 25);
			ShowAlpha(Savings.name, inactive, 330, 150, -5);
			ShowAlpha(FirstGoal.name, inactive, 330, 275, -5);
			ShowAlpha(SecondGoal.name, active, 330, 400, -5);
			ShowAlpha(Bus.name, inactive, 330, 550, -5);
			ShowImage(Quit, 235, 650);
			break;

		case Add_BusPass:
			ShowImage(btn_Salary, 235, 25);
			ShowAlpha(Savings.name, inactive, 330, 150, -5);
			ShowAlpha(FirstGoal.name, inactive, 330, 275, -5);
			ShowAlpha(SecondGoal.name, inactive, 330, 400, -5);
			ShowAlpha(Bus.name, active, 330, 550, -5);
			ShowImage(Quit, 235, 650);
			break;

		case Add_Quit:
			ShowImage(btn_Salary, 235, 25);
			ShowAlpha(Savings.name, inactive, 330, 150, -5);
			ShowAlpha(FirstGoal.name, inactive, 330, 275, -5);
			ShowAlpha(SecondGoal.name, inactive, 330, 400, -5);
			ShowAlpha(Bus.name, inactive, 330, 550, -5);
			ShowImage(aQuit, 235, 650);
			break;
		}
		Refresh();
#pragma endregion

		// Read Input
		PollMouseAndKeyboard(mouseEvent, keyPress);

#pragma region INPUT ACTION
		switch (keyPress)
		{
		case KEvent_Up: // Going Up
			if (cursor > Add_Salary)
			{
				--cursor;
			}
			PlayEffect(navigation);
			break;
		case KEvent_Down: // Going Down
			if (cursor < Add_Quit)
			{
				++cursor;
			}
			PlayEffect(navigation);
			break;
		case KEvent_Space:
		case KEvent_Return:
			PlayEffect(confirm);
			// We convert the cursor (int) into the choice (Menu_Selection)
			// We can't just do choice = cursor since they aren't the same type
			switch (cursor)
			{
			case Add_Salary:
				choice = Add_Salary;
				break;

			case Add_Savings:
				choice = Add_Savings;
				break;

			case Add_FirstGoal:
				choice = Add_FirstGoal;
				break;

			case Add_SecondGoal:
				choice = Add_SecondGoal;
				break;

			case Add_BusPass:
				choice = Add_BusPass;
				break;

			case Add_Quit:
				choice = Add_Quit;
				break;
			}
			break;
		case KEvent_Esc:
			choice = Add_Quit;
			PlayEffect(confirm);
			break;
		}

		// Here we check if the cursor is in an option area
		if (inArea(mouseEvent, SalaryHB))
		{
			cursor = Add_Salary;
			if (soundEvent != cursor)
			{
				PlayEffect(navigation);
			}
		}
		else if (inArea(mouseEvent, SavingsHB))
		{
			cursor = Add_Savings;
			if (soundEvent != cursor)
			{
				PlayEffect(navigation);
			}
		}
		else if (inArea(mouseEvent, First))
		{
			cursor = Add_FirstGoal;
			if (soundEvent != cursor)
			{
				PlayEffect(navigation);
			}
		}
		else if (inArea(mouseEvent, Second))
		{
			cursor = Add_SecondGoal;
			if (soundEvent != cursor)
			{
				PlayEffect(navigation);
			}
		}
		else if (inArea(mouseEvent, BusP))
		{
			cursor = Add_BusPass;
			if (soundEvent != cursor)
			{
				PlayEffect(navigation);
			}
		}
		else if (inArea(mouseEvent, QuitHB))
		{
			cursor = Add_Quit;
			if (soundEvent != cursor)
			{
				PlayEffect(navigation);
			}
		}

		soundEvent = cursor;

		if ((mouseEvent.button == MEvent_Left) &&
			(inArea(mouseEvent, SalaryHB) ||
				inArea(mouseEvent, SavingsHB) ||
				inArea(mouseEvent, First) ||
				inArea(mouseEvent, Second) ||
				inArea(mouseEvent, BusP) ||
				inArea(mouseEvent, QuitHB))) // If the user clicks an option in it's hitbox, we assign that option to the choice
		{
			PlayEffect(confirm);
			switch (cursor)
			{
			case Add_Salary:
				choice = Add_Salary;
				break;

			case Add_Savings:
				choice = Add_Savings;
				break;

			case Add_FirstGoal:
				choice = Add_FirstGoal;
				break;

			case Add_SecondGoal:
				choice = Add_SecondGoal;
				break;

			case Add_BusPass:
				choice = Add_BusPass;
				break;

			case Add_Quit:
				choice = Add_Quit;
			}
		}
#pragma endregion


	} while (choice == Add_NA);
	FreeImages();
	FreeText();
	return choice;
}



void AddSalaryMenu()
{
#pragma region VARIABLES
	// Variables
	// Elements data
	Element Salary;
	Salary.name = "Salary";
	Salary.amount = 0;
	Element BusCost;
	BusCost.name = "Bus Cost";
	BusCost.amount = 0;
	Element Personnal;
	Personnal.name = "Personnal";
	Personnal.percentage = PersonnalPercentage;
	Element Savings;
	Savings.percentage = SavingsPercentage;
	Element FirstGoal;
	FirstGoal.percentage = FirstPercentage;
	Element SecondGoal;
	SecondGoal.percentage = SecondPercentage;
	Element Bus;
	string file = "Ressources\\savings.sav";
	ifstream Read(file);
	if (!Read)
	{
		string errTitle = "Couldn't open .sav file";
		string errMsg = "Unable to find file: " + file;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, errTitle.c_str(), errMsg.c_str(), NULL);
		exit(1);
	}
	Read >>
		Savings.name >> Savings.amount >>
		FirstGoal.name >> FirstGoal.amount >>
		SecondGoal.name >> SecondGoal.amount >>
		Bus.name >> Bus.amount;
	Read.close();
	// Images
	ImageId Background = LoadBmp("Images\\background.bmp");
	ImageId AmountBox = LoadBmp("Images\\amountBox.bmp");
	ImageId PercentBox = LoadBmp("Images\\percentageBox.bmp");
	ImageId aAmountBox = LoadBmp("Images\\aamountBox.bmp");
	ImageId aPercentBox = LoadBmp("Images\\apercentageBox.bmp");
	ImageId hAmountBox = LoadBmp("Images\\hamountBox.bmp");
	ImageId hPercentBox = LoadBmp("Images\\hpercentageBox.bmp");
	ImageId AddButton = LoadBmp("Images\\add.bmp");
	ImageId aAddButton = LoadBmp("Images\\aAdd.bmp");
	ImageId QuitButton = LoadBmp("Images\\quit.bmp");
	ImageId aQuitButton = LoadBmp("Images\\aQuit.bmp");
	// Alphabets
	AlphaId inactive = LoadAlphabet("Alphabets\\inactive");
	AlphaId active = LoadAlphabet("Alphabets\\active");
	// Selection
	int cursor = AddSalary_Amount;
	int tempCursor = AddSalary_Amount;
	AddSalary_Selection modifier = AddSalary_NA;
	AddSalary_Selection choice = AddSalary_NA;
	Mouse mouse;
	KEvent key = KEvent_None;
	int soundEvent = AddSalary_NA;
	// Hitboxes
	Area SalaryBox = CreateImageArea(350, 25, AmountBox, -25);
	Area BusCostBox = CreateImageArea(350, 150, AmountBox, -25);
	Area SavingsBox = CreateImageArea(350, 275, PercentBox, -25);
	Area PersonnalBox = CreateImageArea(350, 400, PercentBox, -25);
	Area FirstBox = CreateImageArea(350, 525, PercentBox, -25);
	Area SecondBox = CreateImageArea(350, 650, PercentBox, -25);
	Area AddBox = CreateImageArea(725, 275, AddButton, -25);
	Area QuitBox = CreateImageArea(715, 375, QuitButton, -25);
	// Sounds
	SndId increase = LoadEffect("Sounds\\increase.wav");
	SndId navigation = LoadEffect("Sounds\\sndNavigation.wav");
	SndId confirm = LoadEffect("Sounds\\sndSelect.wav");
#pragma endregion
	do
	{

#pragma region do{NAVIGATION}
		do // Navigation
		{
			choice = AddSalary_NA;
			Wait(10);

			// We set the precision of double numbers
			stringstream SalaryStream;
			stringstream BusStream;
			SalaryStream << fixed << setprecision(2) << Salary.amount;
			string SalaryAmount = SalaryStream.str();
			BusStream << fixed << setprecision(2) << BusCost.amount;
			string BusCostAmount = BusStream.str();

#pragma region SHOWMENU
			ShowImage(Background, 0, 0);

			ShowAlpha(Salary.name, inactive, 550, 10, -75);
			ShowAlpha(BusCost.name, inactive, 535, 140, -75);
			ShowAlpha(Savings.name + " Cut", inactive, 510, 265, -75);
			ShowAlpha(Personnal.name + " Cut", inactive, 500, 390, -75);
			ShowAlpha(FirstGoal.name, inactive, 500, 515, -75);
			ShowAlpha(SecondGoal.name, inactive, 500, 640, -75);

			switch (cursor)
			{
			case AddSalary_Amount:
				ShowImage(hAmountBox, 350, 25, -25); // Salary Amount
				ShowImage(AmountBox, 350, 150, -25); // Bus Cost Amount
				ShowImage(PercentBox, 350, 275, -25); // Savings Percentage
				ShowImage(PercentBox, 350, 400, -25); // Personnal Percentage
				ShowImage(PercentBox, 350, 525, -25); // FirstGoal Percentage
				ShowImage(PercentBox, 350, 650, -25); // SecondGoal Percentage
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				break;
			case AddSalary_Bus:
				ShowImage(AmountBox, 350, 25, -25);
				ShowImage(hAmountBox, 350, 150, -25);
				ShowImage(PercentBox, 350, 275, -25);
				ShowImage(PercentBox, 350, 400, -25);
				ShowImage(PercentBox, 350, 525, -25);
				ShowImage(PercentBox, 350, 650, -25);
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				break;
			case AddSalary_Savings:
				ShowImage(AmountBox, 350, 25, -25);
				ShowImage(AmountBox, 350, 150, -25);
				ShowImage(hPercentBox, 350, 275, -25);
				ShowImage(PercentBox, 350, 400, -25);
				ShowImage(PercentBox, 350, 525, -25);
				ShowImage(PercentBox, 350, 650, -25);
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				break;
			case AddSalary_Personnal:
				ShowImage(AmountBox, 350, 25, -25);
				ShowImage(AmountBox, 350, 150, -25);
				ShowImage(PercentBox, 350, 275, -25);
				ShowImage(hPercentBox, 350, 400, -25);
				ShowImage(PercentBox, 350, 525, -25);
				ShowImage(PercentBox, 350, 650, -25);
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				break;
			case AddSalary_First:
				ShowImage(AmountBox, 350, 25, -25);
				ShowImage(AmountBox, 350, 150, -25);
				ShowImage(PercentBox, 350, 275, -25);
				ShowImage(PercentBox, 350, 400, -25);
				ShowImage(hPercentBox, 350, 525, -25);
				ShowImage(PercentBox, 350, 650, -25);
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				break;
			case AddSalary_Second:
				ShowImage(AmountBox, 350, 25, -25);
				ShowImage(AmountBox, 350, 150, -25);
				ShowImage(PercentBox, 350, 275, -25);
				ShowImage(PercentBox, 350, 400, -25);
				ShowImage(PercentBox, 350, 525, -25);
				ShowImage(hPercentBox, 350, 650, -25);
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				break;
			case AddSalary_Add:
				ShowImage(AmountBox, 350, 25, -25);
				ShowImage(AmountBox, 350, 150, -25);
				ShowImage(PercentBox, 350, 275, -25);
				ShowImage(PercentBox, 350, 400, -25);
				ShowImage(PercentBox, 350, 525, -25);
				ShowImage(PercentBox, 350, 650, -25);
				ShowImage(aAddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				break;
			case AddSalary_Back:
				ShowImage(AmountBox, 350, 25, -25);
				ShowImage(AmountBox, 350, 150, -25);
				ShowImage(PercentBox, 350, 275, -25);
				ShowImage(PercentBox, 350, 400, -25);
				ShowImage(PercentBox, 350, 525, -25);
				ShowImage(PercentBox, 350, 650, -25);
				ShowImage(AddButton, 725, 275);
				ShowImage(aQuitButton, 715, 375);
				break;
			}

			ShowAlpha(SalaryAmount, inactive, 485, 55, -55);
			ShowAlpha(BusCostAmount, inactive, 485, 180, -55);
			ShowAlpha(to_string(Savings.percentage), inactive, 485, 305, -55);
			ShowAlpha(to_string(Personnal.percentage), inactive, 485, 430, -55);
			ShowAlpha(to_string(FirstGoal.percentage), inactive, 485, 555, -55);
			ShowAlpha(to_string(SecondGoal.percentage), inactive, 485, 680, -55);

			Refresh();
#pragma endregion

			// Read input
			PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION
#pragma region KEYBOARD
			switch (key)
			{
			case KEvent_Down:
				if (cursor < AddSalary_Second && (cursor != AddSalary_Add && cursor != AddSalary_Back))
				{
					cursor++;
					PlayEffect(navigation);
				}
				else if (cursor == AddSalary_Add)
				{
					cursor++;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Up:
				if (cursor > AddSalary_Amount && (cursor != AddSalary_Add && cursor != AddSalary_Back))
				{
					cursor--;
					PlayEffect(navigation);
				}
				else if (cursor == AddSalary_Back)
				{
					cursor--;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Right:
				if (cursor != AddSalary_Back && cursor != AddSalary_Add)
				{
					tempCursor = cursor;
					cursor = AddSalary_Add;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Left:
				if (cursor == AddSalary_Back || cursor == AddSalary_Add)
				{
					cursor = tempCursor;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Esc:
				choice = AddSalary_Back;
				PlayEffect(confirm);
				break;

			case KEvent_Space:
			case KEvent_Return:
				PlayEffect(confirm);
				switch (cursor)
				{
				case AddSalary_Amount:
					choice = AddSalary_Amount;
					break;

				case AddSalary_Bus:
					choice = AddSalary_Bus;
					break;

				case AddSalary_Savings:
					choice = AddSalary_Savings;
					break;

				case AddSalary_Personnal:
					choice = AddSalary_Personnal;
					break;

				case AddSalary_First:
					choice = AddSalary_First;
					break;

				case AddSalary_Second:
					choice = AddSalary_Second;
					break;

				case AddSalary_Add:
					choice = AddSalary_Add;
					break;

				case AddSalary_Back:
					choice = AddSalary_Back;
					break;
				}
				break;
			}
#pragma endregion

#pragma region MOUSE
			if (inArea(mouse, SalaryBox))
			{
				cursor = AddSalary_Amount;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, BusCostBox))
			{
				cursor = AddSalary_Bus;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, SavingsBox))
			{
				cursor = AddSalary_Savings;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, PersonnalBox))
			{
				cursor = AddSalary_Personnal;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, FirstBox))
			{
				cursor = AddSalary_First;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, SecondBox))
			{
				cursor = AddSalary_Second;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, AddBox))
			{
				cursor = AddSalary_Add;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, QuitBox))
			{
				cursor = AddSalary_Back;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}

			soundEvent = cursor;

			if (mouse.button == MEvent_Left &&
				(inArea(mouse, SalaryBox) ||
					inArea(mouse, BusCostBox) ||
					inArea(mouse, SavingsBox) ||
					inArea(mouse, PersonnalBox) ||
					inArea(mouse, FirstBox) ||
					inArea(mouse, SecondBox) ||
					inArea(mouse, AddBox) ||
					inArea(mouse, QuitBox))
				)
			{
				PlayEffect(confirm);
				switch (cursor)
				{
				case AddSalary_Amount:
					choice = AddSalary_Amount;
					break;

				case AddSalary_Bus:
					choice = AddSalary_Bus;
					break;

				case AddSalary_Savings:
					choice = AddSalary_Savings;
					break;

				case AddSalary_Personnal:
					choice = AddSalary_Personnal;
					break;

				case AddSalary_First:
					choice = AddSalary_First;
					break;

				case AddSalary_Second:
					choice = AddSalary_Second;
					break;

				case AddSalary_Add:
					choice = AddSalary_Add;
					break;

				case AddSalary_Back:
					choice = AddSalary_Back;
					break;
				}
			}
#pragma endregion
#pragma endregion

		} while (choice == AddSalary_NA);
#pragma endregion

		// We reset it back to none so we can come back again
		modifier = AddSalary_NA;

#pragma region CHOICE ACTION
		// We clear the text
		string ValueString = "";
		// This will be used to check what the string would be after input to check if it would be valid
		string tempValue = "";
		// This will check if the user can enter the input
		bool valid = true;
		// We'll start modifying the values
		switch (choice)
		{
		case AddSalary_Amount:
#pragma region EDIT SALARY
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream SalaryStream;
				stringstream BusStream;
				SalaryStream << fixed << setprecision(2) << Salary.amount;
				string initial = SalaryStream.str();
				BusStream << fixed << setprecision(2) << BusCost.amount;
				string BusCostAmount = BusStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Salary.name, inactive, 550, 10, -75);
				ShowAlpha(BusCost.name, inactive, 535, 140, -75);
				ShowAlpha(Savings.name + " Cut", inactive, 510, 265, -75);
				ShowAlpha(Personnal.name + " Cut", inactive, 500, 390, -75);
				ShowAlpha(FirstGoal.name, inactive, 500, 515, -75);
				ShowAlpha(SecondGoal.name, inactive, 500, 640, -75);
				ShowImage(aAmountBox, 350, 25, -25); // Salary Amount
				ShowImage(AmountBox, 350, 150, -25); // Bus Cost Amount
				ShowImage(PercentBox, 350, 275, -25); // Savings Percentage
				ShowImage(PercentBox, 350, 400, -25); // Personnal Percentage
				ShowImage(PercentBox, 350, 525, -25); // FirstGoal Percentage
				ShowImage(PercentBox, 350, 650, -25); // SecondGoal Percentage
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(ValueString, active, 485, 55, -55);
				ShowAlpha(BusCostAmount, inactive, 485, 180, -55);
				ShowAlpha(to_string(Savings.percentage), inactive, 485, 305, -55);
				ShowAlpha(to_string(Personnal.percentage), inactive, 485, 430, -55);
				ShowAlpha(to_string(FirstGoal.percentage), inactive, 485, 555, -55);
				ShowAlpha(to_string(SecondGoal.percentage), inactive, 485, 680, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = AddSalary_Back;
					break;

				case KEvent_Return:
					modifier = AddSalary_Add;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_Period:
					// We check to see if there is already a period in the string and if a period is the first input
					for (int i = 0; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							valid = false;
						}
					}

					if (ValueString.size() == 0)
					{
						valid = false;
					}

					if (valid)
					{
						ValueString += ".";
					}
					// We set it back to it's original state
					valid = true;
					break;

				case KEvent_1:
					// We check if there is two decimals
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, SalaryBox))
				{
					modifier = AddSalary_Back;
				}
#pragma endregion

#pragma endregion

			} while (modifier != AddSalary_Back && modifier != AddSalary_Add);

			// If the user choses to confirm, we update the new amount
			if (modifier == AddSalary_Add)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				Salary.amount = stod(ValueString);
			}
#pragma endregion
			break;

		case AddSalary_Bus:
#pragma region EDIT BUS
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream SalaryStream;
				stringstream BusStream;
				SalaryStream << fixed << setprecision(2) << Salary.amount;
				string initial = SalaryStream.str();
				BusStream << fixed << setprecision(2) << BusCost.amount;
				string BusCostAmount = BusStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Salary.name, inactive, 550, 10, -75);
				ShowAlpha(BusCost.name, inactive, 535, 140, -75);
				ShowAlpha(Savings.name + " Cut", inactive, 510, 265, -75);
				ShowAlpha(Personnal.name + " Cut", inactive, 500, 390, -75);
				ShowAlpha(FirstGoal.name, inactive, 500, 515, -75);
				ShowAlpha(SecondGoal.name, inactive, 500, 640, -75);
				ShowImage(AmountBox, 350, 25, -25); // Salary Amount
				ShowImage(aAmountBox, 350, 150, -25); // Bus Cost Amount
				ShowImage(PercentBox, 350, 275, -25); // Savings Percentage
				ShowImage(PercentBox, 350, 400, -25); // Personnal Percentage
				ShowImage(PercentBox, 350, 525, -25); // FirstGoal Percentage
				ShowImage(PercentBox, 350, 650, -25); // SecondGoal Percentage
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(initial, inactive, 485, 55, -55);
				ShowAlpha(ValueString, active, 485, 180, -55);
				ShowAlpha(to_string(Savings.percentage), inactive, 485, 305, -55);
				ShowAlpha(to_string(Personnal.percentage), inactive, 485, 430, -55);
				ShowAlpha(to_string(FirstGoal.percentage), inactive, 485, 555, -55);
				ShowAlpha(to_string(SecondGoal.percentage), inactive, 485, 680, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = AddSalary_Back;
					break;

				case KEvent_Return:
					modifier = AddSalary_Add;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_Period:
					// We check to see if there is already a period in the string and if a period is the first input
					for (int i = 0; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							valid = false;
						}
					}

					if (ValueString.size() == 0)
					{
						valid = false;
					}

					if (valid)
					{
						ValueString += ".";
					}
					// We set it back to it's original state
					valid = true;
					break;

				case KEvent_1:
					// We check if there is two decimals
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, BusCostBox))
				{
					modifier = AddSalary_Back;
				}
#pragma endregion

#pragma endregion

			} while (modifier != AddSalary_Back && modifier != AddSalary_Add);

			// If the user choses to confirm, we update the new amount
			if (modifier == AddSalary_Add)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				BusCost.amount = stod(ValueString);
			}
#pragma endregion
			break;

		case AddSalary_Savings:
#pragma region EDIT SAVINGS
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream SalaryStream;
				stringstream BusStream;
				SalaryStream << fixed << setprecision(2) << Salary.amount;
				string initial = SalaryStream.str();
				BusStream << fixed << setprecision(2) << BusCost.amount;
				string BusCostAmount = BusStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Salary.name, inactive, 550, 10, -75);
				ShowAlpha(BusCost.name, inactive, 535, 140, -75);
				ShowAlpha(Savings.name + " Cut", inactive, 510, 265, -75);
				ShowAlpha(Personnal.name + " Cut", inactive, 500, 390, -75);
				ShowAlpha(FirstGoal.name, inactive, 500, 515, -75);
				ShowAlpha(SecondGoal.name, inactive, 500, 640, -75);
				ShowImage(AmountBox, 350, 25, -25); // Salary Amount
				ShowImage(AmountBox, 350, 150, -25); // Bus Cost Amount
				ShowImage(aPercentBox, 350, 275, -25); // Savings Percentage
				ShowImage(PercentBox, 350, 400, -25); // Personnal Percentage
				ShowImage(PercentBox, 350, 525, -25); // FirstGoal Percentage
				ShowImage(PercentBox, 350, 650, -25); // SecondGoal Percentage
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(initial, inactive, 485, 55, -55);
				ShowAlpha(BusCostAmount, inactive, 485, 180, -55);
				ShowAlpha(ValueString, active, 485, 305, -55);
				ShowAlpha(to_string(Personnal.percentage), inactive, 485, 430, -55);
				ShowAlpha(to_string(FirstGoal.percentage), inactive, 485, 555, -55);
				ShowAlpha(to_string(SecondGoal.percentage), inactive, 485, 680, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = AddSalary_Back;
					break;

				case KEvent_Return:
					modifier = AddSalary_Add;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_1:
					// We don't want the value to be over 100
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "1";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "2";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "3";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "4";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "5";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "6";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "7";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "8";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "9";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "0";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, SavingsBox))
				{
					modifier = AddSalary_Back;
				}
#pragma endregion

#pragma endregion

			} while (modifier != AddSalary_Back && modifier != AddSalary_Add);

			// If the user choses to confirm, we update the new amount
			if (modifier == AddSalary_Add)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				Savings.percentage = stoi(ValueString);
			}
#pragma endregion
			break;

		case AddSalary_Personnal:
#pragma region EDIT PERSONNAL
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream SalaryStream;
				stringstream BusStream;
				SalaryStream << fixed << setprecision(2) << Salary.amount;
				string initial = SalaryStream.str();
				BusStream << fixed << setprecision(2) << BusCost.amount;
				string BusCostAmount = BusStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Salary.name, inactive, 550, 10, -75);
				ShowAlpha(BusCost.name, inactive, 535, 140, -75);
				ShowAlpha(Savings.name + " Cut", inactive, 510, 265, -75);
				ShowAlpha(Personnal.name + " Cut", inactive, 500, 390, -75);
				ShowAlpha(FirstGoal.name, inactive, 500, 515, -75);
				ShowAlpha(SecondGoal.name, inactive, 500, 640, -75);
				ShowImage(AmountBox, 350, 25, -25); // Salary Amount
				ShowImage(AmountBox, 350, 150, -25); // Bus Cost Amount
				ShowImage(PercentBox, 350, 275, -25); // Savings Percentage
				ShowImage(aPercentBox, 350, 400, -25); // Personnal Percentage
				ShowImage(PercentBox, 350, 525, -25); // FirstGoal Percentage
				ShowImage(PercentBox, 350, 650, -25); // SecondGoal Percentage
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(initial, inactive, 485, 55, -55);
				ShowAlpha(BusCostAmount, inactive, 485, 180, -55);
				ShowAlpha(to_string(Savings.percentage), inactive, 485, 305, -55);
				ShowAlpha(ValueString, active, 485, 430, -55);
				ShowAlpha(to_string(FirstGoal.percentage), inactive, 485, 555, -55);
				ShowAlpha(to_string(SecondGoal.percentage), inactive, 485, 680, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = AddSalary_Back;
					break;

				case KEvent_Return:
					modifier = AddSalary_Add;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_1:
					// We don't want the value to be over 100
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "1";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "2";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "3";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "4";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "5";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "6";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "7";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "8";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "9";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "0";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, PersonnalBox))
				{
					modifier = AddSalary_Back;
				}
#pragma endregion

#pragma endregion

			} while (modifier != AddSalary_Back && modifier != AddSalary_Add);

			// If the user choses to confirm, we update the new amount
			if (modifier == AddSalary_Add)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				Personnal.percentage = stoi(ValueString);
			}
#pragma endregion
			break;

		case AddSalary_First:
#pragma region EDIT FIRST
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream SalaryStream;
				stringstream BusStream;
				SalaryStream << fixed << setprecision(2) << Salary.amount;
				string initial = SalaryStream.str();
				BusStream << fixed << setprecision(2) << BusCost.amount;
				string BusCostAmount = BusStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Salary.name, inactive, 550, 10, -75);
				ShowAlpha(BusCost.name, inactive, 535, 140, -75);
				ShowAlpha(Savings.name + " Cut", inactive, 510, 265, -75);
				ShowAlpha(Personnal.name + " Cut", inactive, 500, 390, -75);
				ShowAlpha(FirstGoal.name, inactive, 500, 515, -75);
				ShowAlpha(SecondGoal.name, inactive, 500, 640, -75);
				ShowImage(AmountBox, 350, 25, -25); // Salary Amount
				ShowImage(AmountBox, 350, 150, -25); // Bus Cost Amount
				ShowImage(PercentBox, 350, 275, -25); // Savings Percentage
				ShowImage(PercentBox, 350, 400, -25); // Personnal Percentage
				ShowImage(aPercentBox, 350, 525, -25); // FirstGoal Percentage
				ShowImage(PercentBox, 350, 650, -25); // SecondGoal Percentage
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(initial, inactive, 485, 55, -55);
				ShowAlpha(BusCostAmount, inactive, 485, 180, -55);
				ShowAlpha(to_string(Savings.percentage), inactive, 485, 305, -55);
				ShowAlpha(to_string(Personnal.percentage), inactive, 485, 430, -55);
				ShowAlpha(ValueString, active, 485, 555, -55);
				ShowAlpha(to_string(SecondGoal.percentage), inactive, 485, 680, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = AddSalary_Back;
					break;

				case KEvent_Return:
					modifier = AddSalary_Add;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_1:
					// We don't want the value to be over 100
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "1";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "2";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "3";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "4";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "5";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "6";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "7";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "8";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "9";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "0";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, FirstBox))
				{
					modifier = AddSalary_Back;
				}
#pragma endregion

#pragma endregion

			} while (modifier != AddSalary_Back && modifier != AddSalary_Add);

			// If the user choses to confirm, we update the new amount
			if (modifier == AddSalary_Add)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				FirstGoal.percentage = stoi(ValueString);
			}
#pragma endregion
			break;

		case AddSalary_Second:
#pragma region EDIT SECOND
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream SalaryStream;
				stringstream BusStream;
				SalaryStream << fixed << setprecision(2) << Salary.amount;
				string initial = SalaryStream.str();
				BusStream << fixed << setprecision(2) << BusCost.amount;
				string BusCostAmount = BusStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Salary.name, inactive, 550, 10, -75);
				ShowAlpha(BusCost.name, inactive, 535, 140, -75);
				ShowAlpha(Savings.name + " Cut", inactive, 510, 265, -75);
				ShowAlpha(Personnal.name + " Cut", inactive, 500, 390, -75);
				ShowAlpha(FirstGoal.name, inactive, 500, 515, -75);
				ShowAlpha(SecondGoal.name, inactive, 500, 640, -75);
				ShowImage(AmountBox, 350, 25, -25); // Salary Amount
				ShowImage(AmountBox, 350, 150, -25); // Bus Cost Amount
				ShowImage(PercentBox, 350, 275, -25); // Savings Percentage
				ShowImage(PercentBox, 350, 400, -25); // Personnal Percentage
				ShowImage(PercentBox, 350, 525, -25); // FirstGoal Percentage
				ShowImage(aPercentBox, 350, 650, -25); // SecondGoal Percentage
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(initial, inactive, 485, 55, -55);
				ShowAlpha(BusCostAmount, inactive, 485, 180, -55);
				ShowAlpha(to_string(Savings.percentage), inactive, 485, 305, -55);
				ShowAlpha(to_string(Personnal.percentage), inactive, 485, 430, -55);
				ShowAlpha(to_string(FirstGoal.percentage), inactive, 485, 555, -55);
				ShowAlpha(ValueString, active, 485, 680, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = AddSalary_Back;
					break;

				case KEvent_Return:
					modifier = AddSalary_Add;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_1:
					// We don't want the value to be over 100
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "1";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "2";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "3";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "4";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "5";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "6";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "7";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "8";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "9";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "0";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, SecondBox))
				{
					modifier = AddSalary_Back;
				}
#pragma endregion

#pragma endregion

			} while (modifier != AddSalary_Back && modifier != AddSalary_Add);

			// If the user choses to confirm, we update the new amount
			if (modifier == AddSalary_Add)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				SecondGoal.percentage = stoi(ValueString);
			}
#pragma endregion
			break;

		case AddSalary_Add:
#pragma region ADD
			string error;
			int sum = Savings.percentage + Personnal.percentage + FirstGoal.percentage + SecondGoal.percentage;
			if (sum != 100)
			{
				error = "The sum of the percentages is " + to_string(Savings.percentage + Personnal.percentage + FirstGoal.percentage + SecondGoal.percentage) + " instead of 100";
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Invalid Percentages", error.c_str(), NULL);
			}
			else if (Salary.amount >= 1000000)
			{
				error = to_string(Salary.amount) + "$ is a too big amount. This program won't be able to read data over time if the amount is too big. \n Value type: double";
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Invalid Salary", error.c_str(), NULL);
			}
			else if (BusCost.amount >= 1000)
			{
				error = "If your bus pass costs this much, consider saving for your car. " + to_string(BusCost.amount) + "$ is a too big amount and will make data unreadable over time. \n Value type: double";
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Invalid Salary", error.c_str(), NULL);
			}
			else
			{
				// We open the file to write in
				ofstream Write(file);

				double newSalary;
				Write.clear();
				newSalary = Salary.amount - BusCost.amount;
				string SCut = "0." + to_string(Savings.percentage);
				string PCut = "0." + to_string(Personnal.percentage);
				string FstCut = "0." + to_string(FirstGoal.percentage);
				string SndCut = "0." + to_string(SecondGoal.percentage);

				// Calculate divisions
				double SavingsCut = stod(SCut) * newSalary;
				double PersonnalCut = stod(PCut) * newSalary;
				double FirstCut = stod(FstCut) * newSalary;
				double SecondCut = stod(SndCut) * newSalary;
				Savings.amount += SavingsCut;
				FirstGoal.amount += FirstCut;
				SecondGoal.amount += SecondCut;
				Bus.amount += BusCost.amount;

				// Update file
				Write <<
					Savings.name << " " << Savings.amount << endl <<
					FirstGoal.name << " " << FirstGoal.amount << endl <<
					SecondGoal.name << " " << SecondGoal.amount << endl <<
					Bus.name << " " << Bus.amount;
				Write.close();

				// Show addings
				// We set the precision of double numbers
				stringstream SalaryStream;
				SalaryStream << fixed << setprecision(2) << Salary.amount;
				string SalaryAmount = SalaryStream.str();

				stringstream BusCostStream;
				BusCostStream << fixed << setprecision(2) << BusCost.amount;
				string BusCostAmount = BusCostStream.str();

				stringstream NewSalaryStream;
				NewSalaryStream << fixed << setprecision(2) << newSalary;
				string NewSalaryAmount = NewSalaryStream.str();

				stringstream SavingsStream;
				SavingsStream << fixed << setprecision(2) << SavingsCut;
				string SavingsAmount = SavingsStream.str();

				stringstream PersonnalStream;
				PersonnalStream << fixed << setprecision(2) << PersonnalCut;
				string PersonnalAmount = PersonnalStream.str();

				stringstream FirstStream;
				FirstStream << fixed << setprecision(2) << FirstCut;
				string FirstAmount = FirstStream.str();

				stringstream SecondStream;
				SecondStream << fixed << setprecision(2) << SecondCut;
				string SecondAmount = SecondStream.str();

				Wait(10);

				ShowImage(Background, 0, 0);

				// Animation
				for (int counter = 0; counter < stod(SavingsAmount); counter++)
				{

					stringstream CounterStream;
					CounterStream << fixed << setprecision(2) << counter;
					string CounterStr = CounterStream.str();

					Wait(1);
					ShowImage(Background, 0, 0);

					ShowAlpha(Salary.name + ":", inactive, 10, 50, -50);
					ShowAlpha(SalaryAmount + "$", active, 725, 50, -50);

					ShowAlpha(BusCost.name + ":", inactive, 10, 110, -50);
					ShowAlpha(BusCostAmount + "$", active, 725, 110, -50);

					ShowAlpha("New " + Salary.name + ":", inactive, 10, 170, -50);
					ShowAlpha(NewSalaryAmount + "$", active, 725, 170, -50);

					ShowAlpha(Savings.name + ":", inactive, 10, 275, -50);
					ShowAlpha("+" + CounterStr + "$", inactive, 725, 275, -50);

					PlayEffect(increase, 5);

					// Skip Animation
					key = PollKey();
					if (key == KEvent_Return || key == KEvent_Space)
					{
						break;
					}

					Refresh();
				}
				ShowAlpha(Savings.name + " cut:", inactive, 10, 275, -50);
				ShowAlpha("+" + SavingsAmount + "$", active, 725, 275, -50);

				// Animation
				for (int counter = 0; counter < stod(PersonnalAmount); counter++)
				{

					stringstream CounterStream;
					CounterStream << fixed << setprecision(2) << counter;
					string CounterStr = CounterStream.str();

					Wait(1);
					ShowImage(Background, 0, 0);

					ShowAlpha(Salary.name + ":", inactive, 10, 50, -50);
					ShowAlpha(SalaryAmount + "$", active, 725, 50, -50);

					ShowAlpha(BusCost.name + ":", inactive, 10, 110, -50);
					ShowAlpha(BusCostAmount + "$", active, 725, 110, -50);

					ShowAlpha("New " + Salary.name + ":", inactive, 10, 170, -50);
					ShowAlpha(NewSalaryAmount + "$", active, 725, 170, -50);

					ShowAlpha(Savings.name + ":", inactive, 10, 275, -50);
					ShowAlpha("+" + SavingsAmount + "$", active, 725, 275, -50);

					ShowAlpha(Personnal.name + ":", inactive, 10, 335, -50);
					ShowAlpha("+" + CounterStr + "$", inactive, 725, 335, -50);

					PlayEffect(increase, 5);

					// Skip Animation
					key = PollKey();
					if (key == KEvent_Return || key == KEvent_Space)
					{
						break;
					}

					Refresh();
				}
				ShowAlpha(Personnal.name + " cut:", inactive, 10, 335, -50);
				ShowAlpha("+" + PersonnalAmount + "$", active, 725, 335, -50);

				// Animation
				for (int counter = 0; counter < stod(FirstAmount); counter++)
				{

					stringstream CounterStream;
					CounterStream << fixed << setprecision(2) << counter;
					string CounterStr = CounterStream.str();

					Wait(1);
					ShowImage(Background, 0, 0);

					ShowAlpha(Salary.name + ":", inactive, 10, 50, -50);
					ShowAlpha(SalaryAmount + "$", active, 725, 50, -50);

					ShowAlpha(BusCost.name + ":", inactive, 10, 110, -50);
					ShowAlpha(BusCostAmount + "$", active, 725, 110, -50);

					ShowAlpha("New " + Salary.name + ":", inactive, 10, 170, -50);
					ShowAlpha(NewSalaryAmount + "$", active, 725, 170, -50);

					ShowAlpha(Savings.name + ":", inactive, 10, 275, -50);
					ShowAlpha("+" + SavingsAmount + "$", active, 725, 275, -50);

					ShowAlpha(Personnal.name + ":", inactive, 10, 335, -50);
					ShowAlpha("+" + PersonnalAmount + "$", active, 725, 335, -50);

					ShowAlpha(FirstGoal.name + ":", inactive, 10, 395, -50);
					ShowAlpha("+" + CounterStr + "$", inactive, 725, 395, -50);

					PlayEffect(increase, 5);

					// Skip Animation
					key = PollKey();
					if (key == KEvent_Return || key == KEvent_Space)
					{
						break;
					}

					Refresh();
				}
				ShowAlpha(FirstGoal.name + " cut:", inactive, 10, 395, -50);
				ShowAlpha("+" + FirstAmount + "$", active, 725, 395, -50);

				// Animation
				for (int counter = 0; counter < stod(SecondAmount); counter++)
				{

					stringstream CounterStream;
					CounterStream << fixed << setprecision(2) << counter;
					string CounterStr = CounterStream.str();

					Wait(1);
					ShowImage(Background, 0, 0);

					ShowAlpha(Salary.name + ":", inactive, 10, 50, -50);
					ShowAlpha(SalaryAmount + "$", active, 725, 50, -50);

					ShowAlpha(BusCost.name + ":", inactive, 10, 110, -50);
					ShowAlpha(BusCostAmount + "$", active, 725, 110, -50);

					ShowAlpha("New " + Salary.name + ":", inactive, 10, 170, -50);
					ShowAlpha(NewSalaryAmount + "$", active, 725, 170, -50);

					ShowAlpha(Savings.name + ":", inactive, 10, 275, -50);
					ShowAlpha("+" + SavingsAmount + "$", active, 725, 275, -50);

					ShowAlpha(Personnal.name + ":", inactive, 10, 335, -50);
					ShowAlpha("+" + PersonnalAmount + "$", active, 725, 335, -50);

					ShowAlpha(FirstGoal.name + ":", inactive, 10, 395, -50);
					ShowAlpha("+" + FirstAmount + "$", active, 725, 395, -50);

					ShowAlpha(SecondGoal.name + ":", inactive, 10, 455, -50);
					ShowAlpha("+" + CounterStr + "$", inactive, 725, 455, -50);

					PlayEffect(increase, 5);

					// Skip Animation
					key = PollKey();
					if (key == KEvent_Return || key == KEvent_Space)
					{
						break;
					}

					Refresh();
				}
				ShowAlpha(SecondGoal.name + " cut:", inactive, 10, 455, -50);
				ShowAlpha("+" + SecondAmount + "$", active, 725, 455, -50);

				ShowImage(Background, 0, 0);

				ShowAlpha(Salary.name + ":", inactive, 10, 50, -50);
				ShowAlpha(SalaryAmount + "$", active, 725, 50, -50);

				ShowAlpha(BusCost.name + ":", inactive, 10, 110, -50);
				ShowAlpha(BusCostAmount + "$", active, 725, 110, -50);

				ShowAlpha("New " + Salary.name + ":", inactive, 10, 170, -50);
				ShowAlpha(NewSalaryAmount + "$", active, 725, 170, -50);

				ShowAlpha(Savings.name + ":", inactive, 10, 275, -50);
				ShowAlpha("+" + SavingsAmount + "$", active, 725, 275, -50);

				ShowAlpha(Personnal.name + ":", inactive, 10, 335, -50);
				ShowAlpha("+" + PersonnalAmount + "$", active, 725, 335, -50);

				ShowAlpha(FirstGoal.name + ":", inactive, 10, 395, -50);
				ShowAlpha("+" + FirstAmount + "$", active, 725, 395, -50);

				ShowAlpha(SecondGoal.name + ":", inactive, 10, 455, -50);
				ShowAlpha("+" + SecondAmount + "$", active, 725, 455, -50);

				ShowAlpha("Press any key to continue", active, 400, 750, -80);

				Refresh();

				// No do while required here, we'll just wait for the user to press a key to continue
				WaitKey();

			}
#pragma endregion

			break;

		}
#pragma endregion

	} while (choice != AddSalary_Add && choice != AddSalary_Back);

	FreeImages();
	FreeText();
}



void AddSavingsMenu()
{

#pragma region VARIABLES
	// Data from .sav file
	Element Savings;
	Element FirstGoal;
	Element SecondGoal;
	Element Bus;
	string file = "Ressources\\savings.sav";
	ifstream Read(file);
	if (!Read)
	{
		string errTitle = "Couldn't open .sav file";
		string errMsg = "Unable to find file: " + file;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, errTitle.c_str(), errMsg.c_str(), NULL);
		exit(1);
	}
	Read >>
		Savings.name >> Savings.amount >>
		FirstGoal.name >> FirstGoal.amount >>
		SecondGoal.name >> SecondGoal.amount >>
		Bus.name >> Bus.amount;
	Read.close();
	// Element of the box we'll be able to edit the amount we want to add
	Element Amount;
	Amount.name = Savings.name;
	Amount.amount = 0;
	// Images
	ImageId Background = LoadBmp("Images\\background.bmp");
	ImageId AmountBox = LoadBmp("Images\\amountBox.bmp");
	ImageId hAmountBox = LoadBmp("Images\\hamountBox.bmp");
	ImageId aAmountBox = LoadBmp("Images\\aamountBox.bmp");
	ImageId AddButton = LoadBmp("Images\\add.bmp");
	ImageId aAddButton = LoadBmp("Images\\aAdd.bmp");
	ImageId QuitButton = LoadBmp("Images\\quit.bmp");
	ImageId aQuitButton = LoadBmp("Images\\aQuit.bmp");
	// Alphabets
	AlphaId inactive = LoadAlphabet("Alphabets\\inactive");
	AlphaId active = LoadAlphabet("Alphabets\\active");
	// Selection
	int cursor = AddSalary_Amount;
	AddSavings_Selection modifier = AddSavings_NA;
	AddSavings_Selection choice = AddSavings_NA;
	Mouse mouse;
	KEvent key = KEvent_None;
	int soundEvent = AddSalary_NA;
	// Hitboxes
	Area AmountHB = CreateImageArea(350, 355, AmountBox, -25);
	Area AddHB = CreateImageArea(725, 275, AddButton);
	Area QuitHB = CreateImageArea(715, 375, QuitButton);
	// Sounds
	SndId increase = LoadEffect("Sounds\\increase.wav");
	SndId navigation = LoadEffect("Sounds\\sndNavigation.wav");
	SndId confirm = LoadEffect("sounds\\sndSelect.wav");
#pragma endregion

	do
	{
		Wait(10);
#pragma region do{NAVIGATION}
		do
		{
			choice = AddSavings_NA;
			Wait(10);

			stringstream AmountStream;
			AmountStream << fixed << setprecision(2) << Amount.amount;
			string AmountStr = AmountStream.str();

#pragma region SHOWMENU
			ShowImage(Background, 0, 0);

			ShowAlpha(Amount.name, inactive, 550, 340, -75);

			switch (cursor)
			{
			case AddSavings_Amount:
				ShowImage(hAmountBox, 350, 355, -25); // Amount
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				break;
			case AddSavings_Add:
				ShowImage(AmountBox, 350, 355, -25);
				ShowImage(aAddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				break;
			case AddSavings_Quit:
				ShowImage(AmountBox, 350, 355, -25);
				ShowImage(AddButton, 725, 275);
				ShowImage(aQuitButton, 715, 375);
				break;
			}

			ShowAlpha(AmountStr, inactive, 485, 385, -55);

			Refresh();
#pragma endregion

			// Read input
			PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION
#pragma region KEYBOARD
			switch (key)
			{
			case KEvent_Down:
				if (cursor == AddSavings_Add)
				{
					cursor++;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Up:
				if (cursor == AddSavings_Quit)
				{
					cursor--;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Right:
				if (cursor == AddSavings_Amount)
				{
					cursor = AddSavings_Add;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Left:
				if (cursor == AddSavings_Quit || cursor == AddSavings_Add)
				{
					cursor = AddSavings_Amount;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Esc:
				choice = AddSavings_Quit;
				PlayEffect(confirm);
				break;

			case KEvent_Space:
			case KEvent_Return:
				PlayEffect(confirm);
				switch (cursor)
				{
				case AddSavings_Amount:
					choice = AddSavings_Amount;
					break;

				case AddSavings_Add:
					choice = AddSavings_Add;
					break;

				case AddSavings_Quit:
					choice = AddSavings_Quit;
					break;
				}
				break;
			}
#pragma endregion

#pragma region MOUSE
			if (inArea(mouse, AmountHB))
			{
				cursor = AddSavings_Amount;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, AddHB))
			{
				cursor = AddSavings_Add;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, QuitHB))
			{
				cursor = AddSavings_Quit;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}

			soundEvent = cursor;

			if (mouse.button == MEvent_Left &&
				(inArea(mouse, AmountHB) ||
					inArea(mouse, AddHB) ||
					inArea(mouse, QuitHB))
				)
			{
				switch (cursor)
				{
				case AddSavings_Amount:
					choice = AddSavings_Amount;
					break;

				case AddSavings_Add:
					choice = AddSavings_Add;
					break;

				case AddSavings_Quit:
					choice = AddSavings_Quit;
					break;

				}
			}
#pragma endregion
#pragma endregion

		} while (choice == AddSavings_NA);
#pragma endregion

		modifier = AddSavings_NA;

		// We clear the text
		string ValueString = "";
		// This will be used to check what the string would be after input to check if it would be valid
		string tempValue = "";
		// This will check if the user can enter the input
		bool valid = true;
		// We'll start modifying the values
		switch (choice)
		{
		case AddSavings_Amount:
#pragma region EDIT SALARY
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream AmountStream;
				AmountStream << fixed << setprecision(2) << Amount.amount;
				string AmountStr = AmountStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Amount.name, active, 550, 340, -75);
				ShowImage(aAmountBox, 350, 355, -25); // Amount
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(ValueString, inactive, 485, 385, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = AddSavings_Quit;
					break;

				case KEvent_Return:
					modifier = AddSavings_Add;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_Period:
					// We check to see if there is already a period in the string and if a period is the first input
					for (int i = 0; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							valid = false;
						}
					}

					if (ValueString.size() == 0)
					{
						valid = false;
					}

					if (valid)
					{
						ValueString += ".";
					}
					// We set it back to it's original state
					valid = true;
					break;

				case KEvent_1:
					// We check if there is two decimals
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, AmountHB))
				{
					modifier = AddSavings_Quit;
				}
#pragma endregion

#pragma endregion

			} while (modifier != AddSavings_Quit && modifier != AddSavings_Add);

			// If the user choses to confirm, we update the new amount
			if (modifier == AddSavings_Add)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				Amount.amount = stod(ValueString);
			}
#pragma endregion
			break;

		case AddSavings_Add:
#pragma region ADD
			string error;

			if (Amount.amount >= 1000000)
			{
				error = to_string(Amount.amount) + "$ is a too big amount. This program won't be able to read data over time if the amount is always that big. \n Value type: double";
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Invalid Salary", error.c_str(), NULL);
			}
			else
			{
				// We open the file to write in
				ofstream Write(file);

				Write.clear();

				// Calculate
				Savings.amount += Amount.amount;

				// Update file
				Write <<
					Savings.name << " " << Savings.amount << endl <<
					FirstGoal.name << " " << FirstGoal.amount << endl <<
					SecondGoal.name << " " << SecondGoal.amount << endl <<
					Bus.name << " " << Bus.amount;
				Write.close();

				// Show addings
				// We set the precision of double numbers
				stringstream AmountStream;
				AmountStream << fixed << setprecision(2) << Amount.amount;
				string AmountStr = AmountStream.str();

				Wait(10);

				// Animation
				int counter = 0;
				for (int i = 0; i < Amount.amount; i++)
				{
					counter = i;

					stringstream CounterStream;
					CounterStream << fixed << setprecision(2) << counter;
					string CounterStr = CounterStream.str();

					Wait(1);
					ShowImage(Background, 0, 0);

					ShowAlpha(Amount.name + ":", inactive, 10, 50, -50);
					ShowAlpha("+" + CounterStr + "$", inactive, 725, 50, -50);

					PlayEffect(increase, 5);

					// Skip Animation
					key = PollKey();
					if (key == KEvent_Return || key == KEvent_Space)
					{
						break;
					}

					Refresh();
				}

				ShowImage(Background, 0, 0);

				ShowAlpha(Amount.name + ":", inactive, 10, 50, -50);
				ShowAlpha("+" + AmountStr + "$", active, 725, 50, -50);

				ShowAlpha("Press any key to continue", active, 400, 750, -80);

				Refresh();

				// No do while required here, we'll just wait for the user to press a key to continue
				WaitKey();

			}
#pragma endregion
			break;
		}

	} while (choice != AddSavings_Add && choice != AddSavings_Quit);



	FreeText();
	FreeImages();
}



void AddFirstMenu()
{
#pragma region VARIABLES
	// Data from .sav file
	Element Savings;
	Element FirstGoal;
	Element SecondGoal;
	Element Bus;
	string file = "Ressources\\savings.sav";
	ifstream Read(file);
	if (!Read)
	{
		string errTitle = "Couldn't open .sav file";
		string errMsg = "Unable to find file: " + file;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, errTitle.c_str(), errMsg.c_str(), NULL);
		exit(1);
	}
	Read >>
		Savings.name >> Savings.amount >>
		FirstGoal.name >> FirstGoal.amount >>
		SecondGoal.name >> SecondGoal.amount >>
		Bus.name >> Bus.amount;
	Read.close();
	// Element of the box we'll be able to edit the amount we want to add
	Element Amount;
	Amount.name = FirstGoal.name;
	Amount.amount = 0;
	// Images
	ImageId Background = LoadBmp("Images\\background.bmp");
	ImageId AmountBox = LoadBmp("Images\\amountBox.bmp");
	ImageId hAmountBox = LoadBmp("Images\\hamountBox.bmp");
	ImageId aAmountBox = LoadBmp("Images\\aamountBox.bmp");
	ImageId AddButton = LoadBmp("Images\\add.bmp");
	ImageId aAddButton = LoadBmp("Images\\aAdd.bmp");
	ImageId QuitButton = LoadBmp("Images\\quit.bmp");
	ImageId aQuitButton = LoadBmp("Images\\aQuit.bmp");
	// Alphabets
	AlphaId inactive = LoadAlphabet("Alphabets\\inactive");
	AlphaId active = LoadAlphabet("Alphabets\\active");
	// Selection
	int cursor = AddFirst_Amount;
	AddFirst_Selection modifier = AddFirst_NA;
	AddFirst_Selection choice = AddFirst_NA;
	Mouse mouse;
	KEvent key = KEvent_None;
	int soundEvent = AddSalary_NA;
	// Hitboxes
	Area AmountHB = CreateImageArea(350, 355, AmountBox, -25);
	Area AddHB = CreateImageArea(725, 275, AddButton);
	Area QuitHB = CreateImageArea(715, 375, QuitButton);
	// Sounds
	SndId increase = LoadEffect("Sounds\\increase.wav");
	SndId navigation = LoadEffect("Sounds\\sndNavigation.wav");
	SndId confirm = LoadEffect("sounds\\sndSelect.wav");

#pragma endregion

	do
	{
		Wait(10);
#pragma region do{NAVIGATION}
		do
		{
			choice = AddFirst_NA;
			Wait(10);

			stringstream AmountStream;
			AmountStream << fixed << setprecision(2) << Amount.amount;
			string AmountStr = AmountStream.str();

#pragma region SHOWMENU
			ShowImage(Background, 0, 0);

			ShowAlpha(Amount.name, inactive, 550, 340, -75);

			switch (cursor)
			{
			case AddFirst_Amount:
				ShowImage(hAmountBox, 350, 355, -25); // Amount
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				break;
			case AddFirst_Add:
				ShowImage(AmountBox, 350, 355, -25);
				ShowImage(aAddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				break;
			case AddFirst_Quit:
				ShowImage(AmountBox, 350, 355, -25);
				ShowImage(AddButton, 725, 275);
				ShowImage(aQuitButton, 715, 375);
				break;
			}

			ShowAlpha(AmountStr, inactive, 485, 385, -55);

			Refresh();
#pragma endregion

			// Read input
			PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION
#pragma region KEYBOARD
			switch (key)
			{
			case KEvent_Down:
				if (cursor == AddFirst_Add)
				{
					cursor++;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Up:
				if (cursor == AddFirst_Quit)
				{
					cursor--;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Right:
				if (cursor == AddFirst_Amount)
				{
					cursor = AddFirst_Add;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Left:
				if (cursor == AddFirst_Quit || cursor == AddFirst_Add)
				{
					cursor = AddFirst_Amount;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Esc:
				choice = AddFirst_Quit;
				PlayEffect(confirm);
				break;

			case KEvent_Space:
			case KEvent_Return:
				PlayEffect(confirm);
				switch (cursor)
				{
				case AddFirst_Amount:
					choice = AddFirst_Amount;
					break;

				case AddFirst_Add:
					choice = AddFirst_Add;
					break;

				case AddFirst_Quit:
					choice = AddFirst_Quit;
					break;
				}
				break;
			}
#pragma endregion

#pragma region MOUSE
			if (inArea(mouse, AmountHB))
			{
				cursor = AddFirst_Amount;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, AddHB))
			{
				cursor = AddFirst_Add;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, QuitHB))
			{
				cursor = AddFirst_Quit;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}

			soundEvent = cursor;

			if (mouse.button == MEvent_Left &&
				(inArea(mouse, AmountHB) ||
					inArea(mouse, AddHB) ||
					inArea(mouse, QuitHB))
				)
			{
				switch (cursor)
				{
				case AddFirst_Amount:
					choice = AddFirst_Amount;
					break;

				case AddFirst_Add:
					choice = AddFirst_Add;
					break;

				case AddFirst_Quit:
					choice = AddFirst_Quit;
					break;

				}
			}
#pragma endregion
#pragma endregion

		} while (choice == AddFirst_NA);
#pragma endregion

		modifier = AddFirst_NA;

		// We clear the text
		string ValueString = "";
		// This will be used to check what the string would be after input to check if it would be valid
		string tempValue = "";
		// This will check if the user can enter the input
		bool valid = true;
		// We'll start modifying the values
		switch (choice)
		{
		case AddFirst_Amount:
#pragma region EDIT SALARY
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream AmountStream;
				AmountStream << fixed << setprecision(2) << Amount.amount;
				string AmountStr = AmountStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Amount.name, active, 550, 340, -75);
				ShowImage(aAmountBox, 350, 355, -25); // Amount
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(ValueString, inactive, 485, 385, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = AddFirst_Quit;
					break;

				case KEvent_Return:
					modifier = AddFirst_Add;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_Period:
					// We check to see if there is already a period in the string and if a period is the first input
					for (int i = 0; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							valid = false;
						}
					}

					if (ValueString.size() == 0)
					{
						valid = false;
					}

					if (valid)
					{
						ValueString += ".";
					}
					// We set it back to it's original state
					valid = true;
					break;

				case KEvent_1:
					// We check if there is two decimals
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, AmountHB))
				{
					modifier = AddFirst_Quit;
				}
#pragma endregion

#pragma endregion

			} while (modifier != AddFirst_Quit && modifier != AddFirst_Add);

			// If the user choses to confirm, we update the new amount
			if (modifier == AddFirst_Add)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				Amount.amount = stod(ValueString);
			}
#pragma endregion
			break;

		case AddFirst_Add:
#pragma region ADD
			string error;

			if (Amount.amount >= 1000000)
			{
				error = to_string(Amount.amount) + "$ is a too big amount. This program won't be able to read data over time if the amount is always that big. \n Value type: double";
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Invalid Salary", error.c_str(), NULL);
			}
			else
			{
				// We open the file to write in
				ofstream Write(file);

				Write.clear();

				// Calculate
				FirstGoal.amount += Amount.amount;

				// Update file
				Write <<
					Savings.name << " " << Savings.amount << endl <<
					FirstGoal.name << " " << FirstGoal.amount << endl <<
					SecondGoal.name << " " << SecondGoal.amount << endl <<
					Bus.name << " " << Bus.amount;
				Write.close();

				// Show addings
				// We set the precision of double numbers
				stringstream AmountStream;
				AmountStream << fixed << setprecision(2) << Amount.amount;
				string AmountStr = AmountStream.str();

				Wait(10);

				// Animation
				int counter = 0;
				for (int i = 0; i < Amount.amount; i++)
				{
					counter = i;

					stringstream CounterStream;
					CounterStream << fixed << setprecision(2) << counter;
					string CounterStr = CounterStream.str();

					Wait(1);
					ShowImage(Background, 0, 0);

					ShowAlpha(Amount.name + ":", inactive, 10, 50, -50);
					ShowAlpha("+" + CounterStr + "$", inactive, 725, 50, -50);

					PlayEffect(increase, 5);

					// Skip Animation
					key = PollKey();
					if (key == KEvent_Return || key == KEvent_Space)
					{
						break;
					}

					Refresh();
				}

				ShowImage(Background, 0, 0);

				ShowAlpha(Amount.name + ":", inactive, 10, 50, -50);
				ShowAlpha("+" + AmountStr + "$", active, 725, 50, -50);

				ShowAlpha("Press any key to continue", active, 400, 750, -80);

				Refresh();

				// No do while required here, we'll just wait for the user to press a key to continue
				WaitKey();

			}
#pragma endregion
			break;
		}

	} while (choice != AddFirst_Add && choice != AddFirst_Quit);



	FreeText();
	FreeImages();
}



void AddSecondMenu()
{
#pragma region VARIABLES
	// Data from .sav file
	Element Savings;
	Element FirstGoal;
	Element SecondGoal;
	Element Bus;
	string file = "Ressources\\savings.sav";
	ifstream Read(file);
	if (!Read)
	{
		string errTitle = "Couldn't open .sav file";
		string errMsg = "Unable to find file: " + file;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, errTitle.c_str(), errMsg.c_str(), NULL);
		exit(1);
	}
	Read >>
		Savings.name >> Savings.amount >>
		FirstGoal.name >> FirstGoal.amount >>
		SecondGoal.name >> SecondGoal.amount >>
		Bus.name >> Bus.amount;
	Read.close();
	// Element of the box we'll be able to edit the amount we want to add
	Element Amount;
	Amount.name = SecondGoal.name;
	Amount.amount = 0;
	// Images
	ImageId Background = LoadBmp("Images\\background.bmp");
	ImageId AmountBox = LoadBmp("Images\\amountBox.bmp");
	ImageId hAmountBox = LoadBmp("Images\\hamountBox.bmp");
	ImageId aAmountBox = LoadBmp("Images\\aamountBox.bmp");
	ImageId AddButton = LoadBmp("Images\\add.bmp");
	ImageId aAddButton = LoadBmp("Images\\aAdd.bmp");
	ImageId QuitButton = LoadBmp("Images\\quit.bmp");
	ImageId aQuitButton = LoadBmp("Images\\aQuit.bmp");
	// Alphabets
	AlphaId inactive = LoadAlphabet("Alphabets\\inactive");
	AlphaId active = LoadAlphabet("Alphabets\\active");
	// Selection
	int cursor = AddSecond_Amount;
	AddSecond_Selection modifier = AddSecond_NA;
	AddSecond_Selection choice = AddSecond_NA;
	Mouse mouse;
	KEvent key = KEvent_None;
	int soundEvent = AddSalary_NA;
	// Hitboxes
	Area AmountHB = CreateImageArea(350, 355, AmountBox, -25);
	Area AddHB = CreateImageArea(725, 275, AddButton);
	Area QuitHB = CreateImageArea(715, 375, QuitButton);
	// Sounds
	SndId increase = LoadEffect("Sounds\\increase.wav");
	SndId navigation = LoadEffect("Sounds\\sndNavigation.wav");
	SndId confirm = LoadEffect("sounds\\sndSelect.wav");

#pragma endregion

	do
	{
		Wait(10);
#pragma region do{NAVIGATION}
		do
		{
			choice = AddSecond_NA;
			Wait(10);

			stringstream AmountStream;
			AmountStream << fixed << setprecision(2) << Amount.amount;
			string AmountStr = AmountStream.str();

#pragma region SHOWMENU
			ShowImage(Background, 0, 0);

			ShowAlpha(Amount.name, inactive, 550, 340, -75);

			switch (cursor)
			{
			case AddSecond_Amount:
				ShowImage(hAmountBox, 350, 355, -25); // Amount
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				break;
			case AddSecond_Add:
				ShowImage(AmountBox, 350, 355, -25);
				ShowImage(aAddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				break;
			case AddSecond_Quit:
				ShowImage(AmountBox, 350, 355, -25);
				ShowImage(AddButton, 725, 275);
				ShowImage(aQuitButton, 715, 375);
				break;
			}

			ShowAlpha(AmountStr, inactive, 485, 385, -55);

			Refresh();
#pragma endregion

			// Read input
			PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION
#pragma region KEYBOARD
			switch (key)
			{
			case KEvent_Down:
				if (cursor == AddSecond_Add)
				{
					cursor++;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Up:
				if (cursor == AddSecond_Quit)
				{
					cursor--;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Right:
				if (cursor == AddSecond_Amount)
				{
					cursor = AddSecond_Add;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Left:
				if (cursor == AddSecond_Quit || cursor == AddSecond_Add)
				{
					cursor = AddSecond_Amount;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Esc:
				choice = AddSecond_Quit;
				PlayEffect(confirm);
				break;

			case KEvent_Space:
			case KEvent_Return:
				PlayEffect(confirm);
				switch (cursor)
				{
				case AddSecond_Amount:
					choice = AddSecond_Amount;
					break;

				case AddSecond_Add:
					choice = AddSecond_Add;
					break;

				case AddSecond_Quit:
					choice = AddSecond_Quit;
					break;
				}
				break;
			}
#pragma endregion

#pragma region MOUSE
			if (inArea(mouse, AmountHB))
			{
				cursor = AddSecond_Amount;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, AddHB))
			{
				cursor = AddSecond_Add;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, QuitHB))
			{
				cursor = AddSecond_Quit;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}

			soundEvent = cursor;

			if (mouse.button == MEvent_Left &&
				(inArea(mouse, AmountHB) ||
					inArea(mouse, AddHB) ||
					inArea(mouse, QuitHB))
				)
			{
				switch (cursor)
				{
				case AddSecond_Amount:
					choice = AddSecond_Amount;
					break;

				case AddSecond_Add:
					choice = AddSecond_Add;
					break;

				case AddSecond_Quit:
					choice = AddSecond_Quit;
					break;

				}
			}
#pragma endregion
#pragma endregion

		} while (choice == AddSecond_NA);
#pragma endregion

		modifier = AddSecond_NA;

		// We clear the text
		string ValueString = "";
		// This will be used to check what the string would be after input to check if it would be valid
		string tempValue = "";
		// This will check if the user can enter the input
		bool valid = true;
		// We'll start modifying the values
		switch (choice)
		{
		case AddSecond_Amount:
#pragma region EDIT SALARY
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream AmountStream;
				AmountStream << fixed << setprecision(2) << Amount.amount;
				string AmountStr = AmountStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Amount.name, active, 550, 340, -75);
				ShowImage(aAmountBox, 350, 355, -25); // Amount
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(ValueString, inactive, 485, 385, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = AddSecond_Quit;
					break;

				case KEvent_Return:
					modifier = AddSecond_Add;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_Period:
					// We check to see if there is already a period in the string and if a period is the first input
					for (int i = 0; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							valid = false;
						}
					}

					if (ValueString.size() == 0)
					{
						valid = false;
					}

					if (valid)
					{
						ValueString += ".";
					}
					// We set it back to it's original state
					valid = true;
					break;

				case KEvent_1:
					// We check if there is two decimals
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, AmountHB))
				{
					modifier = AddSecond_Quit;
				}
#pragma endregion

#pragma endregion

			} while (modifier != AddSecond_Quit && modifier != AddSecond_Add);

			// If the user choses to confirm, we update the new amount
			if (modifier == AddSecond_Add)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				Amount.amount = stod(ValueString);
			}
#pragma endregion
			break;

		case AddSecond_Add:
#pragma region ADD
			string error;

			if (Amount.amount >= 1000000)
			{
				error = to_string(Amount.amount) + "$ is a too big amount. This program won't be able to read data over time if the amount is always that big. \n Value type: double";
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Invalid Salary", error.c_str(), NULL);
			}
			else
			{
				// We open the file to write in
				ofstream Write(file);

				Write.clear();

				// Calculate
				SecondGoal.amount += Amount.amount;

				// Update file
				Write <<
					Savings.name << " " << Savings.amount << endl <<
					FirstGoal.name << " " << FirstGoal.amount << endl <<
					SecondGoal.name << " " << SecondGoal.amount << endl <<
					Bus.name << " " << Bus.amount;
				Write.close();

				// Show addings
				// We set the precision of double numbers
				stringstream AmountStream;
				AmountStream << fixed << setprecision(2) << Amount.amount;
				string AmountStr = AmountStream.str();

				Wait(10);

				// Animation
				int counter = 0;
				for (int i = 0; i < Amount.amount; i++)
				{
					counter = i;

					stringstream CounterStream;
					CounterStream << fixed << setprecision(2) << counter;
					string CounterStr = CounterStream.str();

					Wait(1);
					ShowImage(Background, 0, 0);

					ShowAlpha(Amount.name + ":", inactive, 10, 50, -50);
					ShowAlpha("+" + CounterStr + "$", inactive, 725, 50, -50);

					PlayEffect(increase, 5);

					// Skip Animation
					key = PollKey();
					if (key == KEvent_Return || key == KEvent_Space)
					{
						break;
					}

					Refresh();
				}

				ShowImage(Background, 0, 0);

				ShowAlpha(Amount.name + ":", inactive, 10, 50, -50);
				ShowAlpha("+" + AmountStr + "$", active, 725, 50, -50);

				ShowAlpha("Press any key to continue", active, 400, 750, -80);

				Refresh();

				// No do while required here, we'll just wait for the user to press a key to continue
				WaitKey();

			}
#pragma endregion
			break;
		}

	} while (choice != AddSecond_Add && choice != AddSecond_Quit);



	FreeText();
	FreeImages();
}



void AddBusMenu()
{
#pragma region VARIABLES
	// Data from .sav file
	Element Savings;
	Element FirstGoal;
	Element SecondGoal;
	Element Bus;
	string file = "Ressources\\savings.sav";
	ifstream Read(file);
	if (!Read)
	{
		string errTitle = "Couldn't open .sav file";
		string errMsg = "Unable to find file: " + file;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, errTitle.c_str(), errMsg.c_str(), NULL);
		exit(1);
	}
	Read >>
		Savings.name >> Savings.amount >>
		FirstGoal.name >> FirstGoal.amount >>
		SecondGoal.name >> SecondGoal.amount >>
		Bus.name >> Bus.amount;
	Read.close();
	// Element of the box we'll be able to edit the amount we want to add
	Element Amount;
	Amount.name = Bus.name;
	Amount.amount = 0;
	// Images
	ImageId Background = LoadBmp("Images\\background.bmp");
	ImageId AmountBox = LoadBmp("Images\\amountBox.bmp");
	ImageId hAmountBox = LoadBmp("Images\\hamountBox.bmp");
	ImageId aAmountBox = LoadBmp("Images\\aamountBox.bmp");
	ImageId AddButton = LoadBmp("Images\\add.bmp");
	ImageId aAddButton = LoadBmp("Images\\aAdd.bmp");
	ImageId QuitButton = LoadBmp("Images\\quit.bmp");
	ImageId aQuitButton = LoadBmp("Images\\aQuit.bmp");
	// Alphabets
	AlphaId inactive = LoadAlphabet("Alphabets\\inactive");
	AlphaId active = LoadAlphabet("Alphabets\\active");
	// Selection
	int cursor = AddBus_Amount;
	AddBus_Selection modifier = AddBus_NA;
	AddBus_Selection choice = AddBus_NA;
	Mouse mouse;
	KEvent key = KEvent_None;
	int soundEvent = AddSalary_NA;
	// Hitboxes
	Area AmountHB = CreateImageArea(350, 355, AmountBox, -25);
	Area AddHB = CreateImageArea(725, 275, AddButton);
	Area QuitHB = CreateImageArea(715, 375, QuitButton);
	// Sounds
	SndId increase = LoadEffect("Sounds\\increase.wav");
	SndId navigation = LoadEffect("Sounds\\sndNavigation.wav");
	SndId confirm = LoadEffect("sounds\\sndSelect.wav");

#pragma endregion

	do
	{
		Wait(10);
#pragma region do{NAVIGATION}
		do
		{
			choice = AddBus_NA;
			Wait(10);

			stringstream AmountStream;
			AmountStream << fixed << setprecision(2) << Amount.amount;
			string AmountStr = AmountStream.str();

#pragma region SHOWMENU
			ShowImage(Background, 0, 0);

			ShowAlpha(Amount.name, inactive, 550, 340, -75);

			switch (cursor)
			{
			case AddBus_Amount:
				ShowImage(hAmountBox, 350, 355, -25); // Amount
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				break;
			case AddBus_Add:
				ShowImage(AmountBox, 350, 355, -25);
				ShowImage(aAddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				break;
			case AddBus_Quit:
				ShowImage(AmountBox, 350, 355, -25);
				ShowImage(AddButton, 725, 275);
				ShowImage(aQuitButton, 715, 375);
				break;
			}

			ShowAlpha(AmountStr, inactive, 485, 385, -55);

			Refresh();
#pragma endregion

			// Read input
			PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION
#pragma region KEYBOARD
			switch (key)
			{
			case KEvent_Down:
				if (cursor == AddBus_Add)
				{
					cursor++;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Up:
				if (cursor == AddBus_Quit)
				{
					cursor--;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Right:
				if (cursor == AddBus_Amount)
				{
					cursor = AddBus_Add;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Left:
				if (cursor == AddBus_Quit || cursor == AddBus_Add)
				{
					cursor = AddBus_Amount;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Esc:
				choice = AddBus_Quit;
				PlayEffect(confirm);
				break;

			case KEvent_Space:
			case KEvent_Return:
				PlayEffect(confirm);
				switch (cursor)
				{
				case AddBus_Amount:
					choice = AddBus_Amount;
					break;

				case AddBus_Add:
					choice = AddBus_Add;
					break;

				case AddBus_Quit:
					choice = AddBus_Quit;
					break;
				}
				break;
			}
#pragma endregion

#pragma region MOUSE
			if (inArea(mouse, AmountHB))
			{
				cursor = AddBus_Amount;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, AddHB))
			{
				cursor = AddBus_Add;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, QuitHB))
			{
				cursor = AddBus_Quit;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}

			soundEvent = cursor;

			if (mouse.button == MEvent_Left &&
				(inArea(mouse, AmountHB) ||
					inArea(mouse, AddHB) ||
					inArea(mouse, QuitHB))
				)
			{
				switch (cursor)
				{
				case AddBus_Amount:
					choice = AddBus_Amount;
					break;

				case AddBus_Add:
					choice = AddBus_Add;
					break;

				case AddBus_Quit:
					choice = AddBus_Quit;
					break;

				}
			}
#pragma endregion
#pragma endregion

		} while (choice == AddBus_NA);
#pragma endregion

		modifier = AddBus_NA;

		// We clear the text
		string ValueString = "";
		// This will be used to check what the string would be after input to check if it would be valid
		string tempValue = "";
		// This will check if the user can enter the input
		bool valid = true;
		// We'll start modifying the values
		switch (choice)
		{
		case AddBus_Amount:
#pragma region EDIT SALARY
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream AmountStream;
				AmountStream << fixed << setprecision(2) << Amount.amount;
				string AmountStr = AmountStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Amount.name, active, 550, 340, -75);
				ShowImage(aAmountBox, 350, 355, -25); // Amount
				ShowImage(AddButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(ValueString, inactive, 485, 385, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = AddBus_Quit;
					break;

				case KEvent_Return:
					modifier = AddBus_Add;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_Period:
					// We check to see if there is already a period in the string and if a period is the first input
					for (int i = 0; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							valid = false;
						}
					}

					if (ValueString.size() == 0)
					{
						valid = false;
					}

					if (valid)
					{
						ValueString += ".";
					}
					// We set it back to it's original state
					valid = true;
					break;

				case KEvent_1:
					// We check if there is two decimals
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, AmountHB))
				{
					modifier = AddBus_Quit;
				}
#pragma endregion

#pragma endregion

			} while (modifier != AddBus_Quit && modifier != AddBus_Add);

			// If the user choses to confirm, we update the new amount
			if (modifier == AddBus_Add)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				Amount.amount = stod(ValueString);
			}
#pragma endregion
			break;

		case AddBus_Add:
#pragma region ADD
			string error;

			if (Amount.amount >= 1000000)
			{
				error = to_string(Amount.amount) + "$ is a too big amount. This program won't be able to read data over time if the amount is always that big. \n Value type: double";
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Invalid Salary", error.c_str(), NULL);
			}
			else
			{
				// We open the file to write in
				ofstream Write(file);

				Write.clear();

				// Calculate
				Bus.amount += Amount.amount;

				// Update file
				Write <<
					Savings.name << " " << Savings.amount << endl <<
					FirstGoal.name << " " << FirstGoal.amount << endl <<
					SecondGoal.name << " " << SecondGoal.amount << endl <<
					Bus.name << " " << Bus.amount;
				Write.close();

				// Show addings
				// We set the precision of double numbers
				stringstream AmountStream;
				AmountStream << fixed << setprecision(2) << Amount.amount;
				string AmountStr = AmountStream.str();

				Wait(10);

				// Animation
				int counter = 0;
				for (int i = 0; i < Amount.amount; i++)
				{
					counter = i;

					stringstream CounterStream;
					CounterStream << fixed << setprecision(2) << counter;
					string CounterStr = CounterStream.str();

					Wait(1);
					ShowImage(Background, 0, 0);

					ShowAlpha(Amount.name + ":", inactive, 10, 50, -50);
					ShowAlpha("+" + CounterStr + "$", inactive, 725, 50, -50);

					PlayEffect(increase, 5);

					// Skip Animation
					key = PollKey();
					if (key == KEvent_Return || key == KEvent_Space)
					{
						break;
					}

					Refresh();
				}

				ShowImage(Background, 0, 0);

				ShowAlpha(Amount.name + ":", inactive, 10, 50, -50);
				ShowAlpha("+" + AmountStr + "$", active, 725, 50, -50);

				ShowAlpha("Press any key to continue", active, 400, 750, -80);

				Refresh();

				// No do while required here, we'll just wait for the user to press a key to continue
				WaitKey();

			}
#pragma endregion
			break;
		}

	} while (choice != AddBus_Add && choice != AddBus_Quit);



	FreeText();
	FreeImages();
}



void Show()
{
#pragma region VARIABLES
	// Data from .sav file
	Element Savings;
	Element FirstGoal;
	Element SecondGoal;
	Element Bus;
	string file = "Ressources\\savings.sav";
	ifstream Read(file);
	if (!Read)
	{
		string errTitle = "Couldn't open .sav file";
		string errMsg = "Unable to find file: " + file;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, errTitle.c_str(), errMsg.c_str(), NULL);
		exit(1);
	}
	Read >>
		Savings.name >> Savings.amount >>
		FirstGoal.name >> FirstGoal.amount >>
		SecondGoal.name >> SecondGoal.amount >>
		Bus.name >> Bus.amount;
	Read.close();
	// Element of the box we'll be able to edit the amount we want to add
	Element Amount;
	Amount.name = Bus.name;
	Amount.amount = 0;
	// Images
	ImageId Background = LoadBmp("Images\\background.bmp");
	// Alphabets
	AlphaId inactive = LoadAlphabet("Alphabets\\inactive");
	AlphaId active = LoadAlphabet("Alphabets\\active");
	// Selection
	int cursor = AddBus_Amount;
	AddBus_Selection modifier = AddBus_NA;
	AddBus_Selection choice = AddBus_NA;
	KEvent key = KEvent_None;
	int soundEvent = AddSalary_NA;
	// Sounds
	SndId confirm = LoadEffect("sounds\\sndSelect.wav");
#pragma endregion

	// Amount to string conversion
	stringstream SavingsStream;
	SavingsStream << fixed << setprecision(2) << Savings.amount;
	string SavingsStr = SavingsStream.str();

	stringstream FirstStream;
	FirstStream << fixed << setprecision(2) << FirstGoal.amount;
	string FirstStr = FirstStream.str();

	stringstream SecondStream;
	SecondStream << fixed << setprecision(2) << SecondGoal.amount;
	string SecondStr = SecondStream.str();

	stringstream BusStream;
	BusStream << fixed << setprecision(2) << Bus.amount;
	string BusStr = BusStream.str();

	double Total = Savings.amount + FirstGoal.amount + SecondGoal.amount; // Does not include Bus Pass
	stringstream TotalStream;
	TotalStream << fixed << setprecision(2) << Total;
	string TotalStr = TotalStream.str();

#pragma region SHOW MENU
	ShowImage(Background, 0, 0);

	ShowAlpha(Savings.name + ":", inactive, 50, 50, -25);
	ShowAlpha(SavingsStr + "$", active, 650, 50, -25);

	ShowAlpha(FirstGoal.name + ":", inactive, 50, 200, -25);
	ShowAlpha(FirstStr + "$", active, 650, 200, -25);

	ShowAlpha(SecondGoal.name + ":", inactive, 50, 350, -25);
	ShowAlpha(SecondStr + "$", active, 650, 350, -25);

	ShowAlpha("Total:", inactive, 50, 500, -25);
	ShowAlpha(TotalStr + "$", active, 650, 500, -25);

	ShowAlpha(Bus.name + ":", inactive, 50, 650, -25);
	ShowAlpha(BusStr + "$", active, 650, 650, -25);

	ShowAlpha("Press C to copy to clipboard. press any key to go back to menu", active, 225, 750, -85, 2);
	Refresh();
#pragma endregion

	// Wait Key
	key = WaitKey();
	PlayEffect(confirm);

	if (key == KEvent_c)
	{
		string ClipboardStr = Savings.name + ": " + SavingsStr + "$\n" +
			FirstGoal.name + ": " + FirstStr + "$\n" +
			SecondGoal.name + ": " + SecondStr + "$\n" +
			"Total: " + TotalStr + "$\n\n" +
			Bus.name + ": " + BusStr + "$\n\n" +
			"---------------\n\n" +
			Savings.name + " " + SavingsStr + "\n" +
			FirstGoal.name + " " + FirstStr + "\n" +
			SecondGoal.name + " " + SecondStr + "\n" +
			Bus.name + " " + BusStr;

		const char* output = ClipboardStr.c_str();
		const size_t len = strlen(output) + 1;
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
		memcpy(GlobalLock(hMem), output, len);
		GlobalUnlock(hMem);
		OpenClipboard(0);
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hMem);
		CloseClipboard();
	}

	FreeText();
	FreeImages();
}



Remove_Selection RemoveMenu()
{
#pragma region VARIABLES
	// Variables
	Element Savings;
	Element FirstGoal;
	Element SecondGoal;
	Element Bus;
	Remove_Selection choice = Remove_NA;
	int cursor = Remove_Salary;
	KEvent keyPress = KEvent_None;
	Mouse mouseEvent;
	SndId navigation = LoadEffect("Sounds\\sndNavigation.wav");
	SndId confirm = LoadEffect("Sounds\\sndSelect.wav");
	int soundEvent = Remove_NA;
	// Read the savings file and place the elements into the 'Element' type variables
	string file = "Ressources\\savings.sav";
	ifstream Read(file);
	if (!Read)
	{
		string errTitle = "Couldn't open .sav file";
		string errMsg = "Unable to find file: " + file;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, errTitle.c_str(), errMsg.c_str(), NULL);
		exit(1);
	}
	Read >>
		Savings.name >> Savings.amount >>
		FirstGoal.name >> FirstGoal.amount >>
		SecondGoal.name >> SecondGoal.amount >>
		Bus.name >> Bus.amount;
	Read.close();
	// Images
	//FontId font = LoadFont("font.ttf", 100);
	ImageId Background = LoadBmp("Images\\background.bmp");
	ImageId Quit = LoadBmp("Images\\quit.bmp");
	Area QuitHB;
	QuitHB.start.x = 497;
	QuitHB.start.y = 690;
	QuitHB.end.x = 761;
	QuitHB.end.y = 760;
	ImageId aQuit = LoadBmp("Images\\aQuit.bmp");
	ImageId btn_Salary = LoadBmp("Images\\salary.bmp");
	Area SalaryHB;
	SalaryHB.start.x = 388;
	SalaryHB.start.y = 58;
	SalaryHB.end.x = 873;
	SalaryHB.end.y = 143;
	ImageId btn_aSalary = LoadBmp("Images\\aSalary.bmp");
	AlphaId active = LoadAlphabet("Alphabets\\active");
	AlphaId inactive = LoadAlphabet("Alphabets\\inactive");
	Area SavingsHB;
	SavingsHB.start.x = 328;
	SavingsHB.start.y = 154;
	SavingsHB.end.x = 914;
	SavingsHB.end.y = 242;
	Area First;
	First.start.x = 328;
	First.start.y = 285;
	First.end.x = 914;
	First.end.y = 373;
	Area Second;
	Second.start.x = 328;
	Second.start.y = 410;
	Second.end.x = 914;
	Second.end.y = 493;
	Area BusP;
	BusP.start.x = 328;
	BusP.start.y = 564;
	BusP.end.x = 914;
	BusP.end.y = 646;
#pragma endregion
	do
	{
		Wait(10);
#pragma region SHOW MENU
		ShowImage(Background, 0, 0);
		// Shows the hovered action depending on the cursor (Make sure the order of the cases matches the enum for better clarity and prevent errors)
		switch (cursor)
		{
		case Remove_Salary:
			ShowImage(btn_aSalary, 235, 25);
			ShowAlpha(Savings.name, inactive, 330, 150, -5);
			ShowAlpha(FirstGoal.name, inactive, 330, 275, -5);
			ShowAlpha(SecondGoal.name, inactive, 330, 400, -5);
			ShowAlpha(Bus.name, inactive, 330, 550, -5);
			ShowImage(Quit, 235, 650);
			break;
		case Remove_Savings:
			ShowImage(btn_Salary, 235, 25);
			ShowAlpha(Savings.name, active, 330, 150, -5);
			ShowAlpha(FirstGoal.name, inactive, 330, 275, -5);
			ShowAlpha(SecondGoal.name, inactive, 330, 400, -5);
			ShowAlpha(Bus.name, inactive, 330, 550, -5);
			ShowImage(Quit, 235, 650);
			break;
		case Remove_FirstGoal:
			ShowImage(btn_Salary, 235, 25);
			ShowAlpha(Savings.name, inactive, 330, 150, -5);
			ShowAlpha(FirstGoal.name, active, 330, 275, -5);
			ShowAlpha(SecondGoal.name, inactive, 330, 400, -5);
			ShowAlpha(Bus.name, inactive, 330, 550, -5);
			ShowImage(Quit, 235, 650);
			break;
		case Remove_SecondGoal:
			ShowImage(btn_Salary, 235, 25);
			ShowAlpha(Savings.name, inactive, 330, 150, -5);
			ShowAlpha(FirstGoal.name, inactive, 330, 275, -5);
			ShowAlpha(SecondGoal.name, active, 330, 400, -5);
			ShowAlpha(Bus.name, inactive, 330, 550, -5);
			ShowImage(Quit, 235, 650);
			break;

		case Remove_BusPass:
			ShowImage(btn_Salary, 235, 25);
			ShowAlpha(Savings.name, inactive, 330, 150, -5);
			ShowAlpha(FirstGoal.name, inactive, 330, 275, -5);
			ShowAlpha(SecondGoal.name, inactive, 330, 400, -5);
			ShowAlpha(Bus.name, active, 330, 550, -5);
			ShowImage(Quit, 235, 650);
			break;

		case Remove_Quit:
			ShowImage(btn_Salary, 235, 25);
			ShowAlpha(Savings.name, inactive, 330, 150, -5);
			ShowAlpha(FirstGoal.name, inactive, 330, 275, -5);
			ShowAlpha(SecondGoal.name, inactive, 330, 400, -5);
			ShowAlpha(Bus.name, inactive, 330, 550, -5);
			ShowImage(aQuit, 235, 650);
			break;
		}
		Refresh();
#pragma endregion

		// Read Input
		PollMouseAndKeyboard(mouseEvent, keyPress);

#pragma region INPUT ACTION
		switch (keyPress)
		{
		case KEvent_Up: // Going Up
			if (cursor > Remove_Salary)
			{
				--cursor;
			}
			PlayEffect(navigation);
			break;
		case KEvent_Down: // Going Down
			if (cursor < Remove_Quit)
			{
				++cursor;
			}
			PlayEffect(navigation);
			break;
		case KEvent_Space:
		case KEvent_Return:
			PlayEffect(confirm);
			// We convert the cursor (int) into the choice (Menu_Selection)
			// We can't just do choice = cursor since they aren't the same type
			switch (cursor)
			{
			case Remove_Salary:
				choice = Remove_Salary;
				break;

			case Remove_Savings:
				choice = Remove_Savings;
				break;

			case Remove_FirstGoal:
				choice = Remove_FirstGoal;
				break;

			case Remove_SecondGoal:
				choice = Remove_SecondGoal;
				break;

			case Remove_BusPass:
				choice = Remove_BusPass;
				break;

			case Remove_Quit:
				choice = Remove_Quit;
				break;
			}
			break;
		case KEvent_Esc:
			choice = Remove_Quit;
			PlayEffect(confirm);
			break;
		}

		// Here we check if the cursor is in an option area
		if (inArea(mouseEvent, SalaryHB))
		{
			cursor = Remove_Salary;
			if (soundEvent != cursor)
			{
				PlayEffect(navigation);
			}
		}
		else if (inArea(mouseEvent, SavingsHB))
		{
			cursor = Remove_Savings;
			if (soundEvent != cursor)
			{
				PlayEffect(navigation);
			}
		}
		else if (inArea(mouseEvent, First))
		{
			cursor = Remove_FirstGoal;
			if (soundEvent != cursor)
			{
				PlayEffect(navigation);
			}
		}
		else if (inArea(mouseEvent, Second))
		{
			cursor = Remove_SecondGoal;
			if (soundEvent != cursor)
			{
				PlayEffect(navigation);
			}
		}
		else if (inArea(mouseEvent, BusP))
		{
			cursor = Remove_BusPass;
			if (soundEvent != cursor)
			{
				PlayEffect(navigation);
			}
		}
		else if (inArea(mouseEvent, QuitHB))
		{
			cursor = Remove_Quit;
			if (soundEvent != cursor)
			{
				PlayEffect(navigation);
			}
		}

		soundEvent = cursor;

		if ((mouseEvent.button == MEvent_Left) &&
			(inArea(mouseEvent, SalaryHB) ||
				inArea(mouseEvent, SavingsHB) ||
				inArea(mouseEvent, First) ||
				inArea(mouseEvent, Second) ||
				inArea(mouseEvent, BusP) ||
				inArea(mouseEvent, QuitHB))) // If the user clicks an option in it's hitbox, we assign that option to the choice
		{
			PlayEffect(confirm);
			switch (cursor)
			{
			case Remove_Salary:
				choice = Remove_Salary;
				break;

			case Remove_Savings:
				choice = Remove_Savings;
				break;

			case Remove_FirstGoal:
				choice = Remove_FirstGoal;
				break;

			case Remove_SecondGoal:
				choice = Remove_SecondGoal;
				break;

			case Remove_BusPass:
				choice = Remove_BusPass;
				break;

			case Remove_Quit:
				choice = Remove_Quit;
			}
		}
#pragma endregion


	} while (choice == Remove_NA);
	FreeImages();
	FreeText();
	return choice;
}



void RemoveSalaryMenu()
{
#pragma region VARIABLES
	// Variables
	// Elements data
	Element Salary;
	Salary.name = "Salary";
	Salary.amount = 0;
	Element BusCost;
	BusCost.name = "Bus Cost";
	BusCost.amount = 0;
	Element Personnal;
	Personnal.name = "Personnal";
	Personnal.percentage = PersonnalPercentage;
	Element Savings;
	Savings.percentage = SavingsPercentage;
	Element FirstGoal;
	FirstGoal.percentage = FirstPercentage;
	Element SecondGoal;
	SecondGoal.percentage = SecondPercentage;
	Element Bus;
	string file = "Ressources\\savings.sav";
	ifstream Read(file);
	if (!Read)
	{
		string errTitle = "Couldn't open .sav file";
		string errMsg = "Unable to find file: " + file;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, errTitle.c_str(), errMsg.c_str(), NULL);
		exit(1);
	}
	Read >>
		Savings.name >> Savings.amount >>
		FirstGoal.name >> FirstGoal.amount >>
		SecondGoal.name >> SecondGoal.amount >>
		Bus.name >> Bus.amount;
	Read.close();
	// Images
	ImageId Background = LoadBmp("Images\\background.bmp");
	ImageId AmountBox = LoadBmp("Images\\amountBox.bmp");
	ImageId PercentBox = LoadBmp("Images\\percentageBox.bmp");
	ImageId aAmountBox = LoadBmp("Images\\aamountBox.bmp");
	ImageId aPercentBox = LoadBmp("Images\\apercentageBox.bmp");
	ImageId hAmountBox = LoadBmp("Images\\hamountBox.bmp");
	ImageId hPercentBox = LoadBmp("Images\\hpercentageBox.bmp");
	ImageId RemoveButton = LoadBmp("Images\\Remove.bmp");
	ImageId aRemoveButton = LoadBmp("Images\\aRemove.bmp");
	ImageId QuitButton = LoadBmp("Images\\quit.bmp");
	ImageId aQuitButton = LoadBmp("Images\\aQuit.bmp");
	// Alphabets
	AlphaId inactive = LoadAlphabet("Alphabets\\inactive");
	AlphaId active = LoadAlphabet("Alphabets\\active");
	// Selection
	int cursor = RemoveSalary_Amount;
	int tempCursor = RemoveSalary_Amount;
	RemoveSalary_Selection modifier = RemoveSalary_NA;
	RemoveSalary_Selection choice = RemoveSalary_NA;
	Mouse mouse;
	KEvent key = KEvent_None;
	int soundEvent = RemoveSalary_NA;
	// Hitboxes
	Area SalaryBox = CreateImageArea(350, 25, AmountBox, -25);
	Area BusCostBox = CreateImageArea(350, 150, AmountBox, -25);
	Area SavingsBox = CreateImageArea(350, 275, PercentBox, -25);
	Area PersonnalBox = CreateImageArea(350, 400, PercentBox, -25);
	Area FirstBox = CreateImageArea(350, 525, PercentBox, -25);
	Area SecondBox = CreateImageArea(350, 650, PercentBox, -25);
	Area RemoveBox = CreateImageArea(725, 275, RemoveButton, -25);
	Area QuitBox = CreateImageArea(715, 375, QuitButton, -25);
	// Sounds
	SndId increase = LoadEffect("Sounds\\increase.wav");
	SndId navigation = LoadEffect("Sounds\\sndNavigation.wav");
	SndId confirm = LoadEffect("Sounds\\sndSelect.wav");
#pragma endregion
	do
	{

#pragma region do{NAVIGATION}
		do // Navigation
		{
			choice = RemoveSalary_NA;
			Wait(10);

			// We set the precision of double numbers
			stringstream SalaryStream;
			stringstream BusStream;
			SalaryStream << fixed << setprecision(2) << Salary.amount;
			string SalaryAmount = SalaryStream.str();
			BusStream << fixed << setprecision(2) << BusCost.amount;
			string BusCostAmount = BusStream.str();

#pragma region SHOWMENU
			ShowImage(Background, 0, 0);

			ShowAlpha(Salary.name, inactive, 550, 10, -75);
			ShowAlpha(BusCost.name, inactive, 535, 140, -75);
			ShowAlpha(Savings.name + " Cut", inactive, 510, 265, -75);
			ShowAlpha(Personnal.name + " Cut", inactive, 500, 390, -75);
			ShowAlpha(FirstGoal.name, inactive, 500, 515, -75);
			ShowAlpha(SecondGoal.name, inactive, 500, 640, -75);

			switch (cursor)
			{
			case RemoveSalary_Amount:
				ShowImage(hAmountBox, 350, 25, -25); // Salary Amount
				ShowImage(AmountBox, 350, 150, -25); // Bus Cost Amount
				ShowImage(PercentBox, 350, 275, -25); // Savings Percentage
				ShowImage(PercentBox, 350, 400, -25); // Personnal Percentage
				ShowImage(PercentBox, 350, 525, -25); // FirstGoal Percentage
				ShowImage(PercentBox, 350, 650, -25); // SecondGoal Percentage
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				break;
			case RemoveSalary_Bus:
				ShowImage(AmountBox, 350, 25, -25);
				ShowImage(hAmountBox, 350, 150, -25);
				ShowImage(PercentBox, 350, 275, -25);
				ShowImage(PercentBox, 350, 400, -25);
				ShowImage(PercentBox, 350, 525, -25);
				ShowImage(PercentBox, 350, 650, -25);
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				break;
			case RemoveSalary_Savings:
				ShowImage(AmountBox, 350, 25, -25);
				ShowImage(AmountBox, 350, 150, -25);
				ShowImage(hPercentBox, 350, 275, -25);
				ShowImage(PercentBox, 350, 400, -25);
				ShowImage(PercentBox, 350, 525, -25);
				ShowImage(PercentBox, 350, 650, -25);
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				break;
			case RemoveSalary_Personnal:
				ShowImage(AmountBox, 350, 25, -25);
				ShowImage(AmountBox, 350, 150, -25);
				ShowImage(PercentBox, 350, 275, -25);
				ShowImage(hPercentBox, 350, 400, -25);
				ShowImage(PercentBox, 350, 525, -25);
				ShowImage(PercentBox, 350, 650, -25);
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				break;
			case RemoveSalary_First:
				ShowImage(AmountBox, 350, 25, -25);
				ShowImage(AmountBox, 350, 150, -25);
				ShowImage(PercentBox, 350, 275, -25);
				ShowImage(PercentBox, 350, 400, -25);
				ShowImage(hPercentBox, 350, 525, -25);
				ShowImage(PercentBox, 350, 650, -25);
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				break;
			case RemoveSalary_Second:
				ShowImage(AmountBox, 350, 25, -25);
				ShowImage(AmountBox, 350, 150, -25);
				ShowImage(PercentBox, 350, 275, -25);
				ShowImage(PercentBox, 350, 400, -25);
				ShowImage(PercentBox, 350, 525, -25);
				ShowImage(hPercentBox, 350, 650, -25);
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				break;
			case RemoveSalary_Remove:
				ShowImage(AmountBox, 350, 25, -25);
				ShowImage(AmountBox, 350, 150, -25);
				ShowImage(PercentBox, 350, 275, -25);
				ShowImage(PercentBox, 350, 400, -25);
				ShowImage(PercentBox, 350, 525, -25);
				ShowImage(PercentBox, 350, 650, -25);
				ShowImage(aRemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				break;
			case RemoveSalary_Back:
				ShowImage(AmountBox, 350, 25, -25);
				ShowImage(AmountBox, 350, 150, -25);
				ShowImage(PercentBox, 350, 275, -25);
				ShowImage(PercentBox, 350, 400, -25);
				ShowImage(PercentBox, 350, 525, -25);
				ShowImage(PercentBox, 350, 650, -25);
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(aQuitButton, 715, 375);
				break;
			}

			ShowAlpha(SalaryAmount, inactive, 485, 55, -55);
			ShowAlpha(BusCostAmount, inactive, 485, 180, -55);
			ShowAlpha(to_string(Savings.percentage), inactive, 485, 305, -55);
			ShowAlpha(to_string(Personnal.percentage), inactive, 485, 430, -55);
			ShowAlpha(to_string(FirstGoal.percentage), inactive, 485, 555, -55);
			ShowAlpha(to_string(SecondGoal.percentage), inactive, 485, 680, -55);

			Refresh();
#pragma endregion

			// Read input
			PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION
#pragma region KEYBOARD
			switch (key)
			{
			case KEvent_Down:
				if (cursor < RemoveSalary_Second && (cursor != RemoveSalary_Remove && cursor != RemoveSalary_Back))
				{
					cursor++;
					PlayEffect(navigation);
				}
				else if (cursor == RemoveSalary_Remove)
				{
					cursor++;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Up:
				if (cursor > RemoveSalary_Amount && (cursor != RemoveSalary_Remove && cursor != RemoveSalary_Back))
				{
					cursor--;
					PlayEffect(navigation);
				}
				else if (cursor == RemoveSalary_Back)
				{
					cursor--;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Right:
				if (cursor != RemoveSalary_Back && cursor != RemoveSalary_Remove)
				{
					tempCursor = cursor;
					cursor = RemoveSalary_Remove;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Left:
				if (cursor == RemoveSalary_Back || cursor == RemoveSalary_Remove)
				{
					cursor = tempCursor;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Esc:
				choice = RemoveSalary_Back;
				PlayEffect(confirm);
				break;

			case KEvent_Space:
			case KEvent_Return:
				PlayEffect(confirm);
				switch (cursor)
				{
				case RemoveSalary_Amount:
					choice = RemoveSalary_Amount;
					break;

				case RemoveSalary_Bus:
					choice = RemoveSalary_Bus;
					break;

				case RemoveSalary_Savings:
					choice = RemoveSalary_Savings;
					break;

				case RemoveSalary_Personnal:
					choice = RemoveSalary_Personnal;
					break;

				case RemoveSalary_First:
					choice = RemoveSalary_First;
					break;

				case RemoveSalary_Second:
					choice = RemoveSalary_Second;
					break;

				case RemoveSalary_Remove:
					choice = RemoveSalary_Remove;
					break;

				case RemoveSalary_Back:
					choice = RemoveSalary_Back;
					break;
				}
				break;
			}
#pragma endregion

#pragma region MOUSE
			if (inArea(mouse, SalaryBox))
			{
				cursor = RemoveSalary_Amount;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, BusCostBox))
			{
				cursor = RemoveSalary_Bus;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, SavingsBox))
			{
				cursor = RemoveSalary_Savings;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, PersonnalBox))
			{
				cursor = RemoveSalary_Personnal;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, FirstBox))
			{
				cursor = RemoveSalary_First;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, SecondBox))
			{
				cursor = RemoveSalary_Second;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, RemoveBox))
			{
				cursor = RemoveSalary_Remove;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, QuitBox))
			{
				cursor = RemoveSalary_Back;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}

			soundEvent = cursor;

			if (mouse.button == MEvent_Left &&
				(inArea(mouse, SalaryBox) ||
					inArea(mouse, BusCostBox) ||
					inArea(mouse, SavingsBox) ||
					inArea(mouse, PersonnalBox) ||
					inArea(mouse, FirstBox) ||
					inArea(mouse, SecondBox) ||
					inArea(mouse, RemoveBox) ||
					inArea(mouse, QuitBox))
				)
			{
				PlayEffect(confirm);
				switch (cursor)
				{
				case RemoveSalary_Amount:
					choice = RemoveSalary_Amount;
					break;

				case RemoveSalary_Bus:
					choice = RemoveSalary_Bus;
					break;

				case RemoveSalary_Savings:
					choice = RemoveSalary_Savings;
					break;

				case RemoveSalary_Personnal:
					choice = RemoveSalary_Personnal;
					break;

				case RemoveSalary_First:
					choice = RemoveSalary_First;
					break;

				case RemoveSalary_Second:
					choice = RemoveSalary_Second;
					break;

				case RemoveSalary_Remove:
					choice = RemoveSalary_Remove;
					break;

				case RemoveSalary_Back:
					choice = RemoveSalary_Back;
					break;
				}
			}
#pragma endregion
#pragma endregion

		} while (choice == RemoveSalary_NA);
#pragma endregion

		// We reset it back to none so we can come back again
		modifier = RemoveSalary_NA;

#pragma region CHOICE ACTION
		// We clear the text
		string ValueString = "";
		// This will be used to check what the string would be after input to check if it would be valid
		string tempValue = "";
		// This will check if the user can enter the input
		bool valid = true;
		// We'll start modifying the values
		switch (choice)
		{
		case RemoveSalary_Amount:
#pragma region EDIT SALARY
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream SalaryStream;
				stringstream BusStream;
				SalaryStream << fixed << setprecision(2) << Salary.amount;
				string initial = SalaryStream.str();
				BusStream << fixed << setprecision(2) << BusCost.amount;
				string BusCostAmount = BusStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Salary.name, inactive, 550, 10, -75);
				ShowAlpha(BusCost.name, inactive, 535, 140, -75);
				ShowAlpha(Savings.name + " Cut", inactive, 510, 265, -75);
				ShowAlpha(Personnal.name + " Cut", inactive, 500, 390, -75);
				ShowAlpha(FirstGoal.name, inactive, 500, 515, -75);
				ShowAlpha(SecondGoal.name, inactive, 500, 640, -75);
				ShowImage(aAmountBox, 350, 25, -25); // Salary Amount
				ShowImage(AmountBox, 350, 150, -25); // Bus Cost Amount
				ShowImage(PercentBox, 350, 275, -25); // Savings Percentage
				ShowImage(PercentBox, 350, 400, -25); // Personnal Percentage
				ShowImage(PercentBox, 350, 525, -25); // FirstGoal Percentage
				ShowImage(PercentBox, 350, 650, -25); // SecondGoal Percentage
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(ValueString, active, 485, 55, -55);
				ShowAlpha(BusCostAmount, inactive, 485, 180, -55);
				ShowAlpha(to_string(Savings.percentage), inactive, 485, 305, -55);
				ShowAlpha(to_string(Personnal.percentage), inactive, 485, 430, -55);
				ShowAlpha(to_string(FirstGoal.percentage), inactive, 485, 555, -55);
				ShowAlpha(to_string(SecondGoal.percentage), inactive, 485, 680, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = RemoveSalary_Back;
					break;

				case KEvent_Return:
					modifier = RemoveSalary_Remove;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_Period:
					// We check to see if there is already a period in the string and if a period is the first input
					for (int i = 0; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							valid = false;
						}
					}

					if (ValueString.size() == 0)
					{
						valid = false;
					}

					if (valid)
					{
						ValueString += ".";
					}
					// We set it back to it's original state
					valid = true;
					break;

				case KEvent_1:
					// We check if there is two decimals
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, SalaryBox))
				{
					modifier = RemoveSalary_Back;
				}
#pragma endregion

#pragma endregion

			} while (modifier != RemoveSalary_Back && modifier != RemoveSalary_Remove);

			// If the user choses to confirm, we update the new amount
			if (modifier == RemoveSalary_Remove)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				Salary.amount = stod(ValueString);
			}
#pragma endregion
			break;

		case RemoveSalary_Bus:
#pragma region EDIT BUS
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream SalaryStream;
				stringstream BusStream;
				SalaryStream << fixed << setprecision(2) << Salary.amount;
				string initial = SalaryStream.str();
				BusStream << fixed << setprecision(2) << BusCost.amount;
				string BusCostAmount = BusStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Salary.name, inactive, 550, 10, -75);
				ShowAlpha(BusCost.name, inactive, 535, 140, -75);
				ShowAlpha(Savings.name + " Cut", inactive, 510, 265, -75);
				ShowAlpha(Personnal.name + " Cut", inactive, 500, 390, -75);
				ShowAlpha(FirstGoal.name, inactive, 500, 515, -75);
				ShowAlpha(SecondGoal.name, inactive, 500, 640, -75);
				ShowImage(AmountBox, 350, 25, -25); // Salary Amount
				ShowImage(aAmountBox, 350, 150, -25); // Bus Cost Amount
				ShowImage(PercentBox, 350, 275, -25); // Savings Percentage
				ShowImage(PercentBox, 350, 400, -25); // Personnal Percentage
				ShowImage(PercentBox, 350, 525, -25); // FirstGoal Percentage
				ShowImage(PercentBox, 350, 650, -25); // SecondGoal Percentage
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(initial, inactive, 485, 55, -55);
				ShowAlpha(ValueString, active, 485, 180, -55);
				ShowAlpha(to_string(Savings.percentage), inactive, 485, 305, -55);
				ShowAlpha(to_string(Personnal.percentage), inactive, 485, 430, -55);
				ShowAlpha(to_string(FirstGoal.percentage), inactive, 485, 555, -55);
				ShowAlpha(to_string(SecondGoal.percentage), inactive, 485, 680, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = RemoveSalary_Back;
					break;

				case KEvent_Return:
					modifier = RemoveSalary_Remove;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_Period:
					// We check to see if there is already a period in the string and if a period is the first input
					for (int i = 0; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							valid = false;
						}
					}

					if (ValueString.size() == 0)
					{
						valid = false;
					}

					if (valid)
					{
						ValueString += ".";
					}
					// We set it back to it's original state
					valid = true;
					break;

				case KEvent_1:
					// We check if there is two decimals
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, BusCostBox))
				{
					modifier = RemoveSalary_Back;
				}
#pragma endregion

#pragma endregion

			} while (modifier != RemoveSalary_Back && modifier != RemoveSalary_Remove);

			// If the user choses to confirm, we update the new amount
			if (modifier == RemoveSalary_Remove)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				BusCost.amount = stod(ValueString);
			}
#pragma endregion
			break;

		case RemoveSalary_Savings:
#pragma region EDIT SAVINGS
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream SalaryStream;
				stringstream BusStream;
				SalaryStream << fixed << setprecision(2) << Salary.amount;
				string initial = SalaryStream.str();
				BusStream << fixed << setprecision(2) << BusCost.amount;
				string BusCostAmount = BusStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Salary.name, inactive, 550, 10, -75);
				ShowAlpha(BusCost.name, inactive, 535, 140, -75);
				ShowAlpha(Savings.name + " Cut", inactive, 510, 265, -75);
				ShowAlpha(Personnal.name + " Cut", inactive, 500, 390, -75);
				ShowAlpha(FirstGoal.name, inactive, 500, 515, -75);
				ShowAlpha(SecondGoal.name, inactive, 500, 640, -75);
				ShowImage(AmountBox, 350, 25, -25); // Salary Amount
				ShowImage(AmountBox, 350, 150, -25); // Bus Cost Amount
				ShowImage(aPercentBox, 350, 275, -25); // Savings Percentage
				ShowImage(PercentBox, 350, 400, -25); // Personnal Percentage
				ShowImage(PercentBox, 350, 525, -25); // FirstGoal Percentage
				ShowImage(PercentBox, 350, 650, -25); // SecondGoal Percentage
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(initial, inactive, 485, 55, -55);
				ShowAlpha(BusCostAmount, inactive, 485, 180, -55);
				ShowAlpha(ValueString, active, 485, 305, -55);
				ShowAlpha(to_string(Personnal.percentage), inactive, 485, 430, -55);
				ShowAlpha(to_string(FirstGoal.percentage), inactive, 485, 555, -55);
				ShowAlpha(to_string(SecondGoal.percentage), inactive, 485, 680, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = RemoveSalary_Back;
					break;

				case KEvent_Return:
					modifier = RemoveSalary_Remove;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_1:
					// We don't want the value to be over 100
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "1";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "2";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "3";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "4";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "5";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "6";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "7";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "8";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "9";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "0";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, SavingsBox))
				{
					modifier = RemoveSalary_Back;
				}
#pragma endregion

#pragma endregion

			} while (modifier != RemoveSalary_Back && modifier != RemoveSalary_Remove);

			// If the user choses to confirm, we update the new amount
			if (modifier == RemoveSalary_Remove)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				Savings.percentage = stoi(ValueString);
			}
#pragma endregion
			break;

		case RemoveSalary_Personnal:
#pragma region EDIT PERSONNAL
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream SalaryStream;
				stringstream BusStream;
				SalaryStream << fixed << setprecision(2) << Salary.amount;
				string initial = SalaryStream.str();
				BusStream << fixed << setprecision(2) << BusCost.amount;
				string BusCostAmount = BusStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Salary.name, inactive, 550, 10, -75);
				ShowAlpha(BusCost.name, inactive, 535, 140, -75);
				ShowAlpha(Savings.name + " Cut", inactive, 510, 265, -75);
				ShowAlpha(Personnal.name + " Cut", inactive, 500, 390, -75);
				ShowAlpha(FirstGoal.name, inactive, 500, 515, -75);
				ShowAlpha(SecondGoal.name, inactive, 500, 640, -75);
				ShowImage(AmountBox, 350, 25, -25); // Salary Amount
				ShowImage(AmountBox, 350, 150, -25); // Bus Cost Amount
				ShowImage(PercentBox, 350, 275, -25); // Savings Percentage
				ShowImage(aPercentBox, 350, 400, -25); // Personnal Percentage
				ShowImage(PercentBox, 350, 525, -25); // FirstGoal Percentage
				ShowImage(PercentBox, 350, 650, -25); // SecondGoal Percentage
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(initial, inactive, 485, 55, -55);
				ShowAlpha(BusCostAmount, inactive, 485, 180, -55);
				ShowAlpha(to_string(Savings.percentage), inactive, 485, 305, -55);
				ShowAlpha(ValueString, active, 485, 430, -55);
				ShowAlpha(to_string(FirstGoal.percentage), inactive, 485, 555, -55);
				ShowAlpha(to_string(SecondGoal.percentage), inactive, 485, 680, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = RemoveSalary_Back;
					break;

				case KEvent_Return:
					modifier = RemoveSalary_Remove;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_1:
					// We don't want the value to be over 100
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "1";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "2";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "3";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "4";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "5";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "6";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "7";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "8";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "9";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "0";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, PersonnalBox))
				{
					modifier = RemoveSalary_Back;
				}
#pragma endregion

#pragma endregion

			} while (modifier != RemoveSalary_Back && modifier != RemoveSalary_Remove);

			// If the user choses to confirm, we update the new amount
			if (modifier == RemoveSalary_Remove)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				Personnal.percentage = stoi(ValueString);
			}
#pragma endregion
			break;

		case RemoveSalary_First:
#pragma region EDIT FIRST
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream SalaryStream;
				stringstream BusStream;
				SalaryStream << fixed << setprecision(2) << Salary.amount;
				string initial = SalaryStream.str();
				BusStream << fixed << setprecision(2) << BusCost.amount;
				string BusCostAmount = BusStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Salary.name, inactive, 550, 10, -75);
				ShowAlpha(BusCost.name, inactive, 535, 140, -75);
				ShowAlpha(Savings.name + " Cut", inactive, 510, 265, -75);
				ShowAlpha(Personnal.name + " Cut", inactive, 500, 390, -75);
				ShowAlpha(FirstGoal.name, inactive, 500, 515, -75);
				ShowAlpha(SecondGoal.name, inactive, 500, 640, -75);
				ShowImage(AmountBox, 350, 25, -25); // Salary Amount
				ShowImage(AmountBox, 350, 150, -25); // Bus Cost Amount
				ShowImage(PercentBox, 350, 275, -25); // Savings Percentage
				ShowImage(PercentBox, 350, 400, -25); // Personnal Percentage
				ShowImage(aPercentBox, 350, 525, -25); // FirstGoal Percentage
				ShowImage(PercentBox, 350, 650, -25); // SecondGoal Percentage
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(initial, inactive, 485, 55, -55);
				ShowAlpha(BusCostAmount, inactive, 485, 180, -55);
				ShowAlpha(to_string(Savings.percentage), inactive, 485, 305, -55);
				ShowAlpha(to_string(Personnal.percentage), inactive, 485, 430, -55);
				ShowAlpha(ValueString, active, 485, 555, -55);
				ShowAlpha(to_string(SecondGoal.percentage), inactive, 485, 680, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = RemoveSalary_Back;
					break;

				case KEvent_Return:
					modifier = RemoveSalary_Remove;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_1:
					// We don't want the value to be over 100
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "1";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "2";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "3";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "4";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "5";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "6";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "7";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "8";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "9";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "0";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, FirstBox))
				{
					modifier = RemoveSalary_Back;
				}
#pragma endregion

#pragma endregion

			} while (modifier != RemoveSalary_Back && modifier != RemoveSalary_Remove);

			// If the user choses to confirm, we update the new amount
			if (modifier == RemoveSalary_Remove)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				FirstGoal.percentage = stoi(ValueString);
			}
#pragma endregion
			break;

		case RemoveSalary_Second:
#pragma region EDIT SECOND
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream SalaryStream;
				stringstream BusStream;
				SalaryStream << fixed << setprecision(2) << Salary.amount;
				string initial = SalaryStream.str();
				BusStream << fixed << setprecision(2) << BusCost.amount;
				string BusCostAmount = BusStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Salary.name, inactive, 550, 10, -75);
				ShowAlpha(BusCost.name, inactive, 535, 140, -75);
				ShowAlpha(Savings.name + " Cut", inactive, 510, 265, -75);
				ShowAlpha(Personnal.name + " Cut", inactive, 500, 390, -75);
				ShowAlpha(FirstGoal.name, inactive, 500, 515, -75);
				ShowAlpha(SecondGoal.name, inactive, 500, 640, -75);
				ShowImage(AmountBox, 350, 25, -25); // Salary Amount
				ShowImage(AmountBox, 350, 150, -25); // Bus Cost Amount
				ShowImage(PercentBox, 350, 275, -25); // Savings Percentage
				ShowImage(PercentBox, 350, 400, -25); // Personnal Percentage
				ShowImage(PercentBox, 350, 525, -25); // FirstGoal Percentage
				ShowImage(aPercentBox, 350, 650, -25); // SecondGoal Percentage
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(initial, inactive, 485, 55, -55);
				ShowAlpha(BusCostAmount, inactive, 485, 180, -55);
				ShowAlpha(to_string(Savings.percentage), inactive, 485, 305, -55);
				ShowAlpha(to_string(Personnal.percentage), inactive, 485, 430, -55);
				ShowAlpha(to_string(FirstGoal.percentage), inactive, 485, 555, -55);
				ShowAlpha(ValueString, active, 485, 680, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = RemoveSalary_Back;
					break;

				case KEvent_Return:
					modifier = RemoveSalary_Remove;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_1:
					// We don't want the value to be over 100
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "1";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "2";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "3";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "4";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "5";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "6";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "7";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "8";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "9";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					if (ValueString != "")
					{
						tempValue = ValueString;
						tempValue += "0";
						if (stoi(tempValue) > 100)
						{
							valid = false;
						}
					}
					tempValue = "";

					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, SecondBox))
				{
					modifier = RemoveSalary_Back;
				}
#pragma endregion

#pragma endregion

			} while (modifier != RemoveSalary_Back && modifier != RemoveSalary_Remove);

			// If the user choses to confirm, we update the new amount
			if (modifier == RemoveSalary_Remove)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				SecondGoal.percentage = stoi(ValueString);
			}
#pragma endregion
			break;

		case RemoveSalary_Remove:
#pragma region Remove
			string error;
			int sum = Savings.percentage + Personnal.percentage + FirstGoal.percentage + SecondGoal.percentage;
			if (sum != 100)
			{
				error = "The sum of the percentages is " + to_string(Savings.percentage + Personnal.percentage + FirstGoal.percentage + SecondGoal.percentage) + " instead of 100";
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Invalid Percentages", error.c_str(), NULL);
			}
			else if (Salary.amount >= 1000000)
			{
				error = to_string(Salary.amount) + "$ is a too big amount. This program won't be able to read data over time if the amount is too big. \n Value type: double";
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Invalid Salary", error.c_str(), NULL);
			}
			else if (BusCost.amount >= 1000)
			{
				error = "If your bus pass costs this much, consider saving for your car. " + to_string(BusCost.amount) + "$ is a too big amount and will make data unreadable over time. \n Value type: double";
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Invalid Salary", error.c_str(), NULL);
			}
			else
			{
				// We open the file to write in
				ofstream Write(file);

				double newSalary;
				Write.clear();
				newSalary = Salary.amount - BusCost.amount;
				string SCut = "0." + to_string(Savings.percentage);
				string PCut = "0." + to_string(Personnal.percentage);
				string FstCut = "0." + to_string(FirstGoal.percentage);
				string SndCut = "0." + to_string(SecondGoal.percentage);

				// Calculate divisions
				double SavingsCut = stod(SCut) * newSalary;
				double PersonnalCut = stod(PCut) * newSalary;
				double FirstCut = stod(FstCut) * newSalary;
				double SecondCut = stod(SndCut) * newSalary;
				Savings.amount -= SavingsCut;
				FirstGoal.amount -= FirstCut;
				SecondGoal.amount -= SecondCut;
				Bus.amount -= BusCost.amount;

				// Update file
				Write <<
					Savings.name << " " << Savings.amount << endl <<
					FirstGoal.name << " " << FirstGoal.amount << endl <<
					SecondGoal.name << " " << SecondGoal.amount << endl <<
					Bus.name << " " << Bus.amount;
				Write.close();

				// Show Removings
				// We set the precision of double numbers
				stringstream SalaryStream;
				SalaryStream << fixed << setprecision(2) << Salary.amount;
				string SalaryAmount = SalaryStream.str();

				stringstream BusCostStream;
				BusCostStream << fixed << setprecision(2) << BusCost.amount;
				string BusCostAmount = BusCostStream.str();

				stringstream NewSalaryStream;
				NewSalaryStream << fixed << setprecision(2) << newSalary;
				string NewSalaryAmount = NewSalaryStream.str();

				stringstream SavingsStream;
				SavingsStream << fixed << setprecision(2) << SavingsCut;
				string SavingsAmount = SavingsStream.str();

				stringstream PersonnalStream;
				PersonnalStream << fixed << setprecision(2) << PersonnalCut;
				string PersonnalAmount = PersonnalStream.str();

				stringstream FirstStream;
				FirstStream << fixed << setprecision(2) << FirstCut;
				string FirstAmount = FirstStream.str();

				stringstream SecondStream;
				SecondStream << fixed << setprecision(2) << SecondCut;
				string SecondAmount = SecondStream.str();

				Wait(10);

				ShowImage(Background, 0, 0);

				// Animation
				for (int counter = 0; counter < stod(SavingsAmount); counter++)
				{

					stringstream CounterStream;
					CounterStream << fixed << setprecision(2) << counter;
					string CounterStr = CounterStream.str();

					Wait(1);
					ShowImage(Background, 0, 0);

					ShowAlpha(Salary.name + ":", inactive, 10, 50, -50);
					ShowAlpha(SalaryAmount + "$", active, 725, 50, -50);

					ShowAlpha(BusCost.name + ":", inactive, 10, 110, -50);
					ShowAlpha(BusCostAmount + "$", active, 725, 110, -50);

					ShowAlpha("New " + Salary.name + ":", inactive, 10, 170, -50);
					ShowAlpha(NewSalaryAmount + "$", active, 725, 170, -50);

					ShowAlpha(Savings.name + ":", inactive, 10, 275, -50);
					ShowAlpha("-" + CounterStr + "$", inactive, 725, 275, -50);

					PlayEffect(increase, 5);

					// Skip Animation
					key = PollKey();
					if (key == KEvent_Return || key == KEvent_Space)
					{
						break;
					}

					Refresh();
				}
				ShowAlpha(Savings.name + " cut:", inactive, 10, 275, -50);
				ShowAlpha("-" + SavingsAmount + "$", active, 725, 275, -50);

				// Animation
				for (int counter = 0; counter < stod(PersonnalAmount); counter++)
				{

					stringstream CounterStream;
					CounterStream << fixed << setprecision(2) << counter;
					string CounterStr = CounterStream.str();

					Wait(1);
					ShowImage(Background, 0, 0);

					ShowAlpha(Salary.name + ":", inactive, 10, 50, -50);
					ShowAlpha(SalaryAmount + "$", active, 725, 50, -50);

					ShowAlpha(BusCost.name + ":", inactive, 10, 110, -50);
					ShowAlpha(BusCostAmount + "$", active, 725, 110, -50);

					ShowAlpha("New " + Salary.name + ":", inactive, 10, 170, -50);
					ShowAlpha(NewSalaryAmount + "$", active, 725, 170, -50);

					ShowAlpha(Savings.name + ":", inactive, 10, 275, -50);
					ShowAlpha("-" + SavingsAmount + "$", active, 725, 275, -50);

					ShowAlpha(Personnal.name + ":", inactive, 10, 335, -50);
					ShowAlpha("-" + CounterStr + "$", inactive, 725, 335, -50);

					PlayEffect(increase, 5);

					// Skip Animation
					key = PollKey();
					if (key == KEvent_Return || key == KEvent_Space)
					{
						break;
					}

					Refresh();
				}
				ShowAlpha(Personnal.name + " cut:", inactive, 10, 335, -50);
				ShowAlpha("-" + PersonnalAmount + "$", active, 725, 335, -50);

				// Animation
				for (int counter = 0; counter < stod(FirstAmount); counter++)
				{

					stringstream CounterStream;
					CounterStream << fixed << setprecision(2) << counter;
					string CounterStr = CounterStream.str();

					Wait(1);
					ShowImage(Background, 0, 0);

					ShowAlpha(Salary.name + ":", inactive, 10, 50, -50);
					ShowAlpha(SalaryAmount + "$", active, 725, 50, -50);

					ShowAlpha(BusCost.name + ":", inactive, 10, 110, -50);
					ShowAlpha(BusCostAmount + "$", active, 725, 110, -50);

					ShowAlpha("New " + Salary.name + ":", inactive, 10, 170, -50);
					ShowAlpha(NewSalaryAmount + "$", active, 725, 170, -50);

					ShowAlpha(Savings.name + ":", inactive, 10, 275, -50);
					ShowAlpha("-" + SavingsAmount + "$", active, 725, 275, -50);

					ShowAlpha(Personnal.name + ":", inactive, 10, 335, -50);
					ShowAlpha("-" + PersonnalAmount + "$", active, 725, 335, -50);

					ShowAlpha(FirstGoal.name + ":", inactive, 10, 395, -50);
					ShowAlpha("-" + CounterStr + "$", inactive, 725, 395, -50);

					PlayEffect(increase, 5);

					// Skip Animation
					key = PollKey();
					if (key == KEvent_Return || key == KEvent_Space)
					{
						break;
					}

					Refresh();
				}
				ShowAlpha(FirstGoal.name + " cut:", inactive, 10, 395, -50);
				ShowAlpha("-" + FirstAmount + "$", active, 725, 395, -50);

				// Animation
				for (int counter = 0; counter < stod(SecondAmount); counter++)
				{

					stringstream CounterStream;
					CounterStream << fixed << setprecision(2) << counter;
					string CounterStr = CounterStream.str();

					Wait(1);
					ShowImage(Background, 0, 0);

					ShowAlpha(Salary.name + ":", inactive, 10, 50, -50);
					ShowAlpha(SalaryAmount + "$", active, 725, 50, -50);

					ShowAlpha(BusCost.name + ":", inactive, 10, 110, -50);
					ShowAlpha(BusCostAmount + "$", active, 725, 110, -50);

					ShowAlpha("New " + Salary.name + ":", inactive, 10, 170, -50);
					ShowAlpha(NewSalaryAmount + "$", active, 725, 170, -50);

					ShowAlpha(Savings.name + ":", inactive, 10, 275, -50);
					ShowAlpha("-" + SavingsAmount + "$", active, 725, 275, -50);

					ShowAlpha(Personnal.name + ":", inactive, 10, 335, -50);
					ShowAlpha("-" + PersonnalAmount + "$", active, 725, 335, -50);

					ShowAlpha(FirstGoal.name + ":", inactive, 10, 395, -50);
					ShowAlpha("-" + FirstAmount + "$", active, 725, 395, -50);

					ShowAlpha(SecondGoal.name + ":", inactive, 10, 455, -50);
					ShowAlpha("-" + CounterStr + "$", inactive, 725, 455, -50);

					PlayEffect(increase, 5);

					// Skip Animation
					key = PollKey();
					if (key == KEvent_Return || key == KEvent_Space)
					{
						break;
					}

					Refresh();
				}
				ShowAlpha(SecondGoal.name + " cut:", inactive, 10, 455, -50);
				ShowAlpha("-" + SecondAmount + "$", active, 725, 455, -50);

				ShowImage(Background, 0, 0);

				ShowAlpha(Salary.name + ":", inactive, 10, 50, -50);
				ShowAlpha(SalaryAmount + "$", active, 725, 50, -50);

				ShowAlpha(BusCost.name + ":", inactive, 10, 110, -50);
				ShowAlpha(BusCostAmount + "$", active, 725, 110, -50);

				ShowAlpha("New " + Salary.name + ":", inactive, 10, 170, -50);
				ShowAlpha(NewSalaryAmount + "$", active, 725, 170, -50);

				ShowAlpha(Savings.name + ":", inactive, 10, 275, -50);
				ShowAlpha("-" + SavingsAmount + "$", active, 725, 275, -50);

				ShowAlpha(Personnal.name + ":", inactive, 10, 335, -50);
				ShowAlpha("-" + PersonnalAmount + "$", active, 725, 335, -50);

				ShowAlpha(FirstGoal.name + ":", inactive, 10, 395, -50);
				ShowAlpha("-" + FirstAmount + "$", active, 725, 395, -50);

				ShowAlpha(SecondGoal.name + ":", inactive, 10, 455, -50);
				ShowAlpha("-" + SecondAmount + "$", active, 725, 455, -50);

				ShowAlpha("Press any key to continue", active, 400, 750, -80);

				Refresh();

				// No do while required here, we'll just wait for the user to press a key to continue
				WaitKey();

			}
#pragma endregion

			break;

		}
#pragma endregion

	} while (choice != RemoveSalary_Remove && choice != RemoveSalary_Back);

	FreeImages();
	FreeText();
}



void RemoveSavingsMenu()
{

#pragma region VARIABLES
	// Data from .sav file
	Element Savings;
	Element FirstGoal;
	Element SecondGoal;
	Element Bus;
	string file = "Ressources\\savings.sav";
	ifstream Read(file);
	if (!Read)
	{
		string errTitle = "Couldn't open .sav file";
		string errMsg = "Unable to find file: " + file;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, errTitle.c_str(), errMsg.c_str(), NULL);
		exit(1);
	}
	Read >>
		Savings.name >> Savings.amount >>
		FirstGoal.name >> FirstGoal.amount >>
		SecondGoal.name >> SecondGoal.amount >>
		Bus.name >> Bus.amount;
	Read.close();
	// Element of the box we'll be able to edit the amount we want to Remove
	Element Amount;
	Amount.name = Savings.name;
	Amount.amount = 0;
	// Images
	ImageId Background = LoadBmp("Images\\background.bmp");
	ImageId AmountBox = LoadBmp("Images\\amountBox.bmp");
	ImageId hAmountBox = LoadBmp("Images\\hamountBox.bmp");
	ImageId aAmountBox = LoadBmp("Images\\aamountBox.bmp");
	ImageId RemoveButton = LoadBmp("Images\\Remove.bmp");
	ImageId aRemoveButton = LoadBmp("Images\\aRemove.bmp");
	ImageId QuitButton = LoadBmp("Images\\quit.bmp");
	ImageId aQuitButton = LoadBmp("Images\\aQuit.bmp");
	// Alphabets
	AlphaId inactive = LoadAlphabet("Alphabets\\inactive");
	AlphaId active = LoadAlphabet("Alphabets\\active");
	// Selection
	int cursor = RemoveSalary_Amount;
	RemoveSavings_Selection modifier = RemoveSavings_NA;
	RemoveSavings_Selection choice = RemoveSavings_NA;
	Mouse mouse;
	KEvent key = KEvent_None;
	int soundEvent = RemoveSalary_NA;
	// Hitboxes
	Area AmountHB = CreateImageArea(350, 355, AmountBox, -25);
	Area RemoveHB = CreateImageArea(725, 275, RemoveButton);
	Area QuitHB = CreateImageArea(715, 375, QuitButton);
	// Sounds
	SndId increase = LoadEffect("Sounds\\increase.wav");
	SndId navigation = LoadEffect("Sounds\\sndNavigation.wav");
	SndId confirm = LoadEffect("sounds\\sndSelect.wav");
#pragma endregion

	do
	{
		Wait(10);
#pragma region do{NAVIGATION}
		do
		{
			choice = RemoveSavings_NA;
			Wait(10);

			stringstream AmountStream;
			AmountStream << fixed << setprecision(2) << Amount.amount;
			string AmountStr = AmountStream.str();

#pragma region SHOWMENU
			ShowImage(Background, 0, 0);

			ShowAlpha(Amount.name, inactive, 550, 340, -75);

			switch (cursor)
			{
			case RemoveSavings_Amount:
				ShowImage(hAmountBox, 350, 355, -25); // Amount
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				break;
			case RemoveSavings_Remove:
				ShowImage(AmountBox, 350, 355, -25);
				ShowImage(aRemoveButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				break;
			case RemoveSavings_Quit:
				ShowImage(AmountBox, 350, 355, -25);
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(aQuitButton, 715, 375);
				break;
			}

			ShowAlpha(AmountStr, inactive, 485, 385, -55);

			Refresh();
#pragma endregion

			// Read input
			PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION
#pragma region KEYBOARD
			switch (key)
			{
			case KEvent_Down:
				if (cursor == RemoveSavings_Remove)
				{
					cursor++;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Up:
				if (cursor == RemoveSavings_Quit)
				{
					cursor--;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Right:
				if (cursor == RemoveSavings_Amount)
				{
					cursor = RemoveSavings_Remove;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Left:
				if (cursor == RemoveSavings_Quit || cursor == RemoveSavings_Remove)
				{
					cursor = RemoveSavings_Amount;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Esc:
				choice = RemoveSavings_Quit;
				PlayEffect(confirm);
				break;

			case KEvent_Space:
			case KEvent_Return:
				PlayEffect(confirm);
				switch (cursor)
				{
				case RemoveSavings_Amount:
					choice = RemoveSavings_Amount;
					break;

				case RemoveSavings_Remove:
					choice = RemoveSavings_Remove;
					break;

				case RemoveSavings_Quit:
					choice = RemoveSavings_Quit;
					break;
				}
				break;
			}
#pragma endregion

#pragma region MOUSE
			if (inArea(mouse, AmountHB))
			{
				cursor = RemoveSavings_Amount;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, RemoveHB))
			{
				cursor = RemoveSavings_Remove;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, QuitHB))
			{
				cursor = RemoveSavings_Quit;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}

			soundEvent = cursor;

			if (mouse.button == MEvent_Left &&
				(inArea(mouse, AmountHB) ||
					inArea(mouse, RemoveHB) ||
					inArea(mouse, QuitHB))
				)
			{
				switch (cursor)
				{
				case RemoveSavings_Amount:
					choice = RemoveSavings_Amount;
					break;

				case RemoveSavings_Remove:
					choice = RemoveSavings_Remove;
					break;

				case RemoveSavings_Quit:
					choice = RemoveSavings_Quit;
					break;

				}
			}
#pragma endregion
#pragma endregion

		} while (choice == RemoveSavings_NA);
#pragma endregion

		modifier = RemoveSavings_NA;

		// We clear the text
		string ValueString = "";
		// This will be used to check what the string would be after input to check if it would be valid
		string tempValue = "";
		// This will check if the user can enter the input
		bool valid = true;
		// We'll start modifying the values
		switch (choice)
		{
		case RemoveSavings_Amount:
#pragma region EDIT SALARY
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream AmountStream;
				AmountStream << fixed << setprecision(2) << Amount.amount;
				string AmountStr = AmountStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Amount.name, active, 550, 340, -75);
				ShowImage(aAmountBox, 350, 355, -25); // Amount
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(ValueString, inactive, 485, 385, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = RemoveSavings_Quit;
					break;

				case KEvent_Return:
					modifier = RemoveSavings_Remove;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_Period:
					// We check to see if there is already a period in the string and if a period is the first input
					for (int i = 0; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							valid = false;
						}
					}

					if (ValueString.size() == 0)
					{
						valid = false;
					}

					if (valid)
					{
						ValueString += ".";
					}
					// We set it back to it's original state
					valid = true;
					break;

				case KEvent_1:
					// We check if there is two decimals
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, AmountHB))
				{
					modifier = RemoveSavings_Quit;
				}
#pragma endregion

#pragma endregion

			} while (modifier != RemoveSavings_Quit && modifier != RemoveSavings_Remove);

			// If the user choses to confirm, we update the new amount
			if (modifier == RemoveSavings_Remove)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				Amount.amount = stod(ValueString);
			}
#pragma endregion
			break;

		case RemoveSavings_Remove:
#pragma region Remove
			string error;

			if (Amount.amount >= 1000000)
			{
				error = to_string(Amount.amount) + "$ is a too big amount. This program won't be able to read data over time if the amount is always that big. \n Value type: double";
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Invalid Salary", error.c_str(), NULL);
			}
			else
			{
				// We open the file to write in
				ofstream Write(file);

				Write.clear();

				// Calculate
				Savings.amount -= Amount.amount;

				// Update file
				Write <<
					Savings.name << " " << Savings.amount << endl <<
					FirstGoal.name << " " << FirstGoal.amount << endl <<
					SecondGoal.name << " " << SecondGoal.amount << endl <<
					Bus.name << " " << Bus.amount;
				Write.close();

				// Show Removeings
				// We set the precision of double numbers
				stringstream AmountStream;
				AmountStream << fixed << setprecision(2) << Amount.amount;
				string AmountStr = AmountStream.str();

				Wait(10);

				// Animation
				int counter = 0;
				for (int i = 0; i < Amount.amount; i++)
				{
					counter = i;

					stringstream CounterStream;
					CounterStream << fixed << setprecision(2) << counter;
					string CounterStr = CounterStream.str();

					Wait(1);
					ShowImage(Background, 0, 0);

					ShowAlpha(Amount.name + ":", inactive, 10, 50, -50);
					ShowAlpha("-" + CounterStr + "$", inactive, 725, 50, -50);

					PlayEffect(increase, 5);

					// Skip Animation
					key = PollKey();
					if (key == KEvent_Return || key == KEvent_Space)
					{
						break;
					}

					Refresh();
				}

				ShowImage(Background, 0, 0);

				ShowAlpha(Amount.name + ":", inactive, 10, 50, -50);
				ShowAlpha("-" + AmountStr + "$", active, 725, 50, -50);

				ShowAlpha("Press any key to continue", active, 400, 750, -80);

				Refresh();

				// No do while required here, we'll just wait for the user to press a key to continue
				WaitKey();

			}
#pragma endregion
			break;
		}

	} while (choice != RemoveSavings_Remove && choice != RemoveSavings_Quit);



	FreeText();
	FreeImages();
}



void RemoveFirstMenu()
{

#pragma region VARIABLES
	// Data from .sav file
	Element Savings;
	Element FirstGoal;
	Element SecondGoal;
	Element Bus;
	string file = "Ressources\\savings.sav";
	ifstream Read(file);
	if (!Read)
	{
		string errTitle = "Couldn't open .sav file";
		string errMsg = "Unable to find file: " + file;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, errTitle.c_str(), errMsg.c_str(), NULL);
		exit(1);
	}
	Read >>
		Savings.name >> Savings.amount >>
		FirstGoal.name >> FirstGoal.amount >>
		SecondGoal.name >> SecondGoal.amount >>
		Bus.name >> Bus.amount;
	Read.close();
	// Element of the box we'll be able to edit the amount we want to Remove
	Element Amount;
	Amount.name = FirstGoal.name;
	Amount.amount = 0;
	// Images
	ImageId Background = LoadBmp("Images\\background.bmp");
	ImageId AmountBox = LoadBmp("Images\\amountBox.bmp");
	ImageId hAmountBox = LoadBmp("Images\\hamountBox.bmp");
	ImageId aAmountBox = LoadBmp("Images\\aamountBox.bmp");
	ImageId RemoveButton = LoadBmp("Images\\Remove.bmp");
	ImageId aRemoveButton = LoadBmp("Images\\aRemove.bmp");
	ImageId QuitButton = LoadBmp("Images\\quit.bmp");
	ImageId aQuitButton = LoadBmp("Images\\aQuit.bmp");
	// Alphabets
	AlphaId inactive = LoadAlphabet("Alphabets\\inactive");
	AlphaId active = LoadAlphabet("Alphabets\\active");
	// Selection
	int cursor = RemoveSalary_Amount;
	RemoveFirst_Selection modifier = RemoveFirst_NA;
	RemoveFirst_Selection choice = RemoveFirst_NA;
	Mouse mouse;
	KEvent key = KEvent_None;
	int soundEvent = RemoveSalary_NA;
	// Hitboxes
	Area AmountHB = CreateImageArea(350, 355, AmountBox, -25);
	Area RemoveHB = CreateImageArea(725, 275, RemoveButton);
	Area QuitHB = CreateImageArea(715, 375, QuitButton);
	// Sounds
	SndId increase = LoadEffect("Sounds\\increase.wav");
	SndId navigation = LoadEffect("Sounds\\sndNavigation.wav");
	SndId confirm = LoadEffect("sounds\\sndSelect.wav");
#pragma endregion

	do
	{
		Wait(10);
#pragma region do{NAVIGATION}
		do
		{
			choice = RemoveFirst_NA;
			Wait(10);

			stringstream AmountStream;
			AmountStream << fixed << setprecision(2) << Amount.amount;
			string AmountStr = AmountStream.str();

#pragma region SHOWMENU
			ShowImage(Background, 0, 0);

			ShowAlpha(Amount.name, inactive, 550, 340, -75);

			switch (cursor)
			{
			case RemoveFirst_Amount:
				ShowImage(hAmountBox, 350, 355, -25); // Amount
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				break;
			case RemoveFirst_Remove:
				ShowImage(AmountBox, 350, 355, -25);
				ShowImage(aRemoveButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				break;
			case RemoveFirst_Quit:
				ShowImage(AmountBox, 350, 355, -25);
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(aQuitButton, 715, 375);
				break;
			}

			ShowAlpha(AmountStr, inactive, 485, 385, -55);

			Refresh();
#pragma endregion

			// Read input
			PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION
#pragma region KEYBOARD
			switch (key)
			{
			case KEvent_Down:
				if (cursor == RemoveFirst_Remove)
				{
					cursor++;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Up:
				if (cursor == RemoveFirst_Quit)
				{
					cursor--;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Right:
				if (cursor == RemoveFirst_Amount)
				{
					cursor = RemoveFirst_Remove;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Left:
				if (cursor == RemoveFirst_Quit || cursor == RemoveFirst_Remove)
				{
					cursor = RemoveFirst_Amount;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Esc:
				choice = RemoveFirst_Quit;
				PlayEffect(confirm);
				break;

			case KEvent_Space:
			case KEvent_Return:
				PlayEffect(confirm);
				switch (cursor)
				{
				case RemoveFirst_Amount:
					choice = RemoveFirst_Amount;
					break;

				case RemoveFirst_Remove:
					choice = RemoveFirst_Remove;
					break;

				case RemoveFirst_Quit:
					choice = RemoveFirst_Quit;
					break;
				}
				break;
			}
#pragma endregion

#pragma region MOUSE
			if (inArea(mouse, AmountHB))
			{
				cursor = RemoveFirst_Amount;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, RemoveHB))
			{
				cursor = RemoveFirst_Remove;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, QuitHB))
			{
				cursor = RemoveFirst_Quit;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}

			soundEvent = cursor;

			if (mouse.button == MEvent_Left &&
				(inArea(mouse, AmountHB) ||
					inArea(mouse, RemoveHB) ||
					inArea(mouse, QuitHB))
				)
			{
				switch (cursor)
				{
				case RemoveFirst_Amount:
					choice = RemoveFirst_Amount;
					break;

				case RemoveFirst_Remove:
					choice = RemoveFirst_Remove;
					break;

				case RemoveFirst_Quit:
					choice = RemoveFirst_Quit;
					break;

				}
			}
#pragma endregion
#pragma endregion

		} while (choice == RemoveFirst_NA);
#pragma endregion

		modifier = RemoveFirst_NA;

		// We clear the text
		string ValueString = "";
		// This will be used to check what the string would be after input to check if it would be valid
		string tempValue = "";
		// This will check if the user can enter the input
		bool valid = true;
		// We'll start modifying the values
		switch (choice)
		{
		case RemoveFirst_Amount:
#pragma region EDIT SALARY
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream AmountStream;
				AmountStream << fixed << setprecision(2) << Amount.amount;
				string AmountStr = AmountStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Amount.name, active, 550, 340, -75);
				ShowImage(aAmountBox, 350, 355, -25); // Amount
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(ValueString, inactive, 485, 385, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = RemoveFirst_Quit;
					break;

				case KEvent_Return:
					modifier = RemoveFirst_Remove;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_Period:
					// We check to see if there is already a period in the string and if a period is the first input
					for (int i = 0; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							valid = false;
						}
					}

					if (ValueString.size() == 0)
					{
						valid = false;
					}

					if (valid)
					{
						ValueString += ".";
					}
					// We set it back to it's original state
					valid = true;
					break;

				case KEvent_1:
					// We check if there is two decimals
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, AmountHB))
				{
					modifier = RemoveFirst_Quit;
				}
#pragma endregion

#pragma endregion

			} while (modifier != RemoveFirst_Quit && modifier != RemoveFirst_Remove);

			// If the user choses to confirm, we update the new amount
			if (modifier == RemoveFirst_Remove)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				Amount.amount = stod(ValueString);
			}
#pragma endregion
			break;

		case RemoveFirst_Remove:
#pragma region Remove
			string error;

			if (Amount.amount >= 1000000)
			{
				error = to_string(Amount.amount) + "$ is a too big amount. This program won't be able to read data over time if the amount is always that big. \n Value type: double";
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Invalid Salary", error.c_str(), NULL);
			}
			else
			{
				// We open the file to write in
				ofstream Write(file);

				Write.clear();

				// Calculate
				FirstGoal.amount -= Amount.amount;

				// Update file
				Write <<
					Savings.name << " " << Savings.amount << endl <<
					FirstGoal.name << " " << FirstGoal.amount << endl <<
					SecondGoal.name << " " << SecondGoal.amount << endl <<
					Bus.name << " " << Bus.amount;
				Write.close();

				// Show Removeings
				// We set the precision of double numbers
				stringstream AmountStream;
				AmountStream << fixed << setprecision(2) << Amount.amount;
				string AmountStr = AmountStream.str();

				Wait(10);

				// Animation
				int counter = 0;
				for (int i = 0; i < Amount.amount; i++)
				{
					counter = i;

					stringstream CounterStream;
					CounterStream << fixed << setprecision(2) << counter;
					string CounterStr = CounterStream.str();

					Wait(1);
					ShowImage(Background, 0, 0);

					ShowAlpha(Amount.name + ":", inactive, 10, 50, -50);
					ShowAlpha("-" + CounterStr + "$", inactive, 725, 50, -50);

					PlayEffect(increase, 5);

					// Skip Animation
					key = PollKey();
					if (key == KEvent_Return || key == KEvent_Space)
					{
						break;
					}

					Refresh();
				}

				ShowImage(Background, 0, 0);

				ShowAlpha(Amount.name + ":", inactive, 10, 50, -50);
				ShowAlpha("-" + AmountStr + "$", active, 725, 50, -50);

				ShowAlpha("Press any key to continue", active, 400, 750, -80);

				Refresh();

				// No do while required here, we'll just wait for the user to press a key to continue
				WaitKey();

			}
#pragma endregion
			break;
		}

	} while (choice != RemoveFirst_Remove && choice != RemoveFirst_Quit);



	FreeText();
	FreeImages();
}



void RemoveSecondMenu()
{

#pragma region VARIABLES
	// Data from .sav file
	Element Savings;
	Element FirstGoal;
	Element SecondGoal;
	Element Bus;
	string file = "Ressources\\savings.sav";
	ifstream Read(file);
	if (!Read)
	{
		string errTitle = "Couldn't open .sav file";
		string errMsg = "Unable to find file: " + file;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, errTitle.c_str(), errMsg.c_str(), NULL);
		exit(1);
	}
	Read >>
		Savings.name >> Savings.amount >>
		FirstGoal.name >> FirstGoal.amount >>
		SecondGoal.name >> SecondGoal.amount >>
		Bus.name >> Bus.amount;
	Read.close();
	// Element of the box we'll be able to edit the amount we want to Remove
	Element Amount;
	Amount.name = SecondGoal.name;
	Amount.amount = 0;
	// Images
	ImageId Background = LoadBmp("Images\\background.bmp");
	ImageId AmountBox = LoadBmp("Images\\amountBox.bmp");
	ImageId hAmountBox = LoadBmp("Images\\hamountBox.bmp");
	ImageId aAmountBox = LoadBmp("Images\\aamountBox.bmp");
	ImageId RemoveButton = LoadBmp("Images\\Remove.bmp");
	ImageId aRemoveButton = LoadBmp("Images\\aRemove.bmp");
	ImageId QuitButton = LoadBmp("Images\\quit.bmp");
	ImageId aQuitButton = LoadBmp("Images\\aQuit.bmp");
	// Alphabets
	AlphaId inactive = LoadAlphabet("Alphabets\\inactive");
	AlphaId active = LoadAlphabet("Alphabets\\active");
	// Selection
	int cursor = RemoveSalary_Amount;
	RemoveSecond_Selection modifier = RemoveSecond_NA;
	RemoveSecond_Selection choice = RemoveSecond_NA;
	Mouse mouse;
	KEvent key = KEvent_None;
	int soundEvent = RemoveSalary_NA;
	// Hitboxes
	Area AmountHB = CreateImageArea(350, 355, AmountBox, -25);
	Area RemoveHB = CreateImageArea(725, 275, RemoveButton);
	Area QuitHB = CreateImageArea(715, 375, QuitButton);
	// Sounds
	SndId increase = LoadEffect("Sounds\\increase.wav");
	SndId navigation = LoadEffect("Sounds\\sndNavigation.wav");
	SndId confirm = LoadEffect("sounds\\sndSelect.wav");
#pragma endregion

	do
	{
		Wait(10);
#pragma region do{NAVIGATION}
		do
		{
			choice = RemoveSecond_NA;
			Wait(10);

			stringstream AmountStream;
			AmountStream << fixed << setprecision(2) << Amount.amount;
			string AmountStr = AmountStream.str();

#pragma region SHOWMENU
			ShowImage(Background, 0, 0);

			ShowAlpha(Amount.name, inactive, 550, 340, -75);

			switch (cursor)
			{
			case RemoveSecond_Amount:
				ShowImage(hAmountBox, 350, 355, -25); // Amount
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				break;
			case RemoveSecond_Remove:
				ShowImage(AmountBox, 350, 355, -25);
				ShowImage(aRemoveButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				break;
			case RemoveSecond_Quit:
				ShowImage(AmountBox, 350, 355, -25);
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(aQuitButton, 715, 375);
				break;
			}

			ShowAlpha(AmountStr, inactive, 485, 385, -55);

			Refresh();
#pragma endregion

			// Read input
			PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION
#pragma region KEYBOARD
			switch (key)
			{
			case KEvent_Down:
				if (cursor == RemoveSecond_Remove)
				{
					cursor++;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Up:
				if (cursor == RemoveSecond_Quit)
				{
					cursor--;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Right:
				if (cursor == RemoveSecond_Amount)
				{
					cursor = RemoveSecond_Remove;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Left:
				if (cursor == RemoveSecond_Quit || cursor == RemoveSecond_Remove)
				{
					cursor = RemoveSecond_Amount;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Esc:
				choice = RemoveSecond_Quit;
				PlayEffect(confirm);
				break;

			case KEvent_Space:
			case KEvent_Return:
				PlayEffect(confirm);
				switch (cursor)
				{
				case RemoveSecond_Amount:
					choice = RemoveSecond_Amount;
					break;

				case RemoveSecond_Remove:
					choice = RemoveSecond_Remove;
					break;

				case RemoveSecond_Quit:
					choice = RemoveSecond_Quit;
					break;
				}
				break;
			}
#pragma endregion

#pragma region MOUSE
			if (inArea(mouse, AmountHB))
			{
				cursor = RemoveSecond_Amount;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, RemoveHB))
			{
				cursor = RemoveSecond_Remove;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, QuitHB))
			{
				cursor = RemoveSecond_Quit;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}

			soundEvent = cursor;

			if (mouse.button == MEvent_Left &&
				(inArea(mouse, AmountHB) ||
					inArea(mouse, RemoveHB) ||
					inArea(mouse, QuitHB))
				)
			{
				switch (cursor)
				{
				case RemoveSecond_Amount:
					choice = RemoveSecond_Amount;
					break;

				case RemoveSecond_Remove:
					choice = RemoveSecond_Remove;
					break;

				case RemoveSecond_Quit:
					choice = RemoveSecond_Quit;
					break;

				}
			}
#pragma endregion
#pragma endregion

		} while (choice == RemoveSecond_NA);
#pragma endregion

		modifier = RemoveSecond_NA;

		// We clear the text
		string ValueString = "";
		// This will be used to check what the string would be after input to check if it would be valid
		string tempValue = "";
		// This will check if the user can enter the input
		bool valid = true;
		// We'll start modifying the values
		switch (choice)
		{
		case RemoveSecond_Amount:
#pragma region EDIT SALARY
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream AmountStream;
				AmountStream << fixed << setprecision(2) << Amount.amount;
				string AmountStr = AmountStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Amount.name, active, 550, 340, -75);
				ShowImage(aAmountBox, 350, 355, -25); // Amount
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(ValueString, inactive, 485, 385, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = RemoveSecond_Quit;
					break;

				case KEvent_Return:
					modifier = RemoveSecond_Remove;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_Period:
					// We check to see if there is already a period in the string and if a period is the first input
					for (int i = 0; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							valid = false;
						}
					}

					if (ValueString.size() == 0)
					{
						valid = false;
					}

					if (valid)
					{
						ValueString += ".";
					}
					// We set it back to it's original state
					valid = true;
					break;

				case KEvent_1:
					// We check if there is two decimals
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, AmountHB))
				{
					modifier = RemoveSecond_Quit;
				}
#pragma endregion

#pragma endregion

			} while (modifier != RemoveSecond_Quit && modifier != RemoveSecond_Remove);

			// If the user choses to confirm, we update the new amount
			if (modifier == RemoveSecond_Remove)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				Amount.amount = stod(ValueString);
			}
#pragma endregion
			break;

		case RemoveSecond_Remove:
#pragma region Remove
			string error;

			if (Amount.amount >= 1000000)
			{
				error = to_string(Amount.amount) + "$ is a too big amount. This program won't be able to read data over time if the amount is always that big. \n Value type: double";
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Invalid Salary", error.c_str(), NULL);
			}
			else
			{
				// We open the file to write in
				ofstream Write(file);

				Write.clear();

				// Calculate
				SecondGoal.amount -= Amount.amount;

				// Update file
				Write <<
					Savings.name << " " << Savings.amount << endl <<
					FirstGoal.name << " " << FirstGoal.amount << endl <<
					SecondGoal.name << " " << SecondGoal.amount << endl <<
					Bus.name << " " << Bus.amount;
				Write.close();

				// Show Removeings
				// We set the precision of double numbers
				stringstream AmountStream;
				AmountStream << fixed << setprecision(2) << Amount.amount;
				string AmountStr = AmountStream.str();

				Wait(10);

				// Animation
				int counter = 0;
				for (int i = 0; i < Amount.amount; i++)
				{
					counter = i;

					stringstream CounterStream;
					CounterStream << fixed << setprecision(2) << counter;
					string CounterStr = CounterStream.str();

					Wait(1);
					ShowImage(Background, 0, 0);

					ShowAlpha(Amount.name + ":", inactive, 10, 50, -50);
					ShowAlpha("-" + CounterStr + "$", inactive, 725, 50, -50);

					PlayEffect(increase, 5);

					// Skip Animation
					key = PollKey();
					if (key == KEvent_Return || key == KEvent_Space)
					{
						break;
					}

					Refresh();
				}

				ShowImage(Background, 0, 0);

				ShowAlpha(Amount.name + ":", inactive, 10, 50, -50);
				ShowAlpha("-" + AmountStr + "$", active, 725, 50, -50);

				ShowAlpha("Press any key to continue", active, 400, 750, -80);

				Refresh();

				// No do while required here, we'll just wait for the user to press a key to continue
				WaitKey();

			}
#pragma endregion
			break;
		}

	} while (choice != RemoveSecond_Remove && choice != RemoveSecond_Quit);



	FreeText();
	FreeImages();
}



void RemoveBusMenu()
{

#pragma region VARIABLES
	// Data from .sav file
	Element Savings;
	Element FirstGoal;
	Element SecondGoal;
	Element Bus;
	string file = "Ressources\\savings.sav";
	ifstream Read(file);
	if (!Read)
	{
		string errTitle = "Couldn't open .sav file";
		string errMsg = "Unable to find file: " + file;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, errTitle.c_str(), errMsg.c_str(), NULL);
		exit(1);
	}
	Read >>
		Savings.name >> Savings.amount >>
		FirstGoal.name >> FirstGoal.amount >>
		SecondGoal.name >> SecondGoal.amount >>
		Bus.name >> Bus.amount;
	Read.close();
	// Element of the box we'll be able to edit the amount we want to Remove
	Element Amount;
	Amount.name = Bus.name;
	Amount.amount = 0;
	// Images
	ImageId Background = LoadBmp("Images\\background.bmp");
	ImageId AmountBox = LoadBmp("Images\\amountBox.bmp");
	ImageId hAmountBox = LoadBmp("Images\\hamountBox.bmp");
	ImageId aAmountBox = LoadBmp("Images\\aamountBox.bmp");
	ImageId RemoveButton = LoadBmp("Images\\Remove.bmp");
	ImageId aRemoveButton = LoadBmp("Images\\aRemove.bmp");
	ImageId QuitButton = LoadBmp("Images\\quit.bmp");
	ImageId aQuitButton = LoadBmp("Images\\aQuit.bmp");
	// Alphabets
	AlphaId inactive = LoadAlphabet("Alphabets\\inactive");
	AlphaId active = LoadAlphabet("Alphabets\\active");
	// Selection
	int cursor = RemoveSalary_Amount;
	RemoveBus_Selection modifier = RemoveBus_NA;
	RemoveBus_Selection choice = RemoveBus_NA;
	Mouse mouse;
	KEvent key = KEvent_None;
	int soundEvent = RemoveSalary_NA;
	// Hitboxes
	Area AmountHB = CreateImageArea(350, 355, AmountBox, -25);
	Area RemoveHB = CreateImageArea(725, 275, RemoveButton);
	Area QuitHB = CreateImageArea(715, 375, QuitButton);
	// Sounds
	SndId increase = LoadEffect("Sounds\\increase.wav");
	SndId navigation = LoadEffect("Sounds\\sndNavigation.wav");
	SndId confirm = LoadEffect("sounds\\sndSelect.wav");
#pragma endregion

	do
	{
		Wait(10);
#pragma region do{NAVIGATION}
		do
		{
			choice = RemoveBus_NA;
			Wait(10);

			stringstream AmountStream;
			AmountStream << fixed << setprecision(2) << Amount.amount;
			string AmountStr = AmountStream.str();

#pragma region SHOWMENU
			ShowImage(Background, 0, 0);

			ShowAlpha(Amount.name, inactive, 550, 340, -75);

			switch (cursor)
			{
			case RemoveBus_Amount:
				ShowImage(hAmountBox, 350, 355, -25); // Amount
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				break;
			case RemoveBus_Remove:
				ShowImage(AmountBox, 350, 355, -25);
				ShowImage(aRemoveButton, 725, 275);
				ShowImage(QuitButton, 715, 375);
				break;
			case RemoveBus_Quit:
				ShowImage(AmountBox, 350, 355, -25);
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(aQuitButton, 715, 375);
				break;
			}

			ShowAlpha(AmountStr, inactive, 485, 385, -55);

			Refresh();
#pragma endregion

			// Read input
			PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION
#pragma region KEYBOARD
			switch (key)
			{
			case KEvent_Down:
				if (cursor == RemoveBus_Remove)
				{
					cursor++;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Up:
				if (cursor == RemoveBus_Quit)
				{
					cursor--;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Right:
				if (cursor == RemoveBus_Amount)
				{
					cursor = RemoveBus_Remove;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Left:
				if (cursor == RemoveBus_Quit || cursor == RemoveBus_Remove)
				{
					cursor = RemoveBus_Amount;
					PlayEffect(navigation);
				}
				break;

			case KEvent_Esc:
				choice = RemoveBus_Quit;
				PlayEffect(confirm);
				break;

			case KEvent_Space:
			case KEvent_Return:
				PlayEffect(confirm);
				switch (cursor)
				{
				case RemoveBus_Amount:
					choice = RemoveBus_Amount;
					break;

				case RemoveBus_Remove:
					choice = RemoveBus_Remove;
					break;

				case RemoveBus_Quit:
					choice = RemoveBus_Quit;
					break;
				}
				break;
			}
#pragma endregion

#pragma region MOUSE
			if (inArea(mouse, AmountHB))
			{
				cursor = RemoveBus_Amount;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, RemoveHB))
			{
				cursor = RemoveBus_Remove;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}
			else if (inArea(mouse, QuitHB))
			{
				cursor = RemoveBus_Quit;
				if (soundEvent != cursor)
				{
					PlayEffect(navigation);
				}
			}

			soundEvent = cursor;

			if (mouse.button == MEvent_Left &&
				(inArea(mouse, AmountHB) ||
					inArea(mouse, RemoveHB) ||
					inArea(mouse, QuitHB))
				)
			{
				switch (cursor)
				{
				case RemoveBus_Amount:
					choice = RemoveBus_Amount;
					break;

				case RemoveBus_Remove:
					choice = RemoveBus_Remove;
					break;

				case RemoveBus_Quit:
					choice = RemoveBus_Quit;
					break;

				}
			}
#pragma endregion
#pragma endregion

		} while (choice == RemoveBus_NA);
#pragma endregion

		modifier = RemoveBus_NA;

		// We clear the text
		string ValueString = "";
		// This will be used to check what the string would be after input to check if it would be valid
		string tempValue = "";
		// This will check if the user can enter the input
		bool valid = true;
		// We'll start modifying the values
		switch (choice)
		{
		case RemoveBus_Amount:
#pragma region EDIT SALARY
			do
			{
#pragma region SHOW MENU
				Wait(10);
				// Keep showing the menu, but it will differ just a bit
				stringstream AmountStream;
				AmountStream << fixed << setprecision(2) << Amount.amount;
				string AmountStr = AmountStream.str();
				ShowImage(Background, 0, 0);
				ShowAlpha(Amount.name, active, 550, 340, -75);
				ShowImage(aAmountBox, 350, 355, -25); // Amount
				ShowImage(RemoveButton, 760, 300, -20);
				ShowImage(QuitButton, 715, 375);
				ShowAlpha(ValueString, inactive, 485, 385, -55);
				Refresh();
#pragma endregion

				// Read input
				PollMouseAndKeyboard(mouse, key);

#pragma region INPUT ACTION

#pragma region KEYBOARD
				switch (key)
				{
				case KEvent_Esc:
					modifier = RemoveBus_Quit;
					break;

				case KEvent_Return:
					modifier = RemoveBus_Remove;
					break;

				case KEvent_Backspace:
					ValueString.clear();
					break;

				case KEvent_Period:
					// We check to see if there is already a period in the string and if a period is the first input
					for (int i = 0; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							valid = false;
						}
					}

					if (ValueString.size() == 0)
					{
						valid = false;
					}

					if (valid)
					{
						ValueString += ".";
					}
					// We set it back to it's original state
					valid = true;
					break;

				case KEvent_1:
					// We check if there is two decimals
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "1";
					}
					valid = true;
					break;

				case KEvent_2:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "2";
					}
					valid = true;
					break;

				case KEvent_3:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "3";
					}
					valid = true;
					break;

				case KEvent_4:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "4";
					}
					valid = true;
					break;

				case KEvent_5:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "5";
					}
					valid = true;
					break;

				case KEvent_6:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "6";
					}
					valid = true;
					break;

				case KEvent_7:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "7";
					}
					valid = true;
					break;

				case KEvent_8:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "8";
					}
					valid = true;
					break;

				case KEvent_9:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "9";
					}
					valid = true;
					break;

				case KEvent_0:
					for (int i = 0, flag; i < ValueString.size(); ++i)
					{
						if (ValueString[i] == '.')
						{
							flag = i + 1;
							if (ValueString.size() - flag == 2)
							{
								valid = false;
							}
						}
					}
					if (valid)
					{
						ValueString += "0";
					}
					valid = true;
					break;
				}
#pragma endregion

#pragma region MOUSE
				if (mouse.button == MEvent_Left && !inArea(mouse, AmountHB))
				{
					modifier = RemoveBus_Quit;
				}
#pragma endregion

#pragma endregion

			} while (modifier != RemoveBus_Quit && modifier != RemoveBus_Remove);

			// If the user choses to confirm, we update the new amount
			if (modifier == RemoveBus_Remove)
			{
				// If the string is empty, we set it to 0 by default
				if (ValueString == "")
				{
					ValueString = "0";
				}
				Amount.amount = stod(ValueString);
			}
#pragma endregion
			break;

		case RemoveBus_Remove:
#pragma region Remove
			string error;

			if (Amount.amount >= 1000000)
			{
				error = to_string(Amount.amount) + "$ is a too big amount. This program won't be able to read data over time if the amount is always that big. \n Value type: double";
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Invalid Salary", error.c_str(), NULL);
			}
			else
			{
				// We open the file to write in
				ofstream Write(file);

				Write.clear();

				// Calculate
				Bus.amount -= Amount.amount;

				// Update file
				Write <<
					Savings.name << " " << Savings.amount << endl <<
					FirstGoal.name << " " << FirstGoal.amount << endl <<
					SecondGoal.name << " " << SecondGoal.amount << endl <<
					Bus.name << " " << Bus.amount;
				Write.close();

				// Show Removeings
				// We set the precision of double numbers
				stringstream AmountStream;
				AmountStream << fixed << setprecision(2) << Amount.amount;
				string AmountStr = AmountStream.str();

				Wait(10);

				// Animation
				int counter = 0;
				for (int i = 0; i < Amount.amount; i++)
				{
					counter = i;

					stringstream CounterStream;
					CounterStream << fixed << setprecision(2) << counter;
					string CounterStr = CounterStream.str();

					Wait(1);
					ShowImage(Background, 0, 0);

					ShowAlpha(Amount.name + ":", inactive, 10, 50, -50);
					ShowAlpha("-" + CounterStr + "$", inactive, 725, 50, -50);

					PlayEffect(increase, 5);

					// Skip Animation
					key = PollKey();
					if (key == KEvent_Return || key == KEvent_Space)
					{
						break;
					}

					Refresh();
				}

				ShowImage(Background, 0, 0);

				ShowAlpha(Amount.name + ":", inactive, 10, 50, -50);
				ShowAlpha("-" + AmountStr + "$", active, 725, 50, -50);

				ShowAlpha("Press any key to continue", active, 400, 750, -80);

				Refresh();

				// No do while required here, we'll just wait for the user to press a key to continue
				WaitKey();

			}
#pragma endregion
			break;
		}

	} while (choice != RemoveBus_Remove && choice != RemoveBus_Quit);



	FreeText();
	FreeImages();
}