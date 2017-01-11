#ifndef __ModuleEntityManager_H__
#define __ModuleEntityManager_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "SimpleTimer.h"
#include "Entity.h"
#include <vector>
#include "Creature.h"

class Player;

class ModuleEntityManager : public Module
{
public:
	ModuleEntityManager(bool active = true);
	~ModuleEntityManager();

	bool Start();
	update_status Update();
	bool CleanUp();
	Entity* createEntity(Entity::Types entityType, iPoint iniPos, Creature::Direction facing);
	void handleCollision(const Entity* e1, const Entity* e2);
public:
	Player* player = nullptr;
private:
	std::vector<Entity*> entities;
	short int aliveEntities = 0;
};

#endif // __ModuleEntityManager_H__