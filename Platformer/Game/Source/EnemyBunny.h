#ifndef __ENEMYBUNNY__
#define __ENEMYBUNNY__

#include "Enemy.h"
#include "Path.h"

class Animation;

class EnemyBunny : public Enemy
{
public:

	EnemyBunny(int x, int y);

	void Update() override;

	bool Cleanup();

	bool Fall(Collider* c1, Collider* c2);

	bool OnCollision(Collider* c1, Collider* c2);

	bool StopMovementY(Collider* c1, Collider* c2);

	bool StopMovement(Collider* c1, Collider* c2);

	Animation idle;
	Animation run;
	Animation fall;
	Animation jump;
	Animation hit;

	float speedX;


private:


};


#endif

