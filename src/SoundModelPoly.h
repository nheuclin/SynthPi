#include "SoundModel.h"
#include <defs.hpp>
#include <string>
#include <vector>
#include <list>
#include <queue>

#include <filter_includes.h>
#include <filter_common.h>

#ifndef SOUND_MODEL_POLY_H
#define SOUND_MODEL_POLY_H

namespace SYNTHPI {
namespace audio {
/**
 * Will play multiple notes at once. 
 *
 * This works by dividing the work amongst an array of SoundModel's which are passed in through the constructor.
 */
class SoundModelPoly : public SoundModel {

	private:

		/*! first instance of a 12dB/oct lowpass filter */
		SO_LPF filter12;

		/*! second instance of a 12dB/oct lowpass filter */
		SO_LPF filter24;
		
		/*! float to represent the resonance amount value */
		float Q=0.707;

		/*! float to represent the filters cutoff frequency */
		float Fc=20000;

		/*! float to hold the target resonance amount value in order to dezipper the midi values */
		float target_Q=0.707;

		/*! float to hold the target filters cutoff frequency in order to dezipper the midi values */
		float target_Fc=20000;
		
		/*! a vector of soundmodelmono, ie a vector of the voices in synthpi */
		std::vector<SoundModel*> soundModelList;

		/*! a vector of int which keeps track of the order the voices are being turned on*/
		std::vector<int> lastSoundModel;
		
		/*! same as std::vector<int> lastSoundModel but keeps track of the midinotes  */
		std::vector<int> midiNoteList;

		/*! unused */
		//intsoundModelNo;

		/*! the number of voices in the synthpi */
		int VoiceNo;

		/*! the position in the lastSoundModel list to erase when a voice gets overriden */
		int position;

		/*! the synthpi master volume */
		float master_vol=0.05;


		/*! float to hold the target master value in order to dezipper the midi values */
		float target_vol=0.2; 

		/*!  a buffer to return the sum of voices to playback engine*/
		std::vector<sample_t> polybuffer;

		/*! a bool to store whether the filter slope should be 12 or 24 dB/oct */
		bool slope;

	public:
		/*! Create a SoundModelPoly with a given number of SoundModelMonos
		 \param poly The numer of monophonic sound models to create
		 \param samplerate Operating sample rate */
		SoundModelPoly(const int poly, const int samplerate);

		/*! Start a note playing. 
		   This will try to prioritise SoundModels which are not
		   playing over ones which are, to ensure that maximum
		   polyphony is achieved.
		   \param midinote Midi note number of note to turn on */
		virtual void setNoteOn(int midinote) override;

		/*! Stop a note playing
		   This simply sends the same message to all contained
		   SoundModels. Hence multiple contained models are playing
		   the same note, the note will be disabled on all models.
		   \param midinote Midi note number of note to turn off */
		virtual void setNoteOff(int midinote) override;


		/*! function to check which voices are playing
		 \return True if a note is currently being played by and of
		 the contained models. */
		virtual bool isPlaying() override;

		/*! Render a frame of samples of all contained models into a buffer.
        \param nSamples number of samples to return.
        \return a buffer of samples. */
		virtual std::vector<sample_t> getSamples(int nSamples) override;

		/*! Thrown when empty SoundModel vector passed into constructor */
		class NoSoundModelException {};

		/*! update the synthpi master volume */
    	virtual void updateVolume(unsigned int parameter) override;


		/*! passes the wavemix values to all soundmodelmono instances */
		virtual void updateWavemix(unsigned int parameter) override;


		/* calls soundmodelmono updateBank for all voices */
		virtual void updateBank(unsigned int parameter) override;


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

		/*! update the filters cutoff frequency */
		virtual void updateCutoff(unsigned int parameter) override;

		/*! update the filters resonance amounts */
  		virtual void updateRes(unsigned int parameter) override;

		/*! update the filter slope */
		virtual void updateSlope(unsigned int parameter) override;

		
};

} // namespace audio
} // namespace SYNTHPI

#endif /* SOUND_MODEL_POLY_H */


 



