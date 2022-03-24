#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <set>
#include <alsa/asoundlib.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include <Thread.h>
#include <Lock.h>
#include <defs.hpp>
#include "Controller.h"
#include "display.h"


/*!
Provides interface for the ALSA Sequencer API.
The sequencer client subscribes to a USB keyboard client with a specified id
and polls it for incoming MIDI events. It passes the appropriate commands
to an instance of Controller based on what it receives. */
namespace SYNTHPI {
namespace audio {
	
class Keyboard : public Thread {

	/*! the instance of controller the keyboard attaches to.*/
	Controller* controller;

	/*! the instance of the display thread to pass midi parameters to */
	DisplayThread* mydisplaythread;

	/*! the ALSA sequencer instance to receive raw midi messages from */
	snd_seq_t* sequencer;

	/*! int to store the ALSA sequencer client ID */
	int myId;

	/*! int to store the ALSA sequencer port number */
	int myPort;

	/*! the client ID number passed from the json config file to connect the midi keyboard to the ASLA sequencer */
	int keyboardId;

	/*! the port number passed from the json config file to connect the midi keyboard to the ASLA sequencer */
	int keyboardPort;
	
	/*! a variable used to print note on and note off messages when it is above 1*/
	int announce;

	/*! custom data type to store deffered note off messages*/
	typedef std::set<int> keyset;
	
	/*! A list of note off commands not yet sent
	because the sustain pedal is pressed.*/
	keyset deferred_noteoff;

	/*! bool to store whether the sustain pedal is pressed or not */ 
	bool sustain=false;
	
	/*! const int to store the 1st CC ID number from the json file */
	const int CC1;

	/*! const int to store the 2nd CC ID number from the json file */
	const int CC2;

	/*! const int to store the 3rd CC ID number from the json file */
	const int CC3;

	/*! const int to store the 4th CC ID number from the json file */
	const int CC4;

	/*! const int to store the 5th CC ID number from the json file */
	const int CC5;

	/*! const int to store the 6th CC ID number from the json file */
	const int CC6;

	/*! const int to store the 7th CC ID number from the json file */
	const int CC7;

	/*! const int to store the 8th CC ID number from the json file */
	const int CC8;

	/*! const int to store the 9th CC ID number from the json file */
	const int CC9;

	/*! const int to store the 10th CC ID number from the json file */
	const int CC10;

	/*! int to store the number of available banks in SynthPi */
	int Bank_No;

	/*! Send a MIDI note on event,  taking the sustain pedal into account
	   If a note is already sounding due to sustain and a note-on message
	   is received,  convention dictates the synth should receive a note-off first.
	   Some synths misbehave if they receive two note-ons on the same channel.
	  
	  \param note The MIDI note number.
	  \param velocity The MIDI note velocity (non-zero).*/
	void noteOn(unsigned char note,  unsigned char velocity);

	/*! Send a MIDI note on event taking the sustain pedal into account (@sa noteOn)
	\param note MIDI note number */
	void noteOff(unsigned char note);
	
	/*! Release all sustained notes and cancel sustain. @sa noteOn */
	void release(void);
	
	/*! Creates a new ALSA sequencer client */		
	void createSequencer();


	/*! Handle a MIDI event
	  Read a MIDI event and takes an appropriate action. Possible events:
	  CONTROLLER, PITCHBEND, NOTEON, NOTEOFF
	  \param seq_handle pointer to the sequencer handle */
	void midiAction();

	/*! Connect the MIDI keyboard to the ALSA sequencer */
	void connectKeyboard();

	public:

		/*! Keyboard constructor
		\param controller An instance of Controller class
		\param mydisplaythread An instance of the display thread to print MIDI parameter and values
		\param keyId ALSA sequencer client id of the keyboard
		\param keyPort Port number on the given client
		\param verbosity Print note on/off announcement if > 0
		\param vol_ID the MIDI CC ID number for volume control
		\param wavemix_ID the MIDI CC ID number for wavemixing control
		\param Bank_ID the MIDI CC ID number for changing banks
		\param Bank_No the number of available banks in synthpi
		\param Attack_ID the MIDI CC ID number for setting the ADSR attack time
		\param Decay_ID the MIDI CC ID number for setting the ADSR decay time
		\param Sustain_ID the MIDI CC ID number for setting the ADSR sustain level
		\param Release_ID the MIDI CC ID number for setting the ADSR release time
		\param Cutoff_ID the MIDI CC ID number for setting the filter cutoff frequency
		\param Res_ID the MIDI CC ID number for setting the filter resonance amount
		\param Slope_ID the MIDI CC ID number for selecting the filter slope
		*/
		Keyboard(Controller* controller, DisplayThread* mydisplaythread, int keyId, int keyPort, int verbosity,
				const int vol_ID, const int wavemix_ID, const int Bank_ID, int Bank_No, const int Attack_ID,const int Decay_ID,
				const int Sustain_ID, const int Release_ID, const int Cutoff_ID,const int Res_ID, const int Slope_ID);
		
		/**
		* Poll the MIDI keyboard for key events.
		*/
		virtual void run();

};
}
}
#endif /* KEYBOARD_H */
