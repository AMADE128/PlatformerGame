#include "EnemyBird.h"

#include "App.h"
#include "Render.h"
#include "Collisions.h"
#include "ModuleParticles.h"
#include "Textures.h"
#include "ModuleEnemies.h"
#include "Player.h"
#include "Map.h"
#include "Audio.h"

EnemyBird::EnemyBird(int x, int y) : Enemy(x, y)
{
	for (int i = 0; i < TILESIZE * 9; i += TILESIZE)
	{
		idle.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	idle.loop = true;
	idle.speed = 0.25f;

	collider = app->collision->AddCollider({ position.x, position.y, 80, 80 }, Collider::Type::ENEMY, (Module*)app->moduleEnemies);
}

void EnemyBird::Update()
{
	currentAnim = &idle;
	texture = app->moduleEnemies->birdFly;

	if (position.DistanceManhattan(app->player->position) < 400)
	{
		iPoint mapPositionEnemy = app->map->WorldToMap(position.x, position.y);
		Pathfinding(position.x, position.y);
		int j = app->moduleEnemies->GetCurrentPositionInPath(mapPositionEnemy);
		if (app->moduleEnemies->lastPath != nullptr)
		{
			if (app->moduleEnemies->lastPath->At(j + 1) != NULL)
			{
				iPoint nextPositionEnemy = *app->moduleEnemies->lastPath->At(j + 1);
				iPoint nextAuxPositionEenemy = app->moduleEnemies->MapToWorld(nextPositionEnemy);
				MoveEnemy(position, nextAuxPositionEenemy, mapPositionEnemy, enemyType);
			}
		}
	}

	collider->SetPos(position.x + 10, position.y + 10);

	Enemy::Update();
}

bool EnemyBird::CleanUp()
{
	app->collision->RemoveCollider(collider);
	return true;
}

bool EnemyBird::Fall(Collider* c1, Collider* c2)
{
	yDownCollision = false;
	xLeftCollision = false;
	xRightCollision = false;
	return true;
}

bool EnemyBird::StopMovementY(Collider* c1, Collider* c2)
{
	if (c1->rect.y < c2->rect.y)
	{
		yUpCollision = false;
		yDownCollision = true;
	}
	else if (c1->rect.y > c2->rect.y)
	{
		yDownCollision = false;
		yUpCollision = true;
	}

	return true;
}

bool EnemyBird::StopMovement(Collider* c1, Collider* c2)
{
	if (c1->rect.x < c2->rect.x + c2->rect.w)
	{
		xRightCollision = true;
		c1->rect.x += 1;
		speedX = 0.f;
	}
	if (c1->rect.x > c2->rect.x)
	{
		xLeftCollision = true;
		c1->rect.x -= 1;
		speedX = 0.f;
	}
	return true;
}

bool EnemyBird::Die(Collider* c1, Collider* c2)
{
	app->audio->PlayFx(app->moduleEnemies->birdDieFx);
	deathFinish = true;
	collider->type = collider->NONE;
	return true;
}