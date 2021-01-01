#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

#define BUTT_WIDTH 110
#define BUTT_HEIGHT 50

class GuiButton : public GuiControl
{
public:

	GuiButton(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiButton();

	bool Update(Input* input, float dt);
	bool Draw(Render* render);
	bool Load(pugi::xml_node& data);

private:

	// Gui Button specific properties
	// Maybe some animation properties for state change?
};

#endif // __GUIBUTTON_H__
