#include "GuiSlider.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, int min, int max) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->minValue = min;
	this->maxValue = max;
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(Input* input, float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		input->GetMousePosition(mouseX, mouseY);

		// Check collision between mouse and button bounds
		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;

			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
			}

			
			// TODO.
		}
		else state = GuiControlState::NORMAL;
	}

	return false;
}

bool GuiSlider::Draw(Render* render)
{
	// Draw the right button depending on state
	switch (state)
	{
	case GuiControlState::DISABLED: 
		break;
	case GuiControlState::NORMAL: 
		section = { 0, 0, SLIDER_WIDTH, SLIDER_HEIGHT };
		render->DrawTexture(texture, bounds.x, bounds.y, &section);
		section = { 0, 50, SLIDER_WIDTH, SLIDER_HEIGHT };
		render->DrawTexture(texture, bounds.x, bounds.y, &section);
		break;
	case GuiControlState::FOCUSED: 
		section = { 0, 0, SLIDER_WIDTH, SLIDER_HEIGHT };
		render->DrawTexture(texture, bounds.x, bounds.y, &section);
		section = { 0, 50, SLIDER_WIDTH, SLIDER_HEIGHT };
		render->DrawTexture(texture, bounds.x, bounds.y, &section);
		break;
	case GuiControlState::PRESSED: 
		section = { 0, 0, SLIDER_WIDTH, SLIDER_HEIGHT };
		render->DrawTexture(texture, bounds.x, bounds.y, &section);
		section = { 0, 50, SLIDER_WIDTH, SLIDER_HEIGHT };
		render->DrawTexture(texture, bounds.x, bounds.y, &section);
		break;
	case GuiControlState::SELECTED: 
		break;
	default:
		break;
	}

	return false;
}
