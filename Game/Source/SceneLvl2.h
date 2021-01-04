#ifndef __SCENELVL2_H__
#define __SCENELVL2_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

class SceneLvl2 : public Module
{
public:

	SceneLvl2();

	// Destructor
	virtual ~SceneLvl2();

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
	Collider* appleColl1Lvl2 = nullptr;
	Collider* appleColl2Lvl2 = nullptr;
	Collider* appleColl3Lvl2 = nullptr;
	Collider* appleColl4Lvl2 = nullptr;
	Collider* appleColl5Lvl2 = nullptr;
	Collider* appleColl6Lvl2 = nullptr;
	Collider* pineappleColl1Lvl2 = nullptr;

	SDL_Texture* appleTex = nullptr;

	unsigned int jumpFx;
	unsigned int pointFx;
	unsigned int damageFx;
	unsigned int checkpointFx;
	int apples2[7];

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
