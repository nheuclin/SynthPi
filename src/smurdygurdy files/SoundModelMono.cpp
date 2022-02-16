#include "StringAllocator.h"
#include "SoundModelMono.h"
#include "OutputAdaptor.h"
#include "Lock.h"
#include "ViolinFingering.h"
#include <string>
#include <iostream>

#define RELEASE_INIT (1024*1600)
#define DEFAULT_PEDAL_SPEED 1;

SoundModelMono::SoundModelMono(const int sr)
 : violin(sr) {

	this->noteOn = false;
	this->currentNote = -1;
	this->release = 0;
	this->pedalSpeed = DEFAULT_PEDAL_SPEED;

}

void SoundModelMono::getSamples(short samples[], int bufferSize) {

	ViolinFingering vf;
	
	lock.acquire();

	/* Allows us to render sound when in release or on state */
	if(noteOn || release > 0) {
		violin.getSamples(samples, bufferSize);
		release -= bufferSize;
		if(release < 0) release = 0;
	}  else {
            std::fill(samples, samples + bufferSize, 0);
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
			violin.setSemitone(midinote);
			violin.setPedalSpeed(pedalSpeed);
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
		violin.silence();
	}

	lock.release();

}

void SoundModelMono::setPedalSpeed(double speed) {

	lock.acquire();

	if(noteOn) violin.setPedalSpeed(speed); /* This avoids the model playing sound */
	pedalSpeed = speed;

	lock.release();

}

