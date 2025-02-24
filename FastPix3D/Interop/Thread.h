#pragma once
#include "../FastPix3D.h"

struct SDL_Mutex;
struct SDL_Condition;
struct SDL_Thread;

class FASTPIX3D_API Thread
{
private:
	const std::function<void()> Function;
	SDL_Mutex *Mutex;
	SDL_Condition *Cond;
	SDL_Thread *SDLThread;
	bool _IsRunning;

public:
	readonly_property(bool, IsRunning)
	{
		return _IsRunning;
	}

	Thread(std::function<void()> function);
	~Thread();

	void Start();
	void Join();

private:
	static int ThreadFunc(void *data);
};