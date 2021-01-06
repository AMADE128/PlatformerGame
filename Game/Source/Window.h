#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class Window : public Module
{
public:

	Window();

	// Destructor
	virtual ~Window();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Changae title
	void SetTitle(const char* title);

	// Retrive window size
	void GetWindowSize(uint& width, uint& height) const;

	// Retrieve window scale
	float GetScale() const;

	void GetScreenSize(uint& width, uint& height) const;

public:
	// The window we'll be rendering to
	SDL_Window* window;
	bool fullScreen = false;

	// The surface contained by the window
	SDL_Surface* screenSurface;
	uint screenWidth;
	uint screenHeight;
	uint width;
	uint height;
	float scale;

private:
	SString title;
};

#endif // __WINDOW_H__