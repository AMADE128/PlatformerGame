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
	currentAnim = &idle;

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
	if (c1->rect.x < c2->rect.x && (c1->rect.y + collider->rect.h - 3) > c2->rect.y)
	{
		xRightCollision = true;
		speedX = 0.f;
	}
	if (c1->rect.x > c2->rect.x && (c1->rect.y + collider->rect.h - 3) > c2->rect.y)
	{
		xLeftCollision = true;
		speedX = 0.f;
	}
	return true;
}