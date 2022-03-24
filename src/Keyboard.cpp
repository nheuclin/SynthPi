#include "Keyboard.h"

//#include "defs.hpp"

using namespace SYNTHPI;
using namespace audio;

Keyboard::Keyboard(Controller *controller, DisplayThread* mydisplaythread, int keyId, int keyPort, int verbosity, const int vol_ID, 
					const int wavemix_ID, const int Bank_ID,int Bank_No, const int Attack_ID,const int Decay_ID, const int Sustain_ID,
		 			const int Release_ID,const int Cutoff_ID,const int Res_ID, const int Slope_ID):
announce(verbosity > 0),
CC1(vol_ID),
CC2(wavemix_ID),
CC3(Bank_ID),
CC4(Attack_ID),
CC5(Decay_ID),
CC6(Sustain_ID),
CC7(Release_ID),
CC8(Cutoff_ID),
CC9(Res_ID),
CC10(Slope_ID),
Bank_No(Bank_No)
{

	this->controller = controller;
	this->mydisplaythread = mydisplaythread;
	this->keyboardId = keyId;
	this->keyboardPort = keyPort;

	createSequencer();
	connectKeyboard();
}



void Keyboard::run() {
		
	int count;
	struct pollfd* descriptors;

	count = snd_seq_poll_descriptors_count(sequencer, POLLIN);
	descriptors = (struct pollfd *)alloca(count * sizeof(struct pollfd));
	snd_seq_poll_descriptors(sequencer, descriptors, count, POLLIN);

	while (1) {
		if (poll(descriptors, count, 100000) > 0){
			midiAction();
		}  
	}

	free(descriptors);

}


void Keyboard::createSequencer() {

	if (snd_seq_open(&sequencer, "default", SND_SEQ_OPEN_INPUT, 0) < 0) {
		fprintf(stderr, "Error opening ALSA sequencer.\n");
		exit(1);
	}
	snd_seq_set_client_name(sequencer, "SynthPi");
	if ((this->myPort = snd_seq_create_simple_port(sequencer, "Default Port",
			SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
			SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
		fprintf(stderr, "Error creating sequencer port.\n");
		exit(1);
	}

	this->myId = snd_seq_client_id(sequencer);

}


void Keyboard::connectKeyboard()
{
/*
*NOTE: myId, myPort, keyboardId, keyboardPort have to be defined before calling this function
*/	

	snd_seq_addr_t sender, dest;
	snd_seq_port_subscribe_t *subs;

	//set sender to the keyboard client
	sender.client = this->keyboardId;
	sender.port = this->keyboardPort;

	//set destination to the application client
	dest.client = this->myId;
	dest.port = this->myPort;

	//set the application client to receive data from the keyboard client
	snd_seq_port_subscribe_alloca(&subs);
	snd_seq_port_subscribe_set_sender(subs, &sender);
	snd_seq_port_subscribe_set_dest(subs, &dest);
	snd_seq_subscribe_port(sequencer, subs);
}

void Keyboard::noteOn(unsigned char note,  unsigned char velocity) {
	//  Send a note off if already sounding
	// if (deferred_noteoff.find(note) !=  deferred_noteoff.end()){
	// 	controller->keyEvent(true, note);
	// }
	//  sound the note (velocity is ignored)
	controller->keyEvent(false, note);
	
	if (announce)
		fprintf(stderr, "Note On %d with velocity %d \n", note, velocity);
}

void Keyboard::noteOff(unsigned char note) {
	//  If the sustain pedal is depressed,  the event is deferred
	if (sustain) {
		deferred_noteoff.insert(note);
	} else {
		controller->keyEvent(true, note);
	}

	if (announce && note != 0)
		fprintf(stderr, "Note Off %d %s\n",
			note,  sustain ? "(deferred)" : "");	
}

void Keyboard::release(void) {
    sustain = false;
	for (keyset::iterator it = deferred_noteoff.begin(); it != deferred_noteoff.end(); ++it)
		noteOff(*it);
	deferred_noteoff.clear();
 }


void Keyboard::midiAction() {

	snd_seq_event_t *event;
	unsigned char note;
	unsigned char velocity;

	do {
		snd_seq_event_input(sequencer, &event);
		
		switch (event->type) {
		case SND_SEQ_EVENT_NOTEON:
			note =  event->data.note.note;
			velocity = event->data.note.velocity;

			if (velocity != 0){
				noteOn(note,  velocity);
				break;
			}
			else{
				noteOff(note);
				break;
			} 

		case SND_SEQ_EVENT_NOTEOFF:
			note =  event->data.note.note;
			velocity = event->data.note.velocity;
			
			noteOff(note);
			break;

		case SND_SEQ_EVENT_CONTROLLER:
			
			const int v = event->data.control.value;
			const int p=event->data.control.param;
			unsigned int display_val=static_cast<unsigned int>(v);
			
			if (p==64){// Process sustain pedal events	
				bool s {p > 63};
				if (sustain && !s) release();
                 	sustain = s;
				
				if (announce) {
					fprintf(stderr, "Sustain pedal change, value %d (sustain %s)\n",
					p,  sustain ? "on" : "off");
				}
				break;
			}

			if (p==CC1){ //master volume CC
				//std::cout<<"in CC1 if loop"<<std::endl;
				controller->updateVolume(v);
				mydisplaythread->ccToDisplay(0,v);
				break;
			}

			if (p==CC2){ //wavemix value CC
				//std::cout<<"in CC2 if loop"<<std::endl;
				controller->updateWavemix(v);
				mydisplaythread->ccToDisplay(1,v);
				break;
			}

			if (p==CC3){ //Bank select CC
				int bank= 1+static_cast<int>((Bank_No-1)*static_cast<float>(v)/127.0);
				//std::cout<<"in CC3 if loop"<<std::endl;
				controller->updateBank(bank);
				mydisplaythread->ccToDisplay(2,bank);
				break;
			}

			if (p==CC4){ //Attack time CC
				//std::cout<<"in CC4 if loop"<<std::endl;
				controller->updateAttack(v);
				mydisplaythread->ccToDisplay(3,v);
				break;
			}

			if (p==CC5){ // Decay time CC
				//std::cout<<"in CC5 if loop"<<std::endl;
				controller->updateDecay(v);
				mydisplaythread->ccToDisplay(4,v);
				break;
			}

			if (p==CC6){ //Sustain Level CC
				//std::cout<<"in CC6 if loop"<<std::endl;
				controller->updateSustain(v);
				mydisplaythread->ccToDisplay(5,v);
				break;
			}

			if (p==CC7){ //Release time CC
				//std::cout<<"in CC7 if loop"<<std::endl;
				controller->updateRelease(v);
				mydisplaythread->ccToDisplay(6,v);
				break;
			}

			if (p==CC8){ // Cutoff Frequency CC
				//std::cout<<"in CC8 if loop"<<std::endl;
				controller->updateCutoff(v);
				mydisplaythread->ccToDisplay(7,v);
				break;
			}

			if (p==CC9){ // Resonance amount CC
				//std::cout<<"in CC9 if loop"<<std::endl;
				controller->updateRes(v);
				mydisplaythread->ccToDisplay(8,v);
				break;
			}
			if (p==CC10){ // Filter slope select CC
				//std::cout<<"in CC10 if loop"<<std::endl;
				controller->updateSlope(v);
				mydisplaythread->ccToDisplay(9,v);
				break;
			}

		}
	
	snd_seq_free_event(event);
	} while (snd_seq_event_input_pending(sequencer, 0) > 0);

}
