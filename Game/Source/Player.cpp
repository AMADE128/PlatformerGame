#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Log.h"
#include "Render.h"
#include "Input.h"
#include "Scene.h"
#include "Window.h"
#include "Map.h"
#include "Audio.h"
#include "SceneMenu.h"
#include "FadeToBlack.h"
#include "ModuleParticles.h"
#include "Fonts.h"
#include "SceneLvl2.h"
#include "ModuleParticles.h"
#include "SceneIntro.h"
#include "SceneWin.h"
#include "SceneLoose.h"

#include "Collisions.h"

#define TILESIZE 96
#define POSXINIT 720
#define POXYINIT 1584

Player::Player() : Module()
{
	name.Create("player");
	
	btnResume = new GuiButton(6, { NULL, NULL, BUTT_WIDTH, BUTT_HEIGHT });
	btnResume->SetObserver(this);

	btnSettings = new GuiButton(7, { NULL, NULL, BUTT_WIDTH, BUTT_HEIGHT });
	btnSettings->SetObserver(this);

	btnBack = new GuiButton(8, { NULL, NULL, BUTT_WIDTH, BUTT_HEIGHT });
	btnBack->SetObserver(this);

	btnExit = new GuiButton(9, { NULL, NULL, BUTT_WIDTH, BUTT_HEIGHT });
	btnExit->SetObserver(this);

	for (int i = 0; i < TILESIZE * 11; i += TILESIZE)
	{
		idleAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	idleAnim.loop = true;
	idleAnim.speed = 5.0f;

	for (int i = 0; i < TILESIZE * 12; i += TILESIZE)
	{
		runAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	runAnim.loop = true;
	runAnim.speed = 5.0f;

	fallAnim.PushBack({ 0, 0, TILESIZE, TILESIZE });

	jumpAnim.PushBack({ 0, 0, TILESIZE, TILESIZE });

	for (int i = 0; i < TILESIZE * 7; i += TILESIZE)
	{
		deathAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	deathAnim.loop = false;
	deathAnim.speed = 5.0f;

	for (int i = 0; i < TILESIZE * 6; i += TILESIZE)
	{
		doubleJumpAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	doubleJumpAnim.loop = false;
	doubleJumpAnim.speed = 5.0f;

	for (int i = 0; i < TILESIZE * 7; i += TILESIZE)
	{
		appeAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	appeAnim.loop = false;
	appeAnim.speed = 5.0f;

	for (int i = 0; i < TILESIZE * 7; i += TILESIZE)
	{
		desAppeAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	desAppeAnim.loop = false;
	desAppeAnim.speed = 5.0f;

	for (int i = 0; i < 60 * 6; i += 60)
	{
		dronAnim.PushBack({ i, 0, 60, 76 });
	}
	dronAnim.loop = true;
	dronAnim.speed = 0.4f;

	for (int i = 0; i < 40 * 8; i += 40)
	{
		chickenAnim.PushBack({ i, 0, 40, 48 });
	}
	chickenAnim.loop = true;
	chickenAnim.speed = 0.3f;


}

// Destructor
Player::~Player()
{}

bool Player::Awake()
{
	LOG("Init Player");
	bool ret = true;

	return ret;
}

bool Player::Start()
{
	LOG("Loading player textures");

	btnResume->texture = btnSettings->texture = btnBack->texture = btnExit->texture = app->tex->Load("Assets/Textures/Interface/button.png");
	boxTex = app->tex->Load("Assets/Textures/Interface/box.png");
	btnResume->font = app->tex->Load("Assets/Textures/Interface/start.png");
	btnSettings->font = app->tex->Load("Assets/Textures/Interface/option.png");
	btnBack->font = app->tex->Load("Assets/Textures/Interface/back.png");
	btnExit->font = app->tex->Load("Assets/Textures/Interface/exit.png");
	
	if (app->scene->active == true && cont == false)
	{
		position.x = 720;
		position.y = 1584;
	}
	else if (app->sceneLvl2->active == true && cont == false)
	{
		position.x = 620;
		position.y = 2256;
	}
	//Cargar texturas
	idleTex = app->tex->Load("Assets/Textures/Character/idle.png");
	fallTex = app->tex->Load("Assets/Textures/Character/fall.png");
	deathTex = app->tex->Load("Assets/Textures/Character/hit.png");
	runTex = app->tex->Load("Assets/Textures/Character/run.png");
	jumpTex = app->tex->Load("Assets/Textures/Character/jump.png");
	doubleJumpTex = app->tex->Load("Assets/Textures/Character/double_jump.png");
	lifesTex = app->tex->Load("Assets/Textures/Character/life.png");
	appeTex = app->tex->Load("Assets/Textures/Character/appearing.png");
	desAppeTex = app->tex->Load("Assets/Textures/Character/desappearing.png");
	appleTex = app->tex->Load("Assets/Textures/Items/Fruits/apple.png");
	dronTex = app->tex->Load("Assets/Textures/Drone/flying.png");
	chickenFlyTex = app->tex->Load("Assets/Textures/Drone/idle_skill.png");

	hitFx = app->audio->LoadFx("Assets/Audio/MyscMusic/damage.wav");
	app->musicList.Add(&hitFx);
	jumpFx = app->audio->LoadFx("Assets/Audio/MyscMusic/jumps.wav");
	app->musicList.Add(&jumpFx);
	pointFx = app->audio->LoadFx("Assets/Audio/MyscMusic/points.wav");
	app->musicList.Add(&pointFx);
	leafFx = app->audio->LoadFx("Assets/Audio/MyscMusic/leaf_shot.wav");
	app->musicList.Add(&leafFx);

	playerColl = app->collision->AddCollider({ (int)position.x, (int)position.y, TILESIZE - 50, TILESIZE - 20}, Collider::Type::PLAYER, this);
	cameraColl = app->collision->AddCollider({ (int)position.x - 100, (int)position.y - 100, app->render->camera.w/4, app->render->camera.h / 3 + 20}, Collider::Type::CAMERA, this);
	app->render->camera.x = ((cameraColl->rect.x + cameraColl->rect.w / 3) - (app->render->camera.w / 2)) * -1;
	app->render->camera.y = ((cameraColl->rect.y + cameraColl->rect.h / 3) - (app->render->camera.h / 2.5)) * -1;

	playerState = NORMAL;

	currentAnimation = &idleAnim;
	death = false;
	appear = true;
	currentTex = idleTex;
	flip = false;
	speedY = 0.3f;
	return true;
}

bool Player::PreUpdate()
{
	if (changeLevel1 == true)
	{
		changeLevel1 = false;
		app->fadeToBlack->Fade(app->sceneLvl2, (Module*)app->scene, 80);
	}
	else if (changeLevel2 == true)
	{
		changeLevel2 = false;
		app->fadeToBlack->Fade(app->scene, (Module*)app->sceneLvl2, 80);
	}

	if (appear == true)
	{
		app->render->camera.x = ((cameraColl->rect.x + cameraColl->rect.w / 3) - (app->render->camera.w / 2)) * -1;
		app->render->camera.y = ((cameraColl->rect.y + cameraColl->rect.h / 3) - (app->render->camera.h / 2.5)) * -1;
		currentAnimation = &appeAnim;
		currentTex = appeTex;
		if (appeAnim.finish == true)
		{
			appear = false;
			appeAnim.Reset();
		}
	}
	return true;
}

bool Player::Update(float dt)
{

	switch (playerState)
	{
	case Player::NORMAL:
		playerSave = false;
		dt *= 6;
		idleAnim.speed = 4.0f * dt;
		runAnim.speed = 5.0f * dt;
		deathAnim.speed = 4.0f * dt;
		doubleJumpAnim.speed = 4.0f * dt;
		desAppeAnim.speed = 4.0f * dt;
		appeAnim.speed = 4.0f * dt;
		dronAnim.speed = 4.0f * dt;
		chickenAnim.speed = 4.0f * dt;
		if (death == false && appear == false)
		{
			if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
			{
				if (app->scene->active == true)
				{
					position.x = 720;
					position.y = 1584;
					playerColl->SetPos(position.x + 25, position.y + 20);
					cameraColl->rect.x = position.x - 100;
					cameraColl->rect.y = position.y - 100;
					speedX = 0;
					speedY = 0;
				}
				else if (app->sceneLvl2->active == true)
				{
					position.x = 620;
					position.y = 2256;
					playerColl->SetPos(position.x + 25, position.y + 20);
					cameraColl->rect.x = position.x - 100;
					cameraColl->rect.y = position.y - 100;
					speedX = 0;
					speedY = 0;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && !isJumping)
			{
				cont = true;
				playerSave = true;
				playerLoadF6 = true;
				auxp.x = position.x;
				auxp.y = position.y;
				if (currentLvl == 1)
				{
					lvl = 1;
				}
				else if (currentLvl == 2)
				{
					lvl = 2;
				}
				app->SaveGameRequest();
			}
			if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
			{
				if (lvl == 2)
				{
					if (app->scene->active == true)
					{
						saveGame = true;
						changeLevel2 = true;
					}
					else if (app->sceneLvl2->active == true) app->LoadGameRequest();
				}
				else if (lvl == 1)
				{
					if (app->sceneLvl2->active == true)
					{
						saveGame = true;
						changeLevel1 = true;
					}
					else if (app->scene->active == true) app->LoadGameRequest();
				}
				appear = true;
			}
			if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && yDownCollision == true)
			{
				if (god == false)
				{
					god = true;
					playerColl->type = Collider::NONE;

				}
				else if (god == true)
				{
					god = false;
					playerColl->type = Collider::PLAYER;
				}

			}
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && isJumping)
			{
				if (!secondJump)
				{
					app->audio->PlayFx(jumpFx);
					speedY = 20 * dt;
					secondJump = true;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && yDownCollision == true && god == false)
			{
				if (!isJumping) // Solo salta cuando no est� en el aire
				{
					app->audio->PlayFx(jumpFx);
					isJumping = true;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
			{
				playerState = SETTINGS;
			}
			if (app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN && flip == true)
			{
				app->moduleParticles->AddParticle(app->moduleParticles->leftLeaf, position.x - 30, position.y, Collider::Type::LEAF);
				app->audio->PlayFx(leafFx);
			}
			if (app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN && flip == false)
			{
				app->moduleParticles->AddParticle(app->moduleParticles->rightLeaf, position.x - 30, position.y, Collider::Type::LEAF);
				app->audio->PlayFx(leafFx);
			}
			if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN && flip == false && skillCoolDown == 0)
			{
				skillCoolDown = 200;
				app->moduleParticles->AddParticle(app->moduleParticles->chickenFall, position.x + 200, position.y - 350, Collider::Type::CHICKEN);

			}
			if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN && flip == true && skillCoolDown == 0)
			{
				skillCoolDown = 200;
				app->moduleParticles->AddParticle(app->moduleParticles->chickenFall, position.x - 200, position.y - 350, Collider::Type::CHICKEN);

			}
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && god == true)
			{
				speedY = -80 * dt;
				position.y += speedY;
			}
			else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && god == true)
			{
				speedY = 80 * dt;
				position.y += speedY;
			}
			else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
			{
				if (god == true)
				{
					speedXLastFrame = speedX;
					speedX = -80 * dt;
					position.x += speedX;
				}
				else if (god == false && xLeftCollision == false)
				{
					xRightCollision = false;
					speedXLastFrame = speedX;
					speedX = -60 * dt;
					position.x += speedX;

					if (yDownCollision == true)
					{
						currentTex = runTex;
						currentAnimation = &runAnim;
						flip = true;
					}
					else
					{
						flip = true;
						if ((-30 * dt + speedY) < 0)
						{
							if (speedX <= 0 && speedXLastFrame < 0)
							{
								if (secondJump && doubleJumpAnim.finish == false)
								{
									currentAnimation = &doubleJumpAnim;
									currentTex = doubleJumpTex;
								}
								if (doubleJumpAnim.finish == true || !secondJump)
								{
									currentAnimation = &jumpAnim;
									currentTex = jumpTex;
								}
							}
						}
						else if ((-30 * dt + speedY) >= 0)
						{
							if (speedX <= 0 && speedXLastFrame < 0)
							{
								if (secondJump && doubleJumpAnim.finish == false)
								{
									currentAnimation = &doubleJumpAnim;
									currentTex = doubleJumpTex;
								}
								if (doubleJumpAnim.finish == true || !secondJump)
								{
									currentAnimation = &fallAnim;
									currentTex = fallTex;
								}
							}
						}
					}
				}

			}
			else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
			{
				if (god == true)
				{
					speedXLastFrame = speedX;
					speedX = 80 * dt;
					position.x += speedX;
				}
				else if (god == false && xRightCollision == false)
				{
					xLeftCollision = false;
					speedXLastFrame = speedX;
					speedX = 60 * dt;
					position.x += speedX;

					if (yDownCollision == true)
					{
						currentTex = runTex;
						currentAnimation = &runAnim;
						flip = false;
					}
					else
					{
						flip = false;
						if ((-30 * dt + speedY) < 0)
						{
							if (speedX >= 0 && speedXLastFrame > 0)
							{
								if (secondJump && doubleJumpAnim.finish == false)
								{
									currentAnimation = &doubleJumpAnim;
									currentTex = doubleJumpTex;
								}
								if (doubleJumpAnim.finish == true || !secondJump)
								{
									currentAnimation = &jumpAnim;
									currentTex = jumpTex;
								}
							}
						}
						else if ((-30 * dt + speedY) >= 0)
						{
							if (speedX >= 0 && speedXLastFrame > 0)
							{
								if (secondJump && doubleJumpAnim.finish == false)
								{
									currentAnimation = &doubleJumpAnim;
									currentTex = doubleJumpTex;
								}
								if (doubleJumpAnim.finish == true || !secondJump)
								{
									currentAnimation = &fallAnim;
									currentTex = fallTex;
								}
							}
						}
					}
				}

			}
			else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP || app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
			{
				speedX = 0;
			}
			else
			{
				if (speedX == 0)
				{
					if (speedY == 0)
					{
						currentAnimation = &idleAnim;
						currentTex = idleTex;
						if (speedXLastFrame > 0)
						{
							flip = false;
						}
						else if (speedXLastFrame < 0)
						{
							flip = true;
						}
					}
					else if ((-30 * dt + speedY) < 0)
					{
						if (secondJump && doubleJumpAnim.finish == false)
						{
							currentAnimation = &doubleJumpAnim;
							currentTex = doubleJumpTex;
						}
						if (doubleJumpAnim.finish == true || !secondJump)
						{
							currentAnimation = &jumpAnim;
							currentTex = jumpTex;
						}
						if (speedXLastFrame > 0)
						{
							flip = false;
						}
						else if (speedXLastFrame < 0)
						{
							flip = true;
						}
					}
					else if ((-30 * dt + speedY) >= 0)
					{
						if (secondJump && doubleJumpAnim.finish == false)
						{
							currentAnimation = &doubleJumpAnim;
							currentTex = doubleJumpTex;
						}
						if (doubleJumpAnim.finish == true || !secondJump)
						{
							currentAnimation = &fallAnim;
							currentTex = fallTex;
						}
						if (speedXLastFrame > 0)
						{
							flip = false;
						}
						else if (speedXLastFrame < 0)
						{
							flip = true;
						}
					}
				}

			}

			if (isJumping)
			{
				xLeftCollision = false;
				xRightCollision = false;
				position.y += -100 * dt;
			}
			if (secondJump)
			{
				xLeftCollision = false;
				xRightCollision = false;
				position.y += -20 * dt;
			}

			if (yDownCollision == true)
			{
				speedY = 0;
			}
			else
			{
				if (app->fps <= 32)
				{
					speedY += 10.0f * dt;
				}
				else
				{
					speedY += 4.0f * dt;
				}
			}

			position.y += speedY;


			playerColl->SetPos(position.x + 25, position.y + 20);
			app->render->camera.x = ((cameraColl->rect.x + cameraColl->rect.w / 3) - (app->render->camera.w / 2)) * -1;
			app->render->camera.y = ((cameraColl->rect.y + cameraColl->rect.h / 3) - (app->render->camera.h / 2.5)) * -1;
		}
		if (skillCoolDown > 0)
		{
			skillCoolDown--;
		}
		break;
	case Player::SETTINGS:
		btnResume->Update(app->input, dt);
		btnSettings->Update(app->input, dt);
		btnBack->Update(app->input, dt);
		btnExit->Update(app->input, dt);
		break;
	case Player::EXIT:
		return false;
		break;
	default:
		break;
	}

	return true;
}

bool Player::PostUpdate()
{

	if (flip == false) 
	{
		Animation* dron = &dronAnim;
		if (playerState == NORMAL)
		{
			dron->Update();
		}
		SDL_Rect dronRect = dron->GetCurrentFrame();
		app->render->DrawTextureFlip(dronTex, position.x - 50, position.y - 10, &dronRect);
	}
	else if (flip == true)
	{
		Animation* dron = &dronAnim;
		if (playerState == NORMAL)
		{
			dron->Update();
		}
		SDL_Rect dronRect = dron->GetCurrentFrame();
		app->render->DrawTexture(dronTex, position.x - 50, position.y - 10, &dronRect);
	}
	if (flip == false && skillCoolDown == 0)
	{
		Animation* chicken = &chickenAnim;
		if (playerState == NORMAL)
		{
			chicken->Update();
		}
		SDL_Rect chickenRect = chicken->GetCurrentFrame();
		app->render->DrawTextureFlip(chickenFlyTex, position.x + 80, position.y - 10, &chickenRect);
	}
	else if (flip == true && skillCoolDown == 0)
	{
		Animation* chicken = &chickenAnim;
		if (playerState == NORMAL)
		{
			chicken->Update();
		}
		SDL_Rect chickenRect = chicken->GetCurrentFrame();
		app->render->DrawTexture(chickenFlyTex, position.x + 80, position.y - 10, &chickenRect);
	}

	if (playerState == NORMAL)
	{
		currentAnimation->Update();
	}
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	if (flip == true)
	{
		app->render->DrawTextureFlip(currentTex, position.x, position.y, &rect);
	}
	else if (flip == false)
	{
		app->render->DrawTexture(currentTex, position.x, position.y, &rect);
	}
	if (lifes >= 1)
	{
		app->render->DrawTexture(lifesTex, (app->render->camera.x - 20) * -1, (app->render->camera.y - 15) * -1, NULL);

	}
	if (lifes >= 2)
	{
		app->render->DrawTexture(lifesTex, (app->render->camera.x - 100) * -1, (app->render->camera.y - 15) * -1, NULL);
	}
	if (lifes >= 3)
	{
		app->render->DrawTexture(lifesTex, (app->render->camera.x - 180) * -1, (app->render->camera.y - 15) * -1, NULL);
	}

	app->render->DrawTexture(appleTex, (app->render->camera.x - 1200) * -1, (app->render->camera.y - 0) * -1, NULL);
	sprintf_s(scoreText, 10, "%d", appleCounter);
	if(appleCounter < 10)
	{
		app->fonts->BlitText((app->render->camera.x - 1185) * -1, (app->render->camera.y - 17) * -1, scoreFont, scoreText);
	}
	else if (appleCounter >= 10)
	{
		app->fonts->BlitText((app->render->camera.x - 1145) * -1, (app->render->camera.y - 17) * -1, scoreFont, scoreText);
	}

	if (playerState == SETTINGS)
	{
		btnResume->bounds.x = (app->render->camera.x - app->render->camera.w / 2 + btnResume->bounds.w / 2) * -1;
		btnResume->bounds.y = (app->render->camera.y - app->render->camera.h / 2 + 100) * -1;

		btnSettings->bounds.x = (app->render->camera.x - app->render->camera.w / 2 + btnResume->bounds.w / 2) * -1;
		btnSettings->bounds.y = (app->render->camera.y - app->render->camera.h / 2 + 45) * -1;

		btnBack->bounds.x = (app->render->camera.x - app->render->camera.w / 2 + btnResume->bounds.w / 2) * -1;
		btnBack->bounds.y = (app->render->camera.y - app->render->camera.h / 2 - 15) * -1;

		btnExit->bounds.x = (app->render->camera.x - app->render->camera.w / 2 + btnResume->bounds.w / 2) * -1;
		btnExit->bounds.y = (app->render->camera.y - app->render->camera.h / 2 - 75) * -1;

		app->render->DrawRectangle({ app->render->camera.x * -1, app->render->camera.y * -1, 1280, 720 }, { 0, 0, 0, 170 });
		app->render->DrawTexture(boxTex, btnResume->bounds.x - 32, btnResume->bounds.y - 20);

		btnResume->Draw(app->render);
		btnSettings->Draw(app->render);
		btnBack->Draw(app->render);
		btnExit->Draw(app->render);
	}


	return true;
}

bool Player::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		switch (control->id)
		{
		case 6:
			playerState = NORMAL;
			break;
		case 7:
			break;
		case 8:
			app->fadeToBlack->Fade(app->scene, (Module*)app->sceneMenu, 10);
			break;
		case 9:
			playerState = EXIT;
			break;
		default:
			break;
		}
	}
	default: break;
	}

	return true;
}

bool Player::CleanUp()
{
	if (!active)
	{
		return true;
	}

	LOG("Unloading player");

	//Unload de texturas
	app->tex->UnLoad(idleTex);
	app->tex->UnLoad(fallTex);
	app->tex->UnLoad(deathTex);
	app->tex->UnLoad(runTex);
	app->tex->UnLoad(jumpTex);
	app->tex->UnLoad(doubleJumpTex);
	app->tex->UnLoad(lifesTex);
	app->tex->UnLoad(appeTex);
	app->tex->UnLoad(desAppeTex);
	app->tex->UnLoad(appleTex);
	app->tex->UnLoad(dronTex);
	app->tex->UnLoad(chickenFlyTex);
	app->tex->UnLoad(btnResume->texture);
	app->tex->UnLoad(btnSettings->texture);
	app->tex->UnLoad(btnBack->texture);
	app->tex->UnLoad(btnExit->texture);
	app->tex->UnLoad(boxTex);
	app->tex->UnLoad(resumeButtTex);
	app->tex->UnLoad(optionButtTex);
	app->tex->UnLoad(returnButtTex);
	app->tex->UnLoad(exitButtTex);

	//Remove de los colliders
	app->collision->RemoveCollider(playerColl);
	app->collision->RemoveCollider(cameraColl);

	//Unload de las pistas de audio
	app->audio->UnloadFX(jumpFx);
	app->audio->UnloadFX(hitFx);
	app->audio->UnloadFX(pointFx);
	app->audio->UnloadFX(leafFx);
	app->musicList.Clear();

	active = false;

	return true;
}

bool Player::LoadPlayerCamera()
{
	if (playerColl != NULL)
	{
		playerColl->SetPos(position.x + 25, position.y + 20);
		cameraColl->rect.x = playerColl->rect.x - 100;
		cameraColl->rect.y = playerColl->rect.y - 100;
		death = false;
	}

	return true;
}


bool Player::StopMovementY(Collider* c1, Collider* c2)
{
	if (c1->rect.y < c2->rect.y)
	{
		yUpCollision = false;
		if (isJumping == true)
		{
			yDownCollision = false;
		}
		else if (isJumping == false)
		{
			yDownCollision = true;
		}
		isJumping = false;
		secondJump = false;
		doubleJumpAnim.Reset();
		position.y = c2->rect.y - TILESIZE + 1;
	}
	else if (c1->rect.y  > c2->rect.y)
	{
		yDownCollision = false;
		yUpCollision = true;
		isJumping = false;
	}

	return true;
}

bool Player::StopMovement(Collider* c1, Collider* c2)
{
	if (c1->rect.x < c2->rect.x && (c1->rect.y + c1->rect.h - 3) > c2->rect.y)
	{
		xRightCollision = true;
		speedX = 0.f;
	}
	if (c1->rect.x > c2->rect.x && (c1->rect.y + c1->rect.h - 3) > c2->rect.y)
	{
		xLeftCollision = true;
		speedX = 0.f;
	}
	return true;
}

bool Player::Fall(Collider* c1, Collider* c2)
{
	yDownCollision = false;
	xLeftCollision = false;
	xRightCollision = false;
	return true;
}

bool Player::Die(Collider* c1, Collider* c2)
{
	if (c1->rect.y + TILESIZE > c2->rect.y + c2->rect.w/2)
	{
		death = true;
		currentAnimation = &deathAnim;
		currentTex = deathTex;
		if (speedXLastFrame > 0)
		{
			flip = false;
		}
		else if (speedXLastFrame < 0)
		{
			flip = true;
		}
		if (deathAnim.finish == true)
		{
			if (lifes <= 1)
			{
				deathAnim.Reset();
				playerColl->SetPos(position.x + 25, position.y + 20);
				cameraColl->rect.x = position.x - 100;
				cameraColl->rect.y = position.y - 100;
				speedX = 0;
				speedY = 0;
				if (app->scene->active == true)
				{
					app->fadeToBlack->Fade(app->scene, (Module*)app->sceneLoose, 10);
				}
				else if (app->sceneLvl2->active == true)
				{
					app->fadeToBlack->Fade(app->sceneLvl2, (Module*)app->sceneLoose, 10);
				}
			}
			else
			{
				speedX = 0;
				speedY = 0;
				isJumping = false;
				if (app->scene->savePoint == true)
				{
					app->LoadGameRequest();
				}
				else
				{
					if (app->scene->active == true)
					{
						position.x = 720;
						position.y = 1584;
					}
					else if (app->sceneLvl2->active == true)
					{
						position.x = 620;
						position.y = 2256;
					}
				}
				deathAnim.Reset();
				playerColl->SetPos(position.x + 25, position.y + 20);
				cameraColl->rect.x = position.x - 100;
				cameraColl->rect.y = position.y - 100;
				appear = true;
				lifes--;
				death = false;
			}
		}
		app->audio->PlayFx(hitFx);
	}
	
	return true;
}


bool Player::CameraScroll(Collider* c1, Collider* c2)
{
	if (c1->rect.x < c2->rect.x)
	{
		c2->rect.x -= c2->rect.x - c1->rect.x;
	}
	if ((c1->rect.x + c1->rect.w) > (c2->rect.x + c2->rect.w))
	{
		c2->rect.x += (c1->rect.x + c1->rect.w) - (c2->rect.x + c2->rect.w);
	}
	if (c1->rect.y < c2->rect.y)
	{
		c2->rect.y -= c2->rect.y - c1->rect.y;
	}
	if ((c1->rect.y + c1->rect.h) > (c2->rect.y + c2->rect.h))
	{
		c2->rect.y += (c1->rect.y + c1->rect.h) - (c2->rect.y + c2->rect.h);
	}

	return true;
}

bool Player::CollectApple(Collider* c1, Collider* c2)
{
	app->audio->PlayFx(pointFx);
	appleCounter += 1;
	c1->isCollected = true;
	app->moduleParticles->AddParticle(app->moduleParticles->fruitGet, c1->rect.x, c1->rect.y);
	app->collision->RemoveCollider(c1);

	return true;
}

bool Player::CollectPineapple(Collider* c1, Collider* c2)
{
	app->audio->PlayFx(pointFx);
	c1->isCollected = true;
	appleCounter += 2;
	app->moduleParticles->AddParticle(app->moduleParticles->fruitGet, c1->rect.x, c1->rect.y);
	if (app->player->lifes < 3 && app->player->lifes > 0)
	{
		app->player->lifes++;
	}
	app->collision->RemoveCollider(c1);

	return true;
}

bool Player::CheckPoint(Collider* c1, Collider* c2)
{
	app->scene->savePoint = true;
	app->sceneLvl2->savePoint = true;
	playerLoadF6 = false;
	app->audio->PlayFx(app->scene->checkpointFx);

    auxc.x = position.x;
	auxc.y = position.y;

	app->SaveGameRequest();
	

	return true;
}

bool Player::LoadState(pugi::xml_node& data)
{
	if (playerLoadF6 == true)
	{
		position.x = data.child("position").attribute("x").as_int();
		position.y = data.child("position").attribute("y").as_int();

		currentLvl = data.child("level").attribute("lvl").as_int();
		playerLoadF6 = false;
	}
	else if (playerLoadF6 == false)
	{

		position.x = data.child("checkpoint").attribute("x").as_int();
		position.y = data.child("checkpoint").attribute("y").as_int();

	}


	cont = data.child("continue").attribute("cont").as_bool();

	appleCounter = data.child("score").attribute("sco").as_int();

	lifes = data.child("lifes").attribute("hp").as_int();

	if (cont == true)
	{
		app->scene->apples[0] = data.child("apples").attribute("app1").as_int();
		app->scene->apples[1] = data.child("apples").attribute("app2").as_int();
		app->scene->apples[2] = data.child("apples").attribute("app3").as_int();
		app->scene->apples[3] = data.child("apples").attribute("app4").as_int();
		app->scene->apples[4] = data.child("apples").attribute("app5").as_int();
		app->scene->apples[5] = data.child("apples").attribute("pine1").as_int();

		app->sceneLvl2->apples2[0] = data.child("apples").attribute("app6").as_int();
		app->sceneLvl2->apples2[1] = data.child("apples").attribute("app7").as_int();
		app->sceneLvl2->apples2[2] = data.child("apples").attribute("app8").as_int();
		app->sceneLvl2->apples2[3] = data.child("apples").attribute("app9").as_int();
		app->sceneLvl2->apples2[4] = data.child("apples").attribute("app10").as_int();
		app->sceneLvl2->apples2[5] = data.child("apples").attribute("app11").as_int();
		app->sceneLvl2->apples2[6] = data.child("apples").attribute("pine2").as_int();
	}

	return true;
}

bool Player::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node player;
	if (playerSave == true)
	{
		player = data.append_child("checkpoint");

		player.append_attribute("x") = auxc.x;
		player.append_attribute("y") = auxc.y;

		player = data.append_child("position");

		player.append_attribute("x") = position.x;
		player.append_attribute("y") = position.y;
	}
	else if (playerSave == false)
	{
		player = data.append_child("checkpoint");

		player.append_attribute("x") = position.x;
		player.append_attribute("y") = position.y;

		player = data.append_child("position");

		player.append_attribute("x") = auxp.x;
	    player.append_attribute("y") = auxp.y;
	}

	player = data.append_child("level");

	player.append_attribute("lvl") = lvl;

	player = data.append_child("continue");

	player.append_attribute("cont") = cont;

	player = data.append_child("score");

	player.append_attribute("sco") = appleCounter;

	player = data.append_child("lifes");

	player.append_attribute("hp") = lifes;

	player = data.append_child("apples");

	player.append_attribute("app1") = app->scene->apples[0];
	player.append_attribute("app2") = app->scene->apples[1];
	player.append_attribute("app3") = app->scene->apples[2];
	player.append_attribute("app4") = app->scene->apples[3];
	player.append_attribute("app5") = app->scene->apples[4];
	player.append_attribute("pine1") = app->scene->apples[5];

	player.append_attribute("app6") = app->sceneLvl2->apples2[0];
	player.append_attribute("app7") = app->sceneLvl2->apples2[1];
	player.append_attribute("app8") = app->sceneLvl2->apples2[2];
	player.append_attribute("app9") = app->sceneLvl2->apples2[3];
	player.append_attribute("app10") = app->sceneLvl2->apples2[4];
	player.append_attribute("app11") = app->sceneLvl2->apples2[5];
	player.append_attribute("pine2") = app->sceneLvl2->apples2[6];

	return true;
}
