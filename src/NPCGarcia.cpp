#include "NPCGarcia.h"
#include "ModuleTextures.h"
#include "Application.h"
#include "Animation.h"

NPCGarcia::NPCGarcia(iPoint iniPos)
	: NPC(Entity::Types::NPC_GARCIA, iniPos, 12)
{
	// Coordinates for Garcia

	// idle animation
	idle.frames.push_back({ 46, 3, 36, 61, 0 });
	idle.loop = false;
	idle.speed = 0.0f;

	// walk animation
	walking.frames.push_back({ 0, 3, 45, 61, 0 });
	walking.frames.push_back({ 46, 3, 36, 61, 0 });
	walking.frames.push_back({ 83, 3, 36, 61, 0 });
	walking.frames.push_back({ 46, 3, 36, 61, 0 });
	walking.speed = 0.1f;

	// being hit
	beingHit.frames.push_back({10, 225, 35, 61, 9});
	beingHit.loop = false;
	beingHit.speed = 0.0f;

	beingHit2.frames.push_back({ 54, 225, 45, 61, 0 });
	beingHit2.loop = false;
	beingHit2.speed = 0.0f;

	beingHit3.frames.push_back({ 54, 225, 45, 61, 0 });
	beingHit3.frames.push_back({ 107, 225, 45, 61, 0 });
	beingHit3.frames.push_back({ 161, 225, 56, 61, 0 });
	beingHit3.loop = false;
	beingHit3.speed = 0.1f;

	knockedOut.frames.push_back({ 221, 225, 57, 61, 0 });
	knockedOut.loop = false;

	gettingUp.frames.push_back({ 278, 225, 57, 61, 12 });
	gettingUp.loop = false;

	attack.frames.push_back({ 124, 3, 57, 61, 0 });
	attack.loop = false;

	attack2.frames.push_back({ 187, 3, 67, 61, 5 });
	attack2.loop = false;

	setCurrentAnimation(&idle);
	position = iniPos;
	status = IDLE;
	facing = LEFT;
}

NPCGarcia::~NPCGarcia()
{}

// Load assets
bool NPCGarcia::Start()
{
	LOG("Loading NPC Garcia");
	graphics = App->textures->Load("graphics/garcia.png");
	return Creature::Start();
}

// Unload assets
bool NPCGarcia::CleanUp()
{
	LOG("Unloading NPC Garcia");
	return Entity::CleanUp();
}
