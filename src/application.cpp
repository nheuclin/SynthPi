//application.cpp

#include <iostream>
#include "application.hpp"

using namespace SYNTHPI;

//Application

Application::Application() {
	mainmodel = new audio::SoundModelPoly(poly, samplerate, output_gain);
	playbackengine = new audio::PlaybackEngine(*mainmodel);
	controller= new audio::Controller(*mainmodel);
	keyboard= new audio::Keyboard(&controller, keyboard_ID, keyboard_port, verbosity);
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
	audioEngine->start(playbackengine);

	while(running) {}

	audioEngine->stop();
}

