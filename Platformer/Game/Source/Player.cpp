#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Log.h"
#include "Render.h"
#include "Input.h"

#define TILESIZE 32

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
	
	position.x = 100;
	position.y = 500;
	//Cargar texturas
	idle = app->tex->Load("Assets/textures/Character/Idle (32x32).png");
	fall = app->tex->Load("Assets/textures/Character/Fall.png");
	death = app->tex->Load("Assets/textures/Character/Hit (32x32).png");
	run = app->tex->Load("Assets/textures/Character/Run (32x32).png");
	jump = app->tex->Load("Assets/textures/Character/Jump (32x32).png");

	currentAnimation = &idleAnim;
	currentTex = idle;

	return true;
}

bool Player::PreUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		if (god == true)
		{
			speed_y = 0.2f;
			position.y -= speed_y;
		}
		else if (god == false)
		{
			speed_y = 0.2f;
			position.y -= speed_y;
		}
		
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		if (god == true)
		{
			speed_y = 0.2f;
			position.y += speed_y;
		}
		else if (god == false)
		{
			speed_y = 0.2f;
			position.y += speed_y;
		}
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (god == true)
		{
			speed_x = 0.2f;
			position.x -= speed_x;
		}
		else if (god == false)
		{
			speed_x = 0.2f;
			position.x -= speed_x;
			currentTex = run;
			currentAnimation = &runAnim;
			flip = SDL_FLIP_HORIZONTAL;
		}
		
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (god == true)
		{
			speed_x = 0.2f;
			position.x += speed_x;
		}
		else if (god == false)
		{
			speed_x = 0.2f;
			position.x += speed_x;
			currentTex = run;
			currentAnimation = &runAnim;
		}
	}
	else
	{
		currentAnimation = &idleAnim;
		currentTex = idle;
	}

	if (gravity == true)
	{
		position.y += speed_y;
	}

	return true;
}

bool Player::Update(float dt)
{
	currentAnimation->Update();

	return true;
}

bool Player::PostUpdate()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(currentTex, position.x, position.y, &rect);

	SDL_RenderCopyEx(app->render->renderer, currentTex, &rect, &rect, NULL, NULL, flip);

	return true;
}

bool Player::CleanUp()
{
	LOG("Unloading particles");

	//Aqui deletamos cosas
	app->tex->UnLoad(idle);
	app->tex->UnLoad(fall);
	app->tex->UnLoad(death);
	app->tex->UnLoad(run);
	app->tex->UnLoad(jump);

	return true;
}