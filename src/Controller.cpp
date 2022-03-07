#include <queue>
#include "Controller.h"
#include "SoundModel.h"
#include "Lock.h"
#include "Thread.h"
#include <string>
#include <iostream>

using namespace SYNTHPI;
using namespace audio;

Controller::Controller(SoundModel *playout) :
	isPlaying(true),
	noteQueue(),
	modulation(0), 
	modulationEventListener(NULL)
{

	this->playout = playout;
	//default_output_gain = playout->getOutputGain();
}

Controller::~Controller(){
}


void Controller::run(){
	while(isPlaying){
		//Wait until a signal is received.
		outputLock.acquire();
		outputLock.wait();
		outputLock.release();

		//If notes are waiting to be passed pass them.
		queueLock.acquire();
		if(!noteQueue.empty()){
			//Acquire lock on noteQueue before reading and popping all notes from it.
		
			while(!noteQueue.empty()){
				if(noteQueue.front().noteOff)
					playout->setNoteOff(noteQueue.front().note);
				else
					playout->setNoteOn(noteQueue.front().note);
				noteQueue.pop();
			}
		
			
		}
		queueLock.release();
			
	}
}

void Controller::keyEvent(bool type, int note){
	//Store incoming note at end of buffer.
	queueLock.acquire();
	noteQueue.push(QueueItem(type, note));
	queueLock.release();
	
	//Broadcast change in status.
	outputLock.acquire();
	outputLock.broadcast();
	outputLock.release();
}

void Controller::modulationEvent(int parameter) {
	// Store new modulation value
	modulation = (double)parameter / 127.0;
	if (modulationEventListener)
		modulationEventListener->modulationEventCallback(parameter);
}

ModulationEventListener *
  Controller::registerEventListener(ModulationEventListener *listener) {
  
  ModulationEventListener *oldListener = modulationEventListener;
  modulationEventListener = listener;

  return oldListener;
}