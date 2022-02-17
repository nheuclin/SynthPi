// File: waveosc.hpp
#ifndef SYNTHPI_WAVEOSC_H
#define SYNTHPI_WAVEOSC_H

#include <vector>
#include <memory>
#include <array>

#include "defs.hpp"
#include "sampleSource.hpp"
#include "audioLibrary.hpp"

namespace SYNTHPI {
namespace audio {

/*! Sample handling class.
Retrieves Samples from the sample sources and mix/interpolate between them to 
return the number of samples requested at the right frequency !*/

class WaveOSC{
    /*! Number of samples to request from sample source to return buffer at the right frequency !*/
    int mSamples; 

    /*! frequency to play at from midinote !*/
    float fm; 

    /*! float value to know which waves to average !*/
    float wavemix_val;


    public:
        /*! Constructor. 
        \param samplerate  Operating sample rate */
        WaveOSC(const int samplerate);

        /*! sets osc frequency 
        \param midinote Semitone value to be played (0-127) */
        void setSemitone(int midinote);

        /*! get current mix value
        \param MIX_CC current value of mix CC control */
        void getMixVal(int MIX_CC);


        /*! Retrieves wave samples.
        \param nSamples number of samples to return.
        \return a buffer of samples. */
        std::vector<sample_t> getSamples(int nSamples) ; //override ? don't think it's needed

        /*! Loads a bank of waves of a homogenous \ref sampleSourceType_t.
        Equivalent to calling \ref setSource for each wave with the given arguments.
        \param bank ID of the bank of waves to load from.
        \param type \ref sampleSourceType_t of sources to load. */
        sampleSourceStatus_t loadBank(int bank, sampleSourceType_t type);

        /*! Sets the source for the specified wave.
        \param wave \ref WaveID_t of the drum to set the type for.
        \param bank ID of the bank of waves to load from.
        \param type \ref sampleSourceType_t of source to load. */
        sampleSourceStatus_t setSource(WaveID_t wave, int bank, sampleSourceType_t type);

        /*! Returns the source \ref sampleSourceStatus_t of the given wave.
        \return source status. */
        sampleSourceStatus_t getSourceStatus(drumID_t drum);

        /*! Returns the source \ref sampleSourceType_t for the given drum. 
        \return source type. */
        sampleSourceType_t getSourceType(drumID_t drum);

        /*! virtual function to trigger ADSR attack stage*/
        virtual void trigAttack() = 0;

        /*! virtual function to trigger ADSR release stage*/
        virtual void trigRelease() = 0;
    

    private:
        /*! Library manager for the audio sources. */
        AudioLibrary library;

        /*! Buffer of samples to allow transfer to SoundModelMono. */
        std::vector<sample_t> buffer;

        /*! \ref SampleSource object pointers. */
        std::array<std::unique_ptr<SampleSource>, NUM_WAVES> sources;
        /*! Switches to store whether each source is being played. */
        std::array<bool, NUM_WAVES> isActive;
};

} // namespace audio
} // namespace SYNTHPI

#endif // define SYNTHPI_WAVEOSC_H