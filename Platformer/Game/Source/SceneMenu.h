#ifndef __SCENE_MENU_H__
#define __SCENE_MENU_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class SceneMenu : public Module
{
public:
	//Constructor
	SceneMenu(bool StartEnabled);

	//Destructor
	~SceneMenu();

	bool Awake();

	bool Start();

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update();

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();

	bool CleanUp();

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* MenuTexture = nullptr;
};

#endif