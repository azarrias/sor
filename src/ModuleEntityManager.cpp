#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
//#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "Player.h"
#include <stdio.h>
#include "MemLeaks.h"
#include "ModuleEntityManager.h"
#include "Entity.h"

ModuleEntityManager::ModuleEntityManager(bool active)
	: Module(active)
{}

ModuleEntityManager::~ModuleEntityManager()
{
	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		RELEASE(*it);
	}

	entities.clear();

//	RELEASE(player);
}

Entity* ModuleEntityManager::createEntity(Entity::Types entityType)
{
	static_assert(Entity::Types::UNKNOWN == 2, "code needs update");
	Entity* ret = nullptr;
	switch (entityType) {
		case Entity::Types::PLAYER:	ret = new Player();	break;
	}

	if (ret != nullptr)
		entities.push_back(ret);

	return ret;
}

bool ModuleEntityManager::Start()
{
	LOG("Loading entities");

	if (player != nullptr)
		player->Start();

	return true;
}

update_status ModuleEntityManager::Update()
{
/*	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		(*it)->Update();
	}*/
	if (player != nullptr)
		player->Update();

	return UPDATE_CONTINUE;
}

bool ModuleEntityManager::CleanUp()
{
	LOG("Unloading entities");

	if (player != nullptr)
		player->CleanUp();

	return true;
}