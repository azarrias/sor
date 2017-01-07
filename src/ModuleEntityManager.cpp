#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
//#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "Player.h"
#include "NPCGarcia.h"
#include <stdio.h>
#include "MemLeaks.h"
#include "ModuleEntityManager.h"
#include "Entity.h"
#include "ModuleCamera.h"
#include <vector>

#define CAMERA_RANGE_MARGIN 50

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
}

Entity* ModuleEntityManager::createEntity(Entity::Types entityType)
{
	static_assert(Entity::Types::UNKNOWN == 2, "code needs update");
	Entity* ret = nullptr;
	switch (entityType) {
		case Entity::Types::PLAYER:	ret = new Player();	break;
		case Entity::Types::NPC_GARCIA: ret = new NPCGarcia(); break;
	}

	if (ret != nullptr)
		entities.push_back(ret);

	return ret;
}

bool ModuleEntityManager::Start()
{
	LOG("Loading entities");

	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		(*it)->Start();
	}

	return true;
}

update_status ModuleEntityManager::Update()
{
	// Subset the entities that are close to the camera range
	std::vector<Entity*> inRangeEntities(entities.size());
	std::vector<Entity*>::iterator it = std::copy_if(entities.begin(), entities.end(), inRangeEntities.begin(),
		[](Entity* e) { return (((Creature*)e)->position.x >= -App->camera->coord.x - CAMERA_RANGE_MARGIN &&
		((Creature*)e)->position.x <= -App->camera->coord.x + SCREEN_WIDTH + CAMERA_RANGE_MARGIN); });
	inRangeEntities.resize(std::distance(inRangeEntities.begin(), it));

	// Update subset of entities 
	for (std::vector<Entity*>::iterator it = inRangeEntities.begin(); it != inRangeEntities.end(); ++it)
	{
		(*it)->Update();
	}

	// Sort in range entities by depth (descending) to take care of overlapping
	// and check for collisions
	std::sort(inRangeEntities.begin(), inRangeEntities.end(), [](Entity* a, Entity* b) {return a->depth > b->depth; });
	for (size_t i = 0; i < inRangeEntities.size() - 1; ++i) {
		for (size_t j = i + 1; j < inRangeEntities.size(); ++j) {
			if (inRangeEntities[i]->baseCollider->CheckCollision(*inRangeEntities[j]->baseCollider))
				LOG("There is a collision");
		}
	}

	return UPDATE_CONTINUE;
}

bool ModuleEntityManager::CleanUp()
{
	LOG("Unloading entities");

	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		(*it)->CleanUp();
	}

	return true;
}
