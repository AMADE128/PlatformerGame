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

#include <SDL_mixer\include\SDL_mixer.h>

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
	logo = app->tex->Load("Assets/Textures/Screens/logo.png");
	logoOff = app->tex->Load("Assets/Textures/Screens/logo_off.png");

	logoMusic = app->audio->LoadFx("Assets/Audio/SceneMusic/light.wav");
	app->audio->PlayFx(logoMusic);

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
	uint logoW, logoH;
	app->tex->GetSize(logo, logoW, logoH);

	int x, y;
	x = app->render->camera.w / 2 - logoW / 2;
	y = app->render->camera.h / 2 - logoH / 2;

	if (frameMusic <= 392)
	{
		frameMusic++;
	}

	app->render->DrawRectangle({ 0, 0, 1280, 720 }, { 0, 0, 0, 255 });
	if (frameMusic < 5 ||
		(frameMusic > 12 && frameMusic < 19) ||
		(frameMusic > 24 && frameMusic < 29) ||
		(frameMusic > 31 && frameMusic < 48) ||
		(frameMusic > 53 && frameMusic < 60) ||
		(frameMusic > 67 && frameMusic < 70) ||
		(frameMusic > 72 && frameMusic < 108) ||
		(frameMusic > 113 && frameMusic < 134) ||
		(frameMusic > 139 && frameMusic < 144) ||
		(frameMusic > 146 && frameMusic < 161) ||
		(frameMusic > 166 && frameMusic < 168) ||
		(frameMusic > 31 && frameMusic < 48))
	{
		app->render->DrawTexture(logoOff, x, y);
	}
	else if ((frameMusic > 5 && frameMusic < 12) ||
		(frameMusic > 19 && frameMusic < 24) ||
		(frameMusic > 29 && frameMusic < 39) ||
		(frameMusic > 48 && frameMusic < 53) ||
		(frameMusic > 60 && frameMusic < 67) ||
		(frameMusic > 70 && frameMusic < 72) ||
		(frameMusic > 108 && frameMusic < 113) ||
		(frameMusic > 134 && frameMusic < 139) ||
		(frameMusic > 144 && frameMusic < 146) ||
		(frameMusic > 161 && frameMusic < 166) ||
		(frameMusic > 168))
	{
		app->render->DrawTexture(logo, x, y);
	}

	if (frameMusic >= 392)
	{
		app->fadeToBlack->Fade(this, (Module*)app->sceneMenu, 80);
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
	Mix_HaltMusic();

	app->tex->UnLoad(logo);
	app->audio->UnloadFX(logoMusic);
	app->fxList.Clear();

	active = false;

	return true;
}