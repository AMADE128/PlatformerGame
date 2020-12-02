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

	List<unsigned int*>	musicList;
	bool startScene1;
	bool startMenu;

	SDL_Texture* img;
	SDL_Texture* menu;
	SDL_Texture* death;
	SDL_Texture* logo;
	SDL_Texture* win;

	unsigned int musicMenu;
	unsigned int musicDeath;
	int volume = 1;
private:
};

#endif // __SCENE_H__