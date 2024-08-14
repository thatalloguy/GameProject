#pragma once
#include "AudioEngine.h"
#include <spdlog/spdlog.h>


#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

void Quack::AudioEngine::soundEffectProccessPCMFrames(ma_node *pNode, const float **ppFramesIn, ma_uint32 *pFrameCountIn, float **ppFramesOut, ma_uint32 *pFrameCountOut)  {
    SoundEffect* soundEffect = (SoundEffect*)pNode;
    IPLBinauralEffectParams  binauralEffectParams;
    IPLAudioBuffer inputBufferDesc;
    IPLAudioBuffer outputBufferDesc;
    ma_uint32 totalFramesToProcess = *pFrameCountOut;
    ma_uint32 totalFramesProcessed = 0;

    binauralEffectParams.direction.x = soundEffect->direction.x;
    binauralEffectParams.direction.y = soundEffect->direction.y;
    binauralEffectParams.direction.z = soundEffect->direction.z;
    binauralEffectParams.interpolation = IPL_HRTFINTERPOLATION_NEAREST;
    binauralEffectParams.spatialBlend = soundEffect->spatialBlend;
    binauralEffectParams.hrtf = soundEffect->HRTF;
    binauralEffectParams.peakDelays = nullptr;


    IPLDirectEffectParams directEffectParams{};
    IPLDistanceAttenuationModel model{};

    model.type = IPL_DISTANCEATTENUATIONTYPE_DEFAULT;
    directEffectParams.flags = IPL_DIRECTEFFECTFLAGS_APPLYDISTANCEATTENUATION;
    directEffectParams.distanceAttenuation = iplDistanceAttenuationCalculate(soundEffect->context, IPLVector3{soundEffect->soundPosition.x, soundEffect->soundPosition.y, soundEffect->soundPosition.z}, IPLVector3{soundEffect->playerPosition.x, soundEffect->playerPosition.y, soundEffect->playerPosition.z}, &model);

    inputBufferDesc.numChannels = (IPLint32) ma_node_get_input_channels(pNode, 0);

    //use a loop in case the deinterleaved buffers are too small
    outputBufferDesc.numSamples = soundEffect->audioSettings.frameSize;
    outputBufferDesc.numChannels = 2;
    outputBufferDesc.data = soundEffect->outBuffer;

    while (totalFramesProcessed < totalFramesToProcess) {

        ma_uint32  framesToProcessThisIteration = totalFramesToProcess - totalFramesProcessed;
        if  (framesToProcessThisIteration > (ma_uint32)soundEffect->audioSettings.frameSize) {
            framesToProcessThisIteration = (ma_uint32)soundEffect->audioSettings.frameSize;
        }

        if (inputBufferDesc.numChannels == 1) {
            // no need for deinterleaving since its a mono stream.
            soundEffect->inBuffer[0] = (float*) ma_offset_pcm_frames_const_ptr_f32(ppFramesIn[0], totalFramesProcessed, 1);
        } else {
            // womp womp we need to deinterleave the input stream.
            ma_deinterleave_pcm_frames(ma_format_f32, inputBufferDesc.numChannels, framesToProcessThisIteration,
                                       ma_offset_pcm_frames_const_ptr_f32(ppFramesIn[0], totalFramesProcessed, inputBufferDesc.numChannels),
                                       (void **) soundEffect->inBuffer); // note the example doenst cast to (void **)
        }

        inputBufferDesc.data = soundEffect->inBuffer;
        inputBufferDesc.numSamples = (IPLint32) framesToProcessThisIteration;
        // spannend



        iplBinauralEffectApply(soundEffect->binauralEffect, &binauralEffectParams, &inputBufferDesc, &outputBufferDesc);

        iplDirectEffectApply(soundEffect->directEffect, &directEffectParams, &outputBufferDesc, &outputBufferDesc);
        // then apply the direct efffect?

        ma_interleave_pcm_frames(
                ma_format_f32,
                2,
                framesToProcessThisIteration,
                (const void **) soundEffect->outBuffer, // note the example also doenst cast anything here.
                ma_offset_pcm_frames_ptr_f32(ppFramesOut[0], totalFramesProcessed, 2)
        );


        totalFramesProcessed += framesToProcessThisIteration;
    }

    (void)pFrameCountIn; // unused?

}

Quack::SoundEffectConfig Quack::AudioEngine::initSoundEffectConfig(ma_uint32 channelsIn, IPLAudioSettings audioSettings, IPLContext context, IPLHRTF hrtf)  {
    SoundEffectConfig config;

    MA_ZERO_OBJECT(&config);
    config.nodeConfig = ma_node_config_init();
    config.channelsIn = channelsIn;
    config.audioSettings = audioSettings;
    config.context = context;
    config.HRTF = hrtf;

    return config;
}

ma_result Quack::AudioEngine::initSoundEffect(ma_node_graph* nodeGraph, const SoundEffectConfig* config, const ma_allocation_callbacks* allocationCallbacks, SoundEffect& soundEffect) {
    ma_result result;
    ma_node_config baseConfig;
    ma_uint32 channelsIn;
    ma_uint32 channelsOut;
    size_t heapSizeInBytes;

    IPLBinauralEffectSettings  effectSettings{};
    IPLDirectEffectSettings  directEffectSettings{};


    MA_ZERO_OBJECT(&soundEffect);

    if (config == NULL || !(config->channelsIn == 2 || config->channelsIn == 1)) {
        return MA_INVALID_ARGS;
    }

    channelsIn = config->channelsIn;
    channelsOut = 2; // must be stereo

    baseConfig = ma_node_config_init();
    baseConfig.vtable = &this->soundNodeVTable;
    baseConfig.pInputChannels = &channelsIn;
    baseConfig.pOutputChannels = &channelsOut;


    soundEffect.audioSettings = config->audioSettings;
    soundEffect.context       = config->context;
    soundEffect.HRTF          = config->HRTF;


    //Initialize Binaural effect
    effectSettings.hrtf = soundEffect.HRTF;


    directEffectSettings.numChannels = 2;

    result = ma_node_init(nodeGraph, &baseConfig, allocationCallbacks, &soundEffect.baseNode);
    if (result != MA_SUCCESS) {
        return result;
    }



    auto success = iplBinauralEffectCreate(soundEffect.context, &soundEffect.audioSettings, &effectSettings, &soundEffect.binauralEffect);
    if (success != IPL_STATUS_SUCCESS) {
        return MA_INVALID_DATA;
    }

    success = iplDirectEffectCreate(soundEffect.context, &soundEffect.audioSettings, &directEffectSettings, &soundEffect.directEffect);
    if (success != IPL_STATUS_SUCCESS) {
        return MA_INVALID_DATA;
    }

    heapSizeInBytes = 0;

    heapSizeInBytes += sizeof(float) * channelsOut * soundEffect.audioSettings.frameSize; // out buffer
    heapSizeInBytes += sizeof(float) * channelsIn * soundEffect.audioSettings.frameSize; // in buffer

    soundEffect._heap = ma_malloc(heapSizeInBytes, allocationCallbacks);
    if (soundEffect._heap == NULL) {
        iplBinauralEffectRelease(&soundEffect.binauralEffect);
        iplDirectEffectRelease(&soundEffect.directEffect);
        ma_node_uninit(&soundEffect.baseNode, allocationCallbacks);
        return MA_OUT_OF_MEMORY;
    }

    soundEffect.outBuffer[0] = (float*)soundEffect._heap;
    soundEffect.outBuffer[1] = (float*)ma_offset_ptr(soundEffect._heap, sizeof(float) * soundEffect.audioSettings.frameSize);

    ma_uint32 iChannelIn;
    for (iChannelIn = 0; iChannelIn < channelsIn; iChannelIn += 1) {
        soundEffect.inBuffer[iChannelIn] = (float*)ma_offset_ptr(soundEffect._heap, sizeof(float) * soundEffect.audioSettings.frameSize * (channelsOut + iChannelIn));
    }

    return MA_SUCCESS;
}

void Quack::AudioEngine::destroySoundEffect(SoundEffect *binauralEffect,
                                            const ma_allocation_callbacks *allocationCallbacks) {
    if (binauralEffect == NULL) {
        spdlog::error("CPP IS TWEAKING IS SWEAR TO GOD");
        return;
    }

    ma_node_uninit(&binauralEffect->baseNode, allocationCallbacks);

    iplBinauralEffectRelease(&binauralEffect->binauralEffect);
    iplDirectEffectRelease(&binauralEffect->directEffect);

    ma_free(binauralEffect->_heap, allocationCallbacks);

}

void Quack::AudioEngine::Init() {
    initializeMiniAudioObjects();
    initializeSteamAudioObjects();
}

void Quack::AudioEngine::CleanUp() {

    destroySteamAudioObjects();
    destroyMiniAudioObjects();


    for (auto soundObject : registry) {
        delete soundObject.second;
    }

}

void Quack::AudioEngine::initializeMiniAudioObjects() {
    // load engine config
    engineConfig = ma_engine_config_init();
    engineConfig.channels = CHANNELS;
    engineConfig.sampleRate = SAMPLE_RATE;
    engineConfig.periodSizeInFrames = 256;

    result = ma_engine_init(&engineConfig, &engine);
    if (result != MA_SUCCESS) {
        spdlog::info("Miniaudio initialization error: {}", ma_result_description(result));
        exit(-105);
    }


}

void Quack::AudioEngine::initializeSteamAudioObjects() {
    // Steam audio initialization.
    MA_ZERO_OBJECT(&iplAudioSettings);
    iplAudioSettings.samplingRate = ma_engine_get_sample_rate(&engine);

    iplAudioSettings.frameSize = engineConfig.periodSizeInFrames;



    iplContextSettings.version = STEAMAUDIO_VERSION;
    auto success = iplContextCreate(&iplContextSettings, &iplContext);
    if (success != IPL_STATUS_SUCCESS) {
        spdlog::info("steamaudio initialization error: {}",success);
        exit(-106);
    }


    //HRTF
    MA_ZERO_OBJECT(&iplhrtfSettings);
    iplhrtfSettings.type = IPL_HRTFTYPE_DEFAULT;
    iplhrtfSettings.volume = 1.0f;

    success = iplHRTFCreate(iplContext, &iplAudioSettings, &iplhrtfSettings, &iplHRTF);

    if (success != IPL_STATUS_SUCCESS) {
        spdlog::info("steamaudio initialization error: {}", success);
        exit(-107);
    }
}

void Quack::AudioEngine::destroySteamAudioObjects() {



    iplContextRelease(&iplContext);
    iplHRTFRelease(&iplHRTF);
}

void Quack::AudioEngine::destroyMiniAudioObjects() {


    for (auto soundObject : registry) {
        ma_sound_uninit(&soundObject.second->g_sound);
        destroySoundEffect(&soundObject.second->soundEffect, nullptr);
    }


    ma_engine_uninit(&engine);

}

Quack::SoundID Quack::AudioEngine::registerSound(SoundCreationInfo info) {
    idCounter++;
    SoundObject* newSoundObject = new SoundObject{};

    ma_sound_config soundConfig;

    soundConfig = ma_sound_config_init();
    soundConfig.pFilePath = info.filePath;
    soundConfig.flags = MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT;
    result = ma_sound_init_ex(&engine, &soundConfig, &newSoundObject->g_sound);
    if (result != MA_SUCCESS) {
        return -108;
    }

    // No need for miniaudio to do this, since steam audio does this already
    ma_sound_set_directional_attenuation_factor(&newSoundObject->g_sound, 0);

    ma_sound_set_looping(&newSoundObject->g_sound, info.shouldLoop);


    newSoundObject->soundEffect.spatialBlend = info.spatialBlend;

    // add binauralEffect
    SoundEffectConfig binauralEffectConfig;

    binauralEffectConfig = initSoundEffectConfig(CHANNELS, iplAudioSettings, iplContext, iplHRTF);

    result = initSoundEffect(ma_engine_get_node_graph(&engine), &binauralEffectConfig, NULL, newSoundObject->soundEffect);

    if (result != MA_SUCCESS) {
        spdlog::error("Could not initialize binaural Effect: {}", result);
        return -1;
    }

    ma_node_attach_output_bus(&newSoundObject->soundEffect, 0, ma_engine_get_endpoint(&engine), 0);

    ma_node_attach_output_bus(&newSoundObject->g_sound, 0, &newSoundObject->soundEffect, 0);
    registry.emplace(idCounter, newSoundObject);
    return idCounter;
}

void Quack::AudioEngine::updateSound(Quack::SoundID id, Quack::Math::Vector3 &playerPos, Quack::Math::Vector3 &soundDirection) {
    auto sound = registry[id];

    sound->soundEffect.playerPosition = playerPos;
    sound->soundEffect.direction = soundDirection;

}

void Quack::AudioEngine::playSound(Quack::SoundID id) {

    ma_sound_start(&registry[id]->g_sound);
}

void Quack::AudioEngine::setSoundPosition(Quack::SoundID id, Quack::Math::Vector3 newPos) {
    auto sound = registry[id];

    sound->soundEffect.soundPosition = newPos;
}
/*

void Quack::AudioEngine::doSillyDirectionTest() {


    float stepAngle = 0.002f;
    float angle = 0;
    float distance = 2;


    int i = 0;
    for (;;) {
        double x = ma_cosd(angle) - ma_sind(angle);
        double y = ma_sind(angle) + ma_cosd(angle);
        ma_vec3f direction;

        ma_sound_set_position(&g_sound, (float)x * distance, 0, (float)y * distance);
        direction = ma_sound_get_direction_to_listener(&g_sound);

        g_soundEffect.direction.x = direction.x;
        g_soundEffect.direction.y = direction.y;
        g_soundEffect.direction.z = direction.z;

        g_soundEffect.soundPosition.x += 0.02f;
        angle += stepAngle;

        spdlog::info("D {} {} {}", direction.x, direction.y, direction.z);

        ma_sleep(1);
        if (i > 1000) {
            break;
        }
        i++;
    };


}
*/
