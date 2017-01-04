#ifndef __Entity_H__
#define __Entity_H__

#include "Point.h"
#include "Globals.h"

struct SDL_Texture;

class Entity
{
public:
	enum Types
	{
		PLAYER,
		NPC_GARCIA,
		UNKNOWN
	};
	Entity(Types entityType);
	virtual ~Entity();
	virtual update_status Entity::Update();
	virtual void paint();
public:
	SDL_Texture* graphics = nullptr;
	Types entityType = UNKNOWN;
	iPoint iniPos = { 23, -30 };
};

#endif // __Entity_H__