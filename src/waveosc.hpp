// File: waveosc.hpp
#ifndef WAVEOSC_H
#define WAVEOSC_H

#include <vector>
#include <memory>
#include <array>
#include <math.h>
#include <iostream>

#include "defs.hpp"
#include "sampleSource.hpp"
#include "audioLibrary.hpp"

namespace SYNTHPI {
namespace audio {

/*! Sample handling class.
Retrieves Samples from the sample sources and mix/interpolate between them to 
return the number of samples requested at the right frequency !*/

class WaveOSC {

    public:
        /*! Constructor. */
        WaveOSC();

        /*! Destructor. */
        ~WaveOSC();

        /*! Retrieves wave samples.
        \param nSamples number of samples to return.
        \return a buffer of samples. */
        virtual std::vector<sample_t> getSamples(int nSamples); //override ? don't think it's needed

        /*! sets osc frequency 
        \param midinote Semitone value to be played (0-127) */
        void setSemitone(int midinote);

        /*! updates get current mix value
        \param parameter current value of mix CC control */
        void updateWavemix(unsigned int parameter);

        /*! Loads a bank of waves of a homogenous \ref sampleSourceType_t.
        Equivalent to calling \ref setSource for each wave with the given arguments.
        \param bank ID of the bank of waves to load from.
        \param type \ref sampleSourceType_t of sources to load. */
        sampleSourceStatus_t loadBank(int bank, sampleSourceType_t type);

        /*! Sets the source for the specified wave.
        \param wave \ref WaveID_t of the wave to set the type for.
        \param bank ID of the bank of waves to load from.
        \param type \ref sampleSourceType_t of source to load. */
        sampleSourceStatus_t setSource(WaveID_t wave, int bank, sampleSourceType_t type);

        /*! Returns the source \ref sampleSourceStatus_t of the given wave.
        \return source status. */
        sampleSourceStatus_t getSourceStatus(WaveID_t wave);

        /*! Returns the source \ref sampleSourceType_t for the given drum. 
        \return source type. */
        sampleSourceType_t getSourceType(WaveID_t wave);
    

    private:
        /*! Library manager for the audio sources. */
        AudioLibrary library;

        /*! Buffer of samples to allow transfer to SoundModelMono. */
        std::vector<sample_t> mixBuffer;

        /*! The number of Samples in the first Wave buffer */
        int numberOfSamples1;

        /*! The number of Samples in the second Wave buffer */
        int numberOfSamples2;

        /*! frequency to play at from midinote !*/
        float frequency; 

        /*! float value to know which waves to average !*/
        float wavemix_val=0.0;

        /*!float to dezipper wave mixing !*/
        float wavemix_val_target=0.0; 

        /*! the wave index of the first wave to interpolate from, this value might change on a per sample basis*/
        int Wave_index=0;

        /*! the wave index of the second wave to interpolate from, this value might change on a per sample basis*/
        int Wave_index2=0;

        /*! the value to average the first wave by , this value might change on a per sample basis*/
        float wave1_avg=0.5;
        
        /*! the value to average the second wave by , this value might change on a per sample basis*/
        float wave2_avg=0.5;
        
        /*! a static cast of the operating samplerate to a float value*/
        float sampleratef;

        /*! a static cast of the midi note to convert to frequency to a float value */
        float midinotef;
        

        /*! a list that holds the number of sample in each wave buffer to calculate the frequency they've been recorded at */
        std::vector<float> numberSamples_bySR;

        /*! \ref SampleSource object pointers. */
        std::array<std::unique_ptr<SampleSource>, _NUM_WAVES> sources;

}; //WaveOSC class

} // namespace audio
} // namespace SYNTHPI

#endif // define WAVEOSC_H