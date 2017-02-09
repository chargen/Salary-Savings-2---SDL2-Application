/////////////////////////////////////////////////////////////////////////////
// KeyMouse.cpp																   
// MaxiJonson															   
// December 22 2016														   
// Groups functions that use both the Keyboard and Mouse module					   	
/////////////////////////////////////////////////////////////////////////////
#include "KeyMouse.h"

// As you can see, this function just groups PollKey and PollMouse in one. Of course, some very minor modifications to make sure everything is compatible with both.
void PollMouseAndKeyboard(Mouse& mouse, KEvent& key)
{
	key = KEvent_None;
	mouse.button = MEvent_None;
	SDL_Event input;

	SDL_PollEvent(&input);

	switch (input.type)
	{
	case SDL_QUIT:
		key = KEvent_Esc;
		break;
	case SDL_KEYDOWN:
		switch (input.key.keysym.sym)
		{

#pragma region ARROWS
			/// ARROWS ///
		case SDLK_UP:
			key = KEvent_Up;
			break;
		case SDLK_DOWN:
			key = KEvent_Down;
			break;
		case SDLK_RIGHT:
			key = KEvent_Right;
			break;
		case SDLK_LEFT:
			key = KEvent_Left;
			break;
#pragma endregion

#pragma region COMMANDS
			/// COMMANDS ///
		case SDLK_ESCAPE:
			key = KEvent_Esc;
			break;
		case SDLK_SPACE:
			key = KEvent_Space;
			break;
		case SDLK_RETURN:
			key = KEvent_Return;
			break;
		case SDLK_DELETE: // Not all computers have this key
			key = KEvent_Delete;
			break;
		case SDLK_BACKSPACE:
			key = KEvent_Backspace;
			break;
		case SDLK_PERIOD:
			key = KEvent_Period;
			break;
#pragma endregion

#pragma region NUMBERS
			/// NUMBERS ///
			// Watch out! Not all computers have numpads!
		case SDLK_KP_1:
			key = KEvent_Num1;
			break;
		case SDLK_KP_2:
			key = KEvent_Num2;
			break;
		case SDLK_KP_3:
			key = KEvent_Num3;
			break;
		case SDLK_KP_4:
			key = KEvent_Num4;
			break;
		case SDLK_KP_5:
			key = KEvent_Num5;
			break;
		case SDLK_KP_6:
			key = KEvent_Num6;
			break;
		case SDLK_KP_7:
			key = KEvent_Num7;
			break;
		case SDLK_KP_8:
			key = KEvent_Num8;
			break;
		case SDLK_KP_9:
			key = KEvent_Num9;
			break;
		case SDLK_KP_0:
			key = KEvent_Num0;
			break;
		case SDLK_1:
			key = KEvent_1;
			break;
		case SDLK_2:
			key = KEvent_2;
			break;
		case SDLK_3:
			key = KEvent_3;
			break;
		case SDLK_4:
			key = KEvent_4;
			break;
		case SDLK_5:
			key = KEvent_5;
			break;
		case SDLK_6:
			key = KEvent_6;
			break;
		case SDLK_7:
			key = KEvent_7;
			break;
		case SDLK_8:
			key = KEvent_8;
			break;
		case SDLK_9:
			key = KEvent_9;
			break;
		case SDLK_0:
			key = KEvent_0;
			break;
#pragma endregion

#pragma region LETTERS
			/// LETTERS ///
		case SDLK_q:
			key = KEvent_q;
			break;
		case SDLK_w:
			key = KEvent_w;
			break;
		case SDLK_e:
			key = KEvent_e;
			break;
		case SDLK_r:
			key = KEvent_r;
			break;
		case SDLK_t:
			key = KEvent_t;
			break;
		case SDLK_y:
			key = KEvent_y;
			break;
		case SDLK_u:
			key = KEvent_u;
			break;
		case SDLK_i:
			key = KEvent_i;
			break;
		case SDLK_o:
			key = KEvent_o;
			break;
		case SDLK_p:
			key = KEvent_p;
			break;
		case SDLK_a:
			key = KEvent_a;
			break;
		case SDLK_s:
			key = KEvent_s;
			break;
		case SDLK_d:
			key = KEvent_d;
			break;
		case SDLK_f:
			key = KEvent_f;
			break;
		case SDLK_g:
			key = KEvent_g;
			break;
		case SDLK_h:
			key = KEvent_h;
			break;
		case SDLK_j:
			key = KEvent_j;
			break;
		case SDLK_k:
			key = KEvent_k;
			break;
		case SDLK_l:
			key = KEvent_l;
			break;
		case SDLK_z:
			key = KEvent_z;
			break;
		case SDLK_x:
			key = KEvent_x;
			break;
		case SDLK_c:
			key = KEvent_c;
			break;
		case SDLK_v:
			key = KEvent_v;
			break;
		case SDLK_b:
			key = KEvent_b;
			break;
		case SDLK_n:
			key = KEvent_n;
			break;
		case SDLK_m:
			key = KEvent_m;
			break;
#pragma endregion

		}
		break;

	case SDL_MOUSEBUTTONDOWN: // When it is pressed
		switch (input.button.button) // This checks a button on the mouse
		{
		case SDL_BUTTON_LEFT: // Left Button

			switch (input.button.clicks) // This checks the amount of time it has been clicked within a reasonable amount of time (ex: double clicks and triple clicks)
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
			switch (input.button.clicks) // This checks the amount of time it has been clicked within a reasonable amount of time (ex: double clicks and triple clicks)
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
		mouse.position.x = input.button.x;
		mouse.position.y = input.button.y;
		break;
	}
}