// File: playback.hpp
#ifndef SYNTHPI_PLAYBACK_H
#define SYNTHPI_PLAYBACK_H

#include <vector>
#include <memory>
#include <array>
#include "SoundModel.h"
#include "defs.hpp"
#include "audio.hpp"
#include "SoundModelPoly.h"

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
        PlaybackEngine(SoundModel &playout);

        /*! Retrieves samples.
        \param nSamples number of samples to return.
        \return a buffer of samples. */
        std::vector<sample_t> getSamples(int nSamples) override;

    
    private:

        SoundModelPoly &playout;
        /*! Buffer of samples to allow rapid transfer to Jack. */
        std::vector<sample_t> buffer;

};

} // namespace audio
} // namespace SYNTHPI

#endif // define SYNTHPI_PLAYBACK_H