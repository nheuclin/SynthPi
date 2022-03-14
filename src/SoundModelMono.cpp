//#include "waveosc.hpp"
#include "SoundModelMono.h"
//#include "OutputAdaptor.h"
#include "Lock.h"
#include <string>
#include <iostream> 

using namespace SYNTHPI;
using namespace audio;

#define RELEASE_INIT (1024*1600) //? maybe just put that to 0.

SoundModelMono::SoundModelMono() {
  	//this-> waveosc = new WaveOSC(samplerate); //is that right ? 
	this-> waveosc.loadBank(1, SOURCE_PREGENERATED);
	this->noteOn = false;
	this->currentNote = -1;
	this->release = 0; //replace by waveosc.getRelease();
	//std::cout << "SMM created " << std::endl;
}

std::vector<sample_t> SoundModelMono::getSamples(int nSamples) {
	
	std::vector<sample_t> buffer(nSamples);
	lock.acquire();
	
	if (noteOn==false){
		
		for (unsigned int i = 0; i<nSamples; i++){
			buffer[i]=0.;
		}
	}
	/* Allows us to render sound when in release or on state */
	if(noteOn==true ) { //  || release > 0
		buffer= waveosc.getSamples(nSamples);
		release -= nSamples;
		if(release < 0) release = 0;
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

	if(noteOn == false || currentNote != midinote) {

		try{
			waveosc.setSemitone(midinote); //get the frequency to play at and point to the according set of wavetables 
			//waveosc.trigAttack(); //trigAttack should rettriger the envelope if it's already playing
			this-> noteOn = true;
			currentNote = midinote;
		} 
		catch (const char* e) {}
	}
	lock.release();
}

void SoundModelMono::setNoteOff(int midinote) { 

	lock.acquire();

	/* Only turn of if we're actually playing that note */
	if(currentNote == midinote) { 
		try{
			this-> noteOn = false;
			//currentNote = -1;
			//release = waveosc.getRelease(); //return release time as a number of samples it will take for the ADSR to get back to 0 based on sampling rate
			//waveosc.trigRelease(); //trigger ADSR release stage.
		}
		catch (const char* e) {}
	}
	lock.release();
}
