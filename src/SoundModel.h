#ifndef SOUNDMODEL_H 
#define SOUNDMODEL_H

#include <vector>
#include <defs.hpp>

namespace SYNTHPI {
namespace audio {

/*! Interface for classes providing the ability to play notes. */
class SoundModel {

	public:

		/*! Set that a note should begin playing, may turn off currently playing notes if 
		maximum polyphony is exceeded
		\param semitone The semitone value of the activated key (0-127) */
		virtual void setNoteOn(int semitone) = 0;


		/*! Set that a note should stop playing. This will only have an effect 
		if the current note is playing.
		\param semitone The semitone value of the deactivated key (0-127) */
		virtual void setNoteOff(int semitone) = 0;


		/*! Return true if there is currently at least one note playing */
		virtual bool isPlaying() = 0;


        /*! Render a period of sound data and store in samples.
        \param nSamples number of samples to return.
        \return a buffer of samples. */
        virtual std::vector<sample_t> getSamples(int nSamples) = 0;


		/*! passes the value from the CC associated to the volume control
  		\param parameter the midi value which represents volume */ 
    	virtual void updateVolume(unsigned int parameter)= 0;


		/*! passes the value from the CC associated to the wavemix control
 		\param parameter the midi value which represents wavemix */
		virtual void updateWavemix(unsigned int parameter)= 0;


		/*! passes the value from the CC associated to the bank sekect control
  		\param parameter the midi value which represents the bank to play from */
		virtual void updateBank(unsigned int parameter)= 0;


		/*! passes the value from the CC associated to the attack time control
  		\param parameter the midi value which represents the attack time */ 
		virtual void updateAttack(unsigned int parameter)= 0;


		/*!	passes the value from the CC associated to the decay time control
  		\param parameter the midi value which represents the decay time */ 
		virtual void updateDecay(unsigned int parameter)= 0;


		/*! passes the value from the CC associated to the sustain level control
  		\param parameter the midi value which represents the sustain level */ 
		virtual void updateSustain(unsigned int parameter)= 0;


		/*! passes the value from the CC associated to the release time control
  		\param parameter the midi value which represents the release time */
		virtual void updateRelease(unsigned int parameter)= 0;


  		/*! passes the value from the CC associated to the filter cutoff frequency control
  		\param parameter the midi value which represents the filter cutoff frequency */ 
		virtual void updateCutoff(unsigned int parameter) =0;


		/*! passes the value from the CC associated to the filter resonance control
  		\param parameter the midi value which represents the amount of resonance to the filter */ 
  		virtual void updateRes(unsigned int parameter) =0;

		/*! passes the value from the CC associated to the slope select control
 		\param parameter the midi value which select the filter slope (12or24dB/octave) */ 
		virtual void updateSlope(unsigned int parameter) =0;
};

} // namespace audio
} // namespace SYNTHPI

#endif /* SOUNDMODEL_H */
