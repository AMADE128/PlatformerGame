#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneIntro.h"
#include "Map.h"
#include "Collisions.h"
#include "Player.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

SceneIntro::SceneIntro() : Module()
{
	name.Create("scene");
}

// Destructor
SceneIntro::~SceneIntro()
{}

// Called before render is available
bool SceneIntro::Awake()
{
	LOG("Loading Scene");
	bool ret = true;



	return ret;
}

// Called before the first frame
bool SceneIntro::Start()
{
	// L03: DONE: Load map
	logo = app->tex->Load("Assets/Textures/Screens/logo_screen.png");

	return true;
}

// Called each loop iteration
bool SceneIntro::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneIntro::Update(float dt)
{
	app->render->DrawTexture(logo, 0, 0);
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->fadeToBlack->Fade(this, (Module*)app->sceneMenu, 1600*dt);
	}

	return true;
}

// Called each loop iteration
bool SceneIntro::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool SceneIntro::CleanUp()
{
	if (!active) return true;

	LOG("Freeing scene");
	app->tex->UnLoad(logo);

	active = false;

	return true;
}