#include "waveosc.hpp"
#include <math.h>
#include <vector>
#include "defs.hpp"
#include <iostream>
using namespace SYNTHPI;
using namespace audio;

WaveOSC::WaveOSC(){ 
    this-> frequency =0.;
    this-> wavemix_val = 0.;
    this-> sampleratef=static_cast<float> (samplerate);
    this-> numberSamples_bySR.resize(_NUM_WAVES);
}


WaveOSC::~WaveOSC(){}

std::vector<sample_t> WaveOSC::getSamples(int nSamples) {
    
    std::vector<std::vector<sample_t>> tempvector(_NUM_WAVES);
    std::vector<float> increments(_NUM_WAVES);

    mixBuffer.clear();
    mixBuffer.resize(nSamples);

    for (unsigned int i=0;i<_NUM_WAVES;i++){
        increments[i]=numberSamples_bySR[i]*frequency;
        tempvector[i].clear();
        tempvector[i].resize(nSamples);
        tempvector[i]=sources[i]->getSamples(nSamples,increments[i]);
    }
    
    float wavemix_step = (wavemix_val_target-wavemix_val)/nSamples; //step to deziper mix value

    for (unsigned int i=0; i <nSamples; i++){
        
        Wave_index=static_cast<int> (wavemix_val);
        
        wave1_avg=wavemix_val-static_cast<float>(Wave_index);
        wave2_avg=1.0-wave1_avg;

        Wave_index2=Wave_index+1;
        if (Wave_index==_NUM_WAVES-1){
            Wave_index2=0;
        }
        
        mixBuffer[i]= wave2_avg*tempvector[Wave_index][i]+wave1_avg*tempvector[Wave_index2][i];
        
        wavemix_val+=wavemix_step;
    }
    wavemix_val=wavemix_val_target;

    return mixBuffer;

}


void WaveOSC::setSemitone(int midinote) { 
    midinotef= static_cast<float> (midinote);
    frequency = 440.0*powf(2.0,((midinotef-69.0)/12.0)); //converts midinote to frequency
}

void WaveOSC::updateWavemix(unsigned int parameter){
    wavemix_val_target=(static_cast<float>(parameter)/127.0)*(static_cast<float>(_NUM_WAVES)-1.0);
}

sampleSourceStatus_t WaveOSC::loadBank(int bank, sampleSourceType_t type) {
    sampleSourceStatus_t Src_status, retStat;
    retStat = SOURCE_READY;
 
    for(int i = 0; i < _NUM_WAVES; i++) {
        Src_status = setSource((WaveID_t)i, bank, type);
        if (Src_status != SOURCE_READY) retStat = Src_status;
        if (Src_status == SOURCE_READY){
        }
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
            numberSamples_bySR[wave]=(static_cast<float>(sources[wave]->getNumSamples())/sampleratef);
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