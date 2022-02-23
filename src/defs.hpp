// File: defs.hpp
#ifndef SYNTHPI_DEFS_H
#define SYNTHPI_DEFS_H

/*! Use this file to define types, constants etc that are needed in the program.
DO NOT instantiate any objects, variables etc here. */

#include <jack/jack.h>

namespace SYNTHPI {

/*! SYNTHPI project root directory.
Used for loading audio files where the absolute path is required. */
#define SYNTHPI_DIR PROJECT_ROOT_DIR

/*! ID numbers for each of the SYNTHPI's waves. */
typedef enum _WaveIDs {
    Wave_1 = 0,
    Wave_2,
    Wave_3,
    Wave_4,

    // Number of Waves
    // ALWAYS LEAVE LAST!
    _NUM_WAVES
} WaveID_t;

/*!number of available voices*/
int poly=8; //number of voices

/*! ID and port to connect midi keyboard to*/
int keyboard_ID=128;
int keyboard_port=0;

/*! ID numbers for each of the SYNTHPI's control changes. */
enum class ControlChanges {
    CC1 = 30, //output volume
    CC2 = 31, //avemix value
    CC3 = 32, //Attack value
    CC4 = 33, //Decay value
    CC5 = 34, //Sustain Value
    CC6 = 35, //Release value

    // Number of CCs
    // ALWAYS LEAVE LAST!
    _NUM_CCs
};

/*! The number of voices available in the SYNTHPI. */
#define NUM_VOICES (int)_WaveIDs::_NUM_VOICES

/*! ID labels for the SYNTHPI's operational modes. */
typedef enum _StateLabels {
    PLAY_MODE,
    SET_WAVE_BANK_MODE
} stateLabel_t;

/*! The namespace for audio system related items. */
namespace audio {

/*! Sample data format used by Jack (`float`). */
typedef jack_default_audio_sample_t sample_t;

/*! Identifies a type of sample source. */
typedef enum _SampleSourceTypes {
    /*! An abstract source, undefined. */
    SOURCE_GENERALISED,

    /*! A pregenerated wave file. */
    SOURCE_PREGENERATED,

    // Number of source types
    _NUM_SOURCE_TYPES
} sampleSourceType_t;

#define NUM_SOURCE_TYPES (int)_SampleSourceTypes::_NUM_SOURCE_TYPES

/*! Defines the status of a \ref SampleSource object. */
typedef enum _SampleSourceStatus {
    /*! The source is loading. */
    SOURCE_LOADING,

    /*! The source is idle and/or ready for output. */
    SOURCE_READY,

    /*! The source is being output or otherwise active. */
    SOURCE_ACTIVE,

    /*! The source has completed its output. */
    SOURCE_FINISHED,

    /*! There was an error with the source. */
    SOURCE_ERROR
} sampleSourceStatus_t;

/*! Error codes for the audio engine system. */
typedef enum _AudioError {
    /*! No error. */
    NO_ERROR = 0,

    /*! The Jack client could not be opened. */
    CLIENT_OPEN_FAILED,

    /*! The Jack client could not be activated. */
    CLIENT_ACTIVATE_FAILED,

    /*! The Jack client could not connect to a server. */
    SERVER_CONNECT_FAILED,

    /*! No ports were available to the Jack client. */
    NO_PORTS_AVAILABLE,

    /*! There are no physical ports connected to the device. */
    NO_PHYSICAL_PORTS,

    /*! Could not connect to a port. */
    PORT_CONNECT_FAILED,

    /*! The Jack client could not be deactivated. */
    CLIENT_DEACTIVATE_ERROR,
    
    /*! The Jack client could not be closed. */
    CLIENT_CLOSE_ERROR
} audioError_t;

} // namespace audio
} // namespace SYNTHPI

#endif
