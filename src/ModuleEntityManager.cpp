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

Entity* ModuleEntityManager::createEntity(Entity::Types entityType, iPoint iniPos, Creature::Direction facing)
{
	static_assert(Entity::Types::UNKNOWN == 2, "code needs update");
	Entity* ret = nullptr;
	switch (entityType) {
		case Entity::Types::PLAYER:	ret = new Player();	break;
		case Entity::Types::NPC_GARCIA: ret = new NPCGarcia(iniPos, facing); break;
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
	// Subset the entities that are close to the camera range and are not dead
	std::vector<Entity*> inRangeEntities(entities.size());
	std::vector<Entity*>::iterator it = std::copy_if(entities.begin(), entities.end(), inRangeEntities.begin(),
		[](Entity* e) { return (//((Creature*)e)->position.x >= -App->camera->coord.x - CAMERA_RANGE_MARGIN &&
		((Creature*)e)->position.x <= -App->camera->coord.x + SCREEN_WIDTH + CAMERA_RANGE_MARGIN &&
		((Creature*)e)->status != Creature::State::DEAD); });
	inRangeEntities.resize(std::distance(inRangeEntities.begin(), it));

	// Sort entities subset by depth (descending) to take care of overlapping and check for collisions
	if (inRangeEntities.size() > 0) {
		std::sort(inRangeEntities.begin(), inRangeEntities.end(), [](Entity* a, Entity* b) {return a->depth > b->depth; });
		for (size_t i = 0; i < inRangeEntities.size() - 1; ++i) {
			for (size_t j = i + 1; j < inRangeEntities.size(); ++j) {
				if (abs(inRangeEntities[i]->depth - inRangeEntities[j]->depth) <= DEPTH_THRESHOLD) {
					if (inRangeEntities[i]->baseCollider->CheckCollision(*inRangeEntities[j]->baseCollider)) {
						LOG("There is a collision");
						handleCollision(inRangeEntities[i], inRangeEntities[j]);
					}
				}
				else break;
			}
		}
	}

	// Update only this subset of entities 
	for (std::vector<Entity*>::iterator it = inRangeEntities.begin(); it != inRangeEntities.end(); ++it)
	{
		// This is a workaround to knock down enemies when the player is respawning
		// and should be refactored
		if (App->entities->player->status == Creature::State::RESPAWNING && (*it)->isCreature() 
			&& (*it) != (Entity*)App->entities->player && ((Creature*)*it)->status != Creature::State::BEING_HIT_2_INI
			&& ((Creature*)*it)->status != Creature::State::BEING_HIT_2 && ((Creature*)*it)->status != Creature::State::BEING_HIT_2_END)
			((Creature*)*it)->status = Creature::State::BEING_HIT_2_INI;
		(*it)->Update();
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

void ModuleEntityManager::handleCollision(const Entity* e1, const Entity* e2)
{
	if (e1->isCreature() && e2->isCreature()) {
		if (((Creature*)e1)->isAttacking() && ((Creature*)e2)->canBeAttacked())
			((Creature*)e1)->hit((Creature*)e2);
		else if (((Creature*)e2)->isAttacking() && ((Creature*)e1)->canBeAttacked())
			((Creature*)e2)->hit((Creature*)e1);
	}
}