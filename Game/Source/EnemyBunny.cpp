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

	for (int i = 0; i < 102 * 5; i += 102)
	{
		hit.PushBack({ i, 0, 102, 132 });
	}
	hit.loop = false;
	hit.speed = 0.20f;

	collider = app->collision->AddCollider({position.x, position.y, 80, 32 * 4 - 15 }, Collider::Type::ENEMY, (Module*)app->moduleEnemies);
}

void EnemyBunny::Update()
{
	switch (enemyState)
	{
	case Enemy::IDLE:
		currentAnim = &idle;
		break;
	case Enemy::WALK:
		currentAnim = &run;
		break;
	case Enemy::FALL:
		currentAnim = &fall;
		break;
	case Enemy::HIT:
		currentAnim = &hit;
		break;
	default:
		break;
	}

	if (yDownCollision == true)
	{
		if (speedX == 0 && enemyState != HIT)
		{
			enemyState = IDLE;
		}
		speedY = 0;
	}
	else if (enemyState != HIT)
	{
		enemyState = FALL;
		speedY += 1.0f;
	}

	if (hit.finish == true)
	{
		deathFinish = true;
	}

	position.y += speedY;

	Enemy::Update();

}

bool EnemyBunny::Cleanup()
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
	enemyState = HIT;
	return true;
}