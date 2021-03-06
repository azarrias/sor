#ifndef __Animation_H__
#define __Animation_H__

#include <vector>
#include "Globals.h"

struct SDL_Rect;

typedef struct Frame {
	SDL_Rect rect;
	short int offset;
} Frame;

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	std::vector<Frame> frames;

private:
	float current_frame = 0.0f;
	int loops = 0;

public:
	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), frames(anim.frames)
	{}

	int GetCurrentFrameIndex()
	{
		float last_frame = (float)frames.size();

		current_frame += speed;
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : MAX(last_frame - 1.0f, 0.0f);
			loops++;
		}
		return (int)current_frame;
	}

	Frame& GetCurrentFrame()
	{
		return frames[GetCurrentFrameIndex()];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0.0f;
	}
};

#endif // __Animation_H__