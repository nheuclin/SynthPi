//application.cpp

#include <iostream>
#include "application.hpp"

using namespace SYNTHPI;


//Application

Application::Application() {
	running = true;
}

void Application::setup() {

	// Jack client
	audioEngine.reset(new audio::JackClient("SynthPi"));
	
	audio::SoundModelPoly *mainmodel;
	mainmodel = new (audio::SoundModelPoly(poly, samplerate, output_gain));
	Controller controller(mainmodel);
	controller.start();
	Keyboard keyboard(&controller, keyboard_ID, keyboard_port, verbosity);
	keyboard.start();
}	

void Application::run() {
	// Start the audio stream
	audioEngine->start(playbackEngine);


	while(running) {}


	audioEngine->stop();
}

