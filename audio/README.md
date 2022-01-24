# Drum Banks

The wave banks and samples contained in this directory are completely 
modifiable, deletable and reorderable, and you can add your own banks and 
samples as long as they follow a simple structure.
Banks and samples are read at runtime, so you do not need to recompile the
DrumPi for changes to take effect.

## Banks:
- A 'bank' is simply a directory of audio files.
- There can be as many or as few banks as you want, but `bank1` must
  always exist.
- Banks must have the name `bankX`, where `X` is a positive number.
    - This number is what will show on the DrumPi's display when 'banking'.
    - Banks should be consecutively numbered, otherwise you won't be able to
      reach some of them!
- Banks must be in this directory (DrumPi/audio/).
    - Banks in other directories (inc. subdirectories) will not be read.
- Banks must contain (at least) 4 .wav files with the names detailed below.

## Waves:
- A 'wave' is a .wav (audio) file.
- Each wav file should be a single cycle waveform recorded at 48kHz playing at A2=110Hz.
- Each bank must contain 4+ .wav files, with the names `wave1.wav`, `wave2.wav`
  etc, up to `WaveN.wav`.
- These waves are played at a frequency dictated by the midi note number and individual samples of the wave files are interpolated according to the morph parameter (mapped to a physical midi CC control)
    - E.g. the midi note number 33 is played which sets the playing speed of the wav files in the active bank to A0=22.5Hz
      then the morph paramater ranges 1 to N (number of wave files)  and its value set the amount of interpolation between the different parallel wave samples; (eg if morph =  only wav file 1 is heard, if morph =1.6 then an interpolation is made with 0.4 weight to wav 1 and 0.6 weight to wav 2 and so on).
