#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

#include "Entity.h"
#include "Player.h"
#include "Enemy.h"

struct EntitySpawnPoint
{
	EntitySpawnPoint(TypeEntity pType, int pX, int pY) : type(pType), x(pX), y(pY) {}
	TypeEntity type = TypeEntity::UNKNOWN;
	int x, y;
};

class EntityManager : public Module
{

public:

	EntityManager();
	~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();
	// Called each loop iteration
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);
	// Called each loop iteration
	bool PostUpdate();
	// Called before quitting
	bool CleanUp();



public:
	List<Entity*> entities;
};

#endif // __MODULE_H__