#include <pthread.h>
#include "Lock.h"

#ifndef THREAD_H
#define THREAD_H

/**
 * A thread allows multiple threads of execution within the application
 *
 * The thread is used by creating a subclass and overriding the run() method. The thread can then be started executing via the start() method, which will execute the run() method concurrently. The thread will continue runing until either the run method completes, or until the application is terminated.
 * There will be no guarentee to determine in which order statements in different threads will be executed relative to eachother. For this reason threads should probably utilise Locks or mutexes to ensure there are no data conflicts.
 *
 */
class Thread {

	pthread_t pthreadHandle;	// The real thread. Only gets instanciated by start()
	bool	  running;		// Will be set to True while the thread is running
	Lock	  runningLock;		// Allows some syncrhonization with the running boolean

	/*
	 * This gets called by pthread_create and will allow us to execute the run() method.
	 */
	static void * threadCallback( void * thread );

	/*
	 * Synchronised method to set the running status of the thread. 
	 */
	void setRunning( bool running );

	public:

		/**
 	  	 * Allocates a new Thread object
		 */
		Thread();

		/**
		 * Called when the object is to be destroyed.
		 */
		~Thread();

		/**
		 * Contains the logic to be run in the new thread, started by invoking the start() method.
 	  	 *
		 * This method will be run in the new thread. It should be
		 * overridden by a subclass of Thread. When invoked by
		 * start(), the run() methods of multiple threads will execute
		 * concurrently.  There will be no guarentee to determine in
		 * which order statements in different threads will be
		 * executed relative to eachother. For this reason threads
		 * should probably utilise Semaphore or mutex's to ensure
		 * there are no data conflicts.
		 *
		 * Note: When this method is overridden it should be defined to be virtual in the class definition
		 */
		virtual void run();

		/**
 		 * Begin execution of the thread
		 *
		 * The effect of this method is to begin execution of the run() method
 		 *
 		 * @throws ThreadCreateFailedException This happens if the thread cannot be started
 		 */
		void start();
	
		/**
		 * Wait for the thread to terminate.
 	  	 *
		 * This method is usually only called if the thread is going
		 * to terminate in the near future. It can be useful in
		 * situations where the thread must have finished executing
		 * before any further progress can be made by the caller. If
		 * the thread never terminates, this method will wait
		 * indefinately.
		 */
		void join();

		/**
		 * Return true if thread is running
		 *
		 * This will be false if the thread hasn't started, or if it has completed execution.
		 *
		 * @return True if thread is running
		 */
		bool isRunning();

		/**
		 * Sleep for a limited amount of time
		 *
		 * This will cause whichever thread of execution calls this
		 * method to wait for a certain number of milliseconds. This
		 * can be useful for performing actions periodically or for
		 * having a time delay in the application.
		 *
		 * @param time The number of milliseconds to sleep 
		 */
		static void sleep(int time);

		/* Exceptions */

		/** Thrown if call to pthread_create fails, usually if there is no memory to allocate the thread */
		class ThreadCreateFailedException {};

};

#endif /* THREAD_H */
