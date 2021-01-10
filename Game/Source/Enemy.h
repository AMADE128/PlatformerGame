#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Point.h"
#include "Animation.h"
#include "ModuleEnemies.h"
#include "DynArray.h"

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

	virtual bool CleanUp();

	// Collision response
	// Triggers an animation and a sound fx
	virtual bool Die(Collider* c1, Collider* c2);

	virtual bool StopMovement(Collider* c1, Collider* c2);

	virtual bool StopMovementY(Collider* c1, Collider* c2);

	virtual bool Fall(Collider* c1, Collider* c2);

	virtual void Pathfinding(int enemyposx, int enemyposy);

	virtual void MoveEnemy(iPoint position, iPoint nextAuxPositionEenemy, iPoint mapPositionEnemy, EnemyType type);

	enum State {
		IDLE = 0,
		WALK,
		JUMP,
		FALL
	};

public:
	// The current position in the world
	iPoint position;

	// The enemy's texture
	SDL_Texture* texture = nullptr;
	bool flip;

	// Sound fx when destroyed
	int destroyedFx = 0;

	// A ptr to the current animation
	Animation* currentAnim = nullptr;

	// The enemy's collider
	Collider* collider = nullptr;

	State enemyState;
	EnemyType enemyType;

	DynArray<iPoint> enemyPath;
	int i = 0;

	float gravity = 0.0f;

	bool yDownCollision;
	bool yUpCollision;
	bool xLeftCollision;
	bool xRightCollision;
	float speedX;
	float speedY;

	bool deathFinish = false;

protected:
	// Original spawn position. Stored for movement calculations
	iPoint spawnPos;

};

#endif // __ENEMY_H__