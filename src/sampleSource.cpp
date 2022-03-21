#include "sampleSource.hpp"

#include <AudioFile.h>

using namespace SYNTHPI;
using namespace audio;

SampleSource::SampleSource() {
    Src_status = SOURCE_ERROR;
    type = SOURCE_GENERALISED;
}

sampleSourceStatus_t SampleSource::getStatus() {
    return Src_status;
}

sampleSourceType_t SampleSource::getType() {
    return type;
}


// class AudioClip

AudioClip::AudioClip(std::string filepath) {
    type = SOURCE_PREGENERATED;
    loadFile(filepath);

    numSamples = clip.size();
   // std::cout<<"samples in clip"<< numSamples<<std::endl;

    reset();
}

std::vector<sample_t> AudioClip::getSamples(int nSamples, double index_increment) {
    
    std::vector<sample_t> b(nSamples);
    Src_status == SOURCE_ACTIVE;

    for (unsigned int i = 0; i < nSamples; i++) {
        if (playhead > numSamples-1) { // if playhead overran the wav file samples then loop back to beginning 
            //std::cout << "playhead overrun if statement" << std::endl;
            playhead =  playhead-static_cast<float>(numSamples-1); //accounting that we might not want to start back at 0 exactly

        }

        int_playhead=static_cast<int> (playhead);
        interpolation_val2= playhead - int_playhead;
        interpolation_val1= 1.0 - interpolation_val2;

        

        // Copy samples from clip 
        if (int_playhead+1==numSamples){
            b[i] = (interpolation_val1*clip[int_playhead]+ interpolation_val2*clip[0]); 
        }
        else {
            b[i] = (interpolation_val1*clip[int_playhead]+ interpolation_val2*clip[int_playhead+1]); //1st order interpolation
        }
        playhead= playhead+index_increment;
        //std::cout << b[i];

    }
    //std::cout << "one samplesource call" <<std::endl;
    return b;

}

void AudioClip::reset() {
    playhead = 0.0;
    updateStatus();
}

void AudioClip::updateStatus() {
    if (clip.empty()) {
        Src_status = SOURCE_ERROR;
        return;
    }

    if (playhead == 0) {
        Src_status = SOURCE_READY;
    } else if (playhead >= 0 && playhead < numSamples) {
        Src_status = SOURCE_ACTIVE;
    } else {
        Src_status = SOURCE_ERROR;
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

    Src_status = SOURCE_LOADING;

    file.shouldLogErrorsToConsole(false);
    loaded = file.load(this->filepath);

    if (!loaded) {
        Src_status = SOURCE_ERROR;
        return;
    }

    clip = file.samples[0];
    numSamples = clip.size();
    Src_status = SOURCE_READY;
}

int AudioClip::getNumSamples(){
    int numberSamples = clip.size();
    return numberSamples;
}