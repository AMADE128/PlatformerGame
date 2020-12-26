#include "Enemy.h"

#include "App.h"
#include "Collisions.h"
#include "ModuleParticles.h"
#include "Audio.h"
#include "Render.h"
#include "Player.h"
#include "SDL/include/SDL.h"

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