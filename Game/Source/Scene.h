#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool savePoint = false;
	bool checkpointFX = false;
	Collider* checkPointColl = nullptr;
	Collider* appleColl1 = nullptr;
	Collider* appleColl2 = nullptr;
	Collider* appleColl3 = nullptr;
	Collider* appleColl4 = nullptr;
	Collider* appleColl5 = nullptr;
	Collider* pineappleColl1 = nullptr;

	SDL_Texture* appleTex = nullptr;

	unsigned int jumpFx;
	unsigned int pointFx;
	unsigned int damageFx;
	unsigned int checkpointFx;

private:

	SDL_Texture* checkPointIdleTex = nullptr;
	SDL_Texture* checkPointStartTex = nullptr;
	SDL_Texture* checkPointTouchTex = nullptr;
	SDL_Texture* pineappleTex = nullptr;

	SDL_Texture* currentTex = nullptr;
	Animation* currentAnimation = nullptr;

	Animation checkPointIdleAnim;
	Animation checkPointStartAnim;
	Animation checkPointTouchAnim;
	Animation appleAnim;
	Animation pineappleAnim;

	unsigned int musicScene1;
	unsigned int checkpointMusic;

};

#endif // __SCENE_H__