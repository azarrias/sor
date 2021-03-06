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
		ATTACKING_2,
		ATTACKING_3,
		ATTACKING_4,
		BEING_HIT,
		BEING_HIT_2_INI,
		BEING_HIT_2,
		BEING_HIT_2_END,
		DYING,
		DEAD,
		UNAVAILABLE
	};
	enum Direction
	{
		LEFT,
		RIGHT
	};
	Creature(Entity::Types entityType, iPoint iniPos, short int hp, Direction facing);
	virtual ~Creature();
	virtual bool Start();
	virtual bool LoadConfigFromJSON(const char* fileName);
	virtual update_status Update();
	virtual void updatePosition();
	void setCurrentAnimation(Animation* anim);
	void handleState();
	bool isAttacking() const;
	bool canBeAttacked() const;
	virtual void paint();
	virtual void spawn();
	virtual void hit(Creature* c2);
	virtual void die();
public:
	Animation* current_animation = nullptr;
	Animation idle;
	Animation waiting;
	Animation walking;
	Animation jumping;
	Animation jumpKick;
	Animation attack;
	Animation attack2;
	Animation respawning;
	Animation beingHit;
	Animation beingHit2;
	Animation beingHit3;
	Animation beingHit4;
	Animation gettingUp;
	Animation knockedOut;
	Animation dying;
	fPoint velocity = { 0.0f, 0.0f };
	fPoint prevVelocity = { 0.0f, 0.0f };
	short int height = 0;
	unsigned short int consecutiveHits = 0;
	float verticalForce = 0.0f;
	float horizontalForce = 0.0f;
	State status = UNAVAILABLE;
	SimpleTimer creatureTimer, refreshTimer, movementTimer, jumpTimer;
	SimpleTimer hitTimer, attackTimer, beingHitTimer;
	Collider* attackCollider = nullptr;
	Direction facing;
	short int hp;
protected:
	uint soundFxJump = 0;
	uint soundFxJumpLand = 0;
	uint soundFxFall = 0;
};

#endif // __Creature_H__