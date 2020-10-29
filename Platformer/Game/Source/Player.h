#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

#include "Point.h"

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

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();
	SDL_Texture* currentTex = nullptr;
	SDL_Texture* idle = nullptr;
	SDL_Texture* fall = nullptr;
	SDL_Texture* run = nullptr;
	SDL_Texture* jump = nullptr;
	SDL_Texture* death = nullptr;
	
	SDL_RendererFlip flip;

	Animation* currentAnimation = nullptr;

	Animation idleAnim;
	Animation fallAnim;
	Animation runAnim;
	Animation jumpAnim;
	Animation deathAnim;

	fPoint position;

	float speed_x = 2;
	float speed_y = 2;

	bool god = false;
	bool gravity = false;

};

#endif // __PLAYER_H__