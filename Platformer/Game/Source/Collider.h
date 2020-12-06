#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "SDL/include/SDL_Rect.h"

class Module;

struct Collider
{
	enum Type
	{
		NONE,
		GROUND,
		WALL,
		PLAYER,
		AIR,
		SPIKE,
		CAMERA,
		CHECKPOINT,
		APPLE,
		PINEAPPLE,
		ENEMY,
		LEAF,
		CHICKEN,

		MAX
	};

	//Methods
	Collider(SDL_Rect rectangle, Type type, Module* listener = nullptr);

	void SetPos(int x, int y);

	bool Intersects(const SDL_Rect& r) const;

	//Variables
	SDL_Rect rect;
	Type type;
	Module* listener = nullptr;
	bool pendingToDelete = false;
	bool isCollected = false;
	bool dead;
};


#endif // !__COLLIDER_H_
