#include "waveosc.hpp"
#include <math.h>

namespace SYNTHPI;
namespace audio;

WaveOSC::WaveOSC(const int samplerate){
    this-> mSamples = 0;
    this-> fm = 0;
}


void WaveOSC::setSemitone(int midinote){ //why does WaveOSC doesn"t show up in green ???
    fm = 2^((midinote-69)/12)*440;
}

void WaveOSC::getMixVal(int MIX_CC){

}


std::vector<sample_t> WaveOSC::getSamples(int nSamples) {
    mSamples = //some kind of combination of nSamples, sample rate, base waveform frequency and fm. 
    getMixVal();
}