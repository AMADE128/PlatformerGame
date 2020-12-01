#ifndef __FADETOBLACK_H__
#define __FADETOBLACK_H__

#include "Module.h"
#include "SDL\include\SDL_rect.h"
#include "App.h"

class FadeToBlack : public Module
{
public:
	//Constructor
	FadeToBlack();

	//Destructor
	virtual ~FadeToBlack();

	// Called when the module is activated
	// Enables the blending mode for transparency
	bool Start();

	// Called at the middle of the application loop
	// Updates the fade logic
	bool Update(float dt);

	// Called at the end of the application loop
	// Performs the render call of a black rectangle with transparency
	bool PostUpdate();

	// Called from another module
	// Starts the fade process which has two steps, fade_out and fade_in
	// After the first step, the modules should be switched
	bool Fade(game_screens screen, float frames = 60);

private:

	enum FadeStep
	{
		NONE,
		TO_BLACK,
		FROM_BLACK
	} currentStep = FadeStep::NONE;

	// A frame count system to handle the fade time and ratio
	Uint32 frameCount = 0;
	Uint32 maxFadeFrames = 0;

	// The rectangle of the screen, used to render the black rectangle
	SDL_Rect screenRect;

	// The modules that should be switched after the first step
	game_screens screenTo;
};

#endif //__MODULEFADETOBLACK_H__