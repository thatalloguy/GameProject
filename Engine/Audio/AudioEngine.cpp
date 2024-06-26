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
    destroySoundBufferCache(bufferInfo);
    iplContextRelease(&_context);
}

void Quack::AudioEngine::processEffect(unsigned int soundId, AudioBuffer& in) {
    // NOTE im unsure if all of this allocation every frame is a good idea :| // UPDATE it was indeed not a good idea ;-;
    // NOTE NOTE we can do all this processing only once if you have something like registering the sound to the audioEngine.

    IPLBinauralEffectParams  params{};

    //make it come from the right?
    params.direction = IPLVector3{0.0f, 0.0f, 0.8f};
    params.hrtf = bufferInfo.hrtf;
    params.interpolation = IPL_HRTFINTERPOLATION_NEAREST;
    params.spatialBlend = 1.0f;
    params.peakDelays = nullptr;

    iplBinauralEffectApply(bufferInfo.effect, &params, &bufferInfo.inBuffer, &bufferInfo.outBuffer);


    in.data = *bufferInfo.outBuffer.data;
}

unsigned int Quack::AudioEngine::registerSound(Quack::SoundCreateInfo &info) {

    bufferInfo.inBuffer.numChannels = 1;
    bufferInfo.inBuffer.numSamples = info.length;

    //iplAudioBufferAllocate(_context, info.channel, info.length, &bufferInfo.inBuffer);
    iplAudioBufferAllocate(_context, info.channel, info.length, &bufferInfo.outBuffer);


    //iplAudioBufferDeinterleave(_context, in.data, &outBuffer);

    IPLAudioSettings audioSettings{};
    audioSettings.samplingRate = 44100; // What the hell is this value?
    audioSettings.frameSize = info.length;

    IPLHRTFSettings hrtfSettings;
    hrtfSettings.type = IPL_HRTFTYPE_DEFAULT;
    hrtfSettings.normType = IPL_HRTFNORMTYPE_NONE;
    hrtfSettings.volume = 1.0f;

    hrtfSettings.sofaData = nullptr;
    hrtfSettings.sofaFileName = nullptr;


    iplHRTFCreate(_context, &audioSettings, &hrtfSettings, &bufferInfo.hrtf);

    if (bufferInfo.hrtf ==  nullptr) {
        spdlog::error("Could not create HRTF");
        exit(-104);
    }


    //Convert SDLmixer interleave audio to deinterleave for steamaudio to use.


    //create a new effect/
    IPLBinauralEffectSettings  effectSettings{};
    effectSettings.hrtf = bufferInfo.hrtf;


    iplBinauralEffectCreate(_context, &audioSettings, &effectSettings, &bufferInfo.effect);


    spdlog::info("Cooked data for soundeffect");
    return idCount++;
}

void Quack::AudioEngine::destroySoundBufferCache(Quack::SoundCache &soundBufferInfo) {
    iplAudioBufferFree(_context, &soundBufferInfo.inBuffer);
    iplAudioBufferFree(_context, &soundBufferInfo.outBuffer);
    iplBinauralEffectRelease(&soundBufferInfo.effect);
    iplHRTFRelease(&soundBufferInfo.hrtf);
}
