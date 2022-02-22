// File: playback.hpp
#ifndef SYNTHPI_PLAYBACK_H
#define SYNTHPI_PLAYBACK_H

#include <vector>
#include <memory>
#include <array>

#include "defs.hpp"
#include "audio.hpp"
#include "sampleSource.hpp"
#include "audioLibrary.hpp"

namespace SYNTHPI {
namespace audio {

/*! Sample handling class.
Manages audio clips for sending to output.
An instance of this class is used as the callback class for the \ref JackClient. */
class PlaybackEngine : public AudioCallback {
    public:
        /*! Constructor.
        *\param playout (*) address of the soundmodelpoly to pass samples to jack
        */
        PlaybackEngine(SoundModel *playout); //pass adress to soundmodelpoly ?     

        /*! Retrieves samples.
        \param nSamples number of samples to return.
        \return a buffer of samples. */
        std::vector<sample_t> getSamples(int nSamples) override;



        /*! Increments the master output volume. */
        void volumeUp();


        /*! Decrements the master output volume. */
        void volumeDown();


        /*! Returns the current master volume as a percentage.
        \return current master volume. */
        int getVolume();
    
    private:
        /*! Library manager for the audio sources. */
        AudioLibrary library;

        /*! Buffer of samples to allow rapid transfer to Jack. */
        std::vector<sample_t> buffer;

        /*! \ref SampleSource object pointers. */
        std::array<std::unique_ptr<SampleSource>, NUM_DRUMS> sources;  //replace sources to pointer to soundmodelpoly ? 

        /*! Lookup table for exponential volume control.
        Indexed as a percentage. */
        std::array<float, 101> volumeTable;

        /*! Default master volume. */
        const int masterVolDef = 75;
        /*! Default drum volume. */
        const int volumeDef = 75;

        /*! Step size for volume increments and decrements. */
        const int volumeStep = 5;
};

} // namespace audio
} // namespace SYNTHPI

#endif // define SYNTHPI_PLAYBACK_H