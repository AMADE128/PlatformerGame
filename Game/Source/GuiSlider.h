#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

#define SLIDER_HEIGHT 50
#define SLIDER_WIDTH 137

class GuiSlider : public GuiControl
{
public:

	GuiSlider(uint32 id, SDL_Rect bounds, int min, int max);
	virtual ~GuiSlider();

	bool Update(Input* input, float dt);
	bool Draw(Render* render);

	int value = 64;

	int minValue;
	int maxValue;

private:
};

#endif // __GUISLIDER_H__
