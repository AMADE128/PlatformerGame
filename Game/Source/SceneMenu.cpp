#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneMenu.h"
#include "Map.h"
#include "Collisions.h"
#include "Player.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

SceneMenu::SceneMenu() : Module()
{
	name.Create("scene");

	btnNew = new GuiButton(1, { 1280/2 - BUTT_WIDTH / 2, 430, BUTT_WIDTH, BUTT_HEIGHT }, "NEW");
	btnNew->SetObserver(this);

	btnLoad = new GuiButton(2, { 1280 / 2 - BUTT_WIDTH / 2, 500, BUTT_WIDTH, BUTT_HEIGHT }, "LOAD");
	btnLoad->SetObserver(this);

	btnOptions = new GuiButton(3, { 1280 / 2 - BUTT_WIDTH / 2, 570, BUTT_WIDTH, BUTT_HEIGHT }, "OPTIONS");
	btnOptions->SetObserver(this);

	btnExit = new GuiButton(4, { 1280 / 2 - BUTT_WIDTH / 2, 640, BUTT_WIDTH, BUTT_HEIGHT }, "EXIT");
	btnExit->SetObserver(this);
}

// Destructor
SceneMenu::~SceneMenu()
{}

// Called before render is available
bool SceneMenu::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneMenu::Start()
{
	// L03: DONE: Load map
	menuTex = app->tex->Load("Assets/Textures/Screens/menu.png");
	musicMenu = app->audio->LoadFx("Assets/Audio/SceneMusic/intro_music.wav");
	btnNew->texture = btnExit->texture = btnLoad->texture = btnOptions->texture = app->tex->Load("Assets/Textures/Interface/button.png");
	boxTex = app->tex->Load("Assets/Textures/Interface/box.png");
	newButtTex = app->tex->Load("Assets/Textures/Interface/new.png");
	loadButtTex = app->tex->Load("Assets/Textures/Interface/load.png");
	optionButtTex = app->tex->Load("Assets/Textures/Interface/option.png");
	exitButtTex = app->tex->Load("Assets/Textures/Interface/exit.png");

	app->musicList.Add(&musicMenu);
	app->audio->PlayFx(musicMenu, -1);

	Menu(loadFile);

	return true;
}

// Called each loop iteration
bool SceneMenu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneMenu::Update(float dt)
{
	switch (menuState)
	{
	case SceneMenu::NORMAL:
		btnNew->Update(app->input, dt);
		btnExit->Update(app->input, dt);
		btnLoad->Update(app->input, dt);
		btnOptions->Update(app->input, dt);
		break;
	case SceneMenu::SETTINGS:
		break;
	case SceneMenu::CREDITS:
		break;
	default:
		break;
	}

	return true;
}

// Called each loop iteration
bool SceneMenu::PostUpdate()
{
	bool ret = true;
	app->render->DrawTexture(menuTex, 0, 0);

	btnNew->Draw(app->render);
	app->render->DrawTexture(newButtTex, btnNew->bounds.x, btnNew->bounds.y);
	btnExit->Draw(app->render);
	app->render->DrawTexture(exitButtTex, btnExit->bounds.x, btnExit->bounds.y);
	btnLoad->Draw(app->render);
	app->render->DrawTexture(loadButtTex, btnLoad->bounds.x, btnLoad->bounds.y);
	btnOptions->Draw(app->render);
	app->render->DrawTexture(optionButtTex, btnOptions->bounds.x, btnOptions->bounds.y);\

	if (menuState == SETTINGS)
	{
		app->render->DrawRectangle({ 0, 0, 1280, 720 }, { 0, 0, 0, 150 });
		app->render->DrawTexture(boxTex, btnNew->bounds.x - 32, btnNew->bounds.y - 20);
	}

	return ret;
}

// Called before quitting
bool SceneMenu::CleanUp()
{
	if (!active) return true;

	LOG("Freeing scene");
	app->tex->UnLoad(menuTex);
	app->tex->UnLoad(btnNew->texture);
	app->tex->UnLoad(btnExit->texture);
	app->tex->UnLoad(btnLoad->texture);
	app->tex->UnLoad(btnOptions->texture);
	app->audio->UnloadFX(musicMenu);
	app->musicList.Clear();

	active = false;

	return true;
}

bool SceneMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1)
		{
			app->player->cont = false;
			app->fadeToBlack->Fade(this, (Module*)app->scene, 80); //Go to LVL 1
		}
		if (control->id == 2)
		{
			app->player->playerLoadF6 = true;
			app->LoadGameRequest();
			Menu(loadFile);
			switch (currentLvl)
			{
			case 1:
				app->fadeToBlack->Fade(this, (Module*)app->scene, 80);
				break;
			case 2:
				app->fadeToBlack->Fade(this, (Module*)app->sceneLvl2, 80);
				break;
			default:
				break;
			}
		}
		if (control->id == 3) menuState = SETTINGS;
		if (control->id == 4) return false;
	}
	default: break;
	}

	return true;
}

bool SceneMenu::LoadMenu(pugi::xml_node& data)
{
	app->player->cont = data.child("continue").attribute("cont").as_bool();
	currentLvl = data.child("level").attribute("lvl").as_int();

	return true;
}

bool SceneMenu::Menu(pugi::xml_document& loadFile)
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
		LoadMenu(load.child("player"));
	}
	return ret;
}

