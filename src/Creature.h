#ifndef __Creature_H__
#define __Creature_H__

enum CreatureState
{
	RESPAWNING,
	IDLE,
	WALK,
	JUMP_INI,
	JUMPING,
	JUMP_END,
	ATTACK_JMP,
	ATTACKING,
	DEAD,
	UNAVAILABLE
};

class Creature : public Entity {

};

#endif // __Creature_H__