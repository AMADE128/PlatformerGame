#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#define MAX_COLLIDERS 2000

#include "Module.h"
#include "Collider.h"

class Collisions : public Module
{
public:

	Collisions();

	// Destructor
	virtual ~Collisions();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	bool PreUpdate();

	// Called at the middle of the application loop
	// Switches the debug mode on/off
	bool Update(float dt);

	// Called at the end of the application loop
	// Draw all colliders (if debug mode is enabled)
	bool PostUpdate();

	// Removes all existing colliders
	bool CleanUp();

	// Adds a new collider to the list
	Collider* AddCollider(SDL_Rect rect, Collider::Type type, Module* listener = nullptr);

	// Draws all existing colliders with some transparency
	void DebugDraw();


	void RemoveCollider(Collider* collider);

	enum CollType
	{
		DIE,
		STOP,
		STOP_Y,
		FALL,
		NOTHING,
		CAMERA_SCROLL,
		CHECK,
		COLLECT,
		EXTRALIFE,
	};

	// All existing colliders in the scene
	Collider* colliders[MAX_COLLIDERS] = { nullptr };

private:

	// The collision matrix. Defines the interaction for two collider types
	// If set two false, collider 1 will ignore collider 2
	CollType matrix[Collider::Type::MAX][Collider::Type::MAX];

	// Simple debugging flag to draw all colliders
	bool debug = false;
};

#endif // __COLLISIONS_H__