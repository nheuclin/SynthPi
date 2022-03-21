#include "SoundModelMono.h"
#include "SoundModelPoly.h"
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace SYNTHPI;
using namespace audio;

SoundModelPoly::SoundModelPoly(const int poly, const int samplerate) {
	VoiceNo=poly;
	for (int i = 0; i < VoiceNo; i++) {
		soundModelList.push_back(new SoundModelMono);

	}
	//std::cout << "SMP created" << std::endl;
}

std::vector<sample_t> SoundModelPoly::getSamples(int nSamples){

    std::vector<sample_t> temp(nSamples);
    
	// Clear object polybuffer and set the size
    polybuffer.clear();
    polybuffer.resize(nSamples);

	for(unsigned int j = 0; j<soundModelList.size(); j++) {
		temp=soundModelList[j]->getSamples(nSamples);

		for (unsigned int i = 0; i < nSamples; i++) {
			polybuffer[i] += temp[i];
		}
	}
		
	// Dezipper the audio output by changing the output gain progressively along the outbut buffer length,
	float gain_step = (target_vol-master_vol)/nSamples;

	for(unsigned int i = 0; i < nSamples; i++) {

		polybuffer[i] = polybuffer[i] * master_vol;

		master_vol += gain_step;
	}
	master_vol = target_vol;
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
		return;
	}

	for(unsigned int i = 0; i < soundModelList.size(); i++){
		
		if(soundModelList[i]->isPlaying()==false) {
			soundModelList[i]->setNoteOn(midinote);
			midiNoteList[i]=midinote;
			lastSoundModel.push_back(i);
			return;
		}
	}
	return;
}

void SoundModelPoly::setNoteOff(int midinote) {

	for(unsigned int i = 0; i < midiNoteList.size(); i++){
		if (midiNoteList[i]==midinote){
			
			soundModelList[i]->setNoteOff(midinote);
			std::vector<int>::iterator it = std::find(lastSoundModel.begin(), lastSoundModel.end(), i);
			position = std::distance(lastSoundModel.begin(), it);
			lastSoundModel.erase(lastSoundModel.begin()+position);
			return;
		}
	}
	//return;
}

bool SoundModelPoly::isPlaying() {
	for(unsigned int i = 0; i < soundModelList.size(); i++) {
		if(soundModelList[i]->isPlaying()) {
			return true;
		}			
	}
	return false;
}


void SoundModelPoly::updateVolume(unsigned int parameter) {
	target_vol=0.4*(static_cast<float>(parameter)/127.0);
}

void SoundModelPoly::updateWavemix(unsigned int parameter){
	for (unsigned int i=0; i<soundModelList.size();i++){
		soundModelList[i]->updateWavemix(parameter);
	}
}

void SoundModelPoly::updateBank(unsigned int parameter) {
		for (unsigned int i=0; i<soundModelList.size();i++){
		soundModelList[i]->updateBank(parameter);
	}
}