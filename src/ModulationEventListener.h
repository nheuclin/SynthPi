/**
 * A class describing devices which
 * can receive modulation wheel event from the
 * Controller
 */
#ifndef MODULATIONEVENTLISTNER_H
#define MODULATIONEVENTLISTNER_H

class Controller;

class ModulationEventListener {
protected:
  /**
  * The device previously expecting modulation events from
  * the MIDI keyboard
  */
  ModulationEventListener *previousModulationListener;
  Controller *theController;

public:
  /**
   * Construct the ModulationEventListener
   * 
   * Registers the pedal with the controller to receive Modulation
   * wheel (a.k.a. Control Change 1) events from the MIDI device.
   */
  explicit ModulationEventListener(Controller *c);
  
  /**
   * Destructor
   * 
   * Deregisters the pedal with the controller, and restores
   * the previouls listener as the first to receive any future
   * modulation events.
   */
  virtual ~ModulationEventListener();
  
  /**
   * Receive a notification that the modulation wheel
   * on the MIDI controller has changed value
   * 
   * @param value The new modulation value (0 - 127)
   */
  virtual void modulationEventCallback(int value);
};

#endif // MODULATIONEVENTLISTNER_H