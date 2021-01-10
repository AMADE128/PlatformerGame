#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Module.h"
#include "Point.h"
#include "Animation.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "Map.h"
#include "Collisions.h"
#include "Defs.h"
#include "Log.h"


enum TypeEntity {

	UNKNOWN = 0,
	PLAYER = 1,
	BUNNY = 2,
	BIRD = 3,
	APPLE = 4,
	PINEAPPLE = 5
};

struct EntityData
{
	iPoint position;
	float velocity;
	TypeEntity type;
	SDL_Texture* texture;

public:
	EntityData(TypeEntity pTypeEntity, iPoint pPosition, float pVelocity, SDL_Texture* pTexture) :
		position(pPosition), texture(pTexture), type(pTypeEntity)
	{};
	EntityData::EntityData() {};
};

class Entity : public Module
{
public:


	Entity(TypeEntity pTypeEntity, iPoint pPosition, float pVelocity, SDL_Texture* pTexture);
	Entity();
	~Entity();

	// Called before render is available
	virtual bool Awake(pugi::xml_node&);

	// Called before the first frame
	virtual bool Start();
	// Called each loop iteration
	virtual bool PreUpdate();

	// Called each loop iteration
	virtual bool Update(float dt);
	// Called each loop iteration
	virtual bool PostUpdate();
	// Called before quitting
	virtual bool CleanUp();

public:

	EntityData* entityData;
	Collisions collision;
};

#endif // __MODULE_H__