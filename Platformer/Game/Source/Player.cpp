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
	idleAnim.speed = 0.1f;

	for (int i = 0; i < TILESIZE * 12; i += TILESIZE)
	{
		runAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	runAnim.loop = true;
	runAnim.speed = 0.1f;

	fallAnim.PushBack({ 0, 0, TILESIZE, TILESIZE });

	jumpAnim.PushBack({ 0, 0, TILESIZE, TILESIZE });

	for (int i = 0; i < TILESIZE * 7; i += TILESIZE)
	{
		deathAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	deathAnim.loop = false;
	deathAnim.speed = 0.1f;
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

	playerColl = app->collision->AddCollider({ (int)position.x, (int)position.y, TILESIZE - 50, TILESIZE - 20}, Collider::Type::PLAYER, this);

	currentAnimation = &idleAnim;
	currentTex = idleTex;
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
	if (app->screen == game_scene1 && death == false)
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
				playerColl->type = Collider::NONE;

			}
			else if (god == true)
			{
				god = false;
				playerColl->type = Collider::PLAYER;
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
					currentTex = runTex;
					currentAnimation = &runAnim;
					flip = true;
				}
				else
				{
					flip = true;
					if ((-3 + speed_y) < 0)
					{
						if (speed_x <= 0 && speed_xLastFrame < 0)
						{
							currentAnimation = &jumpAnim;
							currentTex = jumpTex;
						}
					}
					else if ((-3 + speed_y) >= 0)
					{
						if (speed_x <= 0 && speed_xLastFrame < 0)
						{
							currentAnimation = &fallAnim;
							currentTex = fallTex;
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
					currentTex = runTex;
					currentAnimation = &runAnim;
					flip = false;
				}
				else
				{
					flip = false;
					if ((-3 + speed_y) < 0)
					{
						if (speed_x >= 0 && speed_xLastFrame > 0)
						{
							currentTex = jumpTex;
							currentAnimation = &jumpAnim;
						}
					}
					else if ((-3 + speed_y) >= 0)
					{
						if (speed_x >= 0 && speed_xLastFrame > 0)
						{
							currentTex = fallTex;
							currentAnimation = &fallAnim;
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
					currentAnimation = &idleAnim;
					currentTex = idleTex;
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
					currentAnimation = &jumpAnim;
					currentTex = jumpTex;
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
					currentTex = fallTex;
					currentAnimation = &fallAnim;
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
		

		
		playerColl->SetPos(position.x + 25, position.y + 20);
	}

	return true;
}

bool Player::PostUpdate()
{
	if (position.x < 720 && position.y < 816)
	{
		win = true;
		app->screen = game_win;
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
	if (c1->rect.x < c2->rect.x && (c1->rect.y + playerColl->rect.h - 3) > c2->rect.y)
	{
		x_rightCollision = true;
		speed_x = 0.f;
		//position.x -= 0.1f;
	}
	if (c1->rect.x > c2->rect.x && (c1->rect.y + playerColl->rect.h - 3) > c2->rect.y)
	{
		x_leftCollision = true;
		speed_x = 0.f;
		//position.x += 0.1f;
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

bool Player::Die(Collider* c1, Collider* c2)
{
	if (c1->rect.y + TILESIZE > c2->rect.y + c2->rect.w/2)
	{
		death = true;
		currentAnimation = &deathAnim;
		currentTex = deathTex;
		if (speed_xLastFrame > 0)
		{
			flip = false;
		}
		else if (speed_xLastFrame < 0)
		{
			flip = true;
		}
		if (deathAnim.finish == true)
		{
			app->scene->start = true;
			deathAnim.Reset();
			app->screen = game_death;
		}
	}
	
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