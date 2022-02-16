#include "sampleSource.hpp"

#include <AudioFile.h>

using namespace SYNTHPI;
using namespace audio;

SampleSource::SampleSource() {
    status = SOURCE_ERROR;
    type = SOURCE_GENERALISED;
}

sampleSourceStatus_t SampleSource::getStatus() {
    return status;
}

sampleSourceType_t SampleSource::getType() {
    return type;
}


// class AudioClip

AudioClip::AudioClip(std::string filepath) {
    type = SOURCE_PREGENERATED;
    loadFile(filepath);

    numSamples = clip.size();

    reset();
}

std::vector<sample_t> AudioClip::getSamples(int nSamples) {
    std::vector<sample_t> b(nSamples);

    status = SOURCE_ACTIVE;

    for (int i = 0; i < nSamples; i++) {
        if (playhead >= numSamples) { // if playhead overran the wav file samples then loop back to beginning
        playhead = 0;
        }
        // Copy samples from clip
        b[i] = clip[playhead];
        playhead++;
    }
    return b;
}

void AudioClip::reset() {
    playhead = 0;
    updateStatus();
}

void AudioClip::updateStatus() {
    if (clip.empty()) {
        status = SOURCE_ERROR;
        return;
    }

    if (playhead == 0) {
        status = SOURCE_READY;
    } else if (playhead >= 0 && playhead < nSamples) {
        status = SOURCE_ACTIVE;
    } else if (playhead >= numSamples) {
        status = SOURCE_FINISHED;
    } else {
        status = SOURCE_ERROR;
    }
}

void AudioClip::hardReset() {
    loadFile(filepath);
    reset();
}

void AudioClip::loadFile(std::string filepath) {
    AudioFile<sample_t> file;
    bool loaded;
    
    this->filepath = filepath;

    status = SOURCE_LOADING;

    file.shouldLogErrorsToConsole(false);
    loaded = file.load(this->filepath);

    if (!loaded) {
        status = SOURCE_ERROR;
        return;
    }

    clip = file.samples[0];
    numSamples = clip.size();
    status = SOURCE_READY;
}