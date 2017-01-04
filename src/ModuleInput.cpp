#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"
#include "ModuleCollision.h"

ModuleInput::ModuleInput() : Module()
{}

// Destructor
ModuleInput::~ModuleInput()
{
	for (std::deque<KeyEvent*>::iterator it = keyEventQueue.begin(); it != keyEventQueue.end(); ++it)
	{
		RELEASE(*it);
	}

	keyEventQueue.clear();
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool ModuleInput::Start()
{
	return true;
}

// Called each loop iteration
update_status ModuleInput::PreUpdate()
{
	static SDL_Event event;
	memset(windowEvents, false, WE_COUNT * sizeof(bool));

	KeyState keyState;

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			}
			break;

			case SDL_KEYDOWN:
			case SDL_KEYUP:
				if (event.type == SDL_KEYDOWN) keyState = IS_DOWN;
				else if (event.type == SDL_KEYUP) keyState = IS_UP;
				if (event.key.repeat == 0)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE: return UPDATE_STOP; break;
					case SDLK_UP:	keyEventQueue.push_back(new KeyEvent{ KEY_UP, keyState }); break;
					case SDLK_DOWN:	keyEventQueue.push_back(new KeyEvent{ KEY_DOWN, keyState }); break;
					case SDLK_LEFT:	keyEventQueue.push_back(new KeyEvent{ KEY_LEFT, keyState }); break;
					case SDLK_RIGHT: keyEventQueue.push_back(new KeyEvent{ KEY_RIGHT, keyState }); break;
					case SDLK_SPACE: keyEventQueue.push_back(new KeyEvent{ KEY_SPACE, keyState }); break;
					case SDLK_a: keyEventQueue.push_back(new KeyEvent{ KEY_A, keyState }); break;
					case SDLK_s: keyEventQueue.push_back(new KeyEvent{ KEY_S, keyState }); break;
					case SDLK_d: keyEventQueue.push_back(new KeyEvent{ KEY_D, keyState }); break;
					case SDLK_F1: if (event.type == SDL_KEYDOWN) App->collision->switchDebugMode(); break;
					}
				}
			break;

		}
	}

	if (GetWindowEvent(EventWindow::WE_QUIT) == true)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL event subsystem");

	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

// ---------
bool ModuleInput::GetWindowEvent(EventWindow ev) const
{
	return windowEvents[ev];
}
