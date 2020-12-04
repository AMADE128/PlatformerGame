#ifndef __SCENELOOSE_H__
#define __SCENELOOSE_H__

#include "Module.h"

struct SDL_Texture;

class SceneLoose : public Module
{
public:

	SceneLoose();

	// Destructor
	virtual ~SceneLoose();

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

	SDL_Texture* loose;

private:
};

#endif // __SCENE_H__