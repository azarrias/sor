#ifndef __Entity_H__
#define __Entity_H__

#include "Point.h"

struct SDL_Texture;

class Entity
{
public:
	enum Types
	{
		PLAYER,
		NPC,
		UNKNOWN
	};
	Entity(Types entityType);
	virtual ~Entity();
public:
	SDL_Texture* graphics = nullptr;
	Types entityType = UNKNOWN;
	iPoint position = { 23, -30 };
};

#endif // __Entity_H__