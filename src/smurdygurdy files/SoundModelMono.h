#include "Thread.h"
#include "StringAllocator.h"
#include "OutputAdaptor.h"
#include "SoundModel.h"
#include "Lock.h"
#include <string>

#ifndef SOUND_MODEL_MONO_H
#define SOUND_MODEL_MONO_H

/**
 * Responsible for creating the violin sound and providing the samples when requested
 *
 * This class is designed to be completely thread safe, and hence all method calls will
 * be serialised.
 */
class SoundModelMono : public SoundModel {

	StringAllocator violin; /* Physical model of violin (adapted) */
	bool noteOn;		/* True if a note is currently playing (not counting release) */
	int  currentNote;	/* Current note being played */
	int release;		/* Countdown for how long isPlaying should return true after note released */
	double pedalSpeed;	/* Current pedal speed (needs to be stored here) */

	Lock lock; /* Main lock for all methods */

	public:

		/**
		 * Default contstructor
		 *
		 * Note that this is not usually called by anything other than a factory.
		 * 
		 * @param sr Operating sample rate
		 */
		SoundModelMono(const int st); 

		/**
		 * @return true if note is currently on
		 */
		bool isPlaying();

		/**
		 * Store samples in provided buffer, and advanced time by the length of the buffer.
		 *
		 * @param samples Array to which samples should be written
		 * @param bufferSize length of array
		 */
		void getSamples(short samples[], int bufferSize);

		/**
 		 * Start playing a note. Will turn of currently playing notes.
 		 *
 		 * @param semitone Semitone value of note (0-127)
 		 */
		virtual void setNoteOn(int semitone);

		/**
 		 * Stop playing a note.
 		 *
 		 * The note will not necessarily end instantly, as there is a period where the sound decays after being stopped.
		 * This will have no effect if the sepcified note was note playing.
 		 *
 		 * @param semitone Semitone value of note (0-127)
 		 */
		virtual void setNoteOff(int semitone);

		/**
 		 * Set the pedal speed.
 		 *
 		 * @param speed Speed of the pedal in metres per second
 		 */
		virtual void setPedalSpeed(double speed);

		/**
		 * Return the current gain. For a mono sound model,
		 * there is no polyphonic mixing so the gain is always 1.0
		 *
		 */
		virtual double getOutputGain(void) { return 1.0; }
};

#endif /* SOUND_MODEL_H */
