//SoundModelMono.cpp

#include "SoundModelMono.h"
#include <iostream> 

using namespace SYNTHPI;
using namespace audio;

#define RELEASE_INIT (1024*1600) //? maybe just put that to 0.

SoundModelMono::SoundModelMono() {
  	this-> sampleratef=static_cast<float>(samplerate);
	this-> waveosc.loadBank(1, SOURCE_PREGENERATED);
	this->noteOn = false;
	this->currentNote = -1;
	this->release = 0; //replace by waveosc.getRelease();
}

std::vector<sample_t> SoundModelMono::getSamples(int nSamples) {
	
	std::vector<sample_t> buffer(nSamples);
	std::vector<sample_t> OSCbuffer(nSamples);
	lock.acquire();
	

	/* Allows us to render sound when in release or on state */
	if(noteOn==true || release>0) { 
		OSCbuffer= waveosc.getSamples(nSamples);
		
		
		for (unsigned int i =0;i<nSamples;i++){
			buffer[i]=OSCbuffer[i] //add ADSR mult here
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
		 
	}
	lock.release();
}

void SoundModelMono::setNoteOff(int midinote) { 

	lock.acquire();

	/* Only turn of if we're actually playing that note */
	if(currentNote == midinote) { 
		try{
			noteOn = false;
			//currentNote = -1;
			//release = waveosc.getRelease(); //return release time as a number of samples it will take for the ADSR to get back to 0 based on sampling rate
			//waveosc.trigRelease(); //trigger ADSR release stage.
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



//next functions are unused

void SoundModelMono::updateVolume(unsigned int parameter) {}
