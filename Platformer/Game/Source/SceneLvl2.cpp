#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneLvl2.h"
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

SceneLvl2::SceneLvl2() : Module()
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
SceneLvl2::~SceneLvl2()
{}

// Called before render is available
bool SceneLvl2::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneLvl2::Start()
{
	// L03: DONE: Load map
	app->map->Load("map2.tmx");

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

	checkPointColl = app->collision->AddCollider({ 3204, 1888, 20, 128 }, Collider::Type::CHECKPOINT, app->player);
	appleColl2 = app->collision->AddCollider({ 2328, 1944, 48, 48 }, Collider::Type::APPLE, app->player);
	appleColl3 = app->collision->AddCollider({ 3408, 1488, 48, 48 }, Collider::Type::APPLE, app->player);           
	appleColl4 = app->collision->AddCollider({ 2400, 1296, 48, 48 }, Collider::Type::APPLE, app->player);
	appleColl5 = app->collision->AddCollider({ 2400, 816, 48, 48 }, Collider::Type::APPLE, app->player);
    pineappleColl1 = app->collision->AddCollider({ 3120, 576, 48, 48 }, Collider::Type::PINEAPPLE, app->player);
	appleColl1 = app->collision->AddCollider({ 1128, 2040, 48, 48 }, Collider::Type::APPLE, app->player);
	appleColl6 = app->collision->AddCollider({ 3408, 2112, 48 }, Collider::Type::APPLE, app->player);

	savePoint = false;
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

	app->moduleEnemies->AddEnemy(EnemyType::BUNNY, 5952/2, 4032/2);
	app->moduleEnemies->AddEnemy(EnemyType::BUNNY, 7872/2, 3072/2);
	app->moduleEnemies->AddEnemy(EnemyType::BIRD, 5568/2, 2592/2);
	app->moduleEnemies->AddEnemy(EnemyType::BIRD, 5472/2, 1152/2);

	app->player->lvl = 2;
	
	if (app->player->saveGame == true)
	{
		app->LoadGameRequest();
	}

	app->player->saveGame = false;

	return true;
}

// Called each loop iteration
bool SceneLvl2::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneLvl2::Update(float dt)
{
	if (app->player->position.x > 4704 && app->player->position.y > 2256)
	{
		app->fadeToBlack->Fade(app->sceneLvl2, (Module*)app->sceneWin, 1600 * dt);
	}

	//Draw Scenes
	app->map->Draw();
	if (savePoint == true)
	{
		if (checkPointTouchAnim.finish == false)
		{
			currentAnimation = &checkPointTouchAnim;
			currentTex = checkPointTouchTex;
		}
		else if (checkPointTouchAnim.finish == true)
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
bool SceneLvl2::PostUpdate()
{
	bool ret = true;

	currentAnimation->Update();
	SDL_Rect checkPointRect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(currentTex, 3144, 1824, &checkPointRect);                     

	Animation* apple = &appleAnim;
	apple->Update();
	if (appleColl1->isCollected != true)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 1104, 2016, &appleRect);                     
	}

	if (appleColl2->isCollected != true)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 2304, 1920, &appleRect);               
	}

	if (appleColl3->isCollected == false)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();   
		app->render->DrawTexture(appleTex, 3384, 1464, &appleRect);
	}

	if (appleColl4->isCollected == false)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 2386, 1272, &appleRect);             
	}

	if (appleColl5->isCollected == false)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 2386, 792, &appleRect);  
	}

	if (appleColl6->isCollected == false)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 3384, 2088, &appleRect);
	}

	Animation* pineapple = &pineappleAnim;
	pineapple->Update();
	if (pineappleColl1->isCollected == false)
	{
		SDL_Rect pineappleRect1 = pineapple->GetCurrentFrame();
		app->render->DrawTexture(pineappleTex, 3094, 552, &pineappleRect1);
	}

	return ret;
}

// Called before quitting
bool SceneLvl2::CleanUp()
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
