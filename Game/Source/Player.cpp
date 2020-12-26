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
	
	if (app->scene->active == true)
	{
		position.x = 720;
		position.y = 1584;
		app->SaveGameRequest();
	}
	else if (app->sceneLvl2->active == true)
	{
		position.x = 620;
		position.y = 2256;
		app->SaveGameRequest();
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
			app->SaveGameRequest();
		}
		if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		{
			if (lvl == 2)
			{
				if (app->scene->active == true)
				{
					saveGame = true;
					app->fadeToBlack->Fade(app->scene, (Module*)app->sceneLvl2, 80);
				}
				app->LoadGameRequest();
			}
			else if (lvl == 1)
			{
				if (app->sceneLvl2->active == true)
				{
					saveGame = true;
					app->fadeToBlack->Fade(app->sceneLvl2, (Module*)app->scene, 80);
				}
				app->LoadGameRequest();
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
				speedY = 20*dt;
				secondJump = true;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && yDownCollision == true && god == false)
		{
			if (!isJumping) // Solo salta cuando no esté en el aire
			{
				app->audio->PlayFx(jumpFx);
				isJumping = true;
			}
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
			app->moduleParticles->AddParticle(app->moduleParticles->chickenFall, position.x + 200, position.y -350, Collider::Type::CHICKEN);
			
		}
		if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN && flip == true && skillCoolDown == 0)
		{
			skillCoolDown = 200;
			app->moduleParticles->AddParticle(app->moduleParticles->chickenFall, position.x - 200, position.y - 350, Collider::Type::CHICKEN);

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
	if(skillCoolDown > 0)
	{
		skillCoolDown--;
	}

	return true;
}

bool Player::PostUpdate()
{
	if (flip == false) 
	{
		Animation* dron = &dronAnim;
		dron->Update();
		SDL_Rect dronRect = dron->GetCurrentFrame();
		app->render->DrawTextureFlip(dronTex, position.x - 50, position.y - 10, &dronRect);
	}
	else if (flip == true)
	{
		Animation* dron = &dronAnim;
		dron->Update();
		SDL_Rect dronRect = dron->GetCurrentFrame();
		app->render->DrawTexture(dronTex, position.x - 50, position.y - 10, &dronRect);
	}
	if (flip == false && skillCoolDown == 0)
	{
		Animation* chicken = &chickenAnim;
		chicken->Update();
		SDL_Rect chickenRect = chicken->GetCurrentFrame();
		app->render->DrawTextureFlip(chickenFlyTex, position.x + 80, position.y - 10, &chickenRect);
	}
	else if (flip == true && skillCoolDown == 0)
	{
		Animation* chicken = &chickenAnim;
		chicken->Update();
		SDL_Rect chickenRect = chicken->GetCurrentFrame();
		app->render->DrawTexture(chickenFlyTex, position.x + 80, position.y - 10, &chickenRect);
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


	return true;
}

bool Player::CleanUp()
{
	if (!active)
	{
		return true;
	}

	LOG("Unloading player");

	//Aqui deletamos cosas
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

	app->collision->RemoveCollider(playerColl);
	app->collision->RemoveCollider(cameraColl);

	app->audio->UnloadFX(jumpFx);
	app->audio->UnloadFX(hitFx);
	app->audio->UnloadFX(pointFx);

	active = false;

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
	app->audio->PlayFx(app->scene->checkpointFx);
	app->SaveGameRequest();


	return true;
}

bool Player::LoadState(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();

	lvl = data.child("level").attribute("lvl").as_int();

	return true;
}

bool Player::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node player = data.append_child("position");

	player.append_attribute("x") = position.x;
	player.append_attribute("y") = position.y;

	player = data.append_child("level");

	player.append_attribute("lvl") = lvl;

	return true;
}