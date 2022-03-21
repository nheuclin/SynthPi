//Controller.cpp
#include "Controller.h"


using namespace SYNTHPI;
using namespace audio;

Controller::Controller(SoundModel *playout) :
	isPlaying(true),
	noteQueue(),
{
	this->playout = playout;
}

Controller::~Controller(){}


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
				if(noteQueue.front().noteOff){
					playout->setNoteOff(noteQueue.front().note);
				}
				else{
					playout->setNoteOn(noteQueue.front().note);
				}
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

void Controller::updateVolume(unsigned int parameter){
	playout->updateVolume(parameter);
}

void Controller::updateWavemix(unsigned int parameter){
	playout->updateWavemix(parameter);
}

void Controller::updateBank(unsigned int parameter){
	playout-> updateBank(parameter);
}