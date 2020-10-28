#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Animation.h"
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

	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	SDL_Texture* idle = nullptr;
	SDL_Texture* fall = nullptr;
	SDL_Texture* hit = nullptr;
	SDL_Texture* run = nullptr;
	SDL_Texture* jump = nullptr;

	Animation* currentAnimation = nullptr;

	Animation idleAnim;

	uint positionX = 0;
	uint positionY = 0;

};

#endif // __PLAYER_H__