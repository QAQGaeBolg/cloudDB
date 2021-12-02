#include "lo_utilities.hpp"

#ifndef WIN32
#include "unistd.h"
#endif

#ifdef WIN32
static DWORD WINAPI ThreadProc(void* lpParameter)
{
	Thread *thread = (Thread *)lpParameter;
	thread->action();
	return 0;
}
#else
static void* ThreadProc(void* lpParameter)
{
	Thread *thread = (Thread *)lpParameter;
	return thread->action();
}
#endif

Thread::Thread(int detached = 0)
{
	status = new_created;
#ifdef WIN32
#else
	pthread_attr_init(&th_attr);
	if (detached)
	{
		pthread_attr_setdetachstate(&th_attr, PTHREAD_CREATE_DETACHED);
	}
#endif
}

Thread::~Thread(){
#ifdef WIN32
	if (status == running) {
		TerminateThread(thread_id, 0);
	}
#else
	//	if ( status == running ) {
	//		pthread_cancel(thread_id);
	//	}
#endif
}

int Thread::start()
{
#ifdef WIN32
	if (status == new_created) {
		thread_id = CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
		if (thread_id){
			status = running;
			return 0;
		}
	}
#else
	if (status == new_created
		&& !pthread_create(&thread_id, &th_attr, ThreadProc, this)) {
		status = running;
		return 0;
	}
#endif
	return -1;
}

int Thread::stop(int nKill)
{
#ifdef WIN32
	if (status == running) {
		if (nKill)
		{
			TerminateThread(thread_id, 0);
		}
		status = stopped;
		return 0;
	}
#else
	if (status == running) {
		if (nKill)
		{
			pthread_cancel(thread_id);
			usleep(100);	// let thread process left work
		}
		status = stopped;
		return 0;
	}
#endif
	return -1;
}

/*****************************************************************************
* implementation of Mutex;
* **************************************************************************/
Mutex::Mutex(int mutex_type)
{
#ifdef WIN32
	mutex = CreateMutex(NULL, false, NULL);
#else
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	if (pthread_mutexattr_settype(&attr, mutex_type))
	{
		// Invalid mutex_type, use default type;
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
	}
	pthread_mutex_init(&mutex, &attr);
#endif
}

Mutex::~Mutex()
{
#ifdef WIN32
	CloseHandle(mutex);
#else
	pthread_mutex_destroy(&mutex);
#endif
}


bool Mutex::trylock()
{
#ifdef WIN32
	return false;
#else
	int rc = pthread_mutex_trylock(&mutex);
	return (0 == rc);
#endif
}


int Mutex::lock()
{
#ifdef WIN32
	if (WAIT_FAILED == WaitForSingleObject(mutex, INFINITE))
		return -1;
	else
		return 0;
#else
	return pthread_mutex_lock(&mutex);
#endif
}


int Mutex::unlock()
{
#ifdef WIN32
	if (ReleaseMutex(mutex))
		return 0;
	else
		return -1;
#else
	return  pthread_mutex_unlock(&mutex);
#endif
}
