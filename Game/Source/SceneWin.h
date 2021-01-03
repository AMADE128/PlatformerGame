#ifndef __SCENEWIN_H__
#define __SCENEWIN_H__

#include "Module.h"
#include "GuiButton.h"

struct SDL_Texture;

class SceneWin : public Module
{
public:

	SceneWin();

	// Destructor
	virtual ~SceneWin();

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

	SDL_Texture* win;

private:

	GuiButton* btnBack;

	unsigned int winMusic;
};

#endif // __SCENE_H__