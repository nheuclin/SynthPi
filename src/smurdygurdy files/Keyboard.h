#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "Thread.h"
#include "Lock.h"
#include "Controller.h"
#include <set>
#include <alsa/asoundlib.h>

/**
* Provides interface for the ALSA Sequencer API.
* The sequencer client subscribes to a USB keyboard client with a specified id
* and polls it for incoming MIDI events. It passes the appropriate commands
* to an instance of Controller based on what it receives.
*/

class Keyboard : public Thread {

	Controller* controller;
	snd_seq_t* sequencer;
	int myId;
	int myPort;
	int keyboardId;
	int keyboardPort;
	int announce;
	typedef std::set<int> keyset;
	/** A list of note off commands not yet sent
	*  because the sustain pedal is pressed.
	*/
	keyset deferred_noteoff;
	bool sustain; /**< whether sustain pedal is depressed */
	
	/**
     * Send a MIDI note on event,  taking the sustain pedal into account
	 * 
	 * If a note is already sounding due to sustain and a note-on message
	 * is received,  convention dictates the synth should receive a note-off first.
	 * Some synths misbehave if they receive two note-ons on the same channel.
	 * 
	 * @param note The MIDI note number.
	 * @param velocity The MIDI note velocity (non-zero).
	 */
	void noteOn(unsigned char note,  unsigned char velocity);

	/**
     * Send a MIDI note on event taking the sustain pedal into account (@sa noteOn)
	 * 
	 * @param note MIDI note number 
     */
	void noteOff(unsigned char note);
	
	/**
     * Release all sustained notes and cancel sustain. @sa noteOn
     */
	void release(void);
	
	/**
	* Creates a new ALSA sequencer client
	*/		
	void createSequencer();


	/**
	* Handle a MIDI event
	*
	* Read a MIDI event and takes an appropriate action. Possible events:
	* CONTROLLER, PITCHBEND, NOTEON, NOTEOFF
	*
	* @param seq_handle pointer to the sequencer handle
	*/
	void midiAction();

	void connectKeyboard();

	public:

		/**
		* Default constructor
		* @param controller An instance of Controller class
		* @param keyId ALSA sequencer client id of the keyboard
		* @param keyPort Port number on the given client
		* @param verbosity Print note on/off announcement if > 0
		*/
		Keyboard(Controller* controller, int keyId, int keyPort,
				int verbosity = 1);
		
		/**
		* Poll the MIDI keyboard for key events.
		*/
		virtual void run();

};

#endif /* KEYBOARD_H */
