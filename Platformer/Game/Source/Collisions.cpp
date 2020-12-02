#include "Collisions.h"

#include "App.h"
#include "Render.h"
#include "Input.h"
#include "Player.h"
#include "Map.h"
#include "Audio.h"

#include "SDL/include/SDL_Scancode.h"

#include "Log.h"

Collisions::Collisions() : Module()
{
	name.Create("collisions");
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[Collider::Type::PLAYER][Collider::Type::WALL] = STOP;
	matrix[Collider::Type::PLAYER][Collider::Type::GROUND] = STOP_Y;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = NOTHING;
	matrix[Collider::Type::PLAYER][Collider::Type::NONE] = NOTHING;
	matrix[Collider::Type::PLAYER][Collider::Type::AIR] = FALL;
	matrix[Collider::Type::PLAYER][Collider::Type::SPIKE] = DIE;
	matrix[Collider::Type::PLAYER][Collider::Type::CAMERA] = CAMERA_SCROLL;
	matrix[Collider::Type::PLAYER][Collider::Type::CHECKPOINT] = CHECK;
	matrix[Collider::Type::PLAYER][Collider::Type::APPLE] = COLLECT;

	matrix[Collider::Type::CAMERA][Collider::Type::WALL] = NOTHING;
	matrix[Collider::Type::CAMERA][Collider::Type::GROUND] = NOTHING;
	matrix[Collider::Type::CAMERA][Collider::Type::PLAYER] = NOTHING;
	matrix[Collider::Type::CAMERA][Collider::Type::NONE] = NOTHING;
	matrix[Collider::Type::CAMERA][Collider::Type::AIR] = NOTHING;
	matrix[Collider::Type::CAMERA][Collider::Type::SPIKE] = NOTHING;
	matrix[Collider::Type::CAMERA][Collider::Type::CAMERA] = NOTHING;
	matrix[Collider::Type::CAMERA][Collider::Type::CHECKPOINT] = NOTHING;
	matrix[Collider::Type::CAMERA][Collider::Type::APPLE] = NOTHING;

	matrix[Collider::Type::NONE][Collider::Type::WALL] = NOTHING;
	matrix[Collider::Type::NONE][Collider::Type::GROUND] = NOTHING;
	matrix[Collider::Type::NONE][Collider::Type::PLAYER] = NOTHING;
	matrix[Collider::Type::NONE][Collider::Type::NONE] = NOTHING;
	matrix[Collider::Type::NONE][Collider::Type::AIR] = NOTHING;
	matrix[Collider::Type::NONE][Collider::Type::SPIKE] = NOTHING;
	matrix[Collider::Type::NONE][Collider::Type::CAMERA] = CAMERA_SCROLL;
	matrix[Collider::Type::NONE][Collider::Type::CHECKPOINT] = NOTHING;
	matrix[Collider::Type::NONE][Collider::Type::APPLE] = NOTHING;

	matrix[Collider::Type::AIR][Collider::Type::WALL] = NOTHING;
	matrix[Collider::Type::AIR][Collider::Type::GROUND] = NOTHING;
	matrix[Collider::Type::AIR][Collider::Type::PLAYER] = NOTHING;
	matrix[Collider::Type::AIR][Collider::Type::NONE] = NOTHING;
	matrix[Collider::Type::AIR][Collider::Type::AIR] = NOTHING;
	matrix[Collider::Type::AIR][Collider::Type::SPIKE] = NOTHING;
	matrix[Collider::Type::AIR][Collider::Type::CAMERA] = NOTHING;
	matrix[Collider::Type::AIR][Collider::Type::CHECKPOINT] = NOTHING;
	matrix[Collider::Type::AIR][Collider::Type::APPLE] = NOTHING;

	matrix[Collider::Type::CHECKPOINT][Collider::Type::WALL] = NOTHING;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::GROUND] = NOTHING;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::PLAYER] = NOTHING;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::NONE] = NOTHING;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::AIR] = NOTHING;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::SPIKE] = NOTHING;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::CAMERA] = NOTHING;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::CHECKPOINT] = NOTHING;
	matrix[Collider::Type::CHECKPOINT][Collider::Type::APPLE] = NOTHING;

	matrix[Collider::Type::APPLE][Collider::Type::WALL] = NOTHING;
	matrix[Collider::Type::APPLE][Collider::Type::GROUND] = NOTHING;
	matrix[Collider::Type::APPLE][Collider::Type::PLAYER] = NOTHING;
	matrix[Collider::Type::APPLE][Collider::Type::NONE] = NOTHING;
	matrix[Collider::Type::APPLE][Collider::Type::AIR] = NOTHING;
	matrix[Collider::Type::APPLE][Collider::Type::SPIKE] = NOTHING;
	matrix[Collider::Type::APPLE][Collider::Type::CAMERA] = NOTHING;
	matrix[Collider::Type::APPLE][Collider::Type::CHECKPOINT] = NOTHING;
	matrix[Collider::Type::APPLE][Collider::Type::APPLE] = NOTHING;

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
	SDL_Rect full = { 0, 0, app->map->data.width * app->map->data.tileWidth , app->map->data.height * app->map->data.tileHeight };

	AddCollider(full, Collider::Type::AIR, this);

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
				if (tileId > 259 && tileId < 269)
				{
					coll = { cord.x + 10, cord.y, tileset->GetTileRect(tileId).w - 20, tileset->GetTileRect(tileId).h };
					AddCollider(coll, Collider::Type::GROUND, this);
				}
				if (tileId > 268 && tileId <277)
				{
					coll = { cord.x, cord.y, tileset->GetTileRect(tileId).w, tileset->GetTileRect(tileId).h };
					AddCollider(coll, Collider::Type::WALL, this);
				}
				if (tileId > 287)
				{
					coll = { cord.x, cord.y + 25, tileset->GetTileRect(tileId).w, tileset->GetTileRect(tileId).h/2 };
					AddCollider(coll, Collider::Type::SPIKE, this);
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

				if (matrix[c1->type][c2->type] == FALL && c1->listener)
					c1->listener->Fall(c1, c2);
				
				if (matrix[c1->type][c2->type] == DIE && c1->listener)
					c1->listener->Die(c1, c2);

				if (matrix[c1->type][c2->type] == CAMERA_SCROLL && c1->listener)
					c1->listener->CameraScroll(c1, c2);

				if (matrix[c1->type][c2->type] == CHECK && c1->listener)
					c1->listener->CheckPoint(c1, c2);

				if (matrix[c1->type][c2->type] == COLLECT && c1->listener)
					c1->listener->CollectApple(c1, c2);
			}
		}
	}

	return true;
}

bool Collisions::Update(float dt)
{
	if (app->screen == game_scene1)
	{
		if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
			debug = !debug;
		if (debug)
			DebugDraw();
	}
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
		case Collider::Type::AIR: // blue
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case Collider::Type::SPIKE: // purple
			app->render->DrawRectangle(colliders[i]->rect, 128, 0, 128, alpha);
			break;
		case Collider::Type::CAMERA: // purple
			app->render->DrawRectangle(colliders[i]->rect, 200, 40, 128, alpha);
			break;
		case Collider::Type::CHECKPOINT: // purple
			app->render->DrawRectangle(colliders[i]->rect, 100, 40, 128, alpha);
			break;
		case Collider::Type::APPLE: // pink
			app->render->DrawRectangle(colliders[i]->rect, 255, 192, 203, alpha);
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