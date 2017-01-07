#include "NPCGarcia.h"
#include "ModuleTextures.h"
#include "Application.h"

NPCGarcia::NPCGarcia()
	: NPC(Entity::Types::NPC_GARCIA)
{
	// Coordinates for Garcia

	// idle animation
	idle.frames.push_back({ 47, 3, 35, 61 });
	idle.loop = false;
	idle.speed = 0.0f;

	// being hit
	beingHit.frames.push_back({0, 225, 45, 61});
	beingHit.loop = false;
	beingHit.speed = 0.0f;

	beingHit2.frames.push_back({ 54, 225, 45, 61 });
	beingHit2.loop = false;
	beingHit2.speed = 0.0f;

	beingHit3.frames.push_back({ 54, 225, 45, 61 });
	beingHit3.frames.push_back({ 107, 225, 45, 61 });
	beingHit3.frames.push_back({ 161, 225, 56, 61 });
	beingHit3.loop = false;
	beingHit3.speed = 0.1f;

	knockedOut.frames.push_back({ 221, 225, 44, 61 });
	knockedOut.loop = false;

	gettingUp.frames.push_back({ 280, 225, 44, 61 });
	gettingUp.loop = false;
	// respawning animation
/*	respawning.frames.push_back({ 48, 1104, 43, 65 });
	respawning.frames.push_back({ 6, 1104, 42, 65 });
	respawning.loop = false;
	respawning.speed = 0.0f;

	// walk animation
	walking.frames.push_back({ 306, 956, 36, 61 });
	walking.frames.push_back({ 344, 956, 41, 61 });
	walking.frames.push_back({ 386, 956, 36, 61 });
	walking.frames.push_back({ 424, 956, 41, 61 });
	walking.speed = 0.1f;

	// jump animation
	jumping.frames.push_back({ 8, 1108, 42, 61 });
	jumping.frames.push_back({ 48, 1104, 43, 65 });
	jumping.loop = false;
	jumping.speed = 0.0f;

	// jump kick
	jumpKick.frames.push_back({ 88, 1114, 42, 55 });
	jumpKick.frames.push_back({ 134, 1104, 43, 65 });
	jumpKick.frames.push_back({ 178, 1114, 70, 55 });
	jumpKick.loop = false;
	jumpKick.speed = 0.5f;

	// chop
	chop.frames.push_back({ 87, 1028, 47, 61 });
	chop.frames.push_back({ 7, 1028, 70, 61 });
	chop.frames.push_back({ 87, 1028, 47, 61 });
	chop.loop = false;
	chop.speed = 0.2f;*/

	position = { 400, 132 };
	setCurrentAnimation(&idle);
	status = IDLE;
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
