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
	controller->start();
	keyboard->start();
}	

void Application::run() {
	// Start the audio stream
	audioEngine->start(playbackengine);

	while(running) {}

	audioEngine->stop();
}

