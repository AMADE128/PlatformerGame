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
	app->map->Load("map1.tmx");
	menu = app->tex->Load("Assets/Textures/Screens/menu.png");
	logo = app->tex->Load("Assets/Textures/Screens/logo_screen.png");

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
	// Set VOLUME
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		if (volume < 128)
		{
			volume += 128 / 32;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		if (volume > 0)
		{
			volume -= 128 / 32;
		}
	}

	if (app->screen == game_logo)
	{
		app->render->DrawTexture(logo, 0, 0);
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			app->fadeToBlack->Fade(120);
			startMenu = true;
			app->screen = game_menu;
		}
	}
	else if (app->screen == game_menu)
	{
		if (startMenu == true)
		{
			musicMenu = app->audio->LoadFx("Assets/Audio/Music/intro_music.wav");
			musicList.add(&musicMenu);
			startMenu = false;
		}
		app->player->currentAnimation = &app->player->idleAnim;
		app->player->currentTex = app->player->idleTex;
		app->player->flip = false;
		app->player->deathAnim.Reset();
		app->player->position.x = 720;
		app->player->position.y = 1584;
		app->audio->PlayFx(musicMenu);
		app->render->DrawTexture(menu, 0, 0);
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			app->fadeToBlack->Fade(120);
			startScene1 = true;
			app->screen = game_scene1;
		}
	}

	// L03: DONE 7: Set the window title with map/tileset info

	return true;
}

// Called each loop iteration
bool SceneMenu::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	ListItem<unsigned int*>* item;
	for (item = musicList.start; item != NULL; item = item->next)
	{
		app->audio->SetVolume(*item->data, volume);
	}

	return ret;
}

// Called before quitting
bool SceneMenu::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(menu);
	app->tex->UnLoad(logo);

	return true;
}