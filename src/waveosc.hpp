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

        /*! function to trigger ADSR attack stage*/
        void trigAttack();

        /*!function to trigger ADSR release stage*/
        void trigRelease();

        /*!function to return the number of samples it will take for the ADSR (depedending 
        on the sample rate) to get back to 0 once the release stage is triggered 
        \return ReleaseSampleNo the number of samples it'll take for the ADSR to get back to 0 */
        int getRelease();

    

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

        int Wave_index=0;

        int Wave_index2=0;

        float wave1_avg=0.5;
        
        float wave2_avg=0.5;
        
        float sampleratef;
        
        float midinotef;
        
        std::vector<float> numberSamples_bySR;

        /*! \ref SampleSource object pointers. */
        std::array<std::unique_ptr<SampleSource>, _NUM_WAVES> sources;

}; //WaveOSC class

} // namespace audio
} // namespace SYNTHPI

#endif // define WAVEOSC_H