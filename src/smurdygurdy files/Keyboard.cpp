#include "Keyboard.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "Controller.h"
#include <set>
#include <alsa/asoundlib.h>


Keyboard::Keyboard(Controller *controller, int keyId, int keyPort, int verbosity) :
announce(verbosity > 0)
{

	this->controller = controller;
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
	snd_seq_set_client_name(sequencer, "SMRGyGurdy");
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
	if (deferred_noteoff.find(note) !=  deferred_noteoff.end())
		controller->keyEvent(true, note);
	
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

			if (velocity != 0)
				noteOn(note,  velocity);
			else
				noteOff(note);
			
			break; 

		case SND_SEQ_EVENT_NOTEOFF:
			note =  event->data.note.note;
			velocity = event->data.note.velocity;
			
			noteOff(note);
			break;

		case SND_SEQ_EVENT_CONTROLLER:
			
			const int p = event->data.control.value;

			switch (event->data.control.param) {
			case 1:
				// Forward modulation events to the controller,
				// just in case anybody is interested.
				controller->modulationEvent(p);
				break;

			case 64:
				// Process sustain pedal events
				bool s {p > 63};
				if (sustain && !s) release();
                sustain = s;
				
				if (announce) {
					fprintf(stderr, "Sustain pedal change, value %d (sustain %s)\n",
							p,  sustain ? "on" : "off");
				}
				break;

			}
		
			break;

		}
	
	snd_seq_free_event(event);
	} while (snd_seq_event_input_pending(sequencer, 0) > 0);

}
