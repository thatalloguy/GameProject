//
// Created by allos on 6/26/2024.
//

#include <cstdlib>
#include <spdlog/spdlog.h>
#include "AudioEngine.h"

void Quack::AudioEngine::initialize() {

    //INitialize SteamAudio
    IPLContextSettings contextSettings{};

    contextSettings.version = STEAMAUDIO_VERSION;

    _context = nullptr;

    IPLerror errorCode = iplContextCreate(&contextSettings, &_context);
    if (errorCode) {
        spdlog::error("Could not initialize steamaudio: {}", errorCode);
        exit(-103);
    }



}

void Quack::AudioEngine::destroy() {
    iplContextRelease(&_context);
}

void Quack::AudioEngine::processEffect(unsigned int soundId, AudioBuffer& in) {



    // NOTE im unsure if all of this allocation every frame is a good idea :|
    // NOTE NOTE we can do all this processing only once if you have something like registering the sound to the audioEngine.

    //First convert Quack::AudioBuffer.data to Deinterleaved format (Required by steam audio).
    float* inData = in.data;

    //copy data to phonon abuf.
    IPLAudioBuffer tempBuffer;
    tempBuffer.data = &in.data;
    tempBuffer.numSamples = in.length;

    iplAudioBufferAllocate(_context, in.channel, in.length, &tempBuffer);


    IPLAudioSettings audioSettings{};
    audioSettings.samplingRate = 44100; // What the hell is this value?
    audioSettings.frameSize = in.length;

    IPLHRTFSettings hrtfSettings;
    hrtfSettings.type = IPL_HRTFTYPE_DEFAULT;

    IPLHRTF hrtf = nullptr;
    iplHRTFCreate(_context, &audioSettings, &hrtfSettings, &hrtf);

    if (!hrtf) {
        spdlog::error("Could not create HRTF");
        exit(-104);
    }


    //Convert SDLmixer interleave audio to deinterleave for steamaudio to use.
    iplAudioBufferDeinterleave(_context, inData, &tempBuffer);



    //destroy
    iplAudioBufferFree(_context, &tempBuffer);

}
