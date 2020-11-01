#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

struct SDL_Texture;

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

	List<unsigned int*>	musicList;

private:
	SDL_Texture* img;
	SDL_Texture* menu;
	unsigned int musicMenu;
	unsigned int musicScene1;

	int volume = 1;
};

#endif // __SCENE_H__