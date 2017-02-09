/////////////////////////////////////////////////////////////////////////////
// Audio.cpp
// MaxiJonson
// December 3 2016
// Contains all function definitions from Audio.h
/////////////////////////////////////////////////////////////////////////////
#include "Audio.h"
#include <SDL_mixer.h>
#include <SDL.h>
#include <vector>
using namespace std;

vector<Mix_Music*> Songs; // Contains mp3 files
vector<Mix_Chunk*> Effects; // Contains wav files

inline void GetAudioError(bool fail, string error)
{
	if (fail) // Fail condition
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", error.c_str(), NULL);

		exit(1);
	}
}


void InitializeAudio()
{
	SDL_Init(SDL_INIT_AUDIO); // Initialize the SDL Audio components
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); // The first parameter can be played with a bit to increase or decrease frequency (speed) of the audio (Normal: 44100)
}



SndId LoadMusic(string FileName)
{
	Mix_Music* music = Mix_LoadMUS(FileName.c_str()); // First, the file is loaded into the Mix_Music type variable
													  // The file name is converted into a char since Mix_Music takes a char

	GetAudioError(!music, "LoadMusic Error:\n Couldn't load " + FileName); // If there is an error, this function will display it and shutdown the program preventing it from moving on

	Songs.push_back(music); // Then, the file is sent into the 'Songs' vector


	return Songs.size() - 1; // We return an SndId which represents the position in the vector
}



SndId LoadEffect(string FileName)
{
	// Same as LoadMusic, but we use Mix_Chunk as the type of the variable and Mix_LoadWAV to load the file
	Mix_Chunk* effect = Mix_LoadWAV(FileName.c_str());

	GetAudioError(!effect, "LoadEffect Error:\n Couldn't load " + FileName); 

	Effects.push_back(effect); // The file is sent into the 'Effects' vector


	return Effects.size() - 1;
}




void PlayMusic(SndId music, int volume, int loops)
{
	Mix_VolumeMusic(volume); // Set the volume
	Mix_PlayMusic(Songs[music], loops); // How many times to play the music. Defaults to -1 which is infinite

}



void PlayEffect(SndId effect, int volume, int loops, int channel)
{
	Mix_VolumeChunk(Effects[effect], volume);
	Mix_PlayChannel(channel, Effects[effect], loops); // -1 : The file is played in the first available channel
													  //  0 : Plays the sound once
}



void SndShutdown()
{

	for (int i = 0; i < Songs.size(); i++) // Loops into the 'Songs' vector to free mp3 ressources
	{
		Mix_FreeMusic(Songs[i]);
	}

	for (int i = 0; i < Effects.size(); i++)  // Loops into the 'Effects' vector to free wav ressources
	{
		Mix_FreeChunk(Effects[i]);
	}

	Mix_CloseAudio(); // The mixer is finally closed
	SDL_AudioQuit();

}



void PauseMusic()
{
	if (Mix_PlayingMusic) // If music is playing
	{
		Mix_PauseMusic(); // Pause the music
	}
}