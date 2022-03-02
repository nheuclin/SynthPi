#ifndef LOCK_H
#define LOCK_H

#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>


/**
* Adapter for handling a lock.
*
* Provides easy mutex and condition variable initialisation.
*/
class Lock {

	//Lock and condition variable to which interface is provided.
	pthread_mutex_t lock;
	pthread_cond_t status;

	public:

	/**
	 * Default constructor.
	 */
	Lock();

	/**
	 *Default destructor.
	 */
	~Lock();

	/**
	 *Acquire a lock on mutex, waiting if necessary.
	 */
	void acquire();

	/**
	 *Unlock mutex, allowing it to be locked.
	 */
	void release();

	/**
	 *Wait until alerted by a broadcast.
	 *
	 *Used in the case that a synchronised variable has not yet received the data required by a thread.  That thread will then wait until informed by a broadcast.
	 */
	void wait();

	/**
	 *Wait for a specified time.
	 *
	 *@param time time to wait in milliseconds.
	 */
	void wait(int time);

	/**
	 *Broadcast that synchronised variable now has data.
	 *
	 *Alerts all waiting threads that data is now available to be read, removing them from the wait state.
	 */
	void broadcast();


};

/*
 *List of Exceptions
 */

/**
 *Thrown when a mutex is not destroyed successfully.
 */
class MutexDestructionException{};

/**
 *Thrown when a condition variable is not destroyed successfully.
 */
class ConditionVariableDestructionException{};

/**
 *Thrown when a lock acquire fails.
 */
class LockAcquireException{};

/**
 *Thrown when a lock release fails.
 */
class LockReleaseException{};

/**
 *Thrown when an attempt to wait fails.
 */
class WaitFailException{};

/**
 *Thrown when an attempt to timed wait fails.
 */
class TimedWaitFailException{};

/**
 *Thrown when an attempt to broadcast fails.
 */
class BroadcastFailException{};

#endif // LOCK_H




