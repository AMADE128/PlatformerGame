#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLoose.h"
#include "Map.h"
#include "Collisions.h"
#include "Player.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

SceneLoose::SceneLoose() : Module()
{
	name.Create("scene");
}

// Destructor
SceneLoose::~SceneLoose()
{}

// Called before render is available
bool SceneLoose::Awake()
{
	LOG("Loading Scene");
	bool ret = true;



	return ret;
}

// Called before the first frame
bool SceneLoose::Start()
{
	// L03: DONE: Load map
	loose = app->tex->Load("Assets/Textures/Screens/lose.png");

	return true;
}

// Called each loop iteration
bool SceneLoose::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneLoose::Update(float dt)
{
	app->render->DrawTexture(loose, 0, 0);
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		//app->fadeToBlack->Fade(game_menu, 80);
	}

	return true;
}

// Called each loop iteration
bool SceneLoose::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool SceneLoose::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(loose);

	return true;
}