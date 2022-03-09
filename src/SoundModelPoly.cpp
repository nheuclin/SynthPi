#include "SoundModelMono.h"
#include "SoundModelPoly.h"
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace SYNTHPI;
using namespace audio;

SoundModelPoly::SoundModelPoly(const int poly, const int samplerate, double gain) {
	for (int i = 0; i < poly; i++) {
		soundModelList.push_back(new SoundModelMono);
		//soundModelList[i].loadBank(1, audio::SOURCE_PREGENERATED); //init synth to bank 1, done in soundmodelmono
	}
	VoiceNo=poly;
	std::cout << "SMP created" << std::endl;
}

std::vector<sample_t> SoundModelPoly::getSamples(int nSamples){

    std::vector<sample_t> temp(nSamples);
    // Clear object polybuffer and set the size
    polybuffer.clear();
    polybuffer.resize(nSamples);

	for(unsigned int j = 0; j<soundModelList.size(); j++) {
		temp=soundModelList[j]->getSamples(nSamples);

		for (unsigned int i = 0; i < nSamples; i++) {
			polybuffer[i] += temp[i] * master_vol;
		}
	}
	return polybuffer;
}

void SoundModelPoly::setNoteOn(int midinote) { //add note priority here
	int active=0;
	for(unsigned int i = 0; i < soundModelList.size(); i++)
		if (soundModelList[i]->isPlaying()){
			active++;
		}

	if (active==VoiceNo){ //if all voices are playing reset the oldest voice to the new note
		soundModelList[lastSoundModel[0]]->setNoteOn(midinote);
		midiNoteList[lastSoundModel[0]]=midinote;
	}

	for(unsigned int i = 0; i < soundModelList.size(); i++)		
		if(!(soundModelList[i]->isPlaying())) {
			soundModelList[i]->setNoteOn(midinote);
			midiNoteList[i]=midinote;
			lastSoundModel.push_back(i);

			return;
		}
}

void SoundModelPoly::setNoteOff(int midinote) {

	for(unsigned int i = 0; i < soundModelList.size(); i++)
		if (midiNoteList[i]==midinote){
			soundModelList[i]->setNoteOff(midinote);
			//position = std::find(lastSoundModel.begin(), lastSoundModel.end(), i); //does that work to get the position 
			std::vector<int>::iterator it = std::find(lastSoundModel.begin(), lastSoundModel.end(), i);
			position = std::distance(lastSoundModel.begin(), it);
			lastSoundModel.erase(lastSoundModel.begin()+position);
		}

}

bool SoundModelPoly::isPlaying() {

	for(unsigned int i = 0; i < soundModelList.size(); i++) {
		if(soundModelList[i]->isPlaying()) {
			return true;
		}
			
	}

	return false;
}



	// Dezipper the audio output by changing the output gain
	// progressively along the outbut buffer length
	//double gain_step { (double)(target_gain-gain)/bufferSize };
	//for(i = 0; i < bufferSize; i++) {
	//	samples[i] = accumulator[i] * gain;
	//	gain += gain_step;
	//}
	//gain = target_gain;