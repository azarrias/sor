#include "Entity.h"

Entity::Entity(Types entityType)
	:entityType(entityType)
{
}

Entity::~Entity()
{}

void Entity::paint() {
}

update_status Entity::Update()
{
	return UPDATE_CONTINUE;
}