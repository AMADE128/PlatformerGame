#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneWin.h"
#include "Map.h"
#include "Collisions.h"
#include "Player.h"
#include "FadeToBlack.h"
#include "ModuleParticles.h"
#include "Fonts.h"

#include "Defs.h"
#include "Log.h"

SceneWin::SceneWin() : Module()
{
	name.Create("scene");
}

// Destructor
SceneWin::~SceneWin()
{}

// Called before render is available
bool SceneWin::Awake()
{
	LOG("Loading Scene");
	bool ret = true;



	return ret;
}

// Called before the first frame
bool SceneWin::Start()
{
	// L03: DONE: Load map
	win = app->tex->Load("Assets/Textures/Screens/win.png");

	winMusic = app->audio->LoadFx("Assets/Audio/SceneMusic/win_music.wav");
	app->musicList.Add(&winMusic);
	app->audio->PlayFx(winMusic);

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return true;
}

// Called each loop iteration
bool SceneWin::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneWin::Update(float dt)
{
	app->render->DrawTexture(win, 0, 0);
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->fadeToBlack->Fade(this, (Module*)app->sceneMenu, 1600 * dt);
	}

	return true;	
}

// Called each loop iteration
bool SceneWin::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool SceneWin::CleanUp()
{
	if (!active) return true;

	LOG("Freeing scene");
	app->tex->UnLoad(win);

	app->musicList.Clear();
	app->audio->UnloadFX(winMusic);

	active = false;

	return true;
}