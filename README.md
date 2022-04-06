# Realtime Wavetable Synth (SynthPi)
SynthPi is an open source Realtime Wavetable Synthesizer which runs on Raspberry Pi. It is made for the Realtime Embedded Programming course as part of our Master of Engineering in Electronics with Music at the University of Glasgow (year 2022). You can find an installation and configuration guide on the other end of this link here: https://youtu.be/ZuCbzoPGmr0

# Credits 
Authors: Noe Heuclin and Aaron Collins.

Credits are due to former students from the DrumPi project (https://github.com/Quickeman/DrumPi) as well as Nick Bailey and his team of old students for their SmurdyGurdy (https://github.com/nickbailey/smrgygurdy) Project. Our code is based and adapted from several files belonging to both projects. 
This code also make use of the following libraries without any modification :
    - Adam Stark's AudioFile library, used to load in wavetable to our DSP engine (https://github.com/adamstark/AudioFile).
    - ADSR files from Nigel Redmon at EarLevel Engineering: earlevel.com, used to generate the envelopes for the SynthPi project. More info at: http://www.earlevel.com/main/2013/06/01/envelope-generators/

# Documentation 

run "sudo apt-get install git cmake libjack-jackd2-dev qjackctl libasound-dev libboost-dev"

run "git clone https://github.com/nheuclin/SynthPi"

"cd SynthPi"
"cmake ."
"make -j"
"chmod u+x SynthPi"

use "aconnect-l" to list the ID/port of connected devices, note down the ID and port number (port number is usually 0) of your midi keyboard. 

use "aseqdump -p xxx" where xxx is the midi ID number to get the ID numbers of the physical midi control changes you want to use to control each of the SYNTHPI's parameters. You can then edit config.json with your midi Keyboard ID and Port number as well as the ID numbers for each CC.

To edit config.json, navigate to the SynthPi folder and run "nano config.json"  
when done editing type ctrl+x then Y and enter

You're ready to go! run "./SynthPi" and enjoy your wavetable synthesizer.

