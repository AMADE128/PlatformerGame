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
	idleLeft = app->tex->Load("Assets/textures/Character/Idle-left (32x32).png");
	fallRight = app->tex->Load("Assets/textures/Character/Fall.png");
	fallLeft = app->tex->Load("Assets/textures/Character/Fall-Left.png");
	deathRight = app->tex->Load("Assets/textures/Character/Hit (32x32).png");
	deathLeft = app->tex->Load("Assets/textures/Character/Hit (32x32)-Left.png");
	runRight = app->tex->Load("Assets/textures/Character/Run (32x32).png");
	runLeft = app->tex->Load("Assets/textures/Character/Run (32x32)-left.png");
	jumpRight = app->tex->Load("Assets/textures/Character/Jump (32x32).png");
	jumpLeft = app->tex->Load("Assets/textures/Character/Jump (32x32)-left.png");

	currentAnimation = &idleRightAnim;
	currentTex = idleRight;

	return true;
}

bool Player::PreUpdate()
{
	return true;
}

bool Player::Update(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		if (god == true)
		{
			speed_yLastFrame = speed_y;
			speed_y = -0.3f;
			position.y += speed_y;
		}
		else if (god == false)
		{
			speed_yLastFrame = speed_y;
			speed_y = -0.3f;
			position.y += speed_y;
			currentTex = jumpRight;
			currentAnimation = &jumpRightAnim;
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
			speed_yLastFrame = speed_y;
			speed_y = 0.3f;
			position.y += speed_y;
			currentTex = fallRight;
			currentAnimation = &fallRightAnim;
		}
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
	{
		if (god == true)
		{
			speed_xLastFrame = speed_x;
			speed_x = -0.3f;
			position.x += speed_x;
		}
		else if (god == false)
		{
			speed_xLastFrame = speed_x;
			speed_x = -0.3f;
			position.x += speed_x;
			currentTex = runLeft;
			currentAnimation = &runLeftAnim;
		}

	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
	{
		if (god == true)
		{
			speed_xLastFrame = speed_x;
			speed_x = 0.3f;
			position.x += speed_x;
		}
		else if (god == false)
		{
			speed_xLastFrame = speed_x;
			speed_x = 0.3f;
			position.x += speed_x;
			currentTex = runRight;
			currentAnimation = &runRightAnim;
		}
	}
	else
	{
		currentAnimation = &idleRightAnim;
		currentTex = idleRight;
	}

	if (gravity == true)
	{
		position.y += speed_y;
	}

	currentAnimation->Update();


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

	return true;
}