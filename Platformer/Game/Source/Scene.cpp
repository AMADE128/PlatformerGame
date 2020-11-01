#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Collisions.h"

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
	//app->map->Load("hello2.tmx");
	app->map->Load("map1.tmx");
	menu = app->tex->Load("Assets/textures/screens/Menu.png");

	musicMenu = app->audio->LoadFx("Assets/audio/music/Intromusic.wav");
	musicScene1 = app->audio->LoadFx("Assets/audio/music/Levelmusic.wav");
	musicList.add(&musicMenu);
	musicList.add(&musicScene1);

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
		app->map->Draw();
		app->audio->PlayFx(musicScene1);
	}
	else if (app->screen == game_menu)
	{
		app->audio->PlayFx(musicMenu);
		app->render->DrawTexture(menu, 0, 0);
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			app->screen = game_scene1;
			app->audio->UnloadFX(musicMenu);
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

	return true;
}
