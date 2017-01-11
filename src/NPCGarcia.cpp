#include "NPCGarcia.h"
#include "ModuleTextures.h"
#include "Application.h"
#include "Animation.h"

NPCGarcia::NPCGarcia(iPoint iniPos, Direction facing)
	: NPC(Entity::Types::NPC_GARCIA, iniPos, 12, facing)
{
	//setCurrentAnimation(&idle);
	position = iniPos;
	//status = IDLE;
}

NPCGarcia::~NPCGarcia()
{}

bool NPCGarcia::LoadConfigFromJSON(const char* fileName)
{
	JSON_Value* root_value;
	JSON_Object* moduleObject;

	root_value = json_parse_file(fileName);
	if (root_value != nullptr)
		moduleObject = json_object(root_value);
	else return false;

	graphics = App->textures->Load(json_object_dotget_string(moduleObject, "NPCGarcia.graphicsFile"));

	if (LoadAnimationFromJSONObject(moduleObject, "NPCGarcia.animations.idle", idle) == false ||
		LoadAnimationFromJSONObject(moduleObject, "NPCGarcia.animations.walking", walking) == false ||
		LoadAnimationFromJSONObject(moduleObject, "NPCGarcia.animations.attack", attack) == false ||
		LoadAnimationFromJSONObject(moduleObject, "NPCGarcia.animations.attack2", attack2) == false ||
		LoadAnimationFromJSONObject(moduleObject, "NPCGarcia.animations.beingHit", beingHit) == false ||
		LoadAnimationFromJSONObject(moduleObject, "NPCGarcia.animations.beingHit2", beingHit2) == false ||
		LoadAnimationFromJSONObject(moduleObject, "NPCGarcia.animations.beingHit3", beingHit3) == false ||
		LoadAnimationFromJSONObject(moduleObject, "NPCGarcia.animations.knockedOut", knockedOut) == false ||
		LoadAnimationFromJSONObject(moduleObject, "NPCGarcia.animations.dying", dying) == false ||
		LoadAnimationFromJSONObject(moduleObject, "NPCGarcia.animations.gettingUp", gettingUp) == false)
		return false;

	json_value_free(root_value);

	return true;
}

// Load assets
bool NPCGarcia::Start()
{
	LOG("Loading NPC Garcia");
	if (LoadConfigFromJSON(CONFIG_FILE) == false)
		return false;
	return NPC::Start();
}

// Unload assets
bool NPCGarcia::CleanUp()
{
	LOG("Unloading NPC Garcia");
	return Entity::CleanUp();
}
