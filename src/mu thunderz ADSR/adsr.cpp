#include "adsr.h"
#include <math.h>

adsr::adsr(int samplerate)
{
	float attack = 0.1;												// take 100ms as initial value for Attack
	float sustain = 0.75;											// take half the DAC_size as initial value for sustain
	float decay = 0.1;												// take 100ms as initial value for Decay
	float release = 0.2;											// take 100ms as initial value for Release

	for (int i = 0; i<ARRAY_SIZE; i++) {												// Create look-up table for Attack
		_attack_table[i] = i;
		_decay_release_table[i] = _vertical_resolution - 1 - i;
	}

	for (int i = 0; i<ARRAY_SIZE-1; i++) {											// Create look-up table for Decay
		_attack_table[i+1] = (1.0 - ATTACK_ALPHA) * (_vertical_resolution - 1) + ATTACK_ALPHA * _attack_table[i];
		_decay_release_table[i+1] = ATTACK_DECAY_RELEASE * _decay_release_table[i];
	}

	for (int i = 0; i<ARRAY_SIZE; i++) {												// normalize table to min and max
		_attack_table[i] = map(_attack_table[i], 0, _attack_table[ARRAY_SIZE-1], 0, _vertical_resolution - 1);
		_decay_release_table[i] = map(_decay_release_table[i], _decay_release_table[ARRAY_SIZE-1], _decay_release_table[0], 0, _vertical_resolution - 1);
	}
}

void adsr::setAttack(int AttackCC)
{
	attack = (AttackCC/127)*100;
}

void adsr::setDecay(int DecayCC)
{
	decay = (DecayCC/127)*100;
}

void adsr::setSustain(int SustainCC)
{
	sustain = (SustainCC/127)*100;
}

void adsr::setRelease(int ReleaseCC)
{
	release = (ReleaseCC/127)*100;
}

void adsr::noteOn() {
	_t_note_on = l_micros;							// set new timestamp for note_on
	_attack_start = _adsr_output;				// set start value new Attack
	_notes_pressed++;										// increase number of pressed notes with one
}

void adsr::noteOff() {
	_notes_pressed--;
	if(_notes_pressed <= 0) {						// if all notes are depressed - start release
		_t_note_off = l_micros;						// set timestamp for note off
		_release_start = _adsr_output;		// set start value for release
		_notes_pressed = 0;
	}
}


std::vector<sample_t> getWave(nSamples){

}

int adsr::getWave(unsigned long l_micros)
{
	unsigned long delta = 0;
	if(_t_note_off < _t_note_on)		{			// if note is pressed
		delta = l_micros - _t_note_on;
		if(delta < _attack)									// Attack
			_adsr_output = map(_attack_table[(int)floor(ARRAY_SIZE * (float) delta / (float)_attack)], 0, _vertical_resolution - 1, _attack_start, _vertical_resolution - 1);		//
		else if(delta < _attack + _decay) { // Decay
			delta = l_micros - _t_note_on - _attack;
			_adsr_output = map(_decay_release_table[(int)floor(ARRAY_SIZE * (float) delta / (float) _decay)], 0, _vertical_resolution - 1, _sustain, _vertical_resolution - 1);
		}
		else
			_adsr_output = _sustain;
	}
	if(_t_note_off > _t_note_on)		{			// if note not pressed
		delta = l_micros - _t_note_off;
		if(delta < _release)								// release
			_adsr_output = map(_decay_release_table[(int)floor(ARRAY_SIZE * (float) delta / (float) _release)], 0, _vertical_resolution - 1, 0, _release_start);
		else
			_adsr_output = 0;									// note off
	}
	return _adsr_output;
}
