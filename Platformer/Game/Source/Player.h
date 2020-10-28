#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"

#include "SDL/include/SDL.h"

class Player : public Module
{

public:

	Player();

	// Destructor
	virtual ~Player();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	SDL_Texture* Idle = nullptr;
	SDL_Texture* Fall = nullptr;
	SDL_Texture* Hit = nullptr;
	SDL_Texture* Run = nullptr;
	SDL_Texture* Jump = nullptr;

};

#endif // __PLAYER_H__