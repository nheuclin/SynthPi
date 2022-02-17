#include "audioLibrary.hpp"

using namespace SYNTHPI;
using namespace audio;

AudioLibrary::AudioLibrary() {
    audioDir = "audio/";

    bankDirPre = "bank";

    waveNamePre = "wave";

    extensions[SOURCE_GENERALISED] = "";
    extensions[SOURCE_PREGENERATED] = ".wav";
}

std::string AudioLibrary::getFilepath(WaveID_t wave, int bank, sampleSourceType_t type) {
    return
        SYNTHPI_DIR // SYNTHPI root directory
        + audioDir // Audio directory
        + bankDirPre + std::to_string(bank) + "/" // Bank directory
        + waveNamePre + std::to_string(wave + 1) // File name
        + extensions[type] // File extension
    ;
}