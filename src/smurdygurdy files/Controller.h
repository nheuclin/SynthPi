#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Thread.h"
#include "Lock.h"
#include "SoundModel.h"
#include "ModulationEventListener.h"
#include <queue>
#include <string>

 /**
 *Controls interaction between input and output stages of the hurdy gurdy.
 *
 */

class Controller: public Thread{

 protected:
	/**
	 *An item on the noteQueue.
	 */
	class QueueItem{
	public:
		//True if the
		bool noteOff;
		int note;

		/**
		 *Creates a new item and initialises its variables.
		 */
		QueueItem(bool noteOff, int note){
			this->noteOff = noteOff;
			this->note = note;
		}

	};
	

  //Locks for signalling of changes in keyboard or pedal input.
  Lock outputLock;

  //SoundModel itself.
  SoundModel *playout;

  //Variable for storing the current status of playout.
  bool isPlaying;

  //Stores whether or not the pedal has triggered a broadcast.
  bool pedalTriggered;

  //Buffers notes sent by keyboard.  Notes are stored in instantiations of type QueueItem.
  std::queue<QueueItem> noteQueue;

  //Lock for accessing the noteQueue.
  Lock queueLock;

  //Lock for accessing the pedal variables.
  Lock pedalLock;

  //Stores last pedal speed sent by pedal.
  double speed;
  
  //The last modulation value sent to the MIDI interface
  double modulation;

  //Pedal value below which the sound model is told to fade out
  double autofade;

  //Default gain of the SoundModel (adjusted by autofading)
  double default_output_gain;
  
 public:

  /**
   *Creates a new controller object.
   *
   *@param playout Soundmodel to which controller passes signals.
   *@param speed_min Bow speed minimum pedal value (0)
   *@param speed_max Bow speed for max pedal value (1.0)
   *@param autofade_point If > 0, the gain the SoundModel's output gain
   *                 is adjusted to fade to silence as the pedal reaches 0.
   */
  Controller(SoundModel *playout,
             double speed_min = 0.15, double speed_max = 1.6,
             double autofade_point = 0);

  /**
   *Called when a controller object is destroyed.
   */
  ~Controller();


   /**
   *Called by keyboard interface whenever a key event happens.
   *
   *Stores notes in the noteQueue while waiting for processor time.
   *
   *@param noteOff is a boolean which is true if the event is turning a note off.
   *@param note indicates which note to turn off.
   */
  void keyEvent(bool noteOff, int note);
  
   /**
    * Called by the keyboard interface whenever a modulation event happens.
    * 
    * The pedal may access the midi modulation parameter, which is normalised
    * in the range 0.0 to 1.0, in order to influence the effect of the pedal
    * position. If a callback is registered by the pedal, it is called
    * every time the modulation control event is received. Alternatively, the
    * pedal may query the current modulation setting by calling
    * get_modulation()
    * 
    * @param midiParam The new MIDI modulation setting (in the range 0..255)
    */
  void modulationEvent(int midiParam);

  /**
   *Starts the running of playout
   */
  virtual void run();

  /**
   * Called by pedal interface whenever speed changes significantly.
   * The value supplied is a double in the range 0.0-1.0.
   * This is mapped into a bow speed in the range given when the
   * object is constructed (defaults are supplied).
   *
   * Calls setPedalSpeed method in playout to pass on new value.
   *
   * @param speed Speed at which bow should rotate.
   */
  void speedChange(double spd);

  /**
   * Access the current physical bow speed
   *
   * @return
   * The current physical speed of the bow supplied to the physical
   * model in m/s.
   */
  double get_bow_speed() const { return speed; }
  
  /**
   * Access the current modulation
   * 
   * @return Modulation setting in the range 0.0 - 1.0.
   */
  double get_modulation() const { return modulation; }
  
  /**
   * Register a new modulation event listener overriding the current one.
   * 
   * The new event listner must deregister itself on destruction.
   * Registering NULL as the listener suppresses any handling of
   * MIDI CC1 (Modulation Wheel) events.
   * 
   * @param listener The new listener
   * @return the previous listner which the new one replaces.
   */
  ModulationEventListener *registerEventListener(ModulationEventListener *listener);

  double max_speed; ///< maximum bow speed (pedal = 1.0)
  double min_speed; ///< minumum bow speed (pedal = 0.0)
  
protected:
  // The Modulation Event Wheel listener to call (if any)
  ModulationEventListener *modulationEventListener;
  

};

#endif /* CONTROLLER_H */
