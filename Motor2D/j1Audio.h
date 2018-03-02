#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"

#include <vector>

#define DEFAULT_MUSIC_FADE_TIME 2
#define MUSIC_FOLDER "audio/music/"
#define FX_FOLDER "audio/fx/"

struct _Mix_Music;
struct Mix_Chunk;

class j1Audio : public j1Module
{
public:

	j1Audio();

	// Destructor
	virtual ~j1Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);

	// Play a music file
	bool PlayMusic(const char* path, uint fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0, uint volume = 128);

	void ModifyMusicVolume(int value);
	void ModifySFXVolume(int value);



private:

	float					musicVolumeModifier = 1;
	float					sfxVolumeModifier = 1;
	_Mix_Music*				music = nullptr;
	std::vector<Mix_Chunk*>	fx;

public:

	p2SString			music_folder;
	p2SString			sfx_folder;
};

#endif // __j1AUDIO_H__