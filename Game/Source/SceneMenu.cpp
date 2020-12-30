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

	btnStart = new GuiButton(1, { 1280 / 2 - 300 / 2, 300, 300, 80 }, "START");
	btnStart->SetObserver(this);

	btnExit = new GuiButton(2, { 1280 / 2 - 300 / 2, 400, 300, 80 }, "EXIT");
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
	menu = app->tex->Load("Assets/Textures/Screens/menu.png");
	musicMenu = app->audio->LoadFx("Assets/Audio/SceneMusic/intro_music.wav");
	app->musicList.Add(&musicMenu);
	app->audio->PlayFx(musicMenu, -1);

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
	app->render->DrawTexture(menu, 0, 0);

	btnStart->Update(app->input, dt);
	btnExit->Update(app->input, dt);

	return true;
}

// Called each loop iteration
bool SceneMenu::PostUpdate()
{
	bool ret = true;

	btnStart->Draw(app->render);
	btnExit->Draw(app->render);

	return ret;
}

// Called before quitting
bool SceneMenu::CleanUp()
{
	if (!active) return true;

	LOG("Freeing scene");
	app->tex->UnLoad(menu);
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
		if (control->id == 1) app->fadeToBlack->Fade(this, (Module*)app->scene, 80);
		else if (control->id == 2) return false; // TODO: Exit request
	}
	default: break;
	}

	return true;
}