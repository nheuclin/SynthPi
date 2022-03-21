#include <Thread.h>
#include <waveosc.hpp>
#include "SoundModel.h"
#include <defs.hpp>
#include <Lock.h>
#include <string>
#include <math.h>
#include <ADSR.h>

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

	protected:
		ADSR myadsr;
		WaveOSC waveosc;    /* instance of a wave oscillator */
		bool noteOn;		/* True if a note is currently playing (not counting release) */
		int  currentNote;	/* Current note being played */
		bool release=false;		/* bool to check if isPlaying should return true even if the note is off */
		int safeBank=1;
		float sampleratef;
		Lock lock; /* Main lock for all methods */
		float sampleratef;

	public:

		/**
		 * Default contstructor
		 *
		 * Note that this is not usually called by anything other than a factory.
		 */
		SoundModelMono(); 

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

    	virtual void updateVolume(unsigned int parameter) override;

		virtual void updateWavemix(unsigned int parameter) override;

		virtual void updateBank(unsigned int parameter) override;
		
		virtual void updateAttack(unsigned int parameter) override;

		virtual void updateDecay(unsigned int parameter) override;

		virtual void updateSustain(unsigned int parameter) override;

		virtual void updateRelease(unsigned int parameter) override;

		virtual void updateCutoff(unsigned int parameter) override;

  		virtual void updateRes(unsigned int parameter) override;

};

} // namespace audio
} // namespace SYNTHPI

#endif /* SOUND_MODEL_MONO_H */
