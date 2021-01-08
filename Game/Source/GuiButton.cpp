#include "GuiButton.h"
#include "Textures.h"
#include "Player.h"
#include "Window.h"
#include "Audio.h"
#include "SceneMenu.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Update(Input* input, float dt)
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
			if (hover == false)
			{
				if(app->sceneMenu->active == true)app->audio->PlayFx(app->sceneMenu->hoverFx);
				else app->audio->PlayFx(app->player->menuHoverFx);
				hover = true;
			}
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
		else
		{
			state = GuiControlState::NORMAL;
			hover = false;
		}
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
		render->DrawTexture(text, bounds.x, bounds.y);
		if (app->guiDebug) render->DrawRectangle(bounds, { 100, 100, 100, 150 });
		break;
	case GuiControlState::NORMAL:
		section = { 0, 0, BUTT_WIDTH, BUTT_HEIGHT };
		render->DrawTexture(texture,bounds.x,bounds.y,&section);
		render->DrawTexture(text, bounds.x, bounds.y);
		if (app->guiDebug) render->DrawRectangle(bounds, { 0, 255, 0, 150 });
		break;
	case GuiControlState::FOCUSED:
		section = { 220, 0, BUTT_WIDTH, BUTT_HEIGHT };
		render->DrawTexture(texture, bounds.x, bounds.y, &section);
		render->DrawTexture(text, bounds.x, bounds.y);
		if (app->guiDebug) render->DrawRectangle(bounds, { 255, 255, 0, 150 });
		break;
	case GuiControlState::PRESSED:
		section = { 111, 0, BUTT_WIDTH - 1, BUTT_HEIGHT };
		render->DrawTexture(texture, bounds.x, bounds.y, &section);
		render->DrawTexture(text, bounds.x, bounds.y + 5);
		if (app->guiDebug) render->DrawRectangle(bounds, { 0, 255, 255, 150 });
		break;
	case GuiControlState::SELECTED:
		break;
	default:
		break;
	}

	return false;
}