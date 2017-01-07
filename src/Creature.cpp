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

	}
}

void Creature::updatePosition() {
	int flipOffset = 0;
	if (status == IDLE || status == WALK || status == JUMPING || status == ATTACK_JMP)
		position.x += (int)velocity.x;
	if (position.x < 0)
		position.x = 0;
	if (height == 0 && status != JUMPING && status != ATTACKING)
		depth -= (int)velocity.y;
	if (status == IDLE || status == WALK)
		position.y += (int)velocity.y;
	if (status == RESPAWNING || status == JUMPING || status == ATTACK_JMP) {
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