#include "NPC.h"
#include "Application.h"
#include "ModuleEntityManager.h"
#include "Player.h"

NPC::NPC(Entity::Types entityType)
	: Creature(entityType)
{}

NPC::~NPC()
{}

// Update: draw background
update_status NPC::Update()
{
	//doAction();
	Creature::Update();
	return UPDATE_CONTINUE;
}

void NPC::doAction() {
	if (position.x > App->entities->player->position.x) {
		velocity.x = -1.0f;
	}
	else velocity.x = 0.0f;
}