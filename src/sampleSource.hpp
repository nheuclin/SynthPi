// File: sampleSource.hpp
#ifndef SYNTHPI_SAMPLE_H
#define SYNTHPI_SAMPLE_H

#include <string>
#include <vector>

#include "defs.hpp"

namespace SYNTHPI {
namespace audio {

/*! Abstract class for sample retieval. */
class SampleSource {
    public:
        /*! Constructor. */
        SampleSource();

        /*! Returns a buffer of samples.
        \param nSamples number of samples to be returned.
        \param index_increment the floating point "perfect" increment to go through the single cycle 
        waveform at the right speed.
        \return a sample buffer of length nSamples. */
        virtual std::vector<sample_t> getSamples(int nSamples, double index_increment) = 0;

        /*! Resets the source to initial conditions. */
        virtual void reset() = 0;

        /*! Updates the status of the source. */
        virtual void updateStatus() = 0;

        /*! returns number of samples in loaded clip */

        virtual int getNumSamples() =0;
        
        /*! Returns the status of the source.
        \return status code of source. */
        sampleSourceStatus_t getStatus();

        /*! Returns the type of source represented by the object.
        \return type code of source */
        sampleSourceType_t getType();

    protected:
        /*! Status of the source. */
        sampleSourceStatus_t Src_status;
        /*! Type of source. */
        sampleSourceType_t type;
};


/*! Specialised abstract class for file-based sample sources, e.g. wave files. */
class SampleSourceFile : public SampleSource {
    protected:
        /*! Loads the specified file.
        \param filepath file path of the file to load. */
        virtual void loadFile(std::string filepath) = 0;

        /*! File path used for loading. */
        std::string filepath;
};


/*! Handler class for pre-generated single cycle waveforms. */
class AudioClip : public SampleSourceFile {
    public:
        /*! Class constructor.
        \param filepath the absolute file path of an audio file. */
        AudioClip(std::string filepath);

        /*! Returns a buffer of samples.
        \param nSamples number of samples to be returned.
        \return a sample buffer of length nSamples. */
        std::vector<sample_t> getSamples(int nSamples, double index_increment);

        /*! Halts playback and returns playhead to start of clip. */
        void reset() override;

        /*! Updates the status of the source. */
        void updateStatus() override;

        /*! Like \ref reset but the clip is completely re-loaded.
        Not recommended for real-time use, but may be useful to recover from
        errors. */
        void hardReset();

        /*! returns number of samples in clip 
        \return NumSamples */
        int getNumSamples() override;


    private:
        /*! Loads the specified file.
        \param filepath file path of the file to load. */
        void loadFile(std::string filepath) override;

        /*! Container for the audio clip. */
        std::vector<sample_t> clip;

        /*! Number of samples in the audio clip. */
        int numSamples;

        /*! The number of "virtual" samples of playback elapsed. */
        float playhead;

        /*! The integrer part of the number of samples of playback elapsed. */
        int int_playhead;

        /*! 1-decimal part of playhead. */
        float interpolation_val1;

        /*! decimal part of playhead. */
        float interpolation_val2;

};

} // namespace audio
} // namespace SYNTHPI

#endif // define SYNTHPI_SAMPLE_H