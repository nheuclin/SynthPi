
#include <iostream>

#ifndef ADSR
#define ADSR

// for array for lookup table
#define ARRAY_SIZE 1024					// number of time points
#define ATTACK_ALPHA 0.995			// varies between 0.9 (steep curve) and 0.9995 (straight line)
#define ATTACK_DECAY_RELEASE 0.997			// fits to ARRAY_SIZE 1024


// Midi trigger -> on/off
class adsr
{
	public:
		// constructor
		adsr(int SampleRate);

		//destructor
		~adsr();

		void setAttack(int AttackCC);
		void setDecay(int DecayCC);
		void setSustain(int SustainCC));
		void setRelease(int ReleaseCC);

		void noteOn();
		void noteOff();

		std::vector<sample_t> getWave(nSamples); /*sample_t is float*/

	private:
		int _attack_table[ARRAY_SIZE];
		int _decay_release_table[ARRAY_SIZE];

		int _vertical_resolution;				// number of bits for output, control, etc
		unsigned long _attack = 0;								// 0 to 20 sec
		unsigned long _decay = 0;									// 1ms to 60 sec
		int _sustain = 0;								// 0 to -60dB -> then -inf
		unsigned long _release = 0;								// 1ms to 60 sec

		// time stamp for note on and note off
		unsigned long _t_note_on = 0;
		unsigned long _t_note_off = 0;

		// internal values needed to transition to new pulse (attack) and to release at any point in time
		int _adsr_output;
		int _release_start;
		int _attack_start;
		int _notes_pressed = 0;
};

#endif