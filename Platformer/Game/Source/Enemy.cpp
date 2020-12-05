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

	if (collider != nullptr)
		collider->SetPos(position.x, position.y);
}

void Enemy::Draw()
{
	if (currentAnim != nullptr)
	{
		app->render->DrawTexture(texture, position.x, position.y, &(currentAnim->GetCurrentFrame()));
	}
}

void Enemy::OnCollision(Collider* collider)
{

}

void Enemy::StopMovement(Collider* collider) {

}

void Enemy::StopMovementY(Collider* collider) {

}

void Enemy::Fall(Collider* collider) {

}