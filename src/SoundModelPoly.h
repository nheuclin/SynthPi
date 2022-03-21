#include "SoundModel.h"
#include "defs.hpp"
#include <string>
#include <vector>
#include <list>
#include <queue>


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
		/*! a vector of soundmodelmono, ie a vector of the voices in synthpi */
		std::vector<SoundModel*> soundModelList;
		
		/*!  */
		int	soundModelNo;

		int	lastSoundModel;

		/*!  */
		int	soundModelNo;

		/*!  */
		int VoiceNo;

		/*!  */
		int position;

		/*!  */
		double master_vol=0.05;

		float target_vol=0.2; 

		/*!  a buffer to return the sum of voices to playback engine*/
		std::vector<sample_t> polybuffer;

	public:
		/**
		 * Create a SoundModelPoly with a given number of SoundModelMonos
		 * @param poly The numer of monophonic sound models to create
		 * @param samplerate Operating sample rate
		 * @param gain
		 * (optional) Multiply model outputs by this to yield the
		 * final result
		 */
		SoundModelPoly(const int poly, const int samplerate);

		/**
		 * Start a note playing. 
		 *
		 * This will try to prioritise SoundModels which are not
		 * playing over ones which are, to ensure that maximum
		 * polyphony is achieved.
		 *
		 * @param midinote Midi note number of note to turn on
		 */
		virtual void setNoteOn(int midinote) override;

		/**
		 * Stop a note playing
		 *
		 * This simply sends the same message to all contained
		 * SoundModels. Hence multiple contained models are playing
		 * the same note, the note will be disabled on all models.
		 *
		 * @param midinote Midi note number of note to turn off
		 */
		virtual void setNoteOff(int midinote) override;


		/**
		 * @return True if a note is currently being played by and of
		 * the contained models.
		 */
		virtual bool isPlaying() override;

		/*!
		 * Render a frame of samples of all contained models into a
		 * buffer.
         *\param nSamples number of samples to return.
         *\return a buffer of samples. 
		 */

		virtual std::vector<sample_t> getSamples(int nSamples) override;

		/** Thrown when empty SoundModel vector passed into constructor */
		class NoSoundModelException {};

    	virtual void updateVolume(unsigned int parameter) override;

		virtual void updateWavemix(unsigned int parameter) override;

		virtual void updateBank(unsigned int parameter) override;

		
};

} // namespace audio
} // namespace SYNTHPI

#endif /* SOUND_MODEL_POLY_H */
