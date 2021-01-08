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

	SDL_Texture* winTex;
	SDL_Texture* appleTex;
	char scoreText[30] = { "\0" };
	char highscoreText[30] = { "\0" };
	char secondsText[10] = { "\0" };
	char minutesText[10] = { "\0" };
	char msText[10] = { "\0" };
	char applesText[10] = { "\0" };

private:

	GuiButton* btnBack;

	unsigned int winMusic;
};

#endif // __SCENE_H__