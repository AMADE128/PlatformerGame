#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Log.h"
#include "Render.h"

#define TILESIZE 32

Player::Player() : Module()
{
	name.Create("map");
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
	
	//Cargar texturas
	idle = app->tex->Load("Assets/textures/Character/Idle (32x32).png");
	fall = app->tex->Load("Assets/textures/Character/Fall.png");
	hit = app->tex->Load("Assets/textures/Character/Hit (32x32).png");
	run = app->tex->Load("Assets/textures/Character/Run (32x32).png");
	jump = app->tex->Load("Assets/textures/Character/Jump (32x32).png");

	// left jump
	Animation idleAnim;
	for (int i = 0; i < TILESIZE*11; i+= TILESIZE)
	{
		idleAnim.PushBack({ i, 0, TILESIZE, TILESIZE });
	}
	idleAnim.loop = true;
	idleAnim.speed = 0.1f;

	currentAnimation = &idleAnim;

	//Posicion x e y
	positionX = 100;
	positionY = 100;


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
	app->render->DrawTexture(idle, positionX, positionY, &rect);

	return true;
}

bool Player::CleanUp()
{
	LOG("Unloading particles");

	//Aqui deletamos cosas
	app->tex->UnLoad(idle);
	app->tex->UnLoad(fall);
	app->tex->UnLoad(hit);
	app->tex->UnLoad(run);
	app->tex->UnLoad(jump);

	return true;
}