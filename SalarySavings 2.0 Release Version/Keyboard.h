#ifndef KEYBOARD_H
#define KEYBOARD_H
/////////////////////////////////////////////////////////////////////////////
// Keyboard.h																   
// MaxiJonson															   
// December 15 2016														   
// Contains all keyboard related functions and elements					   	
/////////////////////////////////////////////////////////////////////////////
#include <SDL.h>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Events linked to the keyboard
///////////////////////////////////////////////////////////////////////////////
enum KEvent
{
	KEvent_None,

	KEvent_Up, KEvent_Left, KEvent_Down, KEvent_Right,

	KEvent_1, KEvent_2, KEvent_3, KEvent_4, KEvent_5, KEvent_6, KEvent_7, KEvent_8, KEvent_9, KEvent_0,
	KEvent_Num1, KEvent_Num2, KEvent_Num3, KEvent_Num4, KEvent_Num5, KEvent_Num6, KEvent_Num7, KEvent_Num8, KEvent_Num9, KEvent_Num0,

	KEvent_q, KEvent_w, KEvent_e, KEvent_r, KEvent_t, KEvent_y, KEvent_u, KEvent_i, KEvent_o, KEvent_p,
	KEvent_a, KEvent_s, KEvent_d, KEvent_f, KEvent_g, KEvent_h, KEvent_j, KEvent_k, KEvent_l,
	KEvent_z, KEvent_x, KEvent_c, KEvent_v, KEvent_b, KEvent_n, KEvent_m,

	KEvent_Space, KEvent_Esc, KEvent_Return, KEvent_Delete, KEvent_Backspace, KEvent_Period
};


///////////////////////////////////////////////////////////////////////////////
// WaitKey
// Waits for the user to do a keyboard input
// Params :	None
// Returns: Key input
///////////////////////////////////////////////////////////////////////////////
KEvent WaitKey();


///////////////////////////////////////////////////////////////////////////////
// PollKey
// Checks if user has made a keyboard input and keeps going if not
// Params :	None
// Returns: Key input or No input
///////////////////////////////////////////////////////////////////////////////
KEvent PollKey();


#endif // !KEYBOARD_H