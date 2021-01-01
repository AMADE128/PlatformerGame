#ifndef __SCENEMENU_H__
#define __SCENEMENU_H__

#include "Module.h"
#include "GuiButton.h"

struct SDL_Texture;

class SceneMenu : public Module
{
public:

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

	bool LoadMenu(pugi::xml_node&);
	bool SaveMenu(pugi::xml_node&);
	bool MenuLoad(pugi::xml_document& loadFile);
	bool MenuSave(pugi::xml_document& loadFile);

	bool OnGuiMouseClickEvent(GuiControl* control);

	SDL_Texture* menu;
	SDL_Texture* newButt;
	SDL_Texture* loadButt;
	SDL_Texture* optionButt;
	SDL_Texture* exitButt;

	unsigned int musicMenu;

	GuiButton* btnNew;
	GuiButton* btnExit;
	GuiButton* btnLoad;
	GuiButton* btnOptions;

	int checkContinue = 0;
	bool newGame = false;
	bool saved = false;

private:

	pugi::xml_node load;
	pugi::xml_document loadFile;
};

#endif // __SCENE_H__