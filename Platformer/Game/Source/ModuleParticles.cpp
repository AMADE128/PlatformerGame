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
}

ModuleParticles::~ModuleParticles()
{

}

bool ModuleParticles::Start()
{
	LOG("Loading particles");

	fruitGetTex = app->tex->Load("Assets/Textures/Items/Fruits/collect.png");

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

	active = false;

	return true;
}

bool ModuleParticles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (particles[i] != nullptr && particles[i]->collider == c1)
		{
			delete particles[i];
			particles[i] = nullptr;
			path.Reset();
			break;
		}
	}

	return true;
}

bool ModuleParticles::Update(float dt)
{
	bool ret = true;

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle == nullptr)	continue;

		// Call particle Update. If it has reached its lifetime, destroy it
		if (particle->Update() == false)
		{
			delete particle;
			particles[i] = nullptr;
			app->collision->RemoveCollider(particle[i].collider);
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

		if (particle != nullptr/* && particle->position.x == fruitGet.position.x && particle->isAlive*/)
		{
			app->render->DrawTexture(fruitGetTex, particle->position.x - 24, particle->position.y - 24, &(particle->anim.GetCurrentFrame()));
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