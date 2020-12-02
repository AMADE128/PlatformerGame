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

#include "Collisions.h"

#define TILESIZE 96
#define POSXINIT 720
#define POXYINIT 1584

Player::Player() : Module()
{
	name.Create("player");
	

	for (int i = 0; i < TILESIZE * 11; i += TILESIZE)
	{
		idleAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	idleAnim.loop = true;
	idleAnim.speed = 0.25f;

	for (int i = 0; i < TILESIZE * 12; i += TILESIZE)
	{
		runAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	runAnim.loop = true;
	runAnim.speed = 0.25f;

	fallAnim.PushBack({ 0, 0, TILESIZE, TILESIZE });
	fallAnim.loop = true;
	fallAnim.speed = 0.25f;

	jumpAnim.PushBack({ 0, 0, TILESIZE, TILESIZE });

	for (int i = 0; i < TILESIZE * 7; i += TILESIZE)
	{
		deathAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	deathAnim.loop = false;
	deathAnim.speed = 0.25f;

	for (int i = 0; i < TILESIZE * 6; i += TILESIZE)
	{
		doubleJumpAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	doubleJumpAnim.loop = false;
	doubleJumpAnim.speed = 0.25f;

	for (int i = 0; i < TILESIZE * 7; i += TILESIZE)
	{
		appeAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	appeAnim.loop = false;
	appeAnim.speed = 0.25f;

	for (int i = 0; i < TILESIZE * 7; i += TILESIZE)
	{
		desAppeAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	desAppeAnim.loop = false;
	desAppeAnim.speed = 0.25f;

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
	
	position.x = POSXINIT;
	position.y = POXYINIT;
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
	appleTexure = app->tex->Load("Assets/Textures/Items/Fruits/apple.png");



	playerColl = app->collision->AddCollider({ (int)position.x, (int)position.y, TILESIZE - 50, TILESIZE - 20}, Collider::Type::PLAYER, this);
	cameraColl = app->collision->AddCollider({ (int)position.x - 100, (int)position.y - 100, app->render->camera.w/4, app->render->camera.h / 3 + 20}, Collider::Type::CAMERA, this);
	
	app->scene->checkPointColl = app->collision->AddCollider({ 3860, 1360, 20, 128 }, Collider::Type::CHECKPOINT, app->player);
	app->scene->appleColl1 = app->collision->AddCollider({ 4124, 924, 48, 48 }, Collider::Type::APPLE, app->player);
	app->scene->appleColl2 = app->collision->AddCollider({ 2568, 1368, 48, 48 }, Collider::Type::APPLE, app->player);
	app->scene->appleColl3 = app->collision->AddCollider({ 1392, 552, 48, 48 }, Collider::Type::APPLE, app->player);

	currentAnimation = &idleAnim;
	currentTex = idleTex;
	flip = false;
	speedY = 0.3f;
	return true;
}

bool Player::PreUpdate()
{
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
	dt *= 6;
	if (death == false && app->screen == game_scene1 && appear == false)
	{
		if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		{
			position.x = POSXINIT;
			position.y = POXYINIT;
			playerColl->SetPos(position.x + 25, position.y + 20);
			cameraColl->rect.x = position.x - 100;
			cameraColl->rect.y = position.y - 100;
			appear = true;
		}
		if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			position.x = POSXINIT;
			position.y = POXYINIT;

		}
		if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && !isJumping)
		{
			app->SaveGameRequest();
		}
		if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		{
			app->LoadGameRequest();
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
				speedY = 20*dt;
				secondJump = true;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && yDownCollision == true && god == false)
		{
			if (!isJumping) // Solo salta cuando no esté en el aire
			{
				isJumping = true;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && god == true)
		{
			speedY = -80*dt;
			position.y += speedY;
		}
		else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && god == true)
		{
			speedY = 80*dt;
			position.y += speedY;
		}
		else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
		{
			if (god == true)
			{
				speedXLastFrame = speedX;
				speedX = -80*dt;
				position.x += speedX;
			}
			else if (god == false && xLeftCollision == false)
			{
				xRightCollision = false;
				speedXLastFrame = speedX;
				speedX = -60*dt;
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
					if ((-30*dt + speedY) < 0)
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
					else if ((-30*dt + speedY) >= 0)
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
				speedX = 80*dt;
				position.x += speedX;
			}
			else if (god == false && xRightCollision == false)
			{
				xLeftCollision = false;
				speedXLastFrame = speedX;
				speedX = 60*dt;
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
					if ((-30*dt + speedY) < 0)
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
					else if ((-30*dt + speedY) >= 0)
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
				else if ((-30*dt + speedY) < 0)
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
				else if ((-30*dt + speedY) >= 0)
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
			position.y += -100*dt;
		}
		if (secondJump)
		{
			xLeftCollision = false;
			xRightCollision = false;
			position.y += -40*dt;
		}

		if (yDownCollision == true)
		{
			speedY = 0;
		}
		else
		{
			speedY += 5.0f*dt;
		}
		position.y += speedY;

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
		
		app->render->DrawTexture(appleTexure, (app->render->camera.x - 1200) * -1, (app->render->camera.y - 0) * -1, NULL);

		playerColl->SetPos(position.x + 25, position.y + 20);
		app->render->camera.x = ((cameraColl->rect.x + cameraColl->rect.w / 3) - (app->render->camera.w / 2)) * -1;
		app->render->camera.y = ((cameraColl->rect.y + cameraColl->rect.h / 3) - (app->render->camera.h / 2.5)) * -1;
	}

	return true;
}

bool Player::PostUpdate()
{
	if (position.x < 720 && position.y < 816)
	{
		win = true;
		playerColl->SetPos(position.x + 25, position.y + 20);
		cameraColl->rect.x = position.x - 100;
		cameraColl->rect.y = position.y - 100;
		speedX = 0;
		speedY = 0;
		app->fadeToBlack->Fade(game_win, 80);
	}
	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	if (flip == true)
	{
		app->render->DrawTextureFlip(currentTex, position.x, position.y, &rect);
	}
	else if (flip == false)
	{
		app->render->DrawTexture(currentTex, position.x, position.y, &rect); 
	}

	
	return true;
}

bool Player::CleanUp()
{
	LOG("Unloading particles");

	//Aqui deletamos cosas
	app->tex->UnLoad(idleTex);
	app->tex->UnLoad(fallTex);
	app->tex->UnLoad(deathTex);
	app->tex->UnLoad(runTex);
	app->tex->UnLoad(jumpTex);

	app->collision->RemoveCollider(playerColl);

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
	if (c1->rect.x < c2->rect.x && (c1->rect.y + playerColl->rect.h - 3) > c2->rect.y)
	{
		xRightCollision = true;
		speedX = 0.f;
	}
	if (c1->rect.x > c2->rect.x && (c1->rect.y + playerColl->rect.h - 3) > c2->rect.y)
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
				app->fadeToBlack->Fade(game_death, 80);
			}
			else
			{
				if (app->scene->savePoint == true)
				{
					app->LoadGameRequest();
				}
				else
				{
					position.x = POSXINIT;
					position.y = POXYINIT;
				}
				deathAnim.Reset();
				playerColl->SetPos(position.x + 25, position.y + 20);
				cameraColl->rect.x = position.x - 100;
				cameraColl->rect.y = position.y - 100;
				speedX = 0;
				speedY = 0;
				appear = true;
				lifes--;
				death = false;
			}
		}
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
	c2->isCollected = true;

	return true;
}

bool Player::CheckPoint(Collider* c1, Collider* c2)
{
	app->scene->savePoint = true;
	app->audio->PlayFx(app->scene->checkpointFx);
	app->SaveGameRequest();


	return true;
}

bool Player::LoadState(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();

	return true;
}

bool Player::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node player = data.append_child("position");

	player.append_attribute("x") = position.x;
	player.append_attribute("y") = position.y;

	return true;
}