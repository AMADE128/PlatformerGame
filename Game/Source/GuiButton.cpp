#include "GuiButton.h"
#include "Textures.h"
#include "Player.h"
#include "Render.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Update(Input* input, float dt)
{
	if (id == 2)
	{
		if (app->player->cont == false)
		{
			state = GuiControlState::DISABLED;
		}
		else if (app->player->cont == true)
		{
			state = GuiControlState::NORMAL;
		}
	
	}
	if (state != GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		input->GetMousePosition(mouseX, mouseY);
		mouseX += app->render->camera.x * -1;
		mouseY += app->render->camera.y * -1;

		// Check collision between mouse and button bounds
		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;

			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
			}

			// If mouse button pressed -> Generate event!
			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				NotifyObserver();
			}
		}
		else state = GuiControlState::NORMAL;
	}

	return false;
}

bool GuiButton::Draw(Render* render)
{
	// Draw the right button depending on state
	switch (state)
	{
	case GuiControlState::DISABLED:
		section = { 330, 0, BUTT_WIDTH, BUTT_HEIGHT };
		render->DrawTexture(texture, bounds.x, bounds.y, &section);
		break;
	case GuiControlState::NORMAL:
		section = { 0, 0, BUTT_WIDTH, BUTT_HEIGHT };
		render->DrawTexture(texture,bounds.x,bounds.y,&section);
		break;
	case GuiControlState::FOCUSED:
		section = { 220, 0, BUTT_WIDTH, BUTT_HEIGHT };
		render->DrawTexture(texture, bounds.x, bounds.y, &section);
		break;
	case GuiControlState::PRESSED:
		section = { 111, 0, BUTT_WIDTH - 1, BUTT_HEIGHT };
		render->DrawTexture(texture, bounds.x, bounds.y, &section);
		break;
	case GuiControlState::SELECTED:
		break;
	default:
		break;
	}

	return false;
}