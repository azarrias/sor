#ifndef __ModuleEntityManager_H__
#define __ModuleEntityManager_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "SimpleTimer.h"
#include "Entity.h"
#include <vector>

class Player;

class ModuleEntityManager : public Module
{
public:
	ModuleEntityManager(bool active = true);
	~ModuleEntityManager();

	bool Start();
//	bool Init();
	update_status Update();
	bool CleanUp();
	Entity* createEntity(Entity::Types entityType);
public:
	Player* player = nullptr;
private:
	std::vector<Entity*> entities;
};

#endif // __ModuleEntityManager_H__