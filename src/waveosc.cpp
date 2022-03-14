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
    //load 1st bank
}


WaveOSC::~WaveOSC(){}

std::vector<sample_t> WaveOSC::getSamples(int nSamples) {
    

    std::vector<sample_t> temp1(nSamples);
    std::vector<sample_t> temp2(nSamples);

    mixBuffer.clear();
    mixBuffer.resize(nSamples);

    //wavemix_val = getMixVal(); CCs not implemented yet
    Wave_index=static_cast<int> (wavemix_val);
    wave1_avg=wavemix_val-Wave_index;
    wave2_avg=1.0-wave1_avg;

    numberOfSamples1=sources[Wave_index] -> getNumSamples();
    numberOfSamples2=sources[Wave_index+1] -> getNumSamples();

    float nsamplef1=static_cast<float> (numberOfSamples1);
    float nsamplef2=static_cast<float> (numberOfSamples2);
    
    index_increment1=(nsamplef1/sampleratef)*frequency;
    index_increment2=(nsamplef2/sampleratef)*frequency;

    temp1=sources[Wave_index] -> getSamples(nSamples,index_increment1);
    temp2=sources[Wave_index+1] -> getSamples(nSamples,index_increment2);

    for (unsigned int i=0; i <nSamples; i++){
        mixBuffer[i]=  wave1_avg*temp1[i]+wave2_avg*temp2[i]; //add ADSR multiplication here
    }

    return mixBuffer;

}

void WaveOSC::trigAttack(){}

void WaveOSC::trigRelease(){}

int WaveOSC::getRelease(){}


void WaveOSC::setSemitone(int midinote) { 
    midinotef= static_cast<float> (midinote);
    frequency = 440.0*powf(2.0,((midinotef-69.0)/12.0)); //converts midinote to frequency
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