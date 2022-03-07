//#include "waveosc.hpp"
#include "SoundModelMono.h"
//#include "OutputAdaptor.h"
#include "Lock.h"
#include <string>
#include <iostream>

using namespace SYNTHPI;
using namespace audio;

#define RELEASE_INIT (1024*1600) //? maybe just put that to 0.

SoundModelMono::SoundModelMono(const int samplerate){
  	waveosc=new WaveOSC(samplerate); //is that right ? 
	waveosc.loadbank(1,SOURCE_PREGENERATED); //init wavetable to 1st bank
	
	this->noteOn = false;
	this->currentNote = -1;
	this->release = 0; //replace by waveosc.getRelease();

}

std::vector<sample_t> SoundModelMono::getSamples(int nSamples) {
	std::vector<sample_t> buffer;
	lock.acquire();

	/* Allows us to render sound when in release or on state */
	if(noteOn || release > 0) {
		buffer=waveosc.getSamples(nSamples);
		release -= nSamples;
		if(release < 0) release = 0;
	}  
	else {
            std::fill(SoundModelMono::getSamples, SoundModelMono::getSamples.begin(), SoundModelMono::getSamples.end(), 0); //might just get rid of that if the ADSR returns 0 when called and not in any states
		/* This is negligable in terms of time compared to rendering the sound */
	}

	lock.release();
	return buffer
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
			waveosc.trigAttack(); //trigAttack should rettriger the envelope if it's already playing
			noteOn = true;
			currentNote = midinote;

		} 
		//catch (const char* e) {
		//	std::cerr << "Midi note on (note " << midinote
		//	          << ") out of range!" << std::endl;
		//}
	}

	lock.release();

}

void SoundModelMono::setNoteOff(int midinote) { 

	lock.acquire();

	/* Only turn of if we're actually playing that note */
	if(currentNote == midinote) { 
		noteOn = false;
		release = waveosc.getRelease(); //return release time as a number of samples it will take for the ADSR to get back to 0 based on sampling rate
		waveosc.trigRelease(); //trigger ADSR release stage.
	}

	lock.release();

}
