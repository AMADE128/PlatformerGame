#include "ModuleParticles.h"

#include "App.h"

#include "Textures.h"
#include "Render.h"
#include "Collisions.h"
#include "Input.h"
#include "Player.h"
#include "Log.h"

#include "SDL/include/SDL_scancode.h"
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles() : Module()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		particles[i] = nullptr;

	for (int i = 0; i < 96 * 6; i += 96)
	{
		fruitGet.anim.PushBack({ i, 0, 96, 96 });
	}
	fruitGet.anim.loop = false;
	fruitGet.anim.speed = 0.4f;
	fruitGet.lifetime = 25;
	fruitGet.speed = { 0, 0 };

	rightLeaf.anim.PushBack({ 0, 0, 32, 32 });
	rightLeaf.anim.speed = 0;
	rightLeaf.speed.x = 15;
	rightLeaf.lifetime = 40;

	leftLeaf.anim.PushBack({ 0, 0, 32, 32 });
	leftLeaf.anim.speed = 0;
	leftLeaf.speed.x = -15;
	leftLeaf.lifetime = 40;

	for (int i = 0; i < 120 * 4; i += 120)
	{
		chickenFall.anim.PushBack({ i, 0, 120, 144 });
	}
	chickenFall.anim.loop = false;
	chickenFall.anim.speed = 0.2f;
	chickenFall.speed.y = 9;
	chickenFall.lifetime = 40;

	for (int i = 0; i < 120 * 5; i += 120)
	{
		chickenHit.anim.PushBack({ i, 0, 120, 144 });
	}
	chickenHit.anim.loop = false;
	chickenHit.anim.speed = 0.2f;
	chickenFall.speed.x = 0.01f;
	chickenHit.lifetime = 40;


}

ModuleParticles::~ModuleParticles()
{

}

bool ModuleParticles::Start()
{
	LOG("Loading particles");

	fruitGetTex = app->tex->Load("Assets/Textures/Items/Fruits/collect.png");
	leafTex = app->tex->Load("Assets/Textures/Drone/leaf.png");
	chickenFallTex = app->tex->Load("Assets/Textures/Drone/fall_skill.png");
	chickenHitTex = app->tex->Load("Assets/Textures/Drone/hit_skill.png");

	return true;
}

bool ModuleParticles::CleanUp()
{
	if (!active)
	{
		return true;
	}
	LOG("Unloading particles");

	// Delete all remaining active particles on application exit 
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}
	app->tex->UnLoad(fruitGetTex);
	app->tex->UnLoad(leafTex);
	app->tex->UnLoad(chickenFallTex);
	app->tex->UnLoad(chickenHitTex);
	active = false;

	return true;
}

bool ModuleParticles::Die(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (particles[i] != nullptr && particles[i]->collider == c1)
		{
			app->collision->RemoveCollider(particles[i]->collider);
			delete particles[i];
			particles[i] = nullptr;
			break;
		}
	}

	return true;
}

bool ModuleParticles::Update(float dt)
{
	bool ret = true;
	chickenFall.speed.y = 500 * dt;

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle == nullptr)	continue;

		// Call particle Update. If it has reached its lifetime, destroy it
		if (particle->Update() == false)
		{
			app->collision->RemoveCollider(particles[i]->collider);
			delete particle;
			particles[i] = nullptr;
		}
	}

	return ret;
}


bool ModuleParticles::PostUpdate()
{
	bool ret = true;
	//Iterating all particle array and drawing any active particles
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle != nullptr && particle->speed.x == fruitGet.speed.x && particle->speed.y != chickenFall.speed.y)
		{
			app->render->DrawTexture(fruitGetTex, particle->position.x - 24, particle->position.y - 24, &(particle->anim.GetCurrentFrame()));
		}
		if (particle != nullptr && (particle->speed.x == rightLeaf.speed.x || particle->speed.x == leftLeaf.speed.x) && particle->speed.x != fruitGet.speed.x)
		{
			app->render->DrawTexture(leafTex, particle->position.x, particle->position.y + 10, &(particle->anim.GetCurrentFrame()));
		}
		if (particle != nullptr && particle->speed.y == chickenFall.speed.y)
		{
			app->render->DrawTexture(chickenFallTex, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));
		}
		if (particle != nullptr && particle->speed.x == chickenHit.speed.x && particle->speed.y != chickenFall.speed.y && particle->speed.x != fruitGet.speed.x)
		{
			app->render->DrawTexture(chickenHitTex, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));
		}

	}

	return ret;
}

void ModuleParticles::AddParticle(const Particle& particle, int x, int y, Collider::Type colliderType, uint delay)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		//Finding an empty slot for a new particle
		if (particles[i] == nullptr)
		{
			Particle* p = new Particle(particle);

			p->frameCount = -(int)delay;			// We start the frameCount as the negative delay
			p->position.x = x;						// so when frameCount reaches 0 the particle will be activated
			p->position.y = y;

			p->collider = app->collision->AddCollider(p->anim.GetCurrentFrame(), colliderType, this);

			particles[i] = p;
			break;
		}
	}
}