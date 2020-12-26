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

	bool Die(Collider* c1, Collider* c2) override;
	bool CameraScroll(Collider* c1, Collider* c2) override;
	bool CheckPoint(Collider* c1, Collider* c2) override;
	bool CollectApple(Collider* c1, Collider* c2) override;
	bool CollectPineapple(Collider* c1, Collider* c2) override;

	// Called before quitting
	bool CleanUp();
	SDL_Texture* currentTex = nullptr;

	SDL_Texture* idleTex = nullptr;
	SDL_Texture* doubleJumpTex = nullptr;
	SDL_Texture* fallTex = nullptr;
	SDL_Texture* runTex = nullptr;
	SDL_Texture* jumpTex = nullptr;
	SDL_Texture* deathTex = nullptr;
	SDL_Texture* lifesTex = nullptr;
	SDL_Texture* appeTex = nullptr;
	SDL_Texture* desAppeTex = nullptr;
	SDL_Texture* appleTex = nullptr;
	SDL_Texture* dronTex = nullptr;
	SDL_Texture* chickenFlyTex = nullptr;


	bool flip;
	
	int skillCoolDown = 0;

	bool skillRest = 0;

	int lifes = 3;

	unsigned int hitFx;
	unsigned int jumpFx;
	unsigned int pointFx;
	unsigned int leafFx;

	Animation* currentAnimation = nullptr;

	Animation idleAnim;
	Animation fallAnim;
	Animation runAnim;
	Animation jumpAnim;
	Animation deathAnim;
	Animation doubleJumpAnim;
	Animation appeAnim;
	Animation desAppeAnim;
	Animation dronAnim;
	Animation chickenAnim;

	iPoint position;

	float speedX;
	float speedY;
	float speedXLastFrame;

	Collider* playerColl = nullptr;
	Collider* cameraColl = nullptr;

	bool stopMovementX;
	bool stopMovementY;

	bool god = false;
	bool death = false;
	bool appear = false;

	int appleCounter = 0;
	int scoreFont = -1;
	char scoreText[10] = { "\0" };

	bool yDownCollision;
	bool yUpCollision;
	bool xLeftCollision;
	bool xRightCollision;

	bool isJumping;
	bool secondJump;

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	unsigned int lvl;
	bool saveGame = false;
};

#endif // __PLAYER_H__