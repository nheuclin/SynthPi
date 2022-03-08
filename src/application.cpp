//application.cpp

#include <iostream>
#include "application.hpp"
#include "defs.hpp"
using namespace SYNTHPI;

//Application

Application::Application(audio::SoundModelPoly mainmodel, audio::PlaybackEngine playbackengine, 
						audio::Controller controller, audio::Keyboard keyboard ) {


	running = true;
}

void Application::setup() {

	// Jack client
	audioEngine.reset(new audio::JackClient("SynthPi"));
	controller->start();
	keyboard->start();
}	

void Application::run() {
	// Start the audio stream
	audioEngine->start(playbackengine);

	while(running) {}

	audioEngine->stop();
}

