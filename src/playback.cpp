#include "playback.hpp"
#include "math.h"
#include "SoundModel.h"

using namespace SYNTHPI;
using namespace audio;

    
PlaybackEngine::PlaybackEngine(SoundModel *playout) {}

std::vector<sample_t> PlaybackEngine::getSamples(int nSamples) {
    // Create temporary buffer for source returns
    std::vector<sample_t> temp(nSamples);

    // Clear object buffer and set the size
    buffer.clear();
    buffer.resize(nSamples);

    //just call getSamples from the soundmodelpoly instance.
    
    return buffer;
}



sampleSourceStatus_t PlaybackEngine::loadBank(int bank, sampleSourceType_t type) {
    sampleSourceStatus_t status, retStat;
    retStat = SOURCE_READY;

    for(int i = 0; i < NUM_DRUMS; i++) {
        status = setSource((drumID_t)i, bank, type);
        if (status != SOURCE_READY) retStat = status;
    }

    return retStat;
}

sampleSourceStatus_t PlaybackEngine::setSource(drumID_t drum, int bank, sampleSourceType_t type) {
    sampleSourceStatus_t status;

    switch (type) {
        case SOURCE_GENERALISED:
        default:
            status = SOURCE_ERROR;
            break;
        
        case SOURCE_PREGENERATED:
            sources[drum].reset(new AudioClip(library.getFilepath(drum, bank, type)));
            status = sources[drum]->getStatus();
            break;
    }

    return status;
}

sampleSourceStatus_t PlaybackEngine::getSourceStatus(drumID_t drum) {
    return sources[drum]->getStatus();
}

sampleSourceType_t PlaybackEngine::getSourceType(drumID_t drum) {
    return sources[drum]->getType();
}