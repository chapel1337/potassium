#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>
#include <random>
#include <thread>

using std::cout, std::string, std::to_string, std::thread, std::mt19937, std::random_device, std::uniform_int_distribution;

/*
* written by chapel1337
* started on 12/22/2022, finsished on 12/23/2022
* this was mainly just a project for experimenting with external sdl libraries (sdl_image and sdl_mixer)
* this has way too many dependencies : (
*/

mt19937 mt{ random_device{}() };
int getRandom(const int  minimum, const int maximum)
{
	uniform_int_distribution random{ minimum, maximum };

	return random(mt);
}

bool random{};

// int windowsOpened{};

void newWindow()
{
	SDL_Window* window{ SDL_CreateWindow("potassium", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 750, 750, NULL) };
	SDL_Renderer* renderer{ SDL_CreateRenderer(window, -1, NULL) };

	string potassiumFile{ "potassium_frames/frame_000_delay-0.1s.gif" };

	SDL_Surface* potassiumSurface{ IMG_Load(potassiumFile.c_str()) };
	SDL_Texture* potassiumTexture{ SDL_CreateTextureFromSurface(renderer, potassiumSurface) };
	SDL_Rect potassiumRect{ 0, 0, 750, 750 };

	Mix_Music* music{ NULL };

	SDL_Event event{};

	int frame{};

	if (random)
	{
		SDL_SetWindowSize(window, getRandom(250, 750), getRandom(250, 750));
		SDL_GetWindowSize(window, &potassiumRect.w, &potassiumRect.h);

		SDL_SetWindowPosition(window, getRandom(0, 500), getRandom(0, 500));
	}

	SDL_SetWindowBordered(window, SDL_FALSE);

	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);

	// Mix_PlayMusic() refuses to play music when the file is declared when intialized, so i am forced to do it here
	music = Mix_LoadMUS("potassium.mp3");
	Mix_PlayMusic(music, -1);

	while (true)
	{
		SDL_PollEvent(&event);

		if (event.type == SDL_QUIT)
		{
			SDL_DestroyWindow(window);
			SDL_FreeSurface(potassiumSurface);

			Mix_HaltMusic();
			Mix_FreeMusic(music);

			SDL_Quit();
			exit(1);
		}
		else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r)
		{
			SDL_DestroyWindow(window);
			SDL_FreeSurface(potassiumSurface);

			Mix_HaltMusic();
			Mix_FreeMusic(music);

			if (random)
			{
				random = false;
				newWindow();
			}
			else
			{
				random = true;
				newWindow();
			}
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, potassiumTexture, NULL, &potassiumRect);

		SDL_RenderPresent(renderer);

		if (frame == 146)
		{
			if (random)
			{
				SDL_DestroyWindow(window);
				SDL_FreeSurface(potassiumSurface);

				Mix_HaltMusic();
				Mix_FreeMusic(music);

				newWindow();
			}
			else
			{
				frame = 0;
			}
		}
		else
		{
			++frame;
		}

		if (frame > 100)
		{
			potassiumFile = "potassium_frames/frame_" + to_string(frame) + "_delay-0.1s.gif";
		}
		else if (frame > 9)
		{
			potassiumFile = "potassium_frames/frame_" + string("0") + to_string(frame) + "_delay-0.1s.gif";
		}
		else
		{
			potassiumFile = "potassium_frames/frame_" + string("00") + to_string(frame) + "_delay-0.1s.gif";
		}

		potassiumSurface = IMG_Load(potassiumFile.c_str());
		potassiumTexture = SDL_CreateTextureFromSurface(renderer, potassiumSurface);

		SDL_Delay(90);

		/*
		if (windowsOpened != 15)
		{
			thread test{ newWindow };
			test.detach();

			++windowsOpened;
		}
		*/
	}
}

int main()
{
	newWindow();
	
	return 0;
}