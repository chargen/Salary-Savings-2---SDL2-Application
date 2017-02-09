/////////////////////////////////////////////////////////////////////////////
// Mouse.cpp																   
// MaxiJonson															   
// December 15 2016														   
// Contains all mouse related functions and elements					   	
/////////////////////////////////////////////////////////////////////////////
#include "Mouse.h"

void PollMouse(Mouse& mouse)
{
	mouse.button = MEvent_None;
	SDL_Event mouseInput;

	SDL_PollEvent(&mouseInput);

	switch (mouseInput.type)
	{
	case SDL_MOUSEBUTTONDOWN: // When it is pressed
		switch (mouseInput.button.button) // This checks a button on the mouse
		{
		case SDL_BUTTON_LEFT: // Left Button

			switch (mouseInput.button.clicks) // This checks the amount of time it has been clicked within a reasonable amount of time (ex: double clicks and triple clicks)
			{
			case 1: // Single Click
				mouse.button = MEvent_Left;
				break;

			case 2: // Double Click
				mouse.button = MEvent_Double_Left;
				break;

			case 3: // Triple Click
				mouse.button = MEvent_Triple_Left;
				break;
			}
			break;

		case SDL_BUTTON_RIGHT: // Right Button
			switch (mouseInput.button.clicks) // This checks the amount of time it has been clicked within a reasonable amount of time (ex: double clicks and triple clicks)
			{
			case 1: // Single Click
				mouse.button = MEvent_Right;
				break;

			case 2: // Double Click
				mouse.button = MEvent_Double_Right;
				break;

			case 3: // Triple Click
				mouse.button = MEvent_Triple_Right;
				break;
			}
			break;
		}
		break;

	case SDL_MOUSEMOTION:
		// This will update the cursor's position everytime the mouse is moved
		mouse.position.x = mouseInput.button.x;
		mouse.position.y = mouseInput.button.y;
		break;

	}
}



bool inArea(Mouse cursor, Area zone)
{
	bool inArea = false;

	// Pretty simple, checks if the cursor is between the start and end coordinates
	if ((cursor.position.x >= zone.start.x &&
		cursor.position.y >= zone.start.y) &&
		(cursor.position.x <= zone.end.x &&
		cursor.position.y <= zone.end.y))
	{
		inArea = true;
	}

	return inArea;
}



Area CreateImageArea(int posX, int posY, ImageId image, int resize)
{
	SDL_Rect position = GetImageRect(image, resize);
	Area zone;

	zone.start.x = posX;
	zone.start.y = posY;
	zone.end.x = posX + position.w;
	zone.end.y = posY + position.h;

	return zone;
}