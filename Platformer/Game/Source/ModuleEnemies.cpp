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
#include "Point.h"
#include "Map.h"

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

	birdFly = app->tex->Load("Assets/Textures/Enemies/Bird/flying.png");
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
		{
			switch (enemies[i]->enemyType)
			{
			case EnemyType::NO_TYPE:
				break;
			case EnemyType::BIRD:
				switch (enemies[i]->enemyState)
				{
				case Enemy::IDLE:
					enemies[i]->texture = birdFly;
					break;
				case Enemy::HIT:
					enemies[i]->texture = birdHit;
					break;
				default:
					break;
				}
				enemies[i]->collider->SetPos(enemies[i]->position.x + 10, enemies[i]->position.y + 10);
				break;
			case EnemyType::BUNNY:
				switch (enemies[i]->enemyState)
				{
				case Enemy::IDLE:
					enemies[i]->texture = bunnyIdle;
					break;
				case Enemy::WALK:
					enemies[i]->texture = bunnyRun;
					break;
				case Enemy::JUMP:
					enemies[i]->texture = bunnyJump;
					break;
				case Enemy::FALL:
					enemies[i]->texture = bunnyFall;
					break;
				case Enemy::HIT:
					enemies[i]->texture = bunnyHit;
					break;
				default:
					break;
				}
				enemies[i]->collider->SetPos(enemies[i]->position.x + 14, enemies[i]->position.y + 20);
				break;
			default:
				break;
			}
			enemies[i]->Update();
			if (enemies[i] != nullptr)
			{
				if (enemies[i]->position.DistanceManhattan(app->player->position) < 400)
				{
					enemies[i]->follow = true;
				}
				else
				{
					enemies[i]->follow = false;
				}
			}
		}
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
				enemies[i]->enemyType = info.type;
				break;
			case EnemyType::BUNNY:
				enemies[i] = new EnemyBunny(info.x, info.y);
				enemies[i]->enemyType = info.type;
				break;
			}
			break;
		}
	}
}

bool ModuleEnemies::Die(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			enemies[i]->OnCollision(c1, c2);

			app->collision->RemoveCollider(enemies[i]->collider);

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
			enemies[i]->yDownCollision = false;
			enemies[i]->xLeftCollision = false;
			enemies[i]->xRightCollision = false;
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
			enemies[i]->StopMovement(c1, c2); //Notify the enemy of a collision
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
			enemies[i]->StopMovementY(c1, c2); //Notify the enemy of a collision
			break;
		}
	}

	return ret;
}