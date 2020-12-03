#ifndef __MODULE_ENEMIES_H__
#define __MODULE_ENEMIES_H__

#include "Module.h"

#define MAX_ENEMIES 100

enum class EnemyType
{
	NO_TYPE,
	BIRD,
	BUNNY,
};

struct EnemySpawnpoint
{
	EnemyType type = EnemyType::NO_TYPE;
	int x, y;
};

class Enemy;
struct SDL_Texture;

class ModuleEnemies : public Module
{
public:
	// Constructor
	ModuleEnemies();

	// Destructor
	~ModuleEnemies();

	bool Start() override;

	bool Update(float dt) override;

	bool PostUpdate() override;

	bool CleanUp() override;

	bool OnCollision(Collider* c1, Collider* c2) override;
	bool Fall(Collider* c1, Collider* c2) override;
	bool StopMovement(Collider* c1, Collider* c2) override;
	bool StopMovementY(Collider* c1, Collider* c2) override;

	bool AddEnemy(EnemyType type, int x, int y);

	void EnemiesSpawn();

	void EnemiesDespawn();

	// The enemies sprite sheet
	SDL_Texture* bird = nullptr;
	SDL_Texture* bunny = nullptr;

	// All spawned enemies in the scene
	Enemy* enemies[MAX_ENEMIES] = { nullptr };

private:
	// Spawns a new enemy using the data from the queue
	void SpawnEnemy(const EnemySpawnpoint& info);

private:
	// A queue with all spawn points information
	EnemySpawnpoint spawnQueue[MAX_ENEMIES];

};

#endif // __MODULE_ENEMIES_H__