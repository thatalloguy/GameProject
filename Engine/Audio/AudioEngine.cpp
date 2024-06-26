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

void Quack::AudioEngine::processEffect(unsigned int soundId, AudioBuffer& buffer) {

}
