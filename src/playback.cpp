#include "playback.hpp"
#include "math.h"
#include "SoundModel.h"

using namespace SYNTHPI;
using namespace audio;

    
PlaybackEngine::PlaybackEngine(SoundModelPoly &playout):
    playout(playout)
{}

std::vector<sample_t> PlaybackEngine::getSamples(int nSamples) {
    // Create temporary buffer for source returns
    std::vector<sample_t> temp(nSamples);

    // Clear object buffer and set the size
    buffer.clear();
    buffer.resize(nSamples);

    //just call getSamples from the soundmodelpoly instance.
    buffer = playout.getSamples(nSamples);
    return buffer;
}
