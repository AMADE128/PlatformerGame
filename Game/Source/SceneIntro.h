#ifndef __SCENEINTRO_H__
#define __SCENEINTRO_H__

#include "Module.h"

struct SDL_Texture;

class SceneIntro : public Module
{
public:

	SceneIntro();

	// Destructor
	virtual ~SceneIntro();

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

	SDL_Texture* logo;
	SDL_Texture* logoOff;
	
	unsigned int logoMusic;
	int frameMusic = 0;

private:
};

#endif // __SCENE_H__