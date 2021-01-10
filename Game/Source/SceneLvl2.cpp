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

#include <SDL_mixer\include\SDL_mixer.h>

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

	app->audio->PlayMusic("Assets/Audio/SceneMusic/level_music.wav");

	checkpointMusic = app->audio->LoadFx("Assets/Audio/MyscMusic/checkpoint.wav");
	app->fxList.Add(&checkpointMusic);

	currentAnimation = &checkPointIdleAnim;
	currentTex = checkPointStartTex;

	checkPointColl = app->collision->AddCollider({ 3204, 1888, 20, 128 }, Collider::Type::CHECKPOINT, app->player);

	if (app->player->cont == false)
	{
		appleColl1Lvl2 = app->collision->AddCollider({ 1128, 2040, 48, 48 }, Collider::Type::APPLE, app->player); apples2[0] = 1;
		appleColl2Lvl2 = app->collision->AddCollider({ 2328, 1944, 48, 48 }, Collider::Type::APPLE, app->player); apples2[1] = 1;
		appleColl3Lvl2 = app->collision->AddCollider({ 3408, 1488, 48, 48 }, Collider::Type::APPLE, app->player); apples2[2] = 1;
		appleColl4Lvl2 = app->collision->AddCollider({ 2400, 1296, 48, 48 }, Collider::Type::APPLE, app->player); apples2[3] = 1;
		appleColl5Lvl2 = app->collision->AddCollider({ 2400, 816, 48, 48 }, Collider::Type::APPLE, app->player); apples2[4] = 1;
		appleColl6Lvl2 = app->collision->AddCollider({ 3408, 2112, 48, 48 }, Collider::Type::APPLE, app->player); apples2[5] = 1;
		pineappleColl1Lvl2 = app->collision->AddCollider({ 3120, 576, 48, 48 }, Collider::Type::PINEAPPLE, app->player); apples2[6] = 1;

		savePoint = false;
		checkPointTouchAnim.Reset();
	}
	else
	{
		if (apples2[0] == 1) appleColl1Lvl2 = app->collision->AddCollider({ 1128, 2040, 48, 48 }, Collider::Type::APPLE, app->player);
		if (apples2[1] == 1) appleColl2Lvl2 = app->collision->AddCollider({ 2328, 1944, 48, 48 }, Collider::Type::APPLE, app->player);
		if (apples2[2] == 1) appleColl3Lvl2 = app->collision->AddCollider({ 3408, 1488, 48, 48 }, Collider::Type::APPLE, app->player);
		if (apples2[3] == 1) appleColl4Lvl2 = app->collision->AddCollider({ 2400, 1296, 48, 48 }, Collider::Type::APPLE, app->player);
		if (apples2[4] == 1) appleColl5Lvl2 = app->collision->AddCollider({ 2400, 816, 48, 48 }, Collider::Type::APPLE, app->player);
		if (apples2[5] == 1) appleColl6Lvl2 = app->collision->AddCollider({ 3408, 2112, 48, 48 }, Collider::Type::APPLE, app->player);
		if (apples2[6] == 1) pineappleColl1Lvl2 = app->collision->AddCollider({ 3120, 576, 48, 48 }, Collider::Type::PINEAPPLE, app->player);
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

	app->moduleEnemies->AddEnemy(EnemyType::BIRD, 5568 / 2, 2592 / 2);
	app->moduleEnemies->AddEnemy(EnemyType::BIRD, 5472 / 2, 1152 / 2);
	app->moduleEnemies->AddEnemy(EnemyType::BUNNY, 5952 / 2, 4032 / 2);
	app->moduleEnemies->AddEnemy(EnemyType::BUNNY, 7872 / 2, 3072 / 2);
	
	app->player->currentLvl = 2;

	if (app->player->saveGame == true)
	{
		app->player->playerLoadF6 = true;
		app->LoadGameRequest();
	}

	if (app->player->cont == false)
	{	
		if(app->player->checkpointLvl2 == true && app->player->dead == false)
		{
			app->player->position.x = 3216;
			app->player->position.y = 1920;
		}
		else
		{
			app->player->position.x = 620;
			app->player->position.y = 2256;
		}
	}
	if (app->player->checkpointLvl2 == true && app->player->dead == false && app->player->newGame == false)
	{
		app->player->position.x = 3216;
		app->player->position.y = 1920;
	}
	else if (app->player->startLevel == true || app->player->newGame == true)
	{
		app->player->position.x = 620;
		app->player->position.y = 2256;
		app->player->startLevel = false;
	}
	app->player->dead = false;
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
		app->player->setScore = true;
		app->player->newHighScore = true;
		app->player->score = app->player->secondsLvl + (app->player->minutesLvl * 60);
		if (app->player->score < app->player->highScore)
		{
			app->player->highScore = app->player->score;
			app->SaveGameRequest();
		}
		app->fadeToBlack->Fade(app->sceneLvl2, (Module*)app->sceneWin, 80);
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

	if (app->player->playerState == app->player->NORMAL)
	{
		currentAnimation->Update();
	}
	SDL_Rect checkPointRect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(currentTex, 3144, 1824, &checkPointRect);                     

	Animation* apple = &appleAnim;
	if (app->player->playerState == app->player->NORMAL)
	{
		apple->Update();
	}
	if (appleColl1Lvl2 != nullptr && appleColl1Lvl2->isCollected != true && apples2[0] == 1)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 1104, 2016, &appleRect);                     
	}
	else if (appleColl1Lvl2 != nullptr && appleColl1Lvl2->isCollected == true) apples2[0] = 0;

	if (appleColl2Lvl2 != nullptr && appleColl2Lvl2->isCollected != true && apples2[1] == 1)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 2304, 1920, &appleRect);               
	}
	else if (appleColl2Lvl2 != nullptr && appleColl2Lvl2->isCollected == true) apples2[1] = 0;

	if (appleColl3Lvl2 != nullptr && appleColl3Lvl2->isCollected!= true && apples2[2] == 1)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();   
		app->render->DrawTexture(appleTex, 3384, 1464, &appleRect);
	}
	else if (appleColl3Lvl2 != nullptr && appleColl3Lvl2->isCollected == true) apples2[2] = 0;

	if (appleColl4Lvl2 != nullptr && appleColl4Lvl2->isCollected != true && apples2[3] == 1)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 2386, 1272, &appleRect);             
	}
	else if (appleColl4Lvl2 != nullptr && appleColl4Lvl2->isCollected == true) apples2[3] = 0;

	if (appleColl5Lvl2 != nullptr && appleColl5Lvl2->isCollected != true && apples2[4] == 1)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 2386, 792, &appleRect);  
	}
	else if (appleColl5Lvl2 != nullptr && appleColl5Lvl2->isCollected == true) apples2[4] = 0;

	if (appleColl6Lvl2 != nullptr && appleColl6Lvl2->isCollected != true && apples2[5] == 1)
	{
		SDL_Rect appleRect = apple->GetCurrentFrame();
		app->render->DrawTexture(appleTex, 3384, 2088, &appleRect);
	}
	else if (appleColl6Lvl2 != nullptr && appleColl6Lvl2->isCollected == true) apples2[5] = 0;

	Animation* pineapple = &pineappleAnim;
	if (app->player->playerState == app->player->NORMAL)
	{
		pineapple->Update();
	}
	if (pineappleColl1Lvl2 != nullptr && pineappleColl1Lvl2->isCollected != true && apples2[6] == 1)
	{
		SDL_Rect pineappleRect1 = pineapple->GetCurrentFrame();
		app->render->DrawTexture(pineappleTex, 3094, 552, &pineappleRect1);
	}
	else if (pineappleColl1Lvl2 != nullptr && pineappleColl1Lvl2->isCollected == true) apples2[6] = 0;
	return ret;
}

// Called before quitting
bool SceneLvl2::CleanUp()
{
	if (!active) return true;

	LOG("Freeing scene");
	Mix_HaltMusic();

	app->tex->UnLoad(checkPointIdleTex);
	app->tex->UnLoad(checkPointStartTex);
	app->tex->UnLoad(checkPointTouchTex);
	app->tex->UnLoad(appleTex);
	app->tex->UnLoad(pineappleTex);

	app->collision->RemoveCollider(appleColl1Lvl2);
	app->collision->RemoveCollider(appleColl2Lvl2);
	app->collision->RemoveCollider(appleColl3Lvl2);
	app->collision->RemoveCollider(appleColl4Lvl2);
	app->collision->RemoveCollider(appleColl5Lvl2);
	app->collision->RemoveCollider(appleColl6Lvl2);
	app->collision->RemoveCollider(pineappleColl1Lvl2);
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
