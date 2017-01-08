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
	// If the Player is in attack range then attack: attack or retreat
	// else if the Player is close be aggressive: attack, chase, retreat
	// else take it easy: patrol, wait
	if (getDistanceToPlayer() < 10) {
		
	}
	if (position.x > App->entities->player->position.x) {
		velocity.x = -1.0f;
	}
	else velocity.x = 0.0f;
}

float NPC::getDistanceToPlayer() const {
	short int depthDist = this->depth - App->entities->player->depth;
	short int xDist = this->position.x - App->entities->player->position.x;
	return (float)(sqrt(pow(depthDist, 2) + pow(xDist, 2)));
}

void NPC::attack() {
	status = ATTACKING;
	attackTimer.reset();
	setCurrentAnimation(&chop);
}