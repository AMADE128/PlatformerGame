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
#include "SceneMenu.h"
#include "FadeToBlack.h"
#include "Animation.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");

	for (int i = 0; i < 192 * 10; i += 192)
	{
		checkPointIdleAnim.PushBack({ i, 0, 192, 192 });
	}
	checkPointIdleAnim.loop = true;
	checkPointIdleAnim.speed = 0.4f;

	for (int i = 0; i < 192 * 26; i += 192)
	{
		checkPointTouchAnim.PushBack({ i, 0, 192, 192 });
	}
	checkPointTouchAnim.loop = false;
	checkPointTouchAnim.speed = 0.4f;

	checkPointStartAnim.PushBack({ 0, 0, 192, 192 });
	checkPointStartAnim.loop = false;
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
	death = app->tex->Load("Assets/Textures/Screens/lose.png");
	win = app->tex->Load("Assets/Textures/Screens/win.png");

	checkPointIdleTex = app->tex->Load("Assets/Textures/Items/Checkpoint/checkpoint_idle.png");
	checkPointStartTex = app->tex->Load("Assets/Textures/Items/Checkpoint/checkpoint_start.png");
	checkPointTouchTex = app->tex->Load("Assets/Textures/Items/Checkpoint/checkpoint_touch.png");
	currentAnimation = &checkPointIdleAnim;
	currentTex = checkPointStartTex;

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
		app->audio->UnloadFX(app->sceneMenu->musicMenu);
		if (app->sceneMenu->startScene1 == true)
		{
			musicScene1 = app->audio->LoadFx("Assets/Audio/Music/level_music.wav");
			musicList.add(&musicScene1);
			app->sceneMenu->startScene1 = false;
		}
		app->map->Draw();
		app->audio->PlayFx(musicScene1);
		if (savePoint == true)
		{
			if (checkPointTouchAnim.finish == false)
			{
				currentAnimation = &checkPointTouchAnim;
				currentTex = checkPointTouchTex;
			}
			else if(checkPointTouchAnim.finish == true)
			{
				currentAnimation = &checkPointIdleAnim;
				currentTex = checkPointIdleTex;
			}
		}
		else
		{
			currentAnimation = &checkPointStartAnim;
			currentTex = checkPointStartTex;
		}
	}
	else if (app->screen == game_death)
	{
		app->audio->UnloadFX(musicScene1);
		savePoint = false;
		checkPointTouchAnim.Reset();
		app->player->death = false;
		app->player->position.x = 720;
		app->player->position.y = 1584;
		app->player->currentAnimation = &app->player->idleAnim;
		app->player->currentTex = app->player->idleTex;
		app->player->flip = false;
		app->player->cameraColl->rect.x = app->player->position.x - 100;
		app->player->cameraColl->rect.y = app->player->position.y - 100;
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		app->render->DrawTexture(death, 0, 0);
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			app->player->appear = true;
			app->fadeToBlack->Fade(game_scene1, 80);
			app->sceneMenu->startScene1 = true;
			app->player->lifes = 4;
			
		}
	}
	else if (app->screen == game_win)
	{
		app->audio->UnloadFX(musicScene1);
		app->player->position.x = 720;
		app->player->position.y = 1584;
		app->render->DrawTexture(win, 0, 0);
		app->player->cameraColl->rect.x = app->player->position.x - 100;
		app->player->cameraColl->rect.y = app->player->position.y - 100;
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			app->fadeToBlack->Fade(game_menu, 80);
			app->sceneMenu->startMenu = true;
			app->sceneMenu->startScene1 = true;
		}
	}
	
	// L03: DONE 7: Set the window title with map/tileset info

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

	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(currentTex, 3800, 1296, &rect);

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(death);
	app->tex->UnLoad(win);

	return true;
}
