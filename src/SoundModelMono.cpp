//SoundModelMono.cpp

#include "SoundModelMono.h"
#include "Lock.h"
#include <string>
#include <iostream> 

using namespace SYNTHPI;
using namespace audio;

//#define RELEASE_INIT (1024*1600) //? maybe just put that to 0.

SoundModelMono::SoundModelMono() {
  	this-> sampleratef=static_cast<float>(samplerate);
	this-> waveosc.loadBank(1, SOURCE_PREGENERATED);
	this->noteOn = false;
	this->currentNote = -1;
	myadsr.reset();
}

std::vector<sample_t> SoundModelMono::getSamples(int nSamples) {
	
	std::vector<sample_t> buffer(nSamples);
	std::vector<sample_t> adsrbuffer(nSamples);
	std::vector<sample_t> OSCbuffer(nSamples);
	lock.acquire();
	release=myadsr.inRelease();

	/* Allows us to render sound when in release or on state */
	if(noteOn==true || release==true) {
		OSCbuffer= waveosc.getSamples(nSamples);
		adsrbuffer=myadsr.getSamples(nSamples);
		
		for (unsigned int i =0;i<nSamples;i++){
			buffer[i]=OSCbuffer[i]*adsrbuffer[i];
		}
	}else{		
		for (unsigned int i = 0; i<nSamples; i++){
			buffer[i]=0.;
		}
	}

	lock.release();
	return buffer;
}

bool SoundModelMono::isPlaying() {

	bool out;

	lock.acquire();

	out = noteOn;

	lock.release();

	return out;

}

void SoundModelMono::setNoteOn(int midinote) {
	lock.acquire();

	if(noteOn == false ){//|| currentNote != midinote) {

		waveosc.setSemitone(midinote); //get the frequency to play at
		noteOn = true;
		currentNote = midinote;
		myadsr.gate(true);
		 
	}
	lock.release();
}

void SoundModelMono::setNoteOff(int midinote) { 

	lock.acquire();

	/* Only turn of if we're actually playing that note */
	if(currentNote == midinote) { 
		try{
			myadsr.gate(false);
			noteOn = false;

		}
		catch (const char* e) {}
	}
	lock.release();
}


void SoundModelMono::updateWavemix(unsigned int parameter){
	waveosc.updateWavemix(parameter);
}

void SoundModelMono::updateBank(unsigned int parameter) {
	sampleSourceStatus_t loadStatus = SOURCE_READY;
	int bank= static_cast<int>(static_cast<float>(((parameter)/127.0)*5.0))+1; //hard limit on 12 banks
	if (bank != safeBank){
		loadStatus=waveosc.loadBank(bank, SOURCE_PREGENERATED);
	}
	if (loadStatus != SOURCE_READY) {
		std::cout << std::endl << "Could not load bank " << bank << std::endl;
		std::cout << "Returning to bank " << safeBank << std::endl;
		bank = safeBank;
		waveosc.loadBank(bank, SOURCE_PREGENERATED);
	}else {
		safeBank = bank;
	}
}

void SoundModelMono::updateAttack(unsigned int parameter){
	float paramf=log10(1.002+99.0*(static_cast<float>(parameter)/127.0));
    float rate=2.0*paramf*sampleratef; //0 to 10s with log response
	//std::cout<<2.0*paramf<<std::endl;
    myadsr.setAttackRate(rate);
}

void SoundModelMono::updateDecay(unsigned int parameter){
	float paramf=log10(1.0+99.0*(static_cast<float>(parameter)/127.0));
    float rate=2.0*paramf*sampleratef; //0 to 10s with log response    std::cout<<rate<<std::endl;
	//std::cout<<paramf*2.0<<std::endl;
	myadsr.setDecayRate(rate);
}

void SoundModelMono::updateSustain(unsigned int parameter){
    float paramf=log10(1.001+99.0*static_cast<float>(parameter)/127.0);
    float level=paramf/2.0; //0 to 10s with log response level=logf(static_cast<float>(parameter+1)/128.0); //0. to 1. with log response
    //std::cout<<level<<std::endl;
	myadsr.setSustainLevel(level);
}

void SoundModelMono::updateRelease(unsigned int parameter){
	float paramf=log10(1.001+99.0*(static_cast<float>(parameter)/127.0));
    float rate=2.0*paramf*sampleratef; //0 to 10s with log response    std::cout<<rate<<std::endl;
	//std::cout<<2.0*paramf<<std::endl;
	myadsr.setReleaseRate(rate);
}



//next functions are unused

void SoundModelMono::updateVolume(unsigned int parameter) {}

void SoundModelMono::updateCutoff(unsigned int parameter) {}

void SoundModelMono::updateRes(unsigned int parameter) {}

void SoundModelMono::updateSlope(unsigned int parameter){}