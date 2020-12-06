#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Point.h"
#include "Animation.h"
#include "ModuleEnemies.h"

struct SDL_Texture;
struct Collider;
enum class EnemyType;

class Enemy
{
public:
	// Constructor
	// Saves the spawn position for later movement calculations
	Enemy(int x, int y);

	// Destructor
	virtual ~Enemy();

	// Returns the enemy's collider
	const Collider* GetCollider() const;

	// Called from inhering enemies' Udpate
	// Updates animation and collider position
	virtual void Update();

	// Called from ModuleEnemies' Update
	virtual void Draw();

	// Collision response
	// Triggers an animation and a sound fx
	virtual bool OnCollision(Collider* c1, Collider* c2);

	virtual bool StopMovement(Collider* c1, Collider* c2);

	virtual bool StopMovementY(Collider* c1, Collider* c2);

	virtual bool Fall(Collider* c1, Collider* c2);

	enum State {
		IDLE = 0,
		WALK,
		JUMP,
		FALL,
		HIT
	};

public:
	// The current position in the world
	iPoint position;

	// The enemy's texture
	SDL_Texture* texture = nullptr;

	// Sound fx when destroyed
	int destroyedFx = 0;

	// A ptr to the current animation
	Animation* currentAnim = nullptr;

	// The enemy's collider
	Collider* collider = nullptr;

	State enemyState;
	EnemyType enemyType;

	float gravity = 0.0f;

	bool death = false;
protected:


	// Original spawn position. Stored for movement calculations
	iPoint spawnPos;

};

#endif // __ENEMY_H__