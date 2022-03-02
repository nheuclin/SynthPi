#include "Thread.h"
#include "Lock.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdio>

Thread::Thread() {
	
	running = false;	
}

Thread::~Thread() {

	// Nothing to do here either
}

void Thread::run() {

	// This should never do anything 
	printf("This does not get called\n");
}

void * Thread::threadCallback(void * thread) {

        // This basically casts the paramater back to a Thread and then calls the run method
	Thread *t = reinterpret_cast<Thread*>(thread);
	
	// This sandwich allows the thread to tell whether its running or not
	t->setRunning(true);
	t->run();
	t->setRunning(false);

	pthread_exit(NULL);	

}

void Thread::setRunning(bool running) {

	runningLock.acquire();

	this->running = running;

	runningLock.broadcast(); 
	runningLock.release();

}

void Thread::start() {

	if(isRunning()) return; // Cannot start thread twice
	
	if(pthread_create(&pthreadHandle, NULL, this->threadCallback, (void*)this) != 0) {

		throw new ThreadCreateFailedException();
	}

}


void Thread::join() {
	
	void * returned;

	pthread_join( pthreadHandle, &returned );

}

void Thread::sleep(int time) {

	usleep(time*1000);
}

bool Thread::isRunning() {

	bool out;

	runningLock.acquire();
	out = running;
	runningLock.release();

	return out;
}
