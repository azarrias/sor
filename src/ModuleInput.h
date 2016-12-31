#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
#include "Point.h"

#include "SDL/include/SDL_scancode.h"
#include <queue>

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	IS_IDLE = 0,
	IS_DOWN,
	IS_REPEAT,
	IS_UP
};

enum KeyId
{
	KEY_SPACE,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_UP,
	KEY_DOWN,
	KEY_A,
	KEY_S,
	KEY_D
};

typedef struct KeyEvent
{
	KeyId key;
	KeyState status;
} KeyEvent;

class ModuleInput : public Module
{

public:

	ModuleInput();

	// Destructor
	virtual ~ModuleInput();

	// Called before render is available
	bool Init();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	update_status PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Check for window events last frame
	bool GetWindowEvent(EventWindow code) const;

public:
	std::deque<KeyEvent*> keyEventQueue;

private:
	bool		windowEvents[WE_COUNT];
};

#endif // __ModuleInput_H__