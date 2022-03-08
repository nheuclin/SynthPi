//application.cpp

#include <iostream>
#include "application.hpp"
#include "defs.hpp"
using namespace SYNTHPI;

//Application

Application::Application(audio::SoundModelPoly mainmodel, audio::PlaybackEngine playbackengine, 
						audio::Controller controller, audio::Keyboard keyboard) {

	this-> model = mainmodel;
	this-> playback = playbackengine;
	this-> control = controller;
	this-> key = keyboard;
	running = true;
}

void Application::setup() {

	// Jack client
	audioEngine.reset(new audio::JackClient("SynthPi"));
	control->start();
	key->start();
}	

void Application::run() {
	// Start the audio stream
	audioEngine->start(playback);

	while(running) {}

	audioEngine->stop();
}

