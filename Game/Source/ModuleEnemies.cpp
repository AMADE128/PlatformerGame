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
#include "Pathfinding.h"

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
			if (app->player->playerState == app->player->NORMAL)
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
				if (enemies[i] != nullptr)
				{
					if (enemies[i]->position.DistanceManhattan(app->player->position) < 400)
					{
						switch (enemies[i]->enemyType)
						{
						case EnemyType::BUNNY:
						{
							iPoint mapPositionEnemy = app->map->WorldToMap(enemies[i]->position.x, enemies[i]->position.y);
							enemies[i]->Pathfinding(enemies[i]->position.x, enemies[i]->position.y);
							int j = GetCurrentPositionInPath(mapPositionEnemy);
							if (lastPath->At(j + 1) != NULL)
							{
								iPoint nextPositionEnemy = *lastPath->At(j + 1);
								iPoint nextAuxPositionEenemy = MapToWorld(nextPositionEnemy);
								enemies[i]->MoveEnemy(enemies[i]->position, nextAuxPositionEenemy, mapPositionEnemy, enemies[i]->enemyType);
							}
						}
						case EnemyType::BIRD:
						{
							iPoint mapPositionEnemy = app->map->WorldToMap(enemies[i]->position.x, enemies[i]->position.y);
							enemies[i]->Pathfinding(enemies[i]->position.x, enemies[i]->position.y);
							int j = GetCurrentPositionInPath(mapPositionEnemy);
							if (lastPath->At(j + 1) != NULL)
							{
								iPoint nextPositionEnemy = *lastPath->At(j + 1);
								iPoint nextAuxPositionEenemy = MapToWorld(nextPositionEnemy);
								enemies[i]->MoveEnemy(enemies[i]->position, nextAuxPositionEenemy, mapPositionEnemy, enemies[i]->enemyType);
							}
							enemies[i]->currentAnim->Update();
						}
						default:
							break;
						}
					}
				}

				enemies[i]->Update();

				if (enemies[i]->deathFinish == true)
				{
					app->collision->RemoveCollider(enemies[i]->collider);
					delete enemies[i];
					enemies[i] = nullptr;
				}
			}
		
			enemies[i]->Draw();
		}
	}

	EnemiesDespawn();

	return ret;
}

bool ModuleEnemies::PostUpdate()
{
	bool ret = true;

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
		if (enemies[i] != nullptr && enemies[i]->GetCollider() == c1)
		{
			enemies[i]->Die(c1, c2);
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

void ModuleEnemies::CreatePathEnemy(iPoint mapPositionEnemy, iPoint mapPositionDestination)
{
	if (checkDestination->Time(1000))
	{
		//destination != mapPositionDestination
		app->pathfinding->ResetPath(mapPositionEnemy);
		checkDestination->Start();
		app->pathfinding->ComputePathAStar(mapPositionEnemy, mapPositionDestination);
		lastPath = app->pathfinding->GetLastPath();
	}
}

int ModuleEnemies::GetCurrentPositionInPath(iPoint mapPositionEnemy)
{
	int i;
	for (i = 0; i < lastPath->Count(); i++)
	{
		if (mapPositionEnemy == iPoint({ lastPath->At(i)->x, lastPath->At(i)->y })) break;
	}
	return i;
}

iPoint ModuleEnemies::MapToWorld(iPoint position)
{
	iPoint mapDimensions = app->map->GetDimensionsMap();
	iPoint ret;
	ret.x = position.x * mapDimensions.x;
	ret.y = position.y * mapDimensions.y;
	return ret;
}