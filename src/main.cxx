/**
 * @mainpage SynthPi
 * @brief A Wavetable Synthesizer made for the Raspberry Pi.
 * @version 0.1
 * @authors Noe Heuclin     <2332897H@student.gla.ac.uk>
 * @authors Aaron Collins
 * @date March 2022
 */

#include "SoundModelPoly.h"
#include "Controller.h"
#include "Keyboard.h"
#include "audio.hpp"
#include "json.hpp"
#include "display.h"

#include <array>
#include <fstream>
#include <string>
#include <iostream>
#include <functional>
#include <signal.h>


using namespace SYNTHPI;

nlohmann::json j;

std::ifstream myfile ;

/*! Wrapper for the signal handling lambda expression. */
std::function<void(int)> shutdownHandler;

/*! Global function that can be given as a signal handler.
Calls \ref shutdownHandler. */
void signalHandler(int signal) { shutdownHandler(signal); }

/*! Main function of execution. */
int main(int argc, char* argv[]){

	myfile.open("config.json");
	myfile >> j;

	int poly=j.at("poly");

	int keyboard_ID=j.at("keyboard_ID");
	int keyboard_port=j.at("keyboard_port");
	int verbosity = j.at("verbosity");

	
	const int vol_ID=j.at("master_volume_ID");
	const int wavemix_ID=j.at("wavemix_ID");
	const int Bank_ID=j.at("Bank_select_ID");
	
	const int Attack_ID=j.at("Attack_time_ID");
	const int Decay_ID=j.at("Decay_time_ID");
	const int Sustain_ID=j.at("Sustain_level_ID");
	const int Release_ID=j.at("Release_time_ID");

	const int Cutoff_ID=j.at("Filter_Cutoff_ID");
	const int Res_ID=j.at("Filter_Resonance_ID");

	const int Slope_ID=j.at("Filter_Slope_ID");

    std::cout << std::endl << PROJECT_NAME << " v" << PROJECT_VERSION << std::endl;

	DisplayThread mydisplay;
	
	/*! Jack Client Object. */
    audio::JackClient audioEngine("SynthPi");
    
	/*! SoundModelPoly object. */
	audio::SoundModelPoly mainmodel(poly, samplerate);

	/*! Controller object. */
	audio::Controller controller(&mainmodel);
	
	/*! Keyboard object. */
	audio::Keyboard keyboard(&controller, &mydisplay, keyboard_ID, keyboard_port, verbosity, vol_ID,
	wavemix_ID, Bank_ID, Attack_ID, Decay_ID, Sustain_ID, Release_ID,  Cutoff_ID, Res_ID, Slope_ID); 
	
    signal(SIGQUIT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGHUP, signalHandler);
    signal(SIGKILL, signalHandler);
    signal(SIGTSTP, signalHandler);

    bool running = false;   
    
    shutdownHandler = [&](int signal) {
        std::cout << "SynthPi: caught signal " << signal << std::endl;
        running = false;

    };
    
	controller.start();
	mydisplay.start();
	keyboard.start();
    audioEngine.start(mainmodel);
    running = true;
	while(running) {}

	audioEngine.stop();

    return 0;
}
