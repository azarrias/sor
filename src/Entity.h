#ifndef __Entity_H__
#define __Entity_H__

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
};

#endif // __Entity_H__