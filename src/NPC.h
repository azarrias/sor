#ifndef __NPC_H__
#define __NPC_H__

#include "Creature.h"

class NPC : public Creature {
public:
	enum Action
	{
		ATTACK,
		ATTACK_RECOVER,
		RETREAT,
		APPROACH,
		ALIGN,
		WAIT,
		PATROL
	};
	NPC(Entity::Types entityType, iPoint iniPos, short int hp);
	virtual ~NPC();
	virtual bool Start();
	virtual bool LoadConfigFromJSON(const char* fileName);
	update_status Update();
	virtual void hit(Creature* c2);
	void behaviour();
	float getDistanceToPlayer() const;
	void retreat();
	void chooseNextAction();
	void getAwayFromPlayer();
	void chase();
	const unsigned short int rollDice(unsigned short int nrDiceFaces) const;
	unsigned short int waitingTime = 1000;
private:
	SimpleTimer NPCTimer;
	Action action = WAIT;
protected:
	uint soundFxNPCHit = 0;
	uint soundFxNPCDie = 0;
};

#endif // __NPC_H__