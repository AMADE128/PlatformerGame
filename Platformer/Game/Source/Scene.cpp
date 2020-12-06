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

	musicScene1 = app->audio->LoadFx("Assets/Audio/SceneMusic/level_music.wav");
	app->musicList.Add(&musicScene1);
	app->audio->PlayFx(musicScene1, -1);

	checkpointMusic = app->audio->LoadFx("Assets/Audio/MyscMusic/checkpoint.wav");
	app->musicList.Add(&checkpointMusic);

	currentAnimation = &checkPointIdleAnim;
	currentTex = checkPointStartTex;

	checkPointColl = app->collision->AddCollider({ 3860, 1360, 20, 128 }, Collider::Type::CHECKPOINT, app->player);
	appleColl2 = app->collision->AddCollider({ 3800, 924, 48, 48 }, Collider::Type::APPLE, app->player);
	appleColl3 = app->collision->AddCollider({ 4000, 850, 48, 48 }, Collider::Type::APPLE, app->player);
	appleColl4 = app->collision->AddCollider({ 4200, 800, 48, 48 }, Collider::Type::APPLE, app->player);
	appleColl5 = app->collision->AddCollider({ 1392, 552, 48, 48 }, Collider::Type::APPLE, app->player);
	pineappleColl1 = app->collision->AddCollider({ 4324, 524, 48, 48 }, Collider::Type::PINEAPPLE, app->player);
	appleColl1 = app->collision->AddCollider({ 2568, 1368, 48, 48 }, Collider::Type::APPLE, app->player);

	savePoint = false;
	app->player->appleCounter = 0;
	app->player->lifes = 3;
	checkPointTouchAnim.Reset();

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

	app->player->lvl = 1;

	if (app->player->saveGame == true)
	{
		app->LoadGameRequest();
	}

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
	dt *= 6;
	checkPointIdleAnim.speed = 4.0f * dt;
	checkPointTouchAnim.speed = 4.0f * dt;
	appleAnim.speed = 4.0f * dt;
	pineappleAnim.speed = 4.0f * dt;
	//Draw Scenes
	if (app->player->position.x < 720 && app->player->position.y < 816 && app->player->god == false)
	{
		app->fadeToBlack->Fade(app->scene, (Module*)app->sceneLvl2, 1600 * dt);
	}

	app->map->Draw();
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

	currentAnimation->Update();
	SDL_Rect checkPointRect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(currentTex, 3800, 1296, &checkPointRect);
	
	Animation* apple = &appleAnim;
	apple->Update();
	if (appleColl1->isCollected != true)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 2544, 1344, &appleRect);
	}

	if (appleColl2->isCollected != true)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 3776, 900, &appleRect);
	}

	if (appleColl3->isCollected == false)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 3976, 836, &appleRect);
	}

	if (appleColl4->isCollected == false)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 4176, 776, &appleRect);
	}

	if (appleColl5->isCollected == false)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 1368, 528, &appleRect);
	}

	Animation* pineapple = &pineappleAnim;
	pineapple->Update();
	if (pineappleColl1->isCollected == false)
	{
		SDL_Rect pineappleRect1 = pineapple->GetCurrentFrame();
		app->render->DrawTexture(pineappleTex, 4300, 500, &pineappleRect1);
	}

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	if (!active) return true;

	LOG("Freeing scene");
	app->tex->UnLoad(checkPointIdleTex);
	app->tex->UnLoad(checkPointStartTex);
	app->tex->UnLoad(checkPointTouchTex);
	app->tex->UnLoad(appleTex);
	app->tex->UnLoad(pineappleTex);

	app->musicList.Clear();
	app->audio->UnloadFX(musicScene1);
	app->audio->UnloadFX(checkpointMusic);

	app->player->CleanUp();
	app->collision->CleanUp();
	app->fonts->CleanUp();
	app->moduleEnemies->CleanUp();
	app->moduleParticles->CleanUp();
	app->map->CleanUp();

	active = false;

	return true;
}
