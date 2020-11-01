#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Collisions.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;



	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// L03: DONE: Load map
	app->map->Load("map1.tmx");
	menu = app->tex->Load("Assets/textures/screens/Menu.png");
	death = app->tex->Load("Assets/textures/screens/Lose.png");
	logo = app->tex->Load("Assets/textures/screens/logoscreen.png");
	win = app->tex->Load("Assets/textures/screens/Win.png");

	musicMenu = app->audio->LoadFx("Assets/audio/music/Intromusic.wav");
	musicList.add(&musicMenu);

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// Set VOLUME
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		if (volume < 128)
		{
			volume+= 128/32;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		if (volume > 0)
		{
			volume-= 128/32;
		}
	}

	//Draw Scenes
	if (app->screen == game_scene1)
	{
		if (start == true)
		{
			musicScene1 = app->audio->LoadFx("Assets/audio/music/Levelmusic.wav");
			musicList.add(&musicScene1);
			start = false;
		}
		app->map->Draw();
		app->audio->PlayFx(musicScene1);
	}
	else if (app->screen == game_logo)
	{
		app->render->DrawTexture(logo, 0, 0);
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			app->screen = game_menu;
		}
	}
	else if (app->screen == game_menu)
	{
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
			start = true;
			app->screen = game_scene1;
			app->audio->UnloadFX(musicMenu);
		}
	}
	else if (app->screen == game_death)
	{
		app->audio->UnloadFX(musicScene1);
		app->player->death = false;
		app->player->position.x = 720;
		app->player->position.y = 1584;
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		app->player->currentAnimation = &app->player->idleAnim;
		app->player->currentTex = app->player->idleTex;
		app->player->flip = false;
		app->player->deathAnim.Reset();
		app->render->DrawTexture(death, 0, 0);
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			app->screen = game_scene1;
			start = true;
		}
	}
	else if (app->screen == game_win)
	{
		app->audio->UnloadFX(musicScene1);
		app->player->position.x = 720;
		app->player->position.y = 1584;
		app->render->DrawTexture(win, 0, 0);
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			app->screen = game_menu;
			start = true;
		}
	}
	
	// L03: DONE 7: Set the window title with map/tileset info
	SString title("Platformer Game");

	app->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	ListItem<unsigned int*>* item;
	for (item = musicList.start; item != NULL; item = item->next)
	{
		app->audio->SetVolume(*item->data, volume);
	}

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(img);
	app->tex->UnLoad(menu);
	app->tex->UnLoad(death);

	return true;
}
