#ifndef __SimpleTimer_H__
#define __SimpleTimer_H__

#include <SDL/include/SDL.h>

class SimpleTimer
{
public:
	SimpleTimer(int p_interval = 1000)
	{
		interval = p_interval;
		accumulator = 0;
		total = 0;
		cur_time = last_time = SDL_GetTicks();
	}
	int ready()
	{
		return accumulator > interval;
	}
	int check()
	{
		if (ready())
		{
			//accumulator -= interval;
			accumulator = 0;
			return 1;
		}
		return 0;
	}
	void update()
	{
		int delta;
		cur_time = SDL_GetTicks();
		delta = cur_time - last_time;
		total += delta;
		accumulator += delta;
		last_time = cur_time;
	}
public:
	int last_time;
	int cur_time;
	int accumulator;
	int total;
	int interval;
};

#endif // __Animation_H__