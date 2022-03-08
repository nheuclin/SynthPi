#include "waveosc.hpp"
#include <math.h>
#include <vector>
//#include "defs.hpp"

using namespace SYNTHPI;
using namespace audio;

WaveOSC::WaveOSC(){ 
    frequency =0.;
    wavemix_val = 0.;
    //load 1st bank
}


WaveOSC::~WaveOSC(){

}

std::vector<sample_t> WaveOSC::getSamples(int nSamples) {
    
    std::vector<sample_t> temp1(nSamples);
    std::vector<sample_t> temp2(nSamples);
    double index_increment1;
    double index_increment2;
    mixBuffer.clear();
    mixBuffer.resize(nSamples);

    //wavemix_val = getMixVal(); CCs not implemented yet
    Wave_index=static_cast<int> (wavemix_val);
    wave1_avg=wavemix_val-Wave_index;
    wave2_avg=1.f-wave1_avg;

    numberOfSamples1=sources[Wave_index] -> getNumSamples();
    numberOfSamples2=sources[Wave_index+1] -> getNumSamples();

    index_increment1= (samplerate/numberOfSamples1)*frequency;
    index_increment2= (samplerate/numberOfSamples2)*frequency;

    temp1=sources[Wave_index] -> getSamples(nSamples,index_increment1);
    temp2=sources[Wave_index+1] -> getSamples(nSamples,index_increment2);

    for (unsigned int i=0; i <nSamples; i++){
        mixBuffer[i]=  wave1_avg*temp1[i]+wave2_avg*temp2[i]; //add ADSR multiplication here
    }

    return mixBuffer;

}

void WaveOSC::setSemitone(int midinote) { 
    frequency = powf(2.f, (((midinote - 69.f) / 12.f ) * 440.f) );
    //set lowpass filter coeff based on frequency
}

float WaveOSC::getMixVal(int MIX_CC){
    return wavemix_val;
}

sampleSourceStatus_t WaveOSC::loadBank(int bank, sampleSourceType_t type) {
    sampleSourceStatus_t Src_status, retStat;
    retStat = SOURCE_READY;

    for(int i = 0; i < _NUM_WAVES; i++) {
        Src_status = setSource((WaveID_t)i, bank, type);
        if (Src_status != SOURCE_READY) retStat = Src_status;
    }

    return retStat;
}

sampleSourceStatus_t WaveOSC::setSource(WaveID_t wave, int bank, sampleSourceType_t type) {
    sampleSourceStatus_t Src_status;

    switch (type) {
        case SOURCE_GENERALISED:
        default:
            Src_status = SOURCE_ERROR;
            break;
        
        case SOURCE_PREGENERATED:
            sources[wave].reset(new AudioClip(library.getFilepath(wave, bank, type)));
            Src_status = sources[wave]->getStatus();
            break;
    }

    return Src_status;
}

sampleSourceStatus_t WaveOSC::getSourceStatus(WaveID_t wave) {
    return sources[wave]->getStatus();
}

sampleSourceType_t WaveOSC::getSourceType(WaveID_t wave) {
    return sources[wave]->getType();
}