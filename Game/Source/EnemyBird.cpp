#include "EnemyBird.h"

#include "App.h"
#include "Render.h"
#include "Collisions.h"
#include "ModuleParticles.h"
#include "Textures.h"
#include "ModuleEnemies.h"
#include "Player.h"

EnemyBird::EnemyBird(int x, int y) : Enemy(x, y)
{
	for (int i = 0; i < TILESIZE * 9; i += TILESIZE)
	{
		idle.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	idle.loop = true;
	idle.speed = 0.25f;

	for (int i = 0; i < TILESIZE * 7; i += TILESIZE)
	{
		hit.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	hit.loop = false;
	hit.speed = 0.25f;

	collider = app->collision->AddCollider({ position.x, position.y, 80, 80 }, Collider::Type::ENEMY, (Module*)app->moduleEnemies);
}

void EnemyBird::Update()
{
	switch (enemyState)
	{
	case Enemy::IDLE:
		currentAnim = &idle;
		break;
	case Enemy::HIT:
		currentAnim = &hit;
		break;
	default:
		break;
	}

	if (follow == true)
	{
		if (position.x < app->player->position.x && (xLeftCollision == false))
		{
			enemyState = IDLE;
			flip = false;
			position.x += 2;
		}
		else if (position.x > app->player->position.x && (xRightCollision == false))
		{
			enemyState = IDLE;
			flip = true;
			position.x -= 2;
		}
		if (position.y < app->player->position.y && (yDownCollision == false || yUpCollision == false))
		{
			enemyState = IDLE;
			position.y += 2;
		}
		else if (position.y > app->player->position.y && (yDownCollision == false || yUpCollision == false))
		{
			enemyState = IDLE;
			position.y -= 2;
		}
	}

	Enemy::Update();
}

bool EnemyBird::Cleanup()
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

bool EnemyBird::OnCollision(Collider* c1, Collider* c2)
{
	enemyState = HIT;
	return true;
}