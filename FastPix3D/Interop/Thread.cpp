#include "Thread.h"
#include <SDL3/SDL.h>

Thread::Thread(std::function<void()> function) :
	Function(function),
	Mutex(SDL_CreateMutex()),
	Cond(SDL_CreateCondition()),
	SDLThread(SDL_CreateThread(ThreadFunc, nullptr, this)),
	_IsRunning(false)
{
}
Thread::~Thread()
{
	SDL_DestroyMutex(Mutex);
	SDL_DestroyCondition(Cond);
}

void Thread::Start()
{
	SDL_LockMutex(Mutex);

	if (!_IsRunning)
	{
		_IsRunning = true;
		SDL_SignalCondition(Cond);
	}

	SDL_UnlockMutex(Mutex);
}
void Thread::Join()
{
	SDL_LockMutex(Mutex);

	while (_IsRunning)
	{
		SDL_WaitCondition(Cond, Mutex);
	}

	SDL_UnlockMutex(Mutex);
}
int Thread::ThreadFunc(void *data)
{
	Thread *self = (Thread*)data;
	_MM_SET_ROUNDING_MODE(_MM_ROUND_TOWARD_ZERO);

	while (true)
	{
		SDL_LockMutex(self->Mutex);

		while (!self->_IsRunning)
		{
			SDL_WaitCondition(self->Cond, self->Mutex);
		}

		SDL_UnlockMutex(self->Mutex);

		self->Function();
		self->_IsRunning = false;

		SDL_LockMutex(self->Mutex);
		SDL_SignalCondition(self->Cond);
		SDL_UnlockMutex(self->Mutex);
	}

	return 0;
}