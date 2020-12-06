#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "SceneLvl2.h"
#include "Map.h"
#include "Player.h"
#include "Collisions.h"
#include "SceneMenu.h"
#include "FadeToBlack.h"
#include "PerfTimer.h"
#include "Fonts.h"
#include "ModuleParticles.h"
#include "ModuleEnemies.h"
#include "SceneIntro.h"
#include "SceneLoose.h"
#include "SceneWin.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);

	win = new Window();
	input = new Input();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	scene = new Scene();
	map = new Map();
	player = new Player();
	collision = new Collisions();
	sceneMenu = new SceneMenu();
	fadeToBlack = new FadeToBlack();
	fonts = new Fonts();
	moduleParticles = new ModuleParticles();
	moduleEnemies = new ModuleEnemies();
	sceneIntro = new SceneIntro();
	sceneLoose = new SceneLoose();
	sceneWin = new SceneWin();
	sceneLvl2 = new SceneLvl2();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(map);
	AddModule(sceneIntro);
	AddModule(sceneMenu);
	AddModule(scene);
	AddModule(sceneLvl2);
	AddModule(player);
	AddModule(sceneLoose);
	AddModule(sceneWin);
	AddModule(moduleParticles);
	AddModule(moduleEnemies);
	AddModule(collision);
	AddModule(fadeToBlack);
	AddModule(fonts);

	sceneLvl2->active = false;
	sceneMenu->active = false;
	scene->active = false;
	sceneLoose->active = false;
	sceneWin->active = false;
	player->active = false;
	map->active = false;
	moduleParticles->active = false;
	moduleEnemies->active = false;
	collision->active = false;
	fonts->active = false;

	// Render last to swap buffer
	AddModule(render);

	PERF_PEEK(ptimer);
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	PERF_START(ptimer);
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	// L01: DONE 3: Load config from XML
	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		// L01: DONE 4: Read the title from the config file
		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());

	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while ((item != NULL) && (ret == true))
		{
			// L01: DONE 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool App::Start()
{
	fpsMseconds = SDL_GetTicks();
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		if (item->data->active == true)
		{
			ret = item->data->Start();
		}
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
// NOTE: Function has been redesigned to avoid storing additional variables on the class
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	secondsSinceStartup = startupTime.ReadSec();
	fpsPreUpdate = SDL_GetTicks();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	uint32 lastFrameMs = 0;
	uint32 framesOnLastUpdate = 0;
	frameCount++;
	float averageFps = frameCount / secondsSinceStartup;
	fpsCounter++;
	float fpsMsecondsAfter = SDL_GetTicks() - fpsPreUpdate;
	if (fpsMseconds < SDL_GetTicks() - 1000)
	{
		fpsMseconds = SDL_GetTicks();
		fps = fpsCounter;
		fpsCounter = 0;
	}
	SString title("Platformer Game: FPS: %.2f Av.FPS: %.2f Last Frame Ms: %.2f Time since startup: %.3f Frame Count: %I64u ",fps,
		averageFps, fpsMsecondsAfter, secondsSinceStartup, frameCount);
	app->win->SetTitle(title.GetString());

	
	if(fpsMsecondsAfter < screenTicks) SDL_Delay(screenTicks - fpsMsecondsAfter);
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;
	screenTicks = 1000 / frameRate;

	dt = dtTimer.ReadSec();
	dtTimer.Start();

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}
	if (loadGameRequested == true)
	{
		LoadGame(loadFile);
		app->player->playerColl->SetPos(app->player->position.x + 25, app->player->position.y + 20);
		app->player->cameraColl->rect.x = app->player->playerColl->rect.x - 100;
		app->player->cameraColl->rect.y = app->player->playerColl->rect.y - 100;
		app->player->death = false;
	}
	if (saveGameRequested == true) SaveGame();

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	if (input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		player->god = false;
		if (sceneIntro->active == true)
		{
			fadeToBlack->Fade(sceneIntro, (Module*)scene, 1600 * dt / 6);
		}
		else if (sceneMenu->active == true)
		{
			fadeToBlack->Fade(app->sceneMenu, (Module*)scene, 1600 * dt / 6);
		}
		else if (scene->active == true)
		{
			fadeToBlack->Fade(app->scene, (Module*)scene, 1600 * dt / 6);
		}
		else if (sceneLvl2->active == true)
		{
			fadeToBlack->Fade(app->sceneLvl2, (Module*)scene, 1600 * dt / 6);
		}
		else if (sceneWin->active == true)
		{
			fadeToBlack->Fade(app->sceneWin, (Module*)scene, 1600 * dt / 6);
		}
		else if (sceneLoose->active == true)
		{
			fadeToBlack->Fade(sceneLoose, (Module*)scene, 1600 * dt / 6);
		}
	}
	if (input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		player->god = false;
		if (sceneIntro->active == true)
		{
			fadeToBlack->Fade(sceneIntro, (Module*)sceneLvl2, 1600 * dt / 6);
		}
		else if (sceneMenu->active == true)
		{
			fadeToBlack->Fade(app->sceneMenu, (Module*)sceneLvl2, 1600 * dt / 6);
		}
		else if (scene->active == true)
		{
			fadeToBlack->Fade(app->scene, (Module*)sceneLvl2, 1600 * dt / 6);
		}
		else if (sceneLvl2->active == true)
		{
			fadeToBlack->Fade(app->sceneLvl2, (Module*)sceneLvl2, 1600 * dt / 6);
		}
		else if (sceneWin->active == true)
		{
			fadeToBlack->Fade(app->sceneWin, (Module*)sceneLvl2, 1600 * dt / 6);
		}
		else if (sceneLoose->active == true)
		{
			fadeToBlack->Fade(sceneLoose, (Module*)sceneLvl2, 1600 * dt / 6);
		}
	}

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = false;
	}

	// Set VOLUME
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		if (volume < 128)
		{
			volume += 128 / 32;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		if (volume > 0)
		{
			volume -= 128 / 32;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		if (frameRate == 30)
		{
			frameRate = 60;
		}
		else if (frameRate == 60)
		{
			frameRate = 30;
		}
	}

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	ListItem<unsigned int*>* itemMusic;
	for (itemMusic = musicList.start; itemMusic != NULL; itemMusic = itemMusic->next)
	{
		app->audio->SetVolume(*itemMusic->data, volume);
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGameRequest()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveGameRequested = true;
}

// ---------------------------------------
// L02: TODO 5: Create a method to actually load an xml file
// then call all the modules to load themselves
bool App::LoadGame(pugi::xml_document& loadFile)
{

	bool ret = false;

	pugi::xml_parse_result result = loadFile.load_file(SAVE_STATE_FILENAME);

	if (result == NULL)
	{
		LOG("Could not load xml file: %s. pugi error: %s", SAVE_STATE_FILENAME, result.description());
	}
	else
	{
		ret = true;
		load = loadFile.child("game_state");
	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->LoadState(load.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	loadGameRequested = false;

	return ret;
}

// L02: TODO 7: Implement the xml save method for current state
bool App::SaveGame() const
{
	bool ret = true;

	pugi::xml_document saveFile;
	pugi::xml_node nodeBase = saveFile.append_child("game_state");

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			pugi::xml_node nodes = nodeBase.append_child(item->data->name.GetString());
			ret = item->data->SaveState(nodeBase.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	saveFile.save_file("save_game.xml");

	saveGameRequested = false;

	return ret;
}


