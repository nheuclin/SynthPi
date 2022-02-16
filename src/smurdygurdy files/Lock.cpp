/**
 *Adapter for handling a lock
 *
 *Provides easy mutex and condition variable initialisation.
 */

#include "Lock.h"
#include <pthread.h>
#include <sys/time.h>
#include <iostream>
#include <stdio.h>

Lock::Lock(){
	//printf("lock created\n");
  lock = PTHREAD_MUTEX_INITIALIZER;
  status = PTHREAD_COND_INITIALIZER;
  //printf("Lock fully created\n");
}

Lock::~Lock(){
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&status);
}

void Lock::acquire(){
	//printf("entering acquire\n");
  pthread_mutex_lock(&lock);
  //printf("leaving acquire\n");
}

void Lock::release(){
	//printf("release entered\n");
  pthread_mutex_unlock(&lock);
  //printf("relese left\n");
}

void Lock::wait(){
	if (pthread_cond_wait(&status, &lock)!=0)
		throw new WaitFailException();
}

void Lock::wait(int time){
	//Pulls standard time from system and converts it into the structure required.
	struct timespec   ts;
	struct timeval    tp;
	gettimeofday(&tp, NULL);
	//ts.tv_sec  = tp.tv_sec;
	ts.tv_sec = tp.tv_sec+time;

	//Wait
	int wait_result(pthread_cond_timedwait(&status, &lock, &ts));
	if (wait_result != 0 && wait_result != ETIMEDOUT)
		throw new TimedWaitFailException();
}



void Lock::broadcast(){
	if (pthread_cond_broadcast(&status)!=0)
		throw new BroadcastFailException();
}
