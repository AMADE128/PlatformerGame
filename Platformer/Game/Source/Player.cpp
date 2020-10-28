#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Log.h"


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
	Idle = app->tex->Load("Output/Assets/textures/Character/Idle (32x32).png");
	Fall = app->tex->Load("Output/Assets/textures/Character/Fall.png");
	Hit = app->tex->Load("Output/Assets/textures/Character/Hit (32x32).png");
	Run = app->tex->Load("Output/Assets/textures/Character/Run (32x32).png");
	Jump = app->tex->Load("Output/Assets/textures/Character/Jump (32x32).png");
	//Posicion x e y

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
	app->tex->UnLoad(Idle);
	app->tex->UnLoad(Fall);
	app->tex->UnLoad(Hit);
	app->tex->UnLoad(Run);
	app->tex->UnLoad(Jump);

	return true;
}