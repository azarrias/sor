#include "Entity.h"
#include "Application.h"
#include "ModuleTextures.h"

Entity::Entity(Types entityType)
	:entityType(entityType)
{
}

Entity::~Entity()
{}

void Entity::paint() {
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