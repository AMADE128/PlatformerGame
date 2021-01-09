#include "GuiSlider.h"
#include "Window.h"
#include "Audio.h"


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
		mouseX += app->render->camera.x * -1 / app->win->scale;
		mouseY += app->render->camera.y * -1 / app->win->scale;

		// Check collision between mouse and button bounds
		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;

			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
				value = mouseX - bounds.x;
			}
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
		render->DrawTexture(text, bounds.x - 120, bounds.y);
		section = { 0, 49, value, SLIDER_HEIGHT };
		render->DrawTexture(texture, bounds.x, bounds.y, &section);
		if (app->guiDebug) render->DrawRectangle(bounds, { 0, 255, 0, 150 });
		break;
	case GuiControlState::FOCUSED: 
		section = { 0, 0, SLIDER_WIDTH, SLIDER_HEIGHT };
		render->DrawTexture(texture, bounds.x, bounds.y, &section);
		render->DrawTexture(text, bounds.x - 120, bounds.y);
		section = { 0, 49, value, SLIDER_HEIGHT };
		render->DrawTexture(texture, bounds.x, bounds.y, &section);
		if (app->guiDebug) render->DrawRectangle(bounds, { 255, 255, 0, 150 });
		break;
	case GuiControlState::PRESSED: 
		section = { 0, 0, SLIDER_WIDTH, SLIDER_HEIGHT };
		render->DrawTexture(texture, bounds.x, bounds.y, &section);
		render->DrawTexture(text, bounds.x - 120, bounds.y);
		section = { 0, 49, value, SLIDER_HEIGHT };
		render->DrawTexture(texture, bounds.x, bounds.y, &section);
		if (app->guiDebug) render->DrawRectangle(bounds, { 0, 255, 255, 150 });
		break;
	case GuiControlState::SELECTED: 
		break;
	default:
		break;
	}

	return false;
}
