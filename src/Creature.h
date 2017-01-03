#ifndef __Creature_H__
#define __Creature_H__

#include "Entity.h"
#include "Animation.h"

class Creature : public Entity {
public:
	enum State
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
	Creature(Entity::Types entityType);
	virtual ~Creature();
public:
	Animation* current_animation = nullptr;
	Animation idle;
	Animation waiting;
	Animation walking;
	Animation jumping;
	Animation jumpKick;
	Animation chop;
	Animation respawning;
	fPoint velocity = { 0.0f, 0.0f };
	fPoint prevVelocity = { 0.0f, 0.0f };
	short int height = 0;
	float verticalForce = 0.0f;
	short int depth = 23;
	State status = UNAVAILABLE;
};

#endif // __Creature_H__