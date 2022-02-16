#include "SoundModel.h"
#include <string>
#include <vector>

#ifndef SOUND_MODEL_POLY_H
#define SOUND_MODEL_POLY_H

/**
 * Will play multiple notes at once. 
 *
 * This works by dividing the work amongst an array of SoundModel's which are passed in through the constructor.
 */
class SoundModelPoly : public SoundModel {

	protected:
		std::vector<SoundModel*> soundModelList;
		int	lastSoundModel;
		int	soundModelNo;

	public:
		/**
		 * Create a new instance of SoundModelPoly
		 *
		 * This takes in a vector of sound models. These will usually
		 * be SoundModelMono instances, to give the polyphonic
		 * effect. However, they could also be SoundModelPoly
		 * instances, which could be useful for allowing each instance
		 * to be rendered by a single thread, but having note and
		 * pedal data controlled from a central instance.
		 * 
		 * @param soundModelList
		 * Vector containing list of models to be contained within
		 * the instance.
		 * @param output_gain
		 * (optional) Multiply model outputs by this to yield the
		 * final result
		 */
		SoundModelPoly(std::vector<SoundModel*> soundModelList,
		               double output_gain=0.2
			      );

		/**
		 * Create a SoundModelPoly with a number of SoundModelMonos
		 *
		 * A less flexible but more convenient form of the constructor
		 * which creates a given number of monophonic sound sources
		 *
		 * @param poly The numer of monophonic sound models to create
		 * @param sr Operating sample rate
		 * @param output_gain
		 * (optional) Multiply model outputs by this to yield the
		 * final result
		 */
		SoundModelPoly(int poly, const int sr, double output_gain=0.2);

		/**
		 * Set the output gain
		 *
		 * The polyphonic model sums the output from a number of
		 * monophonic string models. The result is multiplied by
		 * the output gain. This exposes a method to control the
		 * gain which should be used, for example, to prevent clipping
		 * in the event that a larger number of strings is deployed.
		 *
		 * @param output_gain Value to set for the output gain
		 */
		virtual void setOutputGain(double output_gain);

		/**
		 * Get the output gain
		 */
		virtual double getOutputGain(void) { return gain; }
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
		 * buffer, and advanced time by the length of the buffer.
		 *
		 * @param samples Buffer where samples should be stored. Must have enough space for requested data.
		 * @param bufferSize Size in samples of the buffer
		 */
		virtual void getSamples(short samples[], int bufferSize);

		/** Thrown when empty SoundModel vector passed into constructor */
		class NoSoundModelException {};

};

#endif /* SOUND_MODEL_POLY_H */
