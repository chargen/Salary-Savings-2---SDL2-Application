/////////////////////////////////////////////////////////////////////////////
// Display.cpp
// MaxiJonson
// December 15 2016
// Contains all function definitions from Display.h
/////////////////////////////////////////////////////////////////////////////

#include "Display.h"
#include <vector>
SDL_Window *window = 0;  // Multiple windows can be created
SDL_Renderer *renderer = 0; // A renderer is created to show textures to the window
vector<SDL_Surface*> Images;  // Images vector
vector<SDL_Texture*> Textures; // Textures vector. Gets texture from Images vector

inline void GetDisplayError(bool fail, string error)
{
	if (fail) // Fail condition
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", error.c_str(), NULL);

		exit(2); // Error code
	}
}



void Fullscreen()
{
	GetDisplayError(SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN), "Fullscreen Error:\nCouldn't launch fullscreen"); // Initializes fullscreen mode and checks if there were errors
}



void Wait(int time)
{
	GetDisplayError(time <= 0, "Wait:\n Invalid time");
	SDL_Delay(time); // No need of Windows.h with this!
}



void InitializeDisplay(string windowTitle, int width, int height)
{
	GetDisplayError(width <= 0, "Initialize Display: \n Invalid width");
	GetDisplayError(height <= 0, "Initialize Display: \n Invalid height");

	SDL_Init(SDL_INIT_VIDEO); // The video components of SDL are initialized
	SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE, &window, &renderer); // A window is then created

	GetDisplayError(!window, "Initialize Display: \n Couldn't load video mode");
	GetDisplayError(!window, "Initialize Display: \n Couldn't load renderer");
	SDL_SetWindowTitle(window, windowTitle.c_str()); // The window title is set here
}



void FillWindow(int red, int green, int blue, int alpha)
{
	GetDisplayError(red<0 || red>255, "FillWindow:\n Red channel invalid");
	GetDisplayError(green<0 || green>255, "FillWindow:\n Green channel invalid");
	GetDisplayError(blue<0 || blue>255, "FillWindow:\n Blue channel invalid");

	SDL_SetRenderDrawColor(renderer, red, green, blue, alpha); // Alpha 255: opaque	by default
	SDL_RenderClear(renderer); // Empties the renderer with the chosen color
}



ImageId LoadBmp(string FileName, int red, int green, int blue)
{
	SDL_Surface* image = SDL_LoadBMP(FileName.c_str()); // File is loaded in the SDL_Surface* type variable

	GetDisplayError(!image, string("LoadBmp:\n Couldn't load image file ") + FileName); // Check if the file is found

	Images.push_back(image); // Send the file to the Images vector


	SDL_SetColorKey(Images[Images.size() - 1], SDL_TRUE, // enable color key (transparency)
		SDL_MapRGB(Images[Images.size() - 1]->format, red, green, blue)); // This is the color that should be taken as being the 'transparent' part of the image

																		  // Create a texture from surface (image)
	SDL_Texture* Texture = SDL_CreateTextureFromSurface(renderer, Images[Images.size() - 1]);
	Textures.push_back(Texture);


	return Images.size() - 1; // ImageId becomes the position of the file in the vector
}



void ShowImage(ImageId Image, int PosX, int PosY, int resize)
{
	GetDisplayError(Image<0 || unsigned int(Image)>Images.size() - 1, "ShowImage:\n Invalid image number");
	GetDisplayError(PosX<0, "ShowImage:\n Invalid X position");
	GetDisplayError(PosY<0, "ShowImage:\n Invalid Y position");
	float W;
	float H;
	float size = resize;
	// Rectangle that represents the image position
	SDL_Rect position;
	position.x = PosX;
	position.y = PosY;
	position.h = Images[Image]->h;
	position.w = Images[Image]->w;
	if (resize != 0)
	{
		if (size >= 0)
		{
			W = (size / 100 * position.w) + position.w;
			H = (size / 100 * position.h) + position.h;
			position.w = W;
			position.h = H;
		}
		else
		{
			size *= -1;
			W = -(size / 100 * position.w) + position.w;
			H = -(size / 100 * position.h) + position.h;
			position.w = W;
			position.h = H;
		}
	}
	// copy texture into renderer
	SDL_RenderCopy(renderer, Textures[Image], NULL, &position); // NULL: Copy the entire texture
}



void Refresh()
{
	SDL_RenderPresent(renderer); // Refreshes the renderer with the new changes
}



void CloseDisplay()
{
	// Free the surfaces
	for (unsigned int i = 0; i<Images.size(); i++)
	{
		SDL_FreeSurface(Images[i]);
	}
	// Free the textures
	for (unsigned int i = 0; i < Textures.size(); i++)
	{
			SDL_DestroyTexture(Textures[i]);
	}
	// Destroy renderer
	SDL_DestroyRenderer(renderer);
	// Destroy the window
	SDL_DestroyWindow(window);
	// Quit
	SDL_Quit();
}



void FreeImages()
{
	for (unsigned int i = 0; i<Images.size(); i++)
	{
		if (Images[i] != NULL)
		{
			SDL_FreeSurface(Images[i]);
			Images[i] = NULL;
		}
	}

	for (unsigned int i = 0; i < Textures.size(); i++)
	{
		if (Textures[i] != NULL)
		{
			SDL_DestroyTexture(Textures[i]);
			Textures[i] = NULL;
		}
	}

}



SDL_Rect GetImageRect(ImageId image, int resize)
{
	SDL_Rect position;
	float size = resize;
	float W;
	float H;

	position.w = Images[image]->w;
	position.h = Images[image]->h;
	if (resize != 0)
	{
		if (size >= 0)
		{
			W = (size / 100 * position.w) + position.w;
			H = (size / 100 * position.h) + position.h;
			position.w = W;
			position.h = H;
		}
		else
		{
			size *= -1;
			W = -(size / 100 * position.w) + position.w;
			H = -(size / 100 * position.h) + position.h;
			position.w = W;
			position.h = H;
		}
	}

	return position;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////// TEXT ////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vector<TTF_Font*> Fonts; // Stores all fonts
vector<SDL_Surface*> Texts;  // Texts vector
vector<SDL_Texture*> TextTextures; // Textures vector. Gets texture from Texts vector
vector<SDL_Surface*> Outlines; // Same as Texts but for the outline
vector<SDL_Texture*> OutlineTextures; // Same as TextTextures but for the outline
vector<int> StrokeSizes; // Stores the stroke size of the outline
vector<Alphabet> Alphabets; // Stores an alphabet of images

inline void GetTextError(bool fail, string error)
{
	if (fail) // Fail condition
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", error.c_str(), NULL);

		exit(3);
	}
}

void InitializeTTF()
{
	GetTextError(TTF_Init(), "InitializeTTF Error:\n Couldn't initialize the TTF engine."); // This initializes the TTF engine
}

FontId LoadFont(string fileName, int fontSize)
{

	TTF_Font *font = TTF_OpenFont(fileName.c_str(), fontSize); // Load the font file into the TTF_Font type variable

	GetTextError(!font, "LoadFont Error:\n The font '" + fileName + "' could not be loaded");

	Fonts.push_back(font); // Add it to the Fonts vector

	return Fonts.size() - 1; // FontId becomes the position of the font file in the Fonts vector
}

TextId LoadText(FontId font, string text, int red, int green, int blue, int strokesize, int strokeRed, int strokeGreen, int strokeBlue)
{
	/// FILL
	SDL_Color color = { red, green, blue, 255 }; // Color is set here
	GetTextError(red < 0 || red > 255, "LoadText Error:\n Red amount invalid");
	GetTextError(green < 0 || green > 255, "LoadText Error:\n Green amount invalid");
	GetTextError(blue < 0 || blue > 255, "LoadText Error:\n Blue amount invalid");

	TTF_Font *Font = Fonts[font]; // Here we get the font from Fonts vector and store it in a TTF_Font* variable
	GetTextError(font<0 || unsigned int(font)>Fonts.size() - 1, "LoadText:\n Invalid font ID");


	SDL_Surface *textSurface = TTF_RenderText_Solid(Font, text.c_str(), color); // We make a surface from the text written in a certain font with a color
	Texts.push_back(textSurface);


	SDL_Texture *TEXTure = SDL_CreateTextureFromSurface(renderer, Texts[Texts.size() - 1]); // We create the texture of the text from the surface we just made
	TextTextures.push_back(TEXTure);

	/// OUTLINE
	SDL_Color outlineColor = { strokeRed, strokeGreen, strokeBlue, 255 }; // Outline color
	GetTextError(strokeRed < 0 || strokeRed > 255, "LoadText Error:\n Outline Red amount invalid");
	GetTextError(strokeGreen < 0 || strokeGreen > 255, "LoadText Error:\n Outline Green amount invalid");
	GetTextError(strokeBlue < 0 || strokeBlue > 255, "LoadText Error:\n Outline Blue amount invalid");

	TTF_SetFontOutline(Font, strokesize); // We set the font to now be an outline
	StrokeSizes.push_back(strokesize); // The stroke size is pushed into the StrokeSizes vector

	SDL_Surface *outlineSurface = TTF_RenderText_Solid(Font, text.c_str(), outlineColor); // We create an outline surface
	Outlines.push_back(outlineSurface); // Push it into the Outlines vector

	SDL_Texture *OUTure = SDL_CreateTextureFromSurface(renderer, outlineSurface); // We create a texture from the outline surface
	OutlineTextures.push_back(OUTure); // Push it into the OutlineTextures vector


	TTF_SetFontOutline(Font, 0); // Then we set back the font to be a fill (so that when we call this again, it doesn't stay an outline for the first steps)
	return Texts.size() - 1; // TextId become the position of the text in the Texts vector
}



void ShowText(TextId text, int x, int y)
{
	GetTextError(text<0 || unsigned int(text)>Texts.size() - 1, "ShowText Error:\n Invalid text number");
	GetTextError(x<0, "ShowText Error:\n Invalid X position");
	GetTextError(y<0, "ShowText Error:\n Invalid Y position");

	// Rectangle that represents where to show the text
	SDL_Rect textRect;
	textRect.x = x;
	textRect.y = y;
	textRect.w = Texts[text]->w;
	textRect.h = Texts[text]->h;
	// Rectangle that representswhere to show the outline
	SDL_Rect outRect;
	outRect.x = x - StrokeSizes[text]; // we substract the position given by the strokesize or else it would be positioned the wrong way
	outRect.y = y - StrokeSizes[text];
	outRect.w = Outlines[text]->w;
	outRect.h = Outlines[text]->h;

	SDL_RenderCopy(renderer, OutlineTextures[text], NULL, &outRect); // We copy the stroke into the renderer
	SDL_RenderCopy(renderer, TextTextures[text], NULL, &textRect); // We copy the fill into the renderer
}


void FreeText()
{

	for (unsigned int i = 0; i<Texts.size(); i++)
	{
		if (Texts[i] != NULL)
		{
			SDL_FreeSurface(Texts[i]);
			Texts[i] = NULL;
		}
	}

}



AlphaId LoadAlphabet(string folderName, int alphaRed, int alphaGreen, int alphaBlue)
{
	Alphabet letters;

	// Load every letter's image (Note the file name as it NEEDS to be in this format)
	letters.a = LoadBmp(folderName + "\\a.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.b = LoadBmp(folderName + "\\b.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.c = LoadBmp(folderName + "\\c.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.d = LoadBmp(folderName + "\\d.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.e = LoadBmp(folderName + "\\e.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.f = LoadBmp(folderName + "\\f.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.g = LoadBmp(folderName + "\\g.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.h = LoadBmp(folderName + "\\h.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.i = LoadBmp(folderName + "\\i.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.j = LoadBmp(folderName + "\\j.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.k = LoadBmp(folderName + "\\k.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.l = LoadBmp(folderName + "\\l.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.m = LoadBmp(folderName + "\\m.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.n = LoadBmp(folderName + "\\n.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.o = LoadBmp(folderName + "\\o.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.p = LoadBmp(folderName + "\\p.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.q = LoadBmp(folderName + "\\q.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.r = LoadBmp(folderName + "\\r.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.s = LoadBmp(folderName + "\\s.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.t = LoadBmp(folderName + "\\t.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.u = LoadBmp(folderName + "\\u.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.v = LoadBmp(folderName + "\\v.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.w = LoadBmp(folderName + "\\w.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.x = LoadBmp(folderName + "\\x.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.y = LoadBmp(folderName + "\\y.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.z = LoadBmp(folderName + "\\z.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.zero = LoadBmp(folderName + "\\0.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.one = LoadBmp(folderName + "\\1.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.two = LoadBmp(folderName + "\\2.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.three = LoadBmp(folderName + "\\3.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.four = LoadBmp(folderName + "\\4.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.five = LoadBmp(folderName + "\\5.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.six = LoadBmp(folderName + "\\6.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.seven = LoadBmp(folderName + "\\7.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.eight = LoadBmp(folderName + "\\8.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.nine = LoadBmp(folderName + "\\9.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.dot = LoadBmp(folderName + "\\dot.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.plus = LoadBmp(folderName + "\\plus.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.dollar = LoadBmp(folderName + "\\dollar.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.colon = LoadBmp(folderName + "\\colon.bmp", alphaRed, alphaGreen, alphaBlue);
	letters.dash = LoadBmp(folderName + "\\dash.bmp", alphaRed, alphaGreen, alphaBlue);
	// At the end, we push the struct into the Alphabets vector
	Alphabets.push_back(letters);

	// Returns the position of the alphabet in the vector
	return Alphabets.size() - 1;

}






void ShowAlpha(string text, AlphaId alphabet, int posX, int posY, int resize, int Xspacing, int Yspacing)
{
	Alphabet pack = Alphabets[alphabet];

	int X = posX;
	int Y = posY;

	int width = 0;
	int height = 0;

	for (int i = 0; i < text.size(); ++i)
	{

		switch (text[i])
		{
		case 'A':
		case 'a':
			ShowLetter(pack.a, X, Y, resize, width);
			break;

		case 'B':
		case 'b':
			ShowLetter(pack.b, X, Y, resize, width);
			break;

		case 'C':
		case 'c':
			ShowLetter(pack.c, X, Y, resize, width);
			break;

		case 'D':
		case 'd':
			ShowLetter(pack.d, X, Y, resize, width);
			break;

		case 'E':
		case 'e':
			ShowLetter(pack.e, X, Y, resize, width);
			break;

		case 'F':
		case 'f':
			ShowLetter(pack.f, X, Y, resize, width);
			break;

		case 'G':
		case 'g':
			ShowLetter(pack.g, X, Y, resize, width);
			break;

		case 'H':
		case 'h':
			ShowLetter(pack.h, X, Y, resize, width);
			break;

		case 'I':
		case 'i':
			ShowLetter(pack.i, X, Y, resize, width);
			break;

		case 'J':
		case 'j':
			ShowLetter(pack.j, X, Y, resize, width);
			break;

		case 'K':
		case 'k':
			ShowLetter(pack.k, X, Y, resize, width);
			break;

		case 'L':
		case 'l':
			ShowLetter(pack.l, X, Y, resize, width);
			break;

		case 'M':
		case 'm':
			ShowLetter(pack.m, X, Y, resize, width);
			break;

		case 'N':
		case 'n':
			ShowLetter(pack.n, X, Y, resize, width);
			break;

		case 'O':
		case 'o':
			ShowLetter(pack.o, X, Y, resize, width);
			break;

		case 'P':
		case 'p':
			ShowLetter(pack.p, X, Y, resize, width);
			break;

		case 'Q':
		case 'q':
			ShowLetter(pack.q, X, Y, resize, width);
			break;

		case 'R':
		case 'r':
			ShowLetter(pack.r, X, Y, resize, width);
			break;

		case 'S':
		case 's':
			ShowLetter(pack.s, X, Y, resize, width);
			break;

		case 'T':
		case 't':
			ShowLetter(pack.t, X, Y, resize, width);
			break;

		case 'U':
		case 'u':
			ShowLetter(pack.u, X, Y, resize, width);
			break;

		case 'V':
		case 'v':
			ShowLetter(pack.v, X, Y, resize, width);
			break;

		case 'W':
		case 'w':
			ShowLetter(pack.w, X, Y, resize, width);
			break;

		case 'X':
		case 'x':
			ShowLetter(pack.x, X, Y, resize, width);
			break;

		case 'Y':
		case 'y':
			ShowLetter(pack.y, X, Y, resize, width);
			break;

		case 'Z':
		case 'z':
			ShowLetter(pack.z, X, Y, resize, width);
			break;

		case '0':
			ShowLetter(pack.zero, X, Y, resize, width);
			break;

		case '1':
			ShowLetter(pack.one, X, Y, resize, width);
			break;

		case '2':
			ShowLetter(pack.two, X, Y, resize, width);
			break;

		case '3':
			ShowLetter(pack.three, X, Y, resize, width);
			break;

		case '4':
			ShowLetter(pack.four, X, Y, resize, width);
			break;

		case '5':
			ShowLetter(pack.five, X, Y, resize, width);
			break;

		case '6':
			ShowLetter(pack.six, X, Y, resize, width);
			break;

		case '7':
			ShowLetter(pack.seven, X, Y, resize, width);
			break;

		case '8':
			ShowLetter(pack.eight, X, Y, resize, width);
			break;

		case '9':
			ShowLetter(pack.nine, X, Y, resize, width);
			break;

		case '.':
			ShowLetter(pack.dot, X, Y, resize, width);
			break;

		case '+':
			ShowLetter(pack.plus, X, Y, resize, width);
			break;

		case '$':
			ShowLetter(pack.dollar, X, Y, resize, width);
			break;

		case ':':
			ShowLetter(pack.colon, X, Y, resize, width);
			break;

		case '-':
			ShowLetter(pack.dash, X, Y, resize, width);
			break;

		case ' ':
			width = Xspacing * 2;
			break;
		}

		X += width + Xspacing;
		Y += Yspacing;
	}
}


// This function should only be called by ShowAlpha, not by the user (you)
void ShowLetter(ImageId Image, int PosX, int PosY, int resize, int& width)
{
	GetDisplayError(Image<0 || unsigned int(Image)>Images.size() - 1, "ShowLetter:\n Invalid image number");
	GetDisplayError(PosX<0, "ShowLetter:\n Invalid X position");
	GetDisplayError(PosY<0, "ShowLetter:\n Invalid Y position");
	float size = resize;
	float W;
	float H;
	// Rectangle that represents the image position
	SDL_Rect position;
	position.x = PosX;
	position.y = PosY;
	position.h = Images[Image]->h;
	position.w = Images[Image]->w;
	if (resize != 0)
	{
		if (size >= 0)
		{
			W = (size / 100 * position.w) + position.w;
			H = (size / 100 * position.h) + position.h;
			position.w = W;
			position.h = H;
		}
		else
		{
			size *= -1;
			W = -(size / 100 * position.w) + position.w;
			H = -(size / 100 * position.h) + position.h;
			position.w = W;
			position.h = H;
		}
	}
	width = position.w;
	// copy texture into renderer
	SDL_RenderCopy(renderer, Textures[Image], NULL, &position); // NULL: Copy the entire texture
}



