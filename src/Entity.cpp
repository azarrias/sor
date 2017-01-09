#include "Entity.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"

Entity::Entity(Types entityType)
	:entityType(entityType)
{
}

Entity::~Entity()
{}

void Entity::paint() {
}

bool Entity::Init()
{
	return true;
}

bool Entity::LoadConfigFromJSON(const char* fileName)
{
	return true;
}

bool Entity::Start()
{
	return true;
}

update_status Entity::Update()
{
	return UPDATE_CONTINUE;
}

bool Entity::CleanUp()
{
	App->textures->Unload(graphics);

	return true;
}

bool Entity::isCreature() const {
	return (entityType == PLAYER || entityType == NPC_GARCIA);
}
