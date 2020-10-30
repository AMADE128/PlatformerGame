#include "Collisions.h"

#include "App.h"
#include "Render.h"
#include "Input.h"
#include "Player.h"
#include "Map.h"

#include "SDL/include/SDL_Scancode.h"

#include "Log.h"

Collisions::Collisions(bool startEnabled) : Module(startEnabled)
{
	name.Create("collisions");
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[Collider::Type::PLAYER][Collider::Type::WALL] = STOP;
	matrix[Collider::Type::PLAYER][Collider::Type::GROUND] = STOP_Y;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = NOTHING;
	matrix[Collider::Type::PLAYER][Collider::Type::NONE] = NOTHING;

}

// Destructor
Collisions::~Collisions()
{}

bool Collisions::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Collisions::Start()
{
	MapLayer* layer;
	TileSet* tileset;
	iPoint cord;
	SDL_Rect coll;

	for (ListItem<MapLayer*>* item = app->map->data.layers.start; item; item = item->next)
	{
		layer = item->data;
		for (int y = 0; y < app->map->data.height; ++y)
		{
			for (int x = 0; x < app->map->data.width; ++x)
			{
				int tileId = layer->Get(x, y);
				cord = app->map->MapToWorld(x, y);
				tileset = app->map->GetTilesetFromTileId(tileId);
				if (tileId > 258 && tileId < 268)
				{
					coll = { cord.x, cord.y, tileset->GetTileRect(tileId).w, tileset->GetTileRect(tileId).h };
					app->collision->AddCollider(coll, Collider::Type::GROUND, this);
				}
				if (tileId > 268)
				{
					coll = { cord.x, cord.y, tileset->GetTileRect(tileId).w, tileset->GetTileRect(tileId).h };
					app->collision->AddCollider(coll, Collider::Type::WALL, this);
				}
			}
		}
	}
	return true;
}

bool Collisions::PreUpdate()
{

	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->Intersects(c2->rect))
			{
				if (matrix[c1->type][c2->type] == STOP && c1->listener)
					c1->listener->StopMovement(c1, c2);

				if (matrix[c1->type][c2->type] == STOP_Y && c1->listener)
					c1->listener->StopMovementY(c1, c2);
			}
		}
	}

	return true;
}

bool Collisions::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;
	if (debug)
		DebugDraw();
	return true;
}

bool Collisions::PostUpdate()
{
	return true;
}

void Collisions::DebugDraw()
{
	Uint8 alpha = 120;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case Collider::Type::NONE: // white
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case Collider::Type::WALL: // green
			app->render->DrawRectangle(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case Collider::Type::GROUND: // red
			app->render->DrawRectangle(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case Collider::Type::PLAYER: // blue
			app->render->DrawRectangle(colliders[i]->rect, 0, 0, 255, alpha);
			break;

		}
	}
}

// Called before quitting
bool Collisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* Collisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			break;
		}
	}

	return ret;
}

void Collisions::RemoveCollider(Collider* collider)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == collider)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
}