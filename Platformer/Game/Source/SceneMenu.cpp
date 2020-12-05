#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneMenu.h"
#include "Map.h"
#include "Collisions.h"
#include "Player.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

SceneMenu::SceneMenu() : Module()
{
	name.Create("scene");
}

// Destructor
SceneMenu::~SceneMenu()
{}

// Called before render is available
bool SceneMenu::Awake()
{
	LOG("Loading Scene");
	bool ret = true;



	return ret;
}

// Called before the first frame
bool SceneMenu::Start()
{
	// L03: DONE: Load map
	menu = app->tex->Load("Assets/Textures/Screens/menu.png");
	musicMenu = app->audio->LoadFx("Assets/Audio/SceneMusic/intro_music.wav");
	app->musicList.Add(&musicMenu);
	app->audio->PlayFx(musicMenu, -1);

	return true;
}

// Called each loop iteration
bool SceneMenu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneMenu::Update(float dt)
{
	app->render->DrawTexture(menu, 0, 0);
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->fadeToBlack->Fade(this, (Module*)app->scene, 1600 * dt);
	}

	return true;
}

// Called each loop iteration
bool SceneMenu::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool SceneMenu::CleanUp()
{
	if (!active) return true;

	LOG("Freeing scene");
	app->tex->UnLoad(menu);
	app->audio->UnloadFX(musicMenu);
	app->musicList.Clear();

	active = false;

	return true;
}