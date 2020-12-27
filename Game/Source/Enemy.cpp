#include "Enemy.h"

#include "App.h"
#include "Collisions.h"
#include "ModuleParticles.h"
#include "Audio.h"
#include "Render.h"
#include "Player.h"
#include "SDL/include/SDL.h"
#include "Map.h"

Enemy::Enemy(int x, int y) : position(x, y)
{
	spawnPos = position;
}

Enemy::~Enemy()
{
	if (collider != nullptr) collider->pendingToDelete = true;
}

const Collider* Enemy::GetCollider() const
{
	return collider;
}

void Enemy::Update()
{
	if (currentAnim != nullptr)
		currentAnim->Update();
}

void Enemy::Draw()
{
	if (currentAnim != nullptr)
	{
		if (flip == true)
		{
			app->render->DrawTexture(texture, position.x, position.y, &(currentAnim->GetCurrentFrame()));
		}
		else
		{
			app->render->DrawTextureFlip(texture, position.x, position.y, &(currentAnim->GetCurrentFrame()));
		}
	}
}

bool Enemy::OnCollision(Collider* c1, Collider* c2)
{
	return true;
}

bool Enemy::StopMovement(Collider* c1, Collider* c2)
{
	return true;
}

bool Enemy::StopMovementY(Collider* c1, Collider* c2)
{
	return true;
}

bool Enemy::Fall(Collider* c1, Collider* c2)
{
	return true;
}

void Enemy::Pathfinding(int positionx, int positiony)
{
	iPoint mapPositionEnemy = app->map->WorldToMap(positionx, positiony);
	iPoint worldPositionPlayer = app->player->position;
	iPoint mapPositionPlayer = app->map->WorldToMap(worldPositionPlayer.x, worldPositionPlayer.y);
	if (app->player->death != true)app->moduleEnemies->CreatePathEnemy(mapPositionEnemy, mapPositionPlayer);
}

void Enemy::MoveEnemy(iPoint position, iPoint nextAuxPositionEenemy, iPoint mapPositionEnemy, EnemyType type)
{
	int positionEnemyX = position.x;
	int positionEnemyY = position.y;
	int velocity = 2;
	if (type == EnemyType::BUNNY)
	{
		if (nextAuxPositionEenemy.x < positionEnemyX)
		{
			position.x -= velocity;
		}
		else if (nextAuxPositionEenemy.x > positionEnemyX)
		{
			position.x += velocity;
		}
	}
	else if (type == EnemyType::BIRD)
	{
		velocity = 2;
		if (nextAuxPositionEenemy.x < positionEnemyX)
		{
			position.x -= velocity;
		}
		else if (nextAuxPositionEenemy.x > positionEnemyX)
		{
			position.x += velocity;
		}
		if (nextAuxPositionEenemy.y < positionEnemyY)
		{
			position.y -= velocity;
		}
		else if (nextAuxPositionEenemy.y > positionEnemyY)
		{
			position.y += velocity;
		}
	}
}