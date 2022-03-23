#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Thread.h>
#include <Lock.h>
#include <SoundModel.h>

#include <queue>
#include <string>
#include <iostream>


namespace SYNTHPI {
namespace audio{

class Controller: public Thread{

 protected:

	/*!An item on the noteQueue.*/
	class QueueItem{
	public:
		
		bool noteOff;
		int note;

		/*! Creates a new item and initialises its variables.*/
		QueueItem(bool noteOff, int note){
			this->noteOff = noteOff;
			this->note = note;
		}

	};
	

  /*!Lock for signalling of changes in keyboard or pedal input.*/
  Lock outputLock;

  /*!SoundModel the controller attaches to.*/
  SoundModel *playout;

  /*!Variable for storing the current status of playout.*/
  bool isPlaying;

  /*!Stores whether or not the pedal has triggered a broadcast.*/
  bool pedalTriggered;

  /*!Buffers notes sent by keyboard.  Notes are stored in instantiations of type QueueItem.*/
  std::queue<QueueItem> noteQueue;

  /*!Lock for accessing the noteQueue. */
  Lock queueLock;

  /*! float to pass the volume from the midi CC to SoundModelPoly 
  float volume;

  /*! float to pass the volume from the midi CC to SoundModelPoly 
  float wavemix_val;
 
  /*! float to pass the volume from the midi CC to SoundModelPoly 
  float attack_time;
  
  /*! float to pass the volume from the midi CC to SoundModelPoly 
  float decay_time;
  
  /*! float to pass the volume from the midi CC to SoundModelPoly 
  float sustain_level;
  
  /*! float to pass the volume from the midi CC to SoundModelPoly 
  float release_time;
  
  /*! float to pass the volume from the midi CC to SoundModelPoly 
  int bank_number;*/
  
 public:

  /*! Creates a new controller object
  \param playout Soundmodel to which controller passes signals.*/
  Controller(SoundModel *playout); 

  /*! Called when a controller object is destroyed.*/
  ~Controller();

   /*! Called by keyboard interface whenever a key event happens.
   Stores notes in the noteQueue while waiting for processor time.
   \param noteOff is a boolean which is true if the event is turning a note off.
   \param note indicates which note to turn off.*/
  void keyEvent(bool noteOff, int note);
  
  /*! Called by keyboard interface when it receives a value from the CC associated to the volume control
  \param parameter the midi value which represents volume */ 
  void updateVolume(unsigned int parameter);

  /*! Called by keyboard interface when it receives a value from the CC associated to the wavemix control
  \param parameter the midi value which represents wavemix */ 
  void updateWavemix(unsigned int parameter);

  /*! Called by keyboard interface when it receives a value from the CC associated to the attack time control
  \param parameter the midi value which represents the attack time */ 
  void updateAttack(unsigned int parameter);

  /*! Called by keyboard interface when it receives a value from the CC associated to the decay time control
  \param parameter the midi value which represents the decay time */ 
  void updateDecay(unsigned int parameter);

  /*! Called by keyboard interface when it receives a value from the CC associated to the sustain level control
  \param parameter the midi value which represents the sustain level */ 
  void updateSustain(unsigned int parameter);

  /*! Called by keyboard interface when it receives a value from the CC associated to the release time control
  \param parameter the midi value which represents the release time */ 
  void updateRelease(unsigned int parameter);

  /*! Called by keyboard interface when it receives a value from the CC associated to the bank sekect control
  \param parameter the midi value which represents the bank to play from */ 
  void updateBank(unsigned int parameter);

  /*! Called by keyboard interface when it receives a value from the CC associated to the filter cutoff frequency
  control
  \param parameter the midi value which represents the filter cutoff frequency */ 
  void updateCutoff(unsigned int parameter);

  /*! Called by keyboard interface when it receives a value from the CC associated to the filter resonance control
  \param parameter the midi value which represents the amount of resonance to the filter*/ 
  void updateRes(unsigned int parameter);

  /*! Called by keyboard interface when it receives a value from the CC associated to the slope select control
  \param parameter the midi value which select the filter slope (12or24dB/octave) */ 
  void updateSlope(unsigned int parameter);
  
  /*! Starts the running of playout*/
  virtual void run();
};

} //close audio namespace
} //close SYNTHPI namespace
#endif /* CONTROLLER_H */
