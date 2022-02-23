#include "waveosc.hpp"
#include <math.h>

namespace SYNTHPI;
namespace audio;

WaveOSC::WaveOSC(const int samplerate){
this -> mSamples=0;
this -> frequency =0.;
}


void WaveOSC::setSemitone(){}

//why does WaveOSC doesn't show up in green 
void WaveOSC::setSemitone(int midinote){ 
    frequency = powf(2.f, (((midinote - 69.f) / 12.f ) * 440.f) );
}

void WaveOSC::getMixVal(int MIX_CC){

}


std::vector<sample_t> WaveOSC::getSamples(int nSamples) {
    mSamples =  //some kind of combination of nSamples, sample rate, base waveform frequency and fm. 
    getMixVal();
}