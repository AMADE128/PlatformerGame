#include "FadeToBlack.h"

#include "App.h"
#include "Render.h"
#include "Window.h"
#include "Log.h"

#include "SDL/include/SDL_render.h"

FadeToBlack::FadeToBlack() : Module()
{
}

FadeToBlack::~FadeToBlack()
{}

bool FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	screenRect = { 0, 0, app->win->screenSurface->w, app->win->screenSurface->h };
	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool FadeToBlack::Update(float dt)
{
	// Exit this function if we are not performing a fade
	if (currentStep == FadeStep::NONE) return true;

	if (currentStep == FadeStep::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			app->screen = screenTo;
			currentStep = FadeStep::FROM_BLACK;
		}
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			currentStep = FadeStep::NONE;
		}
	}

	return true;
}

bool FadeToBlack::PostUpdate()
{
	// Exit this function if we are not performing a fade
	if (currentStep == FadeStep::NONE) return true;

	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screenRect);

	return true;
}

bool FadeToBlack::Fade(game_screens screen, float frames)
{
	bool ret = false;

	// If we are already in a fade process, ignore this call
	if (currentStep == FadeStep::NONE)
	{
		currentStep = FadeStep::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;
		screenTo = screen;

		ret = true;
	}

	return ret;
}