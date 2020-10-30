#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Log.h"
#include "Render.h"
#include "Input.h"

#include "Collisions.h"

#define TILESIZE 32

Player::Player() : Module()
{
	name.Create("player");
	

	for (int i = 0; i < TILESIZE * 11; i += TILESIZE)
	{
		idleRightAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	idleRightAnim.loop = true;
	idleRightAnim.speed = 0.1f;

	for (int i = 0; i < TILESIZE * 11; i += TILESIZE)
	{
		idleLeftAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	idleLeftAnim.loop = true;
	idleLeftAnim.speed = 0.1f;

	for (int i = 0; i < TILESIZE * 12; i += TILESIZE)
	{
		runRightAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	runRightAnim.loop = true;
	runRightAnim.speed = 0.1f;

	for (int i = 0; i < TILESIZE * 12; i += TILESIZE)
	{
		runLeftAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	runLeftAnim.loop = true;
	runLeftAnim.speed = 0.1f;

	fallRightAnim.PushBack({ 0, 0, TILESIZE, TILESIZE });

	fallLeftAnim.PushBack({ 0, 0, TILESIZE, TILESIZE });

	jumpRightAnim.PushBack({ 0, 0, TILESIZE, TILESIZE });

	jumpLeftAnim.PushBack({ 0, 0, TILESIZE, TILESIZE });

	for (int i = 0; i < TILESIZE * 7; i += TILESIZE)
	{
		deathRightAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	deathRightAnim.speed = 0.1f;

	for (int i = 0; i < TILESIZE * 7; i += TILESIZE)
	{
		deathLeftAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	deathLeftAnim.speed = 0.1f;
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
	
	position.x = 100;
	position.y = 500;
	//Cargar texturas
	idleRight = app->tex->Load("Assets/textures/Character/Idle (32x32).png");
	idleLeft = app->tex->Load("Assets/textures/Character/Idle (32x32)-left.png");
	fallRight = app->tex->Load("Assets/textures/Character/Fall.png");
	fallLeft = app->tex->Load("Assets/textures/Character/Fall-left.png");
	deathRight = app->tex->Load("Assets/textures/Character/Hit (32x32).png");
	deathLeft = app->tex->Load("Assets/textures/Character/Hit (32x32)-left.png");
	runRight = app->tex->Load("Assets/textures/Character/Run (32x32).png");
	runLeft = app->tex->Load("Assets/textures/Character/Run (32x32)-left.png");
	jumpRight = app->tex->Load("Assets/textures/Character/Jump (32x32).png");
	jumpLeft = app->tex->Load("Assets/textures/Character/Jump (32x32)-left.png");

	collider = app->collision->AddCollider({ (int)position.x, (int)position.y, TILESIZE, TILESIZE}, Collider::Type::PLAYER, this);

	currentAnimation = &idleRightAnim;
	currentTex = idleRight;
	speed_y = 0.3f;
	return true;
}

bool Player::PreUpdate()
{
	return true;
}

bool Player::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && JumpCounter == 180 && y_downcollision == true)
	{
		if (god == true)
		{
			speed_yLastFrame = speed_y;
			speed_y = -0.3f;
			position.y += speed_y;
		}
		else if (god == false)
		{
			y_downcollision = false;
			speed_yLastFrame = speed_y;
			speed_y = -0.7f;
			JumpCounter = 0;
			position.y += speed_y;
		}

	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		if (god == true)
		{
			speed_yLastFrame = speed_y;
			speed_y = 0.3f;
			position.y += speed_y;
		}
		else if (god == false)
		{
			/*speed_yLastFrame = speed_y;
			speed_y = 0.3f;
			position.y += speed_y;
			currentTex = fallRight;
			currentAnimation = &fallRightAnim;
			if (app->render->camera.y < 0)
			{
				app->render->camera.y -= 2;
			}*/
		}
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
	{
		if (god == true)
		{
			speed_xLastFrame = speed_x;
			speed_x = 1;
			position.x -= speed_x;
		}
		else if (god == false && x_leftcollision == false)
		{
			x_rightcollision = false;
			speed_xLastFrame = speed_x;
			speed_x = 1;
			position.x -= speed_x;
			if (app->render->camera.x < 0)
			{
				app->render->camera.x += 1;
			}
			if (y_downcollision == true || x_leftcollision == true)
			{
				currentTex = runLeft;
				currentAnimation = &runLeftAnim;
			}
			else
			{
				currentTex = jumpLeft;
				currentAnimation = &jumpLeftAnim;
			}
		}

	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
	{
		if (god == true)
		{
			speed_xLastFrame = speed_x;
			speed_x = 1;
			position.x += speed_x;
		}
		else if (god == false && x_rightcollision == false)
		{
			x_leftcollision = false;
			speed_xLastFrame = speed_x;
			speed_x = 1;
			position.x += speed_x;
			if (app->render->camera.x > -2271)
			{
				app->render->camera.x -= 1;
			}
			if (y_downcollision == true || x_rightcollision == true)
			{
				currentTex = runRight;
				currentAnimation = &runRightAnim;
			}
			else
			{
				currentTex = jumpRight;
				currentAnimation = &jumpRightAnim;
			}
		}
		
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP || app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		speed_x = 0;
	}
	else
	{
		if (speed_y == 0 && speed_x == 0)
		{
			if (speed_xLastFrame > 0)
			{
				currentAnimation = &idleRightAnim;
				currentTex = idleRight;
			}
			else if (speed_xLastFrame < 0)
			{
				currentAnimation = &idleLeftAnim;
				currentTex = idleLeft;
			}
		}
		if (speed_y < 0)
		{
			if (speed_x > 0 || speed_x == 0 && speed_xLastFrame > 0)
			{
				currentTex = jumpRight;
				currentAnimation = &jumpRightAnim;
			}
			else if (speed_x < 0 || speed_x == 0 && speed_xLastFrame > 0)
			{
				currentAnimation = &jumpLeftAnim;
				currentTex = jumpLeft;
			}
		}
		else if (speed_y > 0)
		{

		}
	}

	if (y_downcollision == false)
	{
		position.y += speed_y;
	}

	if (JumpCounter < 179)
	{
		JumpCounter++;
	}
	if (JumpCounter == 90) 
	{
		speed_y = -0.5f;
	}
	else if (JumpCounter == 110) 
	{
		speed_y = -0.4f;
	}
	else if (JumpCounter == 135)
	{
		speed_y = -0.3f;
	}
	else if (JumpCounter == 130)
	{
		speed_y = -0.2f;
	}
	else if (JumpCounter == 140) 
	{
		speed_y = -0.1f;
	}
	else if (JumpCounter == 165)
	{
		speed_y = 0.1f;
	}
	else if (JumpCounter == 174)
	{
		speed_y = 0.2f;
	}
	else if (JumpCounter == 179) {
		speed_y = 0.4f;
		JumpCounter = 180;
	}

	currentAnimation->Update();
	collider->SetPos(position.x, position.y);

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(currentTex, position.x, position.y, &rect);

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
		y_downcollision = true;
		speed_y = 0.f;
	}

	return true;
}

bool Player::StopMovement(Collider* c1, Collider* c2)
{
	if (c1->rect.x < c2->rect.x && c1->rect.y > c2->rect.y)
	{
		x_rightcollision = true;
		speed_x = 0.f;
	}
	else if (c1->rect.x > c2->rect.x && c1->rect.y > c2->rect.y)
	{
		x_leftcollision = true;
		speed_x = 0.f;
	}
	return true;
}