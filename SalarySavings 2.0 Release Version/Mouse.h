#ifndef MOUSE_H
#define MOUSE_H
/////////////////////////////////////////////////////////////////////////////
// Mouse.h																   
// MaxiJonson															   
// December 15 2016														   
// Contains all mouse related functions and elements					   	
/////////////////////////////////////////////////////////////////////////////
#include <SDL.h>
#include "Display.h"
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// Defines pressed buttons				   	
/////////////////////////////////////////////////////////////////////////////
enum MEvent // Mouse event
{
	MEvent_None, 
	
	MEvent_Left, MEvent_Right,
	MEvent_Double_Left, MEvent_Double_Right,
	MEvent_Triple_Left, MEvent_Triple_Right

};

/////////////////////////////////////////////////////////////////////////////
// Defines x and y coordinates			   	
/////////////////////////////////////////////////////////////////////////////
struct Coordinates
{
	int x; // X coordinates
	int y; // Y coordinates
};

/////////////////////////////////////////////////////////////////////////////
// Defines mouse info such as position and button pressed			   	
/////////////////////////////////////////////////////////////////////////////
struct Mouse // Mouse info
{
	Coordinates position; // Cursor position
	MEvent button; // Button pressed
};

/////////////////////////////////////////////////////////////////////////////
// Defines defines an area (for mouse position checks)
// Note: as a convention and in order to make other functions work the right
// way, start is the closest to the origin (0,0) and end is the furthest. Example:
// *Start Point*
//
//
//															*End Point*
/////////////////////////////////////////////////////////////////////////////
struct Area
{
	Coordinates start; // First point of the area
	Coordinates end; // Second point of the area
};



///////////////////////////////////////////////////////////////////////////////
// PollMouse
// Checks if user has made a mouse input and keeps going if not
// Params :	Mouse to poll
// Returns: None
///////////////////////////////////////////////////////////////////////////////
void PollMouse(Mouse& mouse);



///////////////////////////////////////////////////////////////////////////////
// inArea
// Checks if cursor is in a certain zone
// Params : Mouse
//			Zone to check
// Returns: in the zone or not
///////////////////////////////////////////////////////////////////////////////
bool inArea(Mouse cursor, Area zone);



///////////////////////////////////////////////////////////////////////////////
// CreateImageArea
// Creates an Area based on an image rectangle
// Params : top left x
//			top left y
//			bottom right x
//			bottom right y
//			resize factor
// Returns: Area
///////////////////////////////////////////////////////////////////////////////
Area CreateImageArea(int posX, int posY, ImageId image, int resize = 0);
#endif // !MOUSE_H