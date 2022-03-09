/**
 * @mainpage SynthPi
 * @brief A Wavetable Synthesizer made for the Raspberry Pi.
 * @version 0.1
 * @authors Noe Heuclin     <2332897H@student.gla.ac.uk>
 * @authors Aaron Collins
 * @date March 2022
 */
#include "application.hpp"
#include "SoundModelPoly.h"
#include "Controller.h"
#include "Keyboard.h"
#include "playback.hpp"
#include <iostream>
#include <functional>
#include <signal.h>
using namespace SYNTHPI;

/*! Wrapper for the signal handling lambda expression. */
std::function<void(int)> shutdownHandler;

int verbosity = 0;

/*!number of available voices*/
const int poly=8; //number of voices


/*! ID and port to connect midi keyboard to*/
int keyboard_ID=28;
int keyboard_port=0;

double output_gain =0.2;


/*! Global function that can be given as a signal handler.
Calls \ref shutdownHandler. */
void signalHandler(int signal) { shutdownHandler(signal); }

/*! Main function of execution. */
int main(int argc, char* argv[]){

    std::cout << std::endl << PROJECT_NAME << " v" << PROJECT_VERSION << std::endl;
    
    audio::SoundModelPoly *mainmodel_ptr;
	/*! SoundModelPoly object. */
	audio::SoundModelPoly mainmodel(poly, samplerate, output_gain);
    
    mainmodel_ptr= &mainmodel;
	/*! PlaybackEngine object. */
	audio::PlaybackEngine playbackengine(mainmodel_ptr);

	audio::Controller controller(mainmodel_ptr);
	
	audio::Keyboard keyboard(&controller, keyboard_ID, keyboard_port, verbosity);

    Application app(&mainmodel,&playbackengine,&controller,&keyboard);

    Application* appPtr;
    //signal(SIGINT, signalHandler);
    signal(SIGQUIT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGHUP, signalHandler);
    signal(SIGKILL, signalHandler);
    signal(SIGTSTP, signalHandler);
    shutdownHandler = [&](int signal) {
        std::cout << "SynthPi: caught signal " << signal << std::endl;
        appPtr->running = false;
    };


    appPtr = &app;

    app.setup();
    app.run();

    return 0;
}
