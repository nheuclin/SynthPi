//application.hpp

#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <memory>

#include "applicationcallback.hpp"
#include "audio.hpp"
#include "playback.hpp"
//#include "defs.hpp" 
#include "SoundModelPoly.h"
#include "Controller.h"
#include "Keyboard.h"

//#include "display.hpp"

namespace SYNTHPI {
	

/*! 
 * \brief Main application.
 * 
 * This is the main application for the DrumPi program.
 * Its \ref setup and \ref run methods are called when the application
 * is started.
 */
class Application : public ApplicationCallback {
public:
	
	/*! Constructor */
	Application(audio::SoundModelPoly &mainmodel, audio::PlaybackEngine &playbackengine, 
				audio::Controller &controller, audio::Keyboard &keyboard);

	/*! \brief Sets up the application. 
	 * 
	 * This method is called on startup to perform various set up tasks,
	 * including connecting the \ref Application to the \ref KeyboardInput
	 * as a callback, resetting the sequencer and display,
	 * and loading the drum sample bank.
	 */
	void setup();

	/*! \brief Runs the application.
	 * 
	 * This method is called on startup after setup has been
	 * performed, starting the audio engine, the display refresh clock,
	 * and creating the keyboard thread.
	 */
	void run();

	/*! AudioEngine object. */
	std::unique_ptr<audio::JackClient> audioEngine = nullptr;






	/*! SoundModelPoly object. */
	audio::SoundModelPoly &mainmodel;

	/*! PlaybackEngine object. */
	audio::PlaybackEngine &playbackengine;

	audio::Controller &controller;
	
	audio::Keyboard &keyboard;

};

} // namespace SYNTHPI

#endif	// define APPLICATION_H
