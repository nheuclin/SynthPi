#include "Thread.h"
#include "waveosc.hpp"
//#include "OutputAdaptor.h"
#include "SoundModel.h"
#include "defs.hpp"
#include "Lock.h"
#include <string>

#ifndef SOUND_MODEL_MONO_H
#define SOUND_MODEL_MONO_H

namespace SYNTHPI {
namespace audio {

/**
 * Responsible for creating the a monophonic wavetable voice with its own ADSR 
 * and providing the samples when requested
 * This class is designed to be completely thread safe, and hence all method calls will
 * be serialised.
 */
class SoundModelMono : public SoundModel {

	WaveOSC waveosc; /* instance of a wave oscillator */
	bool noteOn;		/* True if a note is currently playing (not counting release) */
	int  currentNote;	/* Current note being played */
	int release;		/* Countdown for how long isPlaying should return true after note released (number of samples it'll take the adsr to get back to 0 based on sampling rate) */
	
	Lock lock; /* Main lock for all methods */

	public:

		/**
		 * Default contstructor
		 *
		 * Note that this is not usually called by anything other than a factory.
		 * 
		 * @param samplerate Operating sample rate
		 */
		SoundModelMono(const int samplerate); 

		/**
		 * @return true if note is currently on
		 */
		bool isPlaying() override;

		/**
		 * Store samples in provided buffer.
		 *
		 * @param nSamples number of samples to retrieve
		 * @return buffer of samples
		 */
		virtual std::vector<sample_t> getSamples(int nSamples) override; //override? 


		/**
 		 * Start playing a note. Will turn of currently playing notes.
 		 *
 		 * @param semitone Semitone value of note (0-127)
 		 */
		virtual void setNoteOn(int semitone) override; 
		/**
 		 * Stop playing a note.
 		 *
 		 * The note will not necessarily end instantly, as there is a period where the sound decays after being stopped.
		 * This will have no effect if the sepcified note was note playing.
 		 *
 		 * @param semitone Semitone value of note (0-127)
 		 */
		virtual void setNoteOff(int semitone) override; //override? 

};

} // namespace audio
} // namespace SYNTHPI

#endif /* SOUND_MODEL_MONO_H */
