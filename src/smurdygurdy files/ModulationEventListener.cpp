#include <stdio.h>

#include "Controller.h"
#include "ModulationEventListener.h"

// Register the current listener as handling modulation events.
ModulationEventListener::ModulationEventListener(Controller *c)
  : theController(c) {
    previousModulationListener = theController->registerEventListener(this);
}

// Deregister this listener and restore the status quo on destruction.
ModulationEventListener::~ModulationEventListener() {
  theController->registerEventListener(previousModulationListener);
}

// Unless overridden (and it should be!) just pass on the event.
void ModulationEventListener::modulationEventCallback(int value) {
  fprintf(stderr,
	  "Warning: modulationEventListener default callback invoked:\n"
          "         The derived class should override this method.\n");
  
  if (previousModulationListener)
    previousModulationListener->modulationEventCallback(value);
}
