#ifndef __SCENEMENU_H__
#define __SCENEMENU_H__

#include "Module.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

struct SDL_Texture;

class SceneMenu : public Module
{
public:

	enum MenuStates
	{
		NORMAL,
		SETTINGS,
		CREDITS,
		EXIT
	};

	SceneMenu();

	// Destructor
	virtual ~SceneMenu();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool OnGuiMouseClickEvent(GuiControl* control);

	SDL_Texture* menuTex;
	SDL_Texture* boxTex;
	SDL_Texture* newButtTex;
	SDL_Texture* loadButtTex;
	SDL_Texture* optionButtTex;
	SDL_Texture* exitButtTex;
	SDL_Texture* backButtTex;

	MenuStates menuState = NORMAL;

	unsigned int musicMenu;

	GuiButton* btnNew;
	GuiButton* btnExit;
	GuiButton* btnLoad;
	GuiButton* btnOptions;
	GuiButton* btnBack;
	
	GuiSlider* sliderMusic;
	GuiSlider* sliderFx;

	GuiCheckBox* checkFullScreen;
	GuiCheckBox* checkVSync;

	bool LoadMenu(pugi::xml_node&);
	bool Menu(pugi::xml_document& loadFile);

private:

	pugi::xml_node load;
	pugi::xml_document loadFile;

	int currentLvl;
};

#endif // __SCENE_H__