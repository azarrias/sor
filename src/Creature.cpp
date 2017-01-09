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
	if (c2->position.x > position.x) {
		c2->facing = LEFT;
	}
	else {
		c2->facing = RIGHT;
	}
	if (height > 0 || c2->height > 0) {
		c2->setCurrentAnimation(&(c2->beingHit3));
		c2->status = BEING_HIT_2_INI;
	}
	else {
		switch (consecutiveHits) {
		case 0:
		case 1:
			this->setCurrentAnimation(&attack);
			this->status = ATTACKING;
			c2->setCurrentAnimation(&(c2->beingHit));
			c2->status = BEING_HIT;
			break;
		case 2:
			this->setCurrentAnimation(&attack2);
			this->status = ATTACKING;
			c2->setCurrentAnimation(&(c2->beingHit2));
			c2->status = BEING_HIT;
			break;
		case 3:
			this->setCurrentAnimation(&attack);
			this->status = ATTACKING;
			c2->setCurrentAnimation(&(c2->beingHit3));
			c2->status = BEING_HIT_2_INI;
			break;
		}
		hitTimer.reset();
		c2->beingHitTimer.reset();
	}
}

bool Creature::isAttacking() const {
	return (status == State::ATTACKING || status == State::ATTACK_JMP);
}

bool Creature::canBeAttacked() const {
	return (status != State::BEING_HIT && status != State::BEING_HIT_2_INI 
		&& status != State::BEING_HIT_2 && status != State::BEING_HIT_2_END);
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
		if (velocity.x > 0) facing = RIGHT;
		else if (velocity.x < 0) facing = LEFT;
		if (velocity.x == 0.0f && velocity.y == 0.0f) {
			status = IDLE;
			setCurrentAnimation(&idle);
		}
		break;

	case JUMP_INI:
		if (jumpTimer.getDelta() >= 100) {
			verticalForce = -8.0f;
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
		if (attackTimer.getDelta() >= attack.frames.size() * 100) {
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
		verticalForce = -5.0f;
		if (facing == RIGHT) {
			horizontalForce = -5.0f;
		}
		else {
			horizontalForce = 5.0f;
		}
		status = BEING_HIT_2;
		break;

	case BEING_HIT_2:
		if (height > 0) {
			verticalForce += 0.5f;
		}
		else {
			verticalForce = 0;
			horizontalForce = 0;
			status = BEING_HIT_2_END;
			beingHitTimer.reset();
		}
		break;

	case BEING_HIT_2_END:
		if (beingHitTimer.getDelta() >= (gettingUp.frames.size() - 1) * 10 / gettingUp.speed + 1100) {
			// remember to check that it might be dead
			status = IDLE;
			setCurrentAnimation(&idle);
		}
		else if (beingHitTimer.getDelta() >= 400) {
			setCurrentAnimation(&gettingUp);
		}
		else setCurrentAnimation(&knockedOut);
		break;
	}
}

void Creature::updatePosition() {
	if (status == IDLE || status == WALK || status == JUMPING || status == ATTACK_JMP)
		position.x += (int)velocity.x;
	if (position.x < 0)
		position.x = 0;
	if (height == 0 && status != JUMPING && status != ATTACKING && status != BEING_HIT_2 
		&& status != BEING_HIT_2_END && status != BEING_HIT && status != BEING_HIT_2_INI)
		depth -= (int)velocity.y;
	if (status == IDLE || status == WALK)
		position.y += (int)velocity.y;
	if (status == RESPAWNING || status == JUMPING || status == ATTACK_JMP || status == BEING_HIT_2) {
		height -= (int)verticalForce;
		position.x += (int)horizontalForce;
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
	if (status == IDLE || status == WALK)
		position.y = 155 - depth - height;
}

void Creature::paint()
{
	// Update colliders in here (in order to use the same SDL_Rect as Blit)
	bool flip = false;
	Frame currentFrame = current_animation->GetCurrentFrame();
	int offset = currentFrame.offset;

	if (facing == LEFT) {
		flip = true;
		offset = currentFrame.rect.w - idle.frames.front().rect.w - offset;
	}
	App->renderer->Blit(graphics, position.x - offset, position.y, &(currentFrame.rect), flip);
	if (baseCollider != nullptr) {
		baseCollider->rect = currentFrame.rect;
		baseCollider->SetPos(position.x - offset, position.y);
	}
}
