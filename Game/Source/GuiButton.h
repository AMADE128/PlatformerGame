#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"
#include "Textures.h"

#define BUTT_WIDTH 110
#define BUTT_HEIGHT 50

class GuiButton : public GuiControl
{
public:

	GuiButton(uint32 id, SDL_Rect bounds);
	virtual ~GuiButton();

	bool Update(Input* input, float dt);
	bool Draw(Render* render);
	bool hover = false;

private:

	// Gui Button specific properties
	// Maybe some animation properties for state change?
};

#endif // __GUIBUTTON_H__
