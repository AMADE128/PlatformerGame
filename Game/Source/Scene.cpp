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
#include "Fonts.h"
#include "ModuleParticles.h"
#include "ModuleEnemies.h"

#include <SDL_mixer\include\SDL_mixer.h>

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

	for (int i = 0; i < 96 * 17; i += 96)
	{
		appleAnim.PushBack({ i, 0, 96, 96 });
	}
	appleAnim.loop = true;
	appleAnim.speed = 0.4f;

	for (int i = 0; i < 96 * 17; i += 96)
	{
		pineappleAnim.PushBack({ i, 0, 96, 96 });
	}
	pineappleAnim.loop = true;
	pineappleAnim.speed = 0.4f;


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

	checkPointIdleTex = app->tex->Load("Assets/Textures/Items/Checkpoint/checkpoint_idle.png");
	checkPointStartTex = app->tex->Load("Assets/Textures/Items/Checkpoint/checkpoint_start.png");
	checkPointTouchTex = app->tex->Load("Assets/Textures/Items/Checkpoint/checkpoint_touch.png");
	appleTex = app->tex->Load("Assets/Textures/Items/Fruits/apple.png");
	pineappleTex = app->tex->Load("Assets/Textures/Items/Fruits/pineapple.png");

	app->audio->PlayMusic("Assets/Audio/SceneMusic/level_music.wav");

	checkpointMusic = app->audio->LoadFx("Assets/Audio/MyscMusic/checkpoint.wav");
	app->fxList.Add(&checkpointMusic);

	currentAnimation = &checkPointIdleAnim;
	currentTex = checkPointStartTex;

	checkPointColl = app->collision->AddCollider({ 3860, 1360, 20, 128 }, Collider::Type::CHECKPOINT, app->player);

	if (app->player->cont == false)
	{
		appleColl1 = app->collision->AddCollider({ 2568, 1368, 48, 48 }, Collider::Type::APPLE, app->player); apples[0] = 1;
		appleColl2 = app->collision->AddCollider({ 3800, 924, 48, 48 }, Collider::Type::APPLE, app->player); apples[1] = 1; 
		appleColl3 = app->collision->AddCollider({ 4000, 850, 48, 48 }, Collider::Type::APPLE, app->player); apples[2] = 1;
		appleColl4 = app->collision->AddCollider({ 4200, 800, 48, 48 }, Collider::Type::APPLE, app->player); apples[3] = 1;
		appleColl5 = app->collision->AddCollider({ 1392, 552, 48, 48 }, Collider::Type::APPLE, app->player); apples[4] = 1;
		pineappleColl1 = app->collision->AddCollider({ 4324, 524, 48, 48 }, Collider::Type::PINEAPPLE, app->player); apples[5] = 1;


		savePoint = false;
        checkPointTouchAnim.Reset();
	}
	else
	{
		if (apples[0] == 1) appleColl1 = app->collision->AddCollider({ 2568, 1368, 48, 48 }, Collider::Type::APPLE, app->player);
		else appleColl1 = nullptr;
		if (apples[1] == 1) appleColl2 = app->collision->AddCollider({ 3800, 924, 48, 48 }, Collider::Type::APPLE, app->player); 
		else appleColl2 = nullptr;
		if (apples[2] == 1) appleColl3 = app->collision->AddCollider({ 4000, 850, 48, 48 }, Collider::Type::APPLE, app->player);
		else appleColl3 = nullptr;
		if (apples[3] == 1) appleColl4 = app->collision->AddCollider({ 4200, 800, 48, 48 }, Collider::Type::APPLE, app->player);
		else appleColl4 = nullptr;
		if (apples[4] == 1) appleColl5 = app->collision->AddCollider({ 1392, 552, 48, 48 }, Collider::Type::APPLE, app->player);
		else appleColl5 = nullptr;
		if (apples[5] == 1) pineappleColl1 = app->collision->AddCollider({ 4324, 524, 48, 48 }, Collider::Type::PINEAPPLE, app->player);
		else pineappleColl1 = nullptr;

	}

	app->player->Init();
	app->player->Start();
	app->collision->Init();
	app->collision->Start();
	app->fonts->Init();
	app->fonts->Start();
	app->map->Init();
	app->map->Start();
	app->moduleParticles->Init();
	app->moduleParticles->Start();
	app->moduleEnemies->Init();
	app->moduleEnemies->Start();

	app->moduleEnemies->AddEnemy(EnemyType::BUNNY, 5088/2, 2880/2);
	app->moduleEnemies->AddEnemy(EnemyType::BUNNY, 5568/2, 1440/2);

	app->player->currentLvl = 1;

	if (app->player->saveGame == true)
	{
		app->player->playerLoadF6 = true;
		app->LoadGameRequest();
	}

	if (app->player->cont == false)
	{
		if (app->player->checkpointLvl1 == true && app->player->dead == false && app->player->newGame == false)
		{
			app->player->position.x = 3860;
			app->player->position.y = 1392;
		}
		else
		{
		    app->player->position.x = 720;
			app->player->position.y = 1584;
		}
	}
	if (app->player->checkpointLvl1 == true && app->player->dead == false && app->player->newGame == false)
	{
		app->player->position.x = 3860;
		app->player->position.y = 1392;
	}
	else if (app->player->startLevel == true || app->player->newGame == true)
	{
		app->player->position.x = 720;
		app->player->position.y = 1584;
		app->player->startLevel = false;
		app->player->newGame = false;
	}
	app->player->dead = false;
	app->player->saveGame = false;

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
	app->map->Draw();

	dt *= 6;
	checkPointIdleAnim.speed = 4.0f * dt;
	checkPointTouchAnim.speed = 4.0f * dt;
	appleAnim.speed = 4.0f * dt;
	pineappleAnim.speed = 4.0f * dt;
	//Draw Scenes
	if (app->player->position.x < 720 && app->player->position.y < 816 && app->player->god == false)
	{
		app->fadeToBlack->Fade(app->scene, (Module*)app->sceneLvl2, 80);
	}

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
	if (savePoint == true && checkpointFX == false)
	{
		checkpointFX = true;
		app->audio->PlayFx(checkpointMusic);
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (app->player->playerState == app->player->NORMAL)
	{
		currentAnimation->Update();
	}
	SDL_Rect checkPointRect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(currentTex, 3800, 1296, &checkPointRect);
	
	Animation* apple = &appleAnim;
	if (app->player->playerState == app->player->NORMAL)
	{
		apple->Update();
	}
	if (appleColl1 != nullptr && appleColl1->isCollected != true && apples[0] == 1)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 2544, 1344, &appleRect);
	}
	else if (appleColl1 != nullptr && appleColl1->isCollected == true) apples[0] = 0;

	if (appleColl2 != nullptr && appleColl2->isCollected != true && apples[1] == 1)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 3776, 900, &appleRect);
	}
	else if (appleColl2 != nullptr && appleColl2->isCollected == true) apples[1] = 0;

	if (appleColl3 != nullptr && appleColl3->isCollected == false && apples[2] == 1)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 3976, 836, &appleRect);
	}
	else if (appleColl3 != nullptr && appleColl3->isCollected == true) apples[2] = 0;

	if (appleColl4 != nullptr && appleColl4->isCollected == false && apples[3] == 1)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 4176, 776, &appleRect);
	}
	else if (appleColl4 != nullptr && appleColl4->isCollected == true) apples[3] = 0;

	if (appleColl5 != nullptr && appleColl5->isCollected == false && apples[4] == 1)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 1368, 528, &appleRect);
	}
	else if (appleColl5 != nullptr && appleColl5->isCollected == true) apples[4] = 0;

	Animation* pineapple = &pineappleAnim;
	if (app->player->playerState == app->player->NORMAL)
	{
		pineapple->Update();
	}
	if (pineappleColl1 != nullptr && pineappleColl1->isCollected == false && apples[5] == 1)
	{
		SDL_Rect pineappleRect1 = pineapple->GetCurrentFrame();
		app->render->DrawTexture(pineappleTex, 4300, 500, &pineappleRect1);
	}
	else if (pineappleColl1 != nullptr && pineappleColl1->isCollected == true) apples[5] = 0;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	if (!active) return true;

	LOG("Freeing scene");
	Mix_HaltMusic();

	app->tex->UnLoad(checkPointIdleTex);
	app->tex->UnLoad(checkPointStartTex);
	app->tex->UnLoad(checkPointTouchTex);
	app->tex->UnLoad(appleTex);
	app->tex->UnLoad(pineappleTex);

	app->collision->RemoveCollider(appleColl1);
	app->collision->RemoveCollider(appleColl2);
	app->collision->RemoveCollider(appleColl3);
	app->collision->RemoveCollider(appleColl4);
	app->collision->RemoveCollider(appleColl5);
	app->collision->RemoveCollider(pineappleColl1);
	app->collision->RemoveCollider(checkPointColl);

	app->audio->UnloadFX(checkpointMusic);
	app->fxList.Clear();

	app->player->CleanUp();
	app->collision->CleanUp();
	app->fonts->CleanUp();
	app->moduleEnemies->CleanUp();
	app->moduleParticles->CleanUp();
	app->map->CleanUp();

	active = false;

	return true;
}

