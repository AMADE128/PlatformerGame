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

	btnNew = new GuiButton(1, { 1280/2 - BUTT_WIDTH / 2, 430, BUTT_WIDTH, BUTT_HEIGHT });
	btnNew->SetObserver(this);

	btnLoad = new GuiButton(2, { 1280 / 2 - BUTT_WIDTH / 2, 500, BUTT_WIDTH, BUTT_HEIGHT });
	btnLoad->SetObserver(this);

	btnOptions = new GuiButton(3, { 1280 / 2 - BUTT_WIDTH / 2, 570, BUTT_WIDTH, BUTT_HEIGHT });
	btnOptions->SetObserver(this);

	btnExit = new GuiButton(4, { 1280 / 2 - BUTT_WIDTH / 2, 640, BUTT_WIDTH, BUTT_HEIGHT });
	btnExit->SetObserver(this);

	btnBack = new GuiButton(5, { 1280 / 2 - BUTT_WIDTH / 2, 570, BUTT_WIDTH, BUTT_HEIGHT });
	btnBack->SetObserver(this);

	checkFullScreen = new GuiCheckBox(1, { 1280 / 2 - SMALL_BUTT_WIDTH / 2, 500, SMALL_BUTT_WIDTH, SMALL_BUTT_HEIGHT });
	checkFullScreen->SetObserver(this);

	sliderMusic = new GuiSlider(1, { 1280 / 2 - SLIDER_WIDTH / 2, 450, SLIDER_WIDTH, SLIDER_HEIGHT }, 0, 128);
	sliderMusic->SetObserver(this);

	sliderFx = new GuiSlider(2, { 1280 / 2 - SLIDER_WIDTH / 2, 400, SLIDER_WIDTH, SLIDER_HEIGHT }, 0, 128);
	sliderFx->SetObserver(this);
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

	btnBack->texture = btnNew->texture = btnExit->texture = btnLoad->texture = btnOptions->texture = app->tex->Load("Assets/Textures/Interface/button.png");
	boxTex = app->tex->Load("Assets/Textures/Interface/box.png");
	btnNew->text = app->tex->Load("Assets/Textures/Interface/new.png");
	btnLoad->text = app->tex->Load("Assets/Textures/Interface/load.png");
	btnOptions->text = app->tex->Load("Assets/Textures/Interface/option.png");
	btnExit->text = app->tex->Load("Assets/Textures/Interface/exit.png");
	btnBack->text = app->tex->Load("Assets/Textures/Interface/back.png");
	sliderMusic->texture = sliderFx->texture = app->tex->Load("Assets/Textures/Interface/slider.png");

	checkFullScreen->texture = app->tex->Load("Assets/Textures/Interface/small_button.png");
	checkFullScreen->text = app->tex->Load("Assets/Textures/Interface/checked.png");

	app->render->camera.x = 0;
	app->render->camera.y = 0;

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
		if (app->player->cont == false)
		{
			btnLoad->state = GuiControlState::DISABLED;
		}
		else if (app->player->cont == true)
		{
			btnLoad->state = GuiControlState::NORMAL;
		}
		btnNew->Update(app->input, dt);
		btnExit->Update(app->input, dt);
		btnLoad->Update(app->input, dt);
		btnOptions->Update(app->input, dt);
		break;
	case SceneMenu::SETTINGS:
		btnBack->Update(app->input, dt);
		checkFullScreen->Update(app->input, dt);
		sliderMusic->Update(app->input, dt);
		app->volumeMusic = (int)(sliderMusic->value*VALUE_TO_VOLUME);
		sliderFx->Update(app->input, dt);
		app->volumeFX = (int)(sliderFx->value * VALUE_TO_VOLUME);
		break;
	case SceneMenu::CREDITS:
		break;
	case SceneMenu::EXIT:
		return false;
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
	
	switch (menuState)
	{
	case SceneMenu::NORMAL:
		btnNew->Draw(app->render);
		btnExit->Draw(app->render);
		btnLoad->Draw(app->render);
		btnOptions->Draw(app->render);
		break;
	case SceneMenu::SETTINGS:
		app->render->DrawRectangle({ 0, 0, 1280, 720 }, { 0, 0, 0, 170 });
		app->render->DrawTexture(boxTex, btnNew->bounds.x - 32, btnNew->bounds.y - 20);
		btnBack->Draw(app->render);
		checkFullScreen->Draw(app->render);
		sliderMusic->Draw(app->render);
		sliderFx->Draw(app->render);
		break;
	case SceneMenu::CREDITS:
		break;
	case SceneMenu::EXIT:
		break;
	default:
		break;
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
	app->tex->UnLoad(btnBack->texture);

	app->tex->UnLoad(boxTex);

	app->tex->UnLoad(btnNew->text);
	app->tex->UnLoad(btnExit->text);
	app->tex->UnLoad(btnLoad->text);
	app->tex->UnLoad(btnOptions->text);
	app->tex->UnLoad(btnBack->text);
	app->tex->UnLoad(sliderMusic->texture);
	app->tex->UnLoad(sliderFx->texture);

	app->tex->UnLoad(checkFullScreen->texture);
	app->tex->UnLoad(checkFullScreen->text);

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
		switch (control->id)
		{
		case 1:
			app->player->cont = false;
			app->fadeToBlack->Fade(this, (Module*)app->scene, 80); //Go to the start of the game
			break;
		case 2:
			app->player->playerLoadF6 = true;
			app->LoadGameRequest();
			Menu(loadFile);
			switch (currentLvl)
			{
			case 1:
				app->fadeToBlack->Fade(this, (Module*)app->scene, 80); //Go to the LVL 1
				break;
			case 2:
				app->fadeToBlack->Fade(this, (Module*)app->sceneLvl2, 80); //Go to the LV 2
				break;
			default:
				break;
			}
			break;
		case 3:
			menuState = SETTINGS;
			break;
		case 4:
			menuState = EXIT;
			break;
		case 5:
			menuState = NORMAL;
			break;
		default:
			break;
		}
	case GuiControlType::CHECKBOX:
		switch (control->id)
		{
		case 1:
			if (checkFullScreen->checked)
			{
				SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				app->win->scale = 1.5f;
			}
			else
			{
				SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_RESIZABLE);
				app->win->scale = 1;
			}
			break;
		default:
			break;
		}
		break;
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

