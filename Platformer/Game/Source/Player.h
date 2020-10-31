#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

#include "Point.h"

struct Collider;

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

	bool StopMovementY(Collider* c1, Collider* c2) override;
	bool StopMovement(Collider* c1, Collider* c2) override;

	bool Fall(Collider* c1, Collider* c2) override;

	// Called before quitting
	bool CleanUp();
	SDL_Texture* currentTex = nullptr;

	SDL_Texture* idleRight = nullptr;
	SDL_Texture* idleLeft = nullptr;
	SDL_Texture* fallRight = nullptr;
	SDL_Texture* fallLeft = nullptr;
	SDL_Texture* runRight = nullptr;
	SDL_Texture* runLeft = nullptr;
	SDL_Texture* jumpRight = nullptr;
	SDL_Texture* jumpLeft = nullptr;
	SDL_Texture* deathRight = nullptr;
	SDL_Texture* deathLeft = nullptr;

	bool flip;

	Animation* currentAnimation = nullptr;

	Animation idleRightAnim;
	Animation idleLeftAnim;
	Animation fallRightAnim;
	Animation fallLeftAnim;
	Animation runRightAnim;
	Animation runLeftAnim;
	Animation jumpRightAnim;
	Animation jumpLeftAnim;
	Animation deathRightAnim;
	Animation deathLeftAnim;

	fPoint position;

	float speed_x;
	float speed_y;
	float speed_xLastFrame;

	Collider* collider = nullptr;

	bool god = false;
	bool y_downCollision;
	bool y_upCollision;
	bool x_leftCollision;
	bool x_rightCollision;

	bool isJumping;


};

#endif // __PLAYER_H__