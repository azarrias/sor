#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
//#include "ModuleParticles.h"
#include "ModuleStageTwo.h"
#include "ModuleHUD.h"
#include "ModuleEntityManager.h"
#include "Player.h"
#include "ModuleCamera.h"
#include "ModuleFadeToBlack.h"

// Reference at https://www.youtube.com/watch?v=J9gWS0ZQaxE

ModuleStageTwo::ModuleStageTwo(bool active) 
	: Module(active), stageTimer()
{}

ModuleStageTwo::~ModuleStageTwo()
{}

// Load assets
bool ModuleStageTwo::Start()
{
	LOG("Loading stage");

	background = App->textures->Load("graphics/stage2_bg.png");
	intro = App->textures->Load("graphics/hud.png");

	App->hud->Enable();
	App->entities->player = (Player*)(App->entities->createEntity(Entity::Types::PLAYER, { 0, 0 }, Creature::Direction::RIGHT));
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 400, 132 }, Creature::Direction::LEFT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 600, 132 }, Creature::Direction::LEFT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 650, 132 }, Creature::Direction::RIGHT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 900, 132 }, Creature::Direction::LEFT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 950, 132 }, Creature::Direction::RIGHT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 1100, 132 }, Creature::Direction::LEFT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 1150, 132 }, Creature::Direction::RIGHT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 1400, 132 }, Creature::Direction::LEFT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 1450, 132 }, Creature::Direction::LEFT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 1450, 132 }, Creature::Direction::RIGHT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 1700, 132 }, Creature::Direction::LEFT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 1900, 132 }, Creature::Direction::LEFT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 1950, 132 }, Creature::Direction::RIGHT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 2200, 132 }, Creature::Direction::LEFT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 2250, 132 }, Creature::Direction::RIGHT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 2400, 132 }, Creature::Direction::LEFT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 2450, 132 }, Creature::Direction::RIGHT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 2700, 132 }, Creature::Direction::LEFT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 2750, 132 }, Creature::Direction::LEFT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 2750, 132 }, Creature::Direction::RIGHT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 3050, 132 }, Creature::Direction::LEFT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 3050, 132 }, Creature::Direction::RIGHT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 3100, 132 }, Creature::Direction::LEFT);
	App->entities->createEntity(Entity::Types::NPC_GARCIA, { 3100, 132 }, Creature::Direction::RIGHT);

	App->entities->Enable();
	App->collision->Enable();

	App->audio->PlayMusic("audio/stage2.ogg", 1.0f);

	// TODO 15: create some colliders for the walls
	// solution wall coords: {0, 224, 3930, 16} {1375, 0, 111, 96} {1375, 145, 111, 96}

	return true;
}

// UnLoad assets
bool ModuleStageTwo::CleanUp()
{
	LOG("Unloading stage");

	App->textures->Unload(background);
	App->entities->Disable();
	App->hud->Disable();
	App->collision->Disable();
//	App->particles->Disable();

	return true;
}

update_status ModuleStageTwo::Update()
{
	// Draw everything --------------------------------------
	SDL_Rect round;
	round.x = 0;
	round.y = 50;
	round.w = 76;
	round.h = 14;

	//stageTimer.update();

	switch (stageState)
	{
	case INTRO:
		App->renderer->Blit(intro, 70, 100, &round);
		if (stageTimer.getDelta() >= 6000) {
			stageState = LEVEL;
			stageTimer.reset();
		}
		break;
	case LEVEL:
		App->renderer->Blit(background, 0, 0, NULL);
		if (App->entities->player->lives == 0) {
			stageTimer.reset();
			stageState = END;
		}
		else if (App->entities->player->status == Creature::State::UNAVAILABLE ||
			App->entities->player->status == Creature::State::DEAD)
			App->entities->player->spawn();
		break;
	case END:
		if (App->fade->isFading() == false) {
			App->fade->FadeToBlack((Module*)App->scene_intro, this, 4.0f);
		}
		break;
	}

	return UPDATE_CONTINUE;
}