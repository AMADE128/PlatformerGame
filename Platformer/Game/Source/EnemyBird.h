#ifndef __ENEMYBIRD__
#define __ENEMYBIRD__

#include "Enemy.h"
#include "Path.h"

class EnemyBird : public Enemy
{
public :

	enum State
	{
		IDLE = 0,
		RUN,
		JUMP,
		FALL,
		HIT

	};

	EnemyBird(int x, int y);

	void Update() override;

	bool Cleanup();

	bool Fall(Collider* c1, Collider* c2);

	bool StopMovementY(Collider* c1, Collider* c2);

	bool StopMovement(Collider* c1, Collider* c2);

	Animation idle;
	Animation hit;

	Animation* currentAnim = nullptr;

	Collider* collider = nullptr;

	float speedX;

private:

	Path path;

	bool yDownCollision;
	bool yUpCollision;
	bool xLeftCollision;
	bool xRightCollision;

};


#endif
