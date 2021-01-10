#ifndef __ENEMYBIRD__
#define __ENEMYBIRD__

#include "Enemy.h"
#include "Path.h"

class EnemyBird : public Enemy
{
public :

	EnemyBird(int x, int y);

	void Update() override;

	bool CleanUp() override;

	bool Fall(Collider* c1, Collider* c2);

	bool StopMovementY(Collider* c1, Collider* c2);

	bool Die(Collider* c1, Collider* c2);

	bool StopMovement(Collider* c1, Collider* c2);

private:

	Animation idle;

	bool yDownCollision;
	bool yUpCollision;
	bool xLeftCollision;
	bool xRightCollision;

};


#endif
