#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

#define SMALL_BUTT_WIDTH 50
#define SMALL_BUTT_HEIGHT 50

class GuiCheckBox : public GuiControl
{
public:

	GuiCheckBox(uint32 id, SDL_Rect bounds);
	virtual ~GuiCheckBox();

	bool Update(Input* input, float dt);
	bool Draw(Render* render);

	bool checked;
	SDL_Texture* leftText = nullptr;

private:
};

#endif // __GUICHECKBOX_H__