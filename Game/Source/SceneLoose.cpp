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
#include "Fonts.h"
#include "ModuleParticles.h"

#include <SDL_mixer\include\SDL_mixer.h>

#include "Defs.h"
#include "Log.h"

SceneLoose::SceneLoose() : Module()
{
	name.Create("scene");

	btnContinue = new GuiButton(10, { 1280 / 2 - BUTT_WIDTH / 2, 590, BUTT_WIDTH, BUTT_HEIGHT });
	btnContinue->SetObserver(this);

	btnBack = new GuiButton(11, { 1280 / 2 - BUTT_WIDTH / 2, 660, BUTT_WIDTH, BUTT_HEIGHT });
	btnBack->SetObserver(this);
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
	btnContinue->texture = btnBack->texture = app->tex->Load("Assets/Textures/Interface/button.png");
	btnContinue->text = app->tex->Load("Assets/Textures/Interface/start.png");
	btnBack->text = app->tex->Load("Assets/Textures/Interface/back.png");

	loseMusic = app->audio->LoadFx("Assets/Audio/SceneMusic/lose_music.wav");
	app->fxList.Add(&loseMusic);
	app->audio->PlayFx(loseMusic);

	app->render->camera.x = 0;
	app->render->camera.y = 0;

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
	btnContinue->Update(app->input, dt);
	btnBack->Update(app->input, dt);

	return true;
}

// Called each loop iteration
bool SceneLoose::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(loose, 0, 0);
	btnContinue->Draw(app->render);
	btnBack->Draw(app->render);

	return ret;
}

bool SceneLoose::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		switch (control->id)
		{
		case 10:
			app->player->cont = false;
			app->fadeToBlack->Fade(this, (Module*)app->scene, 80);
			break;
		case 11:
			app->fadeToBlack->Fade(this, (Module*)app->sceneMenu, 80);
			break;
		default:
			break;
		}
	}
	default: break;
	}

	return true;
}

// Called before quitting
bool SceneLoose::CleanUp()
{
	if (!active) return true;
	LOG("Freeing scene");
	Mix_HaltMusic();

	app->audio->UnloadFX(loseMusic);
	app->fxList.Clear();

	app->tex->UnLoad(loose);
	app->tex->UnLoad(btnContinue->texture);
	app->tex->UnLoad(btnBack->texture);
	app->tex->UnLoad(btnContinue->text);
	app->tex->UnLoad(btnBack->text);

	active = false; 

	return true;
}