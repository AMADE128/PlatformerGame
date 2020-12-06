#ifndef __ENEMYBIRD__
#define __ENEMYBIRD__

#include "Enemy.h"
#include "Path.h"

class EnemyBird : public Enemy
{
public :

	EnemyBird(int x, int y);

	void Update() override;

	bool Cleanup();

	bool Fall(Collider* c1, Collider* c2);

	bool StopMovementY(Collider* c1, Collider* c2);

	bool OnCollision(Collider* c1, Collider* c2);

	bool StopMovement(Collider* c1, Collider* c2);

	Animation idle;
	Animation hit;

	float speedX;

private:

	bool yDownCollision;
	bool yUpCollision;
	bool xLeftCollision;
	bool xRightCollision;

};


#endif
