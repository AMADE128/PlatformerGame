#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"

#include "PugiXml/src/pugixml.hpp"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"
#define VALUE_TO_VOLUME 128/137

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class Map;
class Player;
class Collisions;
class SceneMenu;
class FadeToBlack;
class Fonts;
class SceneLvl2;
class ModuleParticles;
class ModuleEnemies;
class SceneIntro;
class SceneLoose;
class SceneWin;
class PathFinding;
class EntityManager;
class Entity;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

    // L02: DONE 1: Create methods to request Load / Save
	void LoadGameRequest();
	void SaveGameRequest() const;

private:

	// Load config file
	// NOTE: It receives config document
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGame(pugi::xml_document& data);
	bool SaveGame() const;

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	SceneLvl2* sceneLvl2;
	SceneMenu* sceneMenu;
	Map* map;
	Player* player;
	Collisions* collision;
	FadeToBlack* fadeToBlack;
	Fonts* fonts;
	ModuleParticles* moduleParticles;
	ModuleEnemies* moduleEnemies;
	SceneIntro* sceneIntro;
	SceneLoose* sceneLoose;
	SceneWin* sceneWin;
	PathFinding* pathfinding;
	EntityManager* entityManager;
	Entity* entity;

	int frameRate = 60;
	float fps = 0.f;
	float fpsMseconds;
    int screenTicks;
	float secondsSinceStartup;

	List<unsigned int*>	fxList;
	int volumeMusic = 64;
	int volumeFX = 64;

	bool guiDebug = false;

private:
	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module *> modules;


	// NOTE: Redesigned LoadConfig() to avoid storing this variables
	pugi::xml_document loadFile;
	pugi::xml_node load;

	PerfTimer ptimer;
	uint64 frameCount = 0;

	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;
	Timer dtTimer;
	uint32 lastSecFrameCount = 0;
	uint32 prevLastSecFrameCount = 0;
	float dt = 0.0f;

	float fpsCounter;
	float fpsPreUpdate;

	uint frames;


	mutable bool saveGameRequested;
	bool loadGameRequested = false;
};

extern App* app;

#endif	// __APP_H__