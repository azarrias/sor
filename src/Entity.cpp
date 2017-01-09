#include "Entity.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"
#include "Animation.h"

Entity::Entity(Types entityType)
	:entityType(entityType)
{
}

Entity::~Entity()
{}

void Entity::paint() {
}

bool Entity::Init()
{
	return true;
}

bool Entity::LoadConfigFromJSON(const char* fileName)
{
	return true;
}

bool Entity::LoadAnimationFromJSONObject(JSON_Object* jsonObject, std::string animationName, Animation& anim) {
	JSON_Array* array = nullptr;
	JSON_Object* frameObject = nullptr;

	anim.speed = (float)json_object_dotget_number(jsonObject, (animationName + ".speed").c_str());
	if (json_object_dotget_boolean(jsonObject, (animationName + ".loop").c_str()) == 1)
		anim.loop = true;
	else if (json_object_dotget_boolean(jsonObject, (animationName + ".loop").c_str()) == 0)
		anim.loop = false;
	else return false;

	array = json_object_dotget_array(jsonObject, (animationName + ".frames").c_str());

	for (size_t i = 0; i < json_array_get_count(array); ++i) {
		frameObject = json_array_get_object(array, i);
		short int x = (short int)json_object_dotget_number(frameObject, "x");
		short int y = (short int)json_object_dotget_number(frameObject, "y");
		short int w = (short int)json_object_dotget_number(frameObject, "w");
		short int h = (short int)json_object_dotget_number(frameObject, "h");
		short int offset = (short int)json_object_dotget_number(frameObject, "offset");
		anim.frames.push_back({ x, y, w, h, offset });
	}

	return true;
}

bool Entity::Start()
{
	return true;
}

update_status Entity::Update()
{
	return UPDATE_CONTINUE;
}

bool Entity::CleanUp()
{
	App->textures->Unload(graphics);

	return true;
}

bool Entity::isCreature() const {
	return (entityType == PLAYER || entityType == NPC_GARCIA);
}
