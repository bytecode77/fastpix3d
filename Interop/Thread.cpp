#include "Thread.h"

Thread::Thread(ThreadFunction function, void *sender, void *parameter) :
	Function(function),
	Sender(sender),
	Parameter(parameter),
	Mutex(SDL_CreateMutex()),
	Cond(SDL_CreateCond()),
	SDLThread(SDL_CreateThread(ThreadFunc, nullptr, this)),
	IsRunning(false)
{
}
Thread::~Thread()
{
	SDL_DestroyMutex(Mutex);
	SDL_DestroyCond(Cond);
}

void Thread::Start()
{
	IsRunning = true;

	SDL_LockMutex(Mutex);
	SDL_CondSignal(Cond);
	SDL_UnlockMutex(Mutex);
}
void Thread::Join()
{
	SDL_LockMutex(Mutex);

	while (IsRunning)
	{
		SDL_CondWait(Cond, Mutex);
	}

	SDL_UnlockMutex(Mutex);
}
int Thread::ThreadFunc(void *data)
{
	Thread *self = (Thread*)data;

	while (true)
	{
		SDL_LockMutex(self->Mutex);

		while (!self->IsRunning)
		{
			SDL_CondWait(self->Cond, self->Mutex);
		}

		SDL_UnlockMutex(self->Mutex);

		((ThreadFunction)self->Function)(self->Sender, self->Parameter);
		self->IsRunning = false;

		SDL_LockMutex(self->Mutex);
		SDL_CondSignal(self->Cond);
		SDL_UnlockMutex(self->Mutex);
	}

	return 0;
}