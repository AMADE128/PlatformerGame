#include "SceneMenu.h"
#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "Render.h"

#include "SDL/include/SDL_scancode.h"
#include "SDL/include/SDL.h"
#include "Log.h"

SceneMenu::SceneMenu(bool startEnabled) : Module(startEnabled)
{
	name.Create("scenemenu");
}

SceneMenu::~SceneMenu()
{}

bool SceneMenu::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

bool SceneMenu::Start()
{
	LOG("Loading Menu assets");

	bool ret = true;

	MenuTexture = app->tex->Load("Assets/textures/screens/Menu.png");

	return ret;
}

bool SceneMenu::Update()
{
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	{
		app->fadetoblack->Fade(this, (Module*)app->scene, 90);
	}

	return true;
}

bool SceneMenu::PostUpdate()
{
	app->render->DrawTexture(MenuTexture, 0, 0, NULL);

	return true;
}

bool SceneMenu::CleanUp()
{
	SDL_DestroyTexture(MenuTexture);

	return true;
}

