#ifndef __Entity_H__
#define __Entity_H__

#include "Point.h"
#include "Globals.h"
#include <string>
#include "parson/parson.h"

struct SDL_Texture;
struct Collider;
class Animation;

class Entity
{
public:
	enum Types
	{
		PLAYER,
		NPC_GARCIA,
		UNKNOWN
	};
	Entity(Types entityType, iPoint iniPos);
	virtual ~Entity();
	virtual bool Init();
	virtual bool LoadConfigFromJSON(const char* fileName);
	bool LoadAnimationFromJSONObject(JSON_Object* jsonObject, std::string animationName, Animation& anim);
	virtual bool Start();
	virtual update_status Update();
	virtual bool CleanUp();
	virtual void paint();
	bool isCreature() const;
public:
	SDL_Texture* graphics = nullptr;
	Types entityType = UNKNOWN;
	iPoint iniPos;
	iPoint position;
	short int depth = 23;
	Collider* baseCollider = nullptr;
};

#endif // __Entity_H__