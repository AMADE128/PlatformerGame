#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Log.h"
#include "Render.h"
#include "Input.h"
#include "Scene.h"
#include "Window.h"
#include "Map.h"

#include "Collisions.h"

#define TILESIZE 96
#define POSXINIT 100
#define POXYINIT 1000

Player::Player() : Module()
{
	name.Create("player");
	

	for (int i = 0; i < TILESIZE * 11; i += TILESIZE)
	{
		idleRightAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	idleRightAnim.loop = true;
	idleRightAnim.speed = 0.1f;

	for (int i = 0; i < TILESIZE * 12; i += TILESIZE)
	{
		runRightAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	runRightAnim.loop = true;
	runRightAnim.speed = 0.1f;

	fallRightAnim.PushBack({ 0, 0, TILESIZE, TILESIZE });

	jumpRightAnim.PushBack({ 0, 0, TILESIZE, TILESIZE });

	for (int i = 0; i < TILESIZE * 7; i += TILESIZE)
	{
		deathRightAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	deathRightAnim.speed = 0.1f;
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
	idleRight = app->tex->Load("Assets/textures/Character/Idle (32x32).png");
	fallRight = app->tex->Load("Assets/textures/Character/Fall.png");
	deathRight = app->tex->Load("Assets/textures/Character/Hit (32x32).png");
	runRight = app->tex->Load("Assets/textures/Character/Run (32x32).png");
	jumpRight = app->tex->Load("Assets/textures/Character/Jump (32x32).png");

	collider = app->collision->AddCollider({ (int)position.x, (int)position.y, TILESIZE, TILESIZE}, Collider::Type::PLAYER, this);

	currentAnimation = &idleRightAnim;
	currentTex = idleRight;
	flip = false;
	speed_y = 0.3f;
	return true;
}

bool Player::PreUpdate()
{
	return true;
}

bool Player::Update(float dt)
{
	if (app->screen == game_scene1)
	{
		if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		{
			position.x = POSXINIT;
			position.y = POXYINIT;
		}
		if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			position.x = POSXINIT;
			position.y = POXYINIT;

		}
		if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		{
			app->SaveGameRequest();
		}
		if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		{
			app->LoadGameRequest();
		}
		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			if (god == false)
			{
				god = true;
				collider->type = Collider::NONE;

			}
			else if (god == true)
			{
				god = false;
				collider->type = Collider::PLAYER;
			}

		}
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && y_downCollision == true)
		{
			if (god == false)

				y_downCollision = false;
			if (!isJumping) // Solo salta cuando no esté en el aire
			{
				isJumping = true;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			if (god == true)
			{
				speed_y = -3;
				position.y += speed_y;
			}
		}
		else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			if (god == true)
			{
				speed_y = 3;
				position.y += speed_y;
			}
		}
		else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
		{
			if (god == true)
			{
				speed_xLastFrame = speed_x;
				speed_x = -3;
				position.x += speed_x;
			}
			else if (god == false && x_leftCollision == false)
			{
				x_rightCollision = false;
				speed_xLastFrame = speed_x;
				speed_x = -3;
				position.x += speed_x;

				if (y_downCollision == true)
				{
					currentTex = runRight;
					currentAnimation = &runRightAnim;
					flip = true;
				}
				else
				{
					flip = true;
					if ((-3 + speed_y) < 0)
					{
						if (speed_x <= 0 && speed_xLastFrame < 0)
						{
							currentAnimation = &jumpRightAnim;
							currentTex = jumpRight;
						}
					}
					else if ((-3 + speed_y) >= 0)
					{
						if (speed_x <= 0 && speed_xLastFrame < 0)
						{
							currentAnimation = &fallRightAnim;
							currentTex = fallRight;
						}
					}
				}
			}

		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
		{
			if (god == true)
			{
				speed_xLastFrame = speed_x;
				speed_x = 3;
				position.x += speed_x;
			}
			else if (god == false && x_rightCollision == false)
			{
				x_leftCollision = false;
				speed_xLastFrame = speed_x;
				speed_x = 3;
				position.x += speed_x;

				if (y_downCollision == true)
				{
					currentTex = runRight;
					currentAnimation = &runRightAnim;
					flip = false;
				}
				else
				{
					flip = false;
					if ((-3 + speed_y) < 0)
					{
						if (speed_x >= 0 && speed_xLastFrame > 0)
						{
							currentTex = jumpRight;
							currentAnimation = &jumpRightAnim;
						}
					}
					else if ((-3 + speed_y) >= 0)
					{
						if (speed_x >= 0 && speed_xLastFrame > 0)
						{
							currentTex = fallRight;
							currentAnimation = &fallRightAnim;
						}
					}
				}
			}

		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP || app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			speed_x = 0;
		}
		else
		{
			if (speed_x == 0)
			{
				if (speed_y == 0)
				{
					currentAnimation = &idleRightAnim;
					currentTex = idleRight;
					if (speed_xLastFrame > 0)
					{
						flip = false;
					}
					else if (speed_xLastFrame < 0)
					{
						flip = true;
					}
				}
				else if ((-3 + speed_y) < 0)
				{
					currentAnimation = &jumpRightAnim;
					currentTex = jumpRight;
					if (speed_xLastFrame > 0)
					{
						flip = false;
					}
					else if (speed_xLastFrame < 0)
					{
						flip = true;
					}
				}
				else if ((-3 + speed_y) >= 0)
				{
					currentTex = fallRight;
					currentAnimation = &fallRightAnim;
					if (speed_xLastFrame > 0)
					{
						flip = false;
					}
					else if (speed_xLastFrame < 0)
					{
						flip = true;
					}
				}
			}

		}

		if (isJumping)
		{
			y_downCollision = false;
			x_leftCollision = false;
			x_rightCollision = false;
			//speed_y = -0.5f;
			position.y += -7;
		}

		if (y_downCollision == true)
		{
			speed_y = 0;
		}
		else
		{
			speed_y += 0.15f;
		}
		position.y += speed_y;

		app->render->camera.x = ((position.x + TILESIZE / 2) - (app->render->camera.w / 2)) * -1;
		app->render->camera.y = ((position.y + TILESIZE / 2) - (app->render->camera.h / 2)) * -1;

		currentAnimation->Update();
		collider->SetPos(position.x, position.y);

		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		if (flip == true)
		{
			app->render->DrawTextureFlip(currentTex, position.x, position.y, &rect);
		}
		else if (flip == false)
		{
			app->render->DrawTexture(currentTex, position.x, position.y, &rect);
		}
	}

	return true;
}

bool Player::PostUpdate()
{
	return true;
}

bool Player::CleanUp()
{
	LOG("Unloading particles");

	//Aqui deletamos cosas
	app->tex->UnLoad(idleRight);
	app->tex->UnLoad(idleLeft);
	app->tex->UnLoad(fallRight);
	app->tex->UnLoad(fallLeft);
	app->tex->UnLoad(deathRight);
	app->tex->UnLoad(deathLeft);
	app->tex->UnLoad(runRight);
	app->tex->UnLoad(runLeft);
	app->tex->UnLoad(jumpRight);
	app->tex->UnLoad(jumpLeft);

	app->collision->RemoveCollider(collider);

	return true;
}


bool Player::StopMovementY(Collider* c1, Collider* c2)
{
	if (c1->rect.y < c2->rect.y)
	{
		y_upCollision = false;
		if (isJumping == true)
		{
			y_downCollision = false;
		}
		else if (isJumping == false)
		{
			y_downCollision = true;
		}
		isJumping = false;
		position.y = c2->rect.y - TILESIZE + 1;
	}
	else if (c1->rect.y  > c2->rect.y)
	{
		y_downCollision = false;
		y_upCollision = true;
		isJumping = false;
	}

	return true;
}

bool Player::StopMovement(Collider* c1, Collider* c2)
{
	if (c1->rect.x < c2->rect.x && (c1->rect.y + TILESIZE-3) > c2->rect.y)
	{
		x_rightCollision = true;
		speed_x = 0.f;
		position.x -= 0.1f;
	}
	if (c1->rect.x > c2->rect.x && (c1->rect.y + TILESIZE-3) > c2->rect.y)
	{
		x_leftCollision = true;
		speed_x = 0.f;
		position.x += 0.1f;
	}
	return true;
}

bool Player::Fall(Collider* c1, Collider* c2)
{
	y_downCollision = false;
	x_leftCollision = false;
	x_rightCollision = false;
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