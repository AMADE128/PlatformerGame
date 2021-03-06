#ifndef __MODULE_ENEMIES_H__
#define __MODULE_ENEMIES_H__

#include "Module.h"
#include "Timer.h"
#include "Point.h"
#include "DynArray.h"
#include "Entity.h"

#define MAX_ENEMIES 100
#define TILESIZE 96
#define SPAWN_MARGIN 50

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

class ModuleEnemies : public Entity
{
public:
	// Constructor
	ModuleEnemies();

	// Destructor
	~ModuleEnemies();

	bool Start() override;

	bool PreUpdate() override;

	bool Update(float dt) override;

	bool PostUpdate() override;

	bool CleanUp() override;

	bool Die(Collider* c1, Collider* c2) override;
	bool Fall(Collider* c1, Collider* c2) override;
	bool StopMovement(Collider* c1, Collider* c2) override;
	bool StopMovementY(Collider* c1, Collider* c2) override;

	bool AddEnemy(EnemyType type, int x, int y);

	void EnemiesSpawn();

	void CreatePathEnemy(iPoint origin, iPoint destination);
	int GetCurrentPositionInPath(iPoint mapPositionEnemy);
	iPoint MapToWorld(iPoint position);

	// All spawned enemies in the scene
	Enemy* enemies[MAX_ENEMIES] = { nullptr };

	SDL_Texture* bunnyIdle = nullptr;
	SDL_Texture* bunnyFall = nullptr;
	SDL_Texture* bunnyHit = nullptr;
	SDL_Texture* bunnyRun = nullptr;

	SDL_Texture* birdFly = nullptr;
	SDL_Texture* birdHit = nullptr;

	EnemyType collType;

	bool found = true;
	DynArray<iPoint>* lastPath;

	unsigned int bunnyDieFx;
	unsigned int birdDieFx;

	// Spawns a new enemy using the data from the queue
	bool pathFinding = false;
	void SpawnEnemy(const EnemySpawnpoint& info);

	// A queue with all spawn points information
	EnemySpawnpoint spawnQueue[MAX_ENEMIES];
	Timer* checkDestination = new Timer();


};

#endif // __MODULE_ENEMIES_H__