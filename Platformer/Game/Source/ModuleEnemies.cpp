#include "ModuleEnemies.h"

#include "App.h"

#include "Collisions.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "FadeToBlack.h"
#include "Player.h"
#include "Fonts.h"
#include "Log.h"
#include "Window.h"
#include <stdio.h>

#include "Enemy.h"
#include "EnemyBunny.h"
#include "EnemyBird.h"

#include "SDL/include/SDL.h"


ModuleEnemies::ModuleEnemies() : Module()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

ModuleEnemies::~ModuleEnemies()
{

}

bool ModuleEnemies::Start()
{
	bunnyIdle = app->tex->Load("Assets/Textures/Enemies/Bunny/idle.png");
	bunnyJump = app->tex->Load("Assets/Textures/Enemies/Bunny/jump.png");
	bunnyRun = app->tex->Load("Assets/Textures/Enemies/Bunny/run.png");
	bunnyFall = app->tex->Load("Assets/Textures/Enemies/Bunny/fall.png");
	bunnyHit = app->tex->Load("Assets/Textures/Enemies/Bunny/hit.png");

	birdFly = app->tex->Load("Assets/Textures/Enemies/Bird/fly.png");
	birdHit = app->tex->Load("Assets/Textures/Enemies/Bird/hit.png");
		 



	return true;
}

bool ModuleEnemies::Update(float dt)
{
	bool ret = true;
	EnemiesSpawn();

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
			enemies[i]->Update();
	}

	EnemiesDespawn();

	return ret;
}

bool ModuleEnemies::PostUpdate()
{
	bool ret = true;
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->death == false)
		{
			enemies[i]->Draw();
		}
	}

	return ret;
}

// Called before quitting
bool ModuleEnemies::CleanUp()
{
	LOG("Freeing all enemies");

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	app->tex->UnLoad(bunnyFall);
	app->tex->UnLoad(bunnyHit);
	app->tex->UnLoad(bunnyIdle);
	app->tex->UnLoad(bunnyJump);
	app->tex->UnLoad(bunnyRun);
	app->tex->UnLoad(birdFly);
	app->tex->UnLoad(birdHit);

	return true;
}

bool ModuleEnemies::AddEnemy(EnemyType type, int x, int y)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (spawnQueue[i].type == EnemyType::NO_TYPE)
		{
			spawnQueue[i].type = type;
			spawnQueue[i].x = x;
			spawnQueue[i].y = y;
			ret = true;
			break;
		}
	}

	return ret;
}

void ModuleEnemies::EnemiesSpawn()
{
	// Iterate all the enemies queue
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (spawnQueue[i].type != EnemyType::NO_TYPE)
		{
			SpawnEnemy(spawnQueue[i]);
			spawnQueue[i].type = EnemyType::NO_TYPE;
		}
	}
}

void ModuleEnemies::EnemiesDespawn()
{
	//// Iterate existing enemies
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->death == true)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}
}

void ModuleEnemies::SpawnEnemy(const EnemySpawnpoint& info)
{
	// Find an empty slot in the enemies array
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] == nullptr)
		{
			switch (info.type)
			{
			case EnemyType::BIRD:
				enemies[i] = new EnemyBird(info.x, info.y);
				enemies[i]->texture = birdFly;
				break;
			case EnemyType::BUNNY:
				enemies[i] = new EnemyBunny(info.x, info.y);
				enemies[i]->texture = bunnyIdle;
				break;
			}
			break;
		}
	}
}

bool ModuleEnemies::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->GetCollider() == c1)
		{
			enemies[i]->OnCollision(c2); //Notify the enemy of a collision

			delete enemies[i];
			enemies[i] = nullptr;
			break;
		}
	}

	return true;
}

bool ModuleEnemies::Fall(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->GetCollider() == c1)
		{
			enemies[i]->Fall(c2); //Notify the enemy of a collision
			break;
		}
	}

	return true;
}

bool ModuleEnemies::StopMovement(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->GetCollider() == c1)
		{
			enemies[i]->StopMovement(c2); //Notify the enemy of a collision
			break;
		}
	}

	return true;
}

bool ModuleEnemies::StopMovementY(Collider* c1, Collider* c2)
{
	bool ret = true;
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->GetCollider() == c1)
		{
			enemies[i]->StopMovementY(c2); //Notify the enemy of a collision
			break;
		}
	}

	return ret;
}