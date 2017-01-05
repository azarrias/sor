#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneIntro.h"
#include "ModuleCamera.h"

// Reference at https://www.youtube.com/watch?v=MXjaCkPWpvU

ModuleSceneIntro::ModuleSceneIntro(bool active) 
	: Module(active), introTimer()
{
	// title animation
	title.frames.push_back({ 656, 4, 168, 101 });
	title.frames.push_back({ 656, 107, 168, 101 });
	title.frames.push_back({ 344, 17, 168, 101 });
	title.frames.push_back({ 656, 4, 168, 101 });
	title.speed = 0.0f;

	// subtitle animation
	subtitle.frames.push_back({ 9, 7, 96, 30 });
	subtitle.frames.push_back({ 118, 7, 96, 30 });
	subtitle.frames.push_back({ 9, 7, 96, 30 });
	subtitle.speed = 0.0f;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading game intro");

	background = App->textures->Load("graphics/intro.png");

	App->audio->PlayMusic("audio/intro.ogg", 1.0f);
	if (fx == 0)
		fx = App->audio->LoadFx("audio/starting.wav");

	App->camera->coord.x = App->camera->coord.y = 0;

	return true;
}

// UnLoad assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading game intro");

	App->textures->Unload(background);

	return true;
}


update_status ModuleSceneIntro::Update()
{
	// Draw background
	SDL_Rect rectBG;
	rectBG.x = 334;
	rectBG.y = 4;
	rectBG.w = 320;
	rectBG.h = 240;
	App->renderer->Blit(background, 0, 0, &rectBG);

	SDL_Rect rectSubtitle;
	rectSubtitle.x = 9;
	rectSubtitle.y = 7;
	rectSubtitle.w = 96;
	rectSubtitle.h = 30;

	size_t animationIdx = 0;
	unsigned short int subtitleLimitPosx = 40;

	SDL_Rect rectAdam;
	rectAdam.x = 1;
	rectAdam.y = 42;
	rectAdam.w = 111;
	rectAdam.h = 165;

	SDL_Rect rectBlaze;
	rectBlaze.x = 113;
	rectBlaze.y = 42;
	rectBlaze.w = 114;
	rectBlaze.h = 165;

	SDL_Rect rectAxel;
	rectAxel.x = 228;
	rectAxel.y = 42;
	rectAxel.w = 101;
	rectAxel.h = 165;
	
	if (introState < 3 && introTimer.getDelta() >= 2000) {
		++introState;
		introTimer.reset();
	}

	// Animate title & subtitle 
	if (introState >= 3)
	{
		if (introTimer.getDelta() >= 2000) {
			title.speed = 0.4f;
			subtitle.speed = 0.1f;
			introTimer.reset();
		}

		if (title.speed > 0.0f) {
			animationIdx = title.GetCurrentFrameIndex();
			App->renderer->Blit(background, 10, 13, &(title.frames[animationIdx]));
			if (animationIdx == title.frames.size() - 1) {
				title.speed = 0.0f;
				title.Reset();
			}
		}

		if (subtitle.speed > 0.0f) {
			animationIdx = subtitle.GetCurrentFrameIndex();
			App->renderer->Blit(background, 40, 15, &(subtitle.frames[animationIdx]));
			if (animationIdx == subtitle.frames.size() - 1) {
				subtitle.speed = 0.0f;
				subtitle.Reset();
			}
		}
		else
		{
			App->renderer->Blit(background, 40, 15, &rectSubtitle);
		}
	}

	// Draw subtitle with left to right transition
	if (introState >= 1 && introState < 3)
	{
		if (subtitlePosx < subtitleLimitPosx) 
			subtitlePosx += 2;
		App->renderer->Blit(background, subtitlePosx, 15, &rectSubtitle);
	}

	// Draw main characters
	if (introState >= 2)
	{
		App->renderer->Blit(background, 115, 72, &rectAdam, false, 1.0f, 0.95f);
		App->renderer->Blit(background, 203, 72, &rectBlaze, false, 1.0f, 0.95f);
		App->renderer->Blit(background, 165, 72, &rectAxel, false, 1.0f, 0.95f);
	}

	while (App->input->keyEventQueue.empty() == false)
	{
  		KeyEvent* keyEvent = App->input->keyEventQueue.front();
		App->input->keyEventQueue.pop_front();

		if (keyEvent->key == KEY_SPACE && keyEvent->status == IS_DOWN && App->fade->isFading() == false)
		{
			App->fade->FadeToBlack((Module*)App->scene_stage, this);
			App->audio->PlayFx(fx);
		}

		RELEASE(keyEvent);
	}

	return UPDATE_CONTINUE;
}