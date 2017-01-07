#include "Creature.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleStageTwo.h"
#include "ModuleCollision.h"

Creature::Creature(Entity::Types entityType)
	: Entity(entityType)
{
}

Creature::~Creature()
{}

bool Creature::Start()
{
	SDL_Rect baseColliderRect = { 0, 0, 0, 0 };
	baseCollider = App->collision->AddCollider(baseColliderRect);
	baseCollider->SetPos(1, 2);
	return true;
}

update_status Creature::Update()
{
	handleState();

	if (status != UNAVAILABLE) {
		updatePosition();
		paint();
	}

	return UPDATE_CONTINUE;
}

// Respawn player
void Creature::spawn()
{
	LOG("Spawning creature");
	position = iniPos;
	height = 0;
	setCurrentAnimation(&idle);
	status = IDLE;
	creatureTimer.reset();
}

void Creature::hit(Creature* c2) {
	if (hitTimer.getDelta() < 500) {
		++consecutiveHits;
	}
	else {
		consecutiveHits = 0;
	}
	switch (consecutiveHits) {
	case 0: 
	case 1:
		this->setCurrentAnimation(&chop);
		this->status = ATTACKING;
		c2->setCurrentAnimation(&(c2->beingHit));
		c2->status = BEING_HIT;
		break;
	case 2:
		this->setCurrentAnimation(&kick);
		this->status = ATTACKING;
		c2->setCurrentAnimation(&(c2->beingHit2));
		c2->status = BEING_HIT;
		break;
	case 3:
		this->setCurrentAnimation(&chop);
		this->status = ATTACKING;
		c2->setCurrentAnimation(&(c2->beingHit3));
		c2->status = BEING_HIT_2_INI;
		break;
	}
	hitTimer.reset();
	c2->beingHitTimer.reset();
}

bool Creature::isAttacking() const {
	return (status == State::ATTACKING || status == State::ATTACK_JMP);
}

bool Creature::canBeAttacked() const {
	return (status != State::BEING_HIT);
}

void Creature::setCurrentAnimation(Animation* anim) {
	if (current_animation != anim)
	{
		anim->Reset();
		current_animation = anim;
	}
}

void Creature::handleState()
{
	switch (status) {
	case UNAVAILABLE:
		if (App->scene_stage->stageState == LEVEL)
			spawn();
		break;

	case RESPAWNING:
		setCurrentAnimation(&respawning);
		if (height > 0)
			verticalForce += 0.3f;
		else {
			respawning.speed = 1.0f;
			if (creatureTimer.getDelta() >= 100) {
				status = IDLE;
				setCurrentAnimation(&idle);
			}
		}
		break;

	case IDLE:
		if (velocity.x != 0.0f || velocity.y != 0.0f) {
			status = WALK;
			setCurrentAnimation(&walking);
		}
		break;

	case WALK:
		if (velocity.x == 0.0f && velocity.y == 0.0f) {
			status = IDLE;
			setCurrentAnimation(&idle);
		}
		break;

	case JUMP_INI:
		if (jumpTimer.getDelta() >= 100) {
			verticalForce = -8.5f;
			jumping.speed = 1.0f;
			height = 0;
			status = JUMPING;
		}
		break;

	case JUMPING:
		if (height > 0) {
			verticalForce += 0.5f;
		}
		else {
			jumping.speed = 0.0f;
			jumping.Reset();
			prevVelocity = velocity;
			velocity.y = 0.0f;
			velocity.x = 0.0f;
			status = JUMP_END;
			jumpTimer.reset();
		}
		break;

	case ATTACK_JMP:
		if (height > 0)
			verticalForce += 0.5f;
		else {
			jumping.speed = 0.0f;
			jumping.Reset();
			prevVelocity = velocity;
			velocity.y = 0.0f;
			velocity.x = 0.0f;
			setCurrentAnimation(&jumping);
			status = JUMP_END;
			jumpTimer.reset();
		}
		break;

	case ATTACKING:
		if (attackTimer.getDelta() >= 300) {
			velocity += prevVelocity;
			if (velocity.x != 0.0f || velocity.y != 0.0f) {
				status = WALK;
				setCurrentAnimation(&walking);
			}
			else {
				status = IDLE;
				setCurrentAnimation(&idle);
			}
		}
		break;

	case ATTACKING_2:
		if (attackTimer.getDelta() >= 200) {
			velocity += prevVelocity;
			if (velocity.x != 0.0f || velocity.y != 0.0f) {
				status = WALK;
				setCurrentAnimation(&walking);
			}
			else {
				status = IDLE;
				setCurrentAnimation(&idle);
			}
		}
		break;

	case ATTACKING_3:
		if (attackTimer.getDelta() >= 200) {
			velocity += prevVelocity;
			if (velocity.x != 0.0f || velocity.y != 0.0f) {
				status = WALK;
				setCurrentAnimation(&walking);
			}
			else {
				status = IDLE;
				setCurrentAnimation(&idle);
			}
		}
		break;

	case ATTACKING_4:
		if (attackTimer.getDelta() >= 200) {
			velocity += prevVelocity;
			if (velocity.x != 0.0f || velocity.y != 0.0f) {
				status = WALK;
				setCurrentAnimation(&walking);
			}
			else {
				status = IDLE;
				setCurrentAnimation(&idle);
			}
		}
		break;

	case JUMP_END:
		if (jumpTimer.getDelta() >= 100) {
			velocity += prevVelocity;
			if (velocity.x != 0.0f || velocity.y != 0.0f) {
				status = WALK;
				setCurrentAnimation(&walking);
			}
			else {
				status = IDLE;
				setCurrentAnimation(&idle);
			}
		}
		break;

	case BEING_HIT:
		if (beingHitTimer.getDelta() >= 300) {
			status = IDLE;
			setCurrentAnimation(&idle);
		}
		break;

	case BEING_HIT_2_INI:
		verticalForce = -4.5f;
		height = 0;
		velocity.x = 4.0f;
		status = BEING_HIT_2;
		break;

	case BEING_HIT_2:
		if (height > 0) {
			verticalForce += 0.5f;
		}
		else {
			velocity.y = 0.0f;
			velocity.x = 0.0f;
			status = BEING_HIT_2_END;
			beingHitTimer.reset();
		}
		break;

	case BEING_HIT_2_END:
		if (beingHitTimer.getDelta() >= 1000) {
			// remember to check that it might be dead
			status = IDLE;
			setCurrentAnimation(&idle);
		}
		else if (beingHitTimer.getDelta() >= 500) {
			setCurrentAnimation(&gettingUp);
		}
		else setCurrentAnimation(&knockedOut);
		break;
	}
}

void Creature::updatePosition() {
	int flipOffset = 0;
	if (status == IDLE || status == WALK || status == JUMPING || status == ATTACK_JMP || status == BEING_HIT_2)
		position.x += (int)velocity.x;
	if (position.x < 0)
		position.x = 0;
	if (height == 0 && status != JUMPING && status != ATTACKING)
		depth -= (int)velocity.y;
	if (status == IDLE || status == WALK)
		position.y += (int)velocity.y;
	if (status == RESPAWNING || status == JUMPING || status == ATTACK_JMP || status == BEING_HIT_2) {
		height -= (int)verticalForce;
		if (height < 0) {
			height = 0;
			verticalForce = 0.0f;
		}
		position.y += (int)verticalForce;
	}
	if (depth > 53) {
		position.y = 102;
		depth = 53;
	}
	if (depth < 0 || position.y > 155) {
		position.y = 155;
		depth = 0;
	}
}

void Creature::paint()
{
	// Update colliders in here (in order to use the same SDL_Rect as Blit)
	int flipOffset = 0;
	bool flip = false;
	SDL_Rect currentFrame = current_animation->GetCurrentFrame();

	if (entityType == PLAYER && facing == LEFT || entityType != PLAYER && facing == RIGHT) {
		flip = true;
		flipOffset = currentFrame.w - idle.frames.front().w;
	}
	App->renderer->Blit(graphics, position.x - flipOffset, position.y, &currentFrame, flip);
	if (baseCollider != nullptr) {
		baseCollider->rect = currentFrame;
		baseCollider->SetPos(position.x - flipOffset, position.y);
	}
}
