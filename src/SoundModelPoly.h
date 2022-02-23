#include "SoundModel.h"
#include <string>
#include <vector>
#include <list>
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

	protected:
		std::vector<SoundModel*> soundModelList;
		std::vector<int> lastSoundModel;
		std::vector<int> midiNoteList;
		int	soundModelNo;
		int VoiceNo;
		int position;
		float master_vol; //should it be public since it is changed by the controller ? 
		std::vector<sample_t> buffer;

	public:
		/**
		 * Create a SoundModelPoly with a given number of SoundModelMonos
		 * @param poly The numer of monophonic sound models to create
		 * @param samplerate Operating sample rate
		 * @param output_gain
		 * (optional) Multiply model outputs by this to yield the
		 * final result
		 */
		SoundModelPoly(int poly, const int samplerate, double output_gain=0.2);

		/**
		 * Start a note playing. 
		 *
		 * This will try to prioritise SoundModels which are not
		 * playing over ones which are, to ensure that maximum
		 * polyphony is achieved.
		 *
		 * @param midinote Midi note number of note to turn on
		 */
		virtual void setNoteOn(int midinote);

		/**
		 * Stop a note playing
		 *
		 * This simply sends the same message to all contained
		 * SoundModels. Hence multiple contained models are playing
		 * the same note, the note will be disabled on all models.
		 *
		 * @param midinote Midi note number of note to turn off
		 */
		virtual void setNoteOff(int midinote);

		/**
		 * Set the pedal speed of all contained sound models
		 *
		 * @param speed Pedal speed in m/s
		 */
		virtual void setPedalSpeed(double speed);

		/**
		 * @return True if a note is currently being played by and of
		 * the contained models.
		 */
		virtual bool isPlaying();

		/**
		 * Render a frame of samples of all contained models into a
		 * buffer.
         *\param nSamples number of samples to return.
         *\return a buffer of samples. 
		 */
		virtual void std::vector<sample_t> getSamples(int nSamples);

		/** Thrown when empty SoundModel vector passed into constructor */
		class NoSoundModelException {};

};

} // namespace audio
} // namespace SYNTHPI

#endif /* SOUND_MODEL_POLY_H */
