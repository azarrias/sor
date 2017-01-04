#ifndef __NPC_H__
#define __NPC_H__

#include "Creature.h"

class NPC : public Creature {
public:
	NPC(Entity::Types entityType);
	virtual ~NPC();
};

#endif // __NPC_H__