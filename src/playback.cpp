#include "playback.hpp"
#include "math.h"
#include "SoundModel.h"

using namespace SYNTHPI;
using namespace audio;

    
PlaybackEngine::PlaybackEngine(SoundModelPoly &playout):
    playout(playout)
{}

std::vector<sample_t> PlaybackEngine::getSamples(int nSamples) {

    // Clear object buffer and set the size
    buffer.clear();
    buffer.resize(nSamples);

    //just call getSamples from the soundmodelpoly instance.
    buffer = playout.getSamples(nSamples);
    for (unsigned int i = 0; i<nSamples; i++){
        buffer[i]=buffer[i]*0.3;
    }   
    return buffer;
}
