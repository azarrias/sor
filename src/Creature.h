#ifndef __Creature_H__
#define __Creature_H__

#include "Entity.h"
#include "Animation.h"
#include "SimpleTimer.h"

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
	virtual update_status Update();
	virtual void updatePosition();
	void setCurrentAnimation(Animation* anim);
	void handleState();
	void paint();
	virtual void spawn();
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
	iPoint position = { 23, -30 };
	short int height = 0;
	float verticalForce = 0.0f;
	short int depth = 23;
	State status = UNAVAILABLE;
	SimpleTimer creatureTimer, refreshTimer, movementTimer, jumpTimer, attackTimer;
};

#endif // __Creature_H__