#ifndef __ENEMYBUNNY__
#define __ENEMYBUNNY__

#include "Enemy.h"
#include "Path.h"

class EnemyBunny : public Enemy
{
public:

	EnemyBunny(int x, int y);

	void Update() override;

	bool Cleanup();

	bool Fall(Collider* c1, Collider* c2);

	bool StopMovementY(Collider* c1, Collider* c2);

	bool StopMovement(Collider* c1, Collider* c2);

	Collider* BunnyColl = nullptr;

	float speedX;


private:

	Path path;
	bool yDownCollision;
	bool yUpCollision;
	bool xLeftCollision;
	bool xRightCollision;

};


#endif

