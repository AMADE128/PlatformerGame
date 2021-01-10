#include "EntityManager.h"
#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entity_manager");

}

// Destructor
EntityManager::~EntityManager()
{}

// Called before EntityManager is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	bool ret = true;


	return ret;
}

// Called before the first frame
bool EntityManager::Start()
{
	LOG("EntityManager start");
	active = true;

	return true;
}

// Called each loop iteration
bool EntityManager::PreUpdate()
{
	for (ListItem<Entity*>* entiti = entities.start; entiti; entiti = entiti->next) entiti->data->PreUpdate();

	return true;
}

bool EntityManager::Update(float dt)
{
	for (ListItem<Entity*>* entiti = entities.start; entiti; entiti = entiti->next) entiti->data->Update(dt);

	return true;
}

bool EntityManager::PostUpdate()
{
	for (ListItem<Entity*>* entiti = entities.start; entiti; entiti = entiti->next) entiti->data->PostUpdate();

	return true;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	LOG("Destroying EntityManager");

	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.start;

	while (item != NULL && ret == true)
	{
		if (item->data->active == true)
			ret = item->data->CleanUp();

		item = item->next;
	}

	entities.Clear();
	active = false;
	return ret;
}