#ifndef __SCENEMENU_H__
#define __SCENEMENU_H__

#include "Module.h"

struct SDL_Texture;

class SceneMenu : public Module
{
public:

	SceneMenu();

	// Destructor
	virtual ~SceneMenu();

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

	SDL_Texture* menu;

	unsigned int musicMenu;

private:
};

#endif // __SCENE_H__