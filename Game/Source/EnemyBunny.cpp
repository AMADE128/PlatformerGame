#include "EnemyBunny.h"

#include "App.h"
#include "Render.h"
#include "Collisions.h"
#include "ModuleParticles.h"
#include "Textures.h"
#include "ModuleEnemies.h"
#include "Player.h"
#include "Animation.h"
#include "Map.h"
#include "Audio.h"

EnemyBunny::EnemyBunny(int x, int y) : Enemy(x, y)
{
	for (int i = 0; i < 102 * 8; i += 102)
	{
		idle.PushBack({ i, 0, 102, 132 });
	}
	idle.loop = true;
	idle.speed = 0.25f;

	for (int i = 0; i < 102 * 12; i += 102)
	{
		run.PushBack({ i, 0, 102, 132 });
	}
	run.loop = true;
	run.speed = 0.25f;

	fall.PushBack({ 0, 0, 102, 132 });
	fall.loop = true;
	fall.speed = 0.25f;

	collider = app->collision->AddCollider({position.x, position.y, 80, 32 * 4 - 15 }, Collider::Type::ENEMY, (Module*)app->moduleEnemies);
}

void EnemyBunny::Update()
{
	switch (enemyState)
	{
	case Enemy::IDLE:
		currentAnim = &idle;
		texture = app->moduleEnemies->bunnyIdle;
		break;
	case Enemy::WALK:
		currentAnim = &run;
		texture = app->moduleEnemies->bunnyRun;
		break;
	case Enemy::FALL:
		currentAnim = &fall;
		texture = app->moduleEnemies->bunnyFall;
		break;
	default:
		break;
	}

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

	if (yDownCollision == true)
	{
		speedY = 0;
	}
	else
	{
		speedY += 1.0f;
	}

	if (speedY != 0)
	{
		enemyState = FALL;
	}
	else if (speedX == 0)
	{
		enemyState = IDLE;
	}

	position.y += speedY;

	collider->SetPos(position.x + 14, position.y + 20);

	Enemy::Update();

}

bool EnemyBunny::CleanUp()
{
	app->collision->RemoveCollider(collider);
	return true;
}

bool EnemyBunny::Fall(Collider* c1, Collider* c2)
{
	xRightCollision = false;
	xLeftCollision = false;
	yUpCollision = false;
	yDownCollision = false;
	return true;
}

bool EnemyBunny::StopMovementY(Collider* c1, Collider* c2)
{
	if (c1->rect.y < c2->rect.y)
	{
		yUpCollision = false;
		yDownCollision = true;
		position.y = c2->rect.y - 132 + 1;
	}
	else if (c1->rect.y > c2->rect.y)
	{
		yDownCollision = false;
		yUpCollision = true;
	}

	return true;
}

bool EnemyBunny::StopMovement(Collider* c1, Collider* c2)
{
	if (c1->rect.x < c2->rect.x && (c1->rect.y + c1->rect.h - 3) > c2->rect.y)
	{
		xRightCollision = true;
		speedX = 0.0f;
	}
	if (c1->rect.x > c2->rect.x && (c1->rect.y + c1->rect.h - 3) > c2->rect.y)
	{
		xLeftCollision = true;
		speedX = 0.0f;
	}
	return true;
}

bool EnemyBunny::Die(Collider* c1, Collider* c2)
{
	app->audio->PlayFx(app->moduleEnemies->bunnyDieFx);
	deathFinish = true;
	collider->type = collider->NONE;
	return true;
}