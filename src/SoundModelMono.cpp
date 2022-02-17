#include "waveosc.hpp"
#include "SoundModelMono.h"
#include "OutputAdaptor.h"
#include "Lock.h"
#include <string>
#include <iostream>

#define RELEASE_INIT (1024*1600)

SoundModelMono::SoundModelMono(const int samplerate)
 : waveosc(samplerate) {

	this->noteOn = false;
	this->currentNote = -1;
	this->release = 0;

}

std::vector<sample_t> SoundModelMono::getSamples(int nSamples) {

	lock.acquire();

	/* Allows us to render sound when in release or on state */
	if(noteOn || release > 0) {
		waveosc.getSamples(nSamples);
		release -= bufferSize;
		if(release < 0) release = 0;
	}  
	else {
            std::fill(SoundModelMono::getSamples, SoundModelMono::getSamples.begin(), SoundModelMono::getSamples.end(), 0); //might just get rid of that if the ADSR returns 0 when called and not in any states
		/* This is negligable in terms of time compared to rendering the sound */
	}

	lock.release();
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
			waveosc.setSemitone(midinote);
			waveosc.trigAttack();
			noteOn = true;
			currentNote = midinote;

		} catch (const char* e) {
			std::cerr << "Midi note on (note " << midinote
			          << ") out of range!" << std::endl;
		}
	}

	lock.release();

}

void SoundModelMono::setNoteOff(int midinote) { 

	lock.acquire();

	/* Only turn of if we're actually playing that note */
	if(currentNote == midinote) { 
		noteOn = false;
		release = RELEASE_INIT;
		waveosc.trigRelease();
	}

	lock.release();

}
