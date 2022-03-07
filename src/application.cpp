//application.cpp

#include <iostream>
#include "application.hpp"

using namespace SYNTHPI;
using namespace audio;

//Application

Application::Application() {
	mainmodel = new (SoundModelPoly(poly, samplerate, output_gain));
	playbackEngine = new PlaybackEngine(*mainmodel);
	controller= new Controller(*mainmodel);
	keyboard= new Keyboard(&controller, keyboard_ID, keyboard_port, verbosity);
	running = true;
}

void Application::setup() {

	// Jack client
	audioEngine.reset(new audio::JackClient("SynthPi"));
	controller.start();
	keyboard.start();
}	

void Application::run() {
	// Start the audio stream
	audioEngine->start(playbackEngine);

	while(running) {}

	audioEngine->stop();
}

