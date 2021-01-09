#include "GuiCheckBox.h"
#include "Window.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
}

GuiCheckBox::~GuiCheckBox()
{
}

bool GuiCheckBox::Update(Input* input, float dt)
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
			}

			// If mouse button pressed -> Generate event!
			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				checked = !checked;
				NotifyObserver();
			}
		}
		else state = GuiControlState::NORMAL;
	}

	return false;
}

bool GuiCheckBox::Draw(Render* render)
{
	// Draw the right button depending on state
	switch (state)
	{
	case GuiControlState::DISABLED:
		break;
	case GuiControlState::NORMAL:
		section = { 0, 0, SMALL_BUTT_WIDTH, SMALL_BUTT_HEIGHT };
		if (leftText != nullptr)
		{
			render->DrawTexture(leftText, bounds.x - 150, bounds.y);
		}
		render->DrawTexture(texture, bounds.x, bounds.y, &section);
		if (id != 2)
		{
			if (checked)
			{
				checkSection = { 50, 0, SMALL_BUTT_WIDTH, SMALL_BUTT_HEIGHT };
				render->DrawTexture(text, bounds.x, bounds.y, &checkSection);
			}
			else
			{
				checkSection = { 0, 0, SMALL_BUTT_WIDTH, SMALL_BUTT_HEIGHT };
				render->DrawTexture(text, bounds.x, bounds.y, &checkSection);
			}
		}
		else
		{
			checkSection = { 0, 0, SMALL_BUTT_WIDTH, SMALL_BUTT_HEIGHT };
			render->DrawTexture(text, bounds.x + 1, bounds.y - 1, &checkSection);
		}
		if (app->guiDebug) render->DrawRectangle(bounds, { 0, 255, 0, 150 });
		break;
	case GuiControlState::FOCUSED:
		section = { 100, 0, SMALL_BUTT_WIDTH, SMALL_BUTT_HEIGHT };
		render->DrawTexture(leftText, bounds.x - 150, bounds.y);
		render->DrawTexture(texture, bounds.x, bounds.y, &section);
		if (id != 2)
		{
			if (checked)
			{
				checkSection = { 50, 0, SMALL_BUTT_WIDTH, SMALL_BUTT_HEIGHT };
				render->DrawTexture(text, bounds.x, bounds.y, &checkSection);
			}
			else
			{
				checkSection = { 0, 0, SMALL_BUTT_WIDTH, SMALL_BUTT_HEIGHT };
				render->DrawTexture(text, bounds.x, bounds.y, &checkSection);
			}
		}
		else
		{
			checkSection = { 0, 0, SMALL_BUTT_WIDTH, SMALL_BUTT_HEIGHT };
			render->DrawTexture(text, bounds.x + 1, bounds.y - 1, &checkSection);
		}
		if (app->guiDebug) render->DrawRectangle(bounds, { 255, 255, 0, 150 });
		break;
	case GuiControlState::PRESSED:
		section = { 50, 0, SMALL_BUTT_WIDTH, SMALL_BUTT_HEIGHT };
		render->DrawTexture(leftText, bounds.x - 150, bounds.y);
		render->DrawTexture(texture, bounds.x, bounds.y, &section);
		if (id != 2)
		{
			if (checked)
			{
				checkSection = { 50, 0, SMALL_BUTT_WIDTH, SMALL_BUTT_HEIGHT };
				render->DrawTexture(text, bounds.x, bounds.y, &checkSection);
			}
			else
			{
				checkSection = { 0, 0, SMALL_BUTT_WIDTH, SMALL_BUTT_HEIGHT };
				render->DrawTexture(text, bounds.x, bounds.y, &checkSection);
			}
		}
		else
		{
			checkSection = { 0, 0, SMALL_BUTT_WIDTH, SMALL_BUTT_HEIGHT };
			render->DrawTexture(text, bounds.x + 1, bounds.y + 4, &checkSection);
		}
		if (app->guiDebug) render->DrawRectangle(bounds, { 0, 255, 255, 150 });
		break;
	case GuiControlState::SELECTED:
		break;
	default:
		break;
	}

	return false;
}