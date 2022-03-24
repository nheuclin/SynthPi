#include "Thread.h"
#include "waveosc.hpp"
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

/*! Responsible for creating the a monophonic wavetable voice with its own ADSR 
    and providing the samples when requested
    This class is designed to be completely thread safe, and hence all method calls will
    be serialised. */
class SoundModelMono : public SoundModel {

	protected:

		/*! instance of an ADSR envelope for shaping the amplitude of the sound */
		ADSR myadsr;

		/* instance of a wave oscillator */
		WaveOSC waveosc;    

		/* True if a note is currently playing (not counting release) */
		bool noteOn;		
		
		/* Current note being played */
		int  currentNote;	
		
		/*! bool to check if the ADSR is in release state */
		bool release=false;	
		
		/*! int to store which bank was last succesfully loaded as a safeguard */
		int safeBank=1;
		
		/*! static cast of the operating samplerate to float */
		float sampleratef;

		/* Main lock for all methods */
		Lock lock; 

	public:

		/*! Default contstructor
		Note that this is not usually called by anything other than a factory. */
		SoundModelMono(); 

		/*! function to check if this voice is playing
		\return true if note is currently on */
		bool isPlaying() override;

		/*! Store samples in provided buffer.
		\param nSamples number of samples to retrieve
		\return buffer of samples */
		virtual std::vector<sample_t> getSamples(int nSamples) override; //override? 


		/*! Start playing a note. Will turn of currently playing notes.
 		\param semitone Semitone value of note (0-127) */
		virtual void setNoteOn(int semitone) override; 
		
		/*!
 		 Stop playing a note.
 		 The note will not necessarily end instantly, as there is a period where the sound decays after being stopped.
		 This will have no effect if the sepcified note was note playing.
 		 \param semitone Semitone value of note (0-127) */
		virtual void setNoteOff(int semitone) override;  


		/*! updates wavemix value for wave interpolation
		\param parameter the midi value which represents the wavemix ratio */
		virtual void updateWavemix(unsigned int parameter) override;


		/*! update associated waveosc banks 
		\param parameter the midi value which represents the bank to play from */
		virtual void updateBank(int parameter) override;


		/*! passes the value from the CC associated to the attack time control
  		\param parameter the midi value which represents the attack time */ 
		virtual void updateAttack(unsigned int parameter) override;


		/*!	passes the value from the CC associated to the decay time control
  		\param parameter the midi value which represents the decay time */ 
		virtual void updateDecay(unsigned int parameter) override;


		/*! passes the value from the CC associated to the sustain level control
  		\param parameter the midi value which represents the sustain level */ 
		virtual void updateSustain(unsigned int parameter) override;


		/*! passes the value from the CC associated to the release time control
  		\param parameter the midi value which represents the release time */
		virtual void updateRelease(unsigned int parameter) override;


		/*! this function is unused */ 
    	virtual void updateVolume(unsigned int parameter) override;


		/*! this function is unused */ 
		virtual void updateCutoff(unsigned int parameter) override;


		/*! this function is unused */ 
  		virtual void updateRes(unsigned int parameter) override;


		/*! this function is unused */
		virtual void updateSlope(unsigned int parameter) override;
};

} // namespace audio
} // namespace SYNTHPI

#endif /* SOUND_MODEL_MONO_H */
