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

	bool CleanUp() override;

	bool Fall(Collider* c1, Collider* c2);

	bool Die(Collider* c1, Collider* c2);

	bool StopMovementY(Collider* c1, Collider* c2);

	bool StopMovement(Collider* c1, Collider* c2);

private:

	Animation idle;
	Animation run;
	Animation fall;
	Animation jump;
};


#endif

