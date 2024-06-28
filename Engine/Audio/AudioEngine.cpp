#pragma once
#include "AudioEngine.h"
#include <spdlog/spdlog.h>


#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

void Quack::AudioEngine::binauralEffectProccessPCMFrames(ma_node *pNode, const float **ppFramesIn, ma_uint32 *pFrameCountIn, float **ppFramesOut, ma_uint32 *pFrameCountOut)  {
    BinauralEffect* binauralEffect = (BinauralEffect*)pNode;
    IPLBinauralEffectParams  binauralEffectParams;
    IPLAudioBuffer inputBufferDesc;
    IPLAudioBuffer outputBufferDesc;
    ma_uint32 totalFramesToProcess = *pFrameCountOut;
    ma_uint32 totalFramesProcessed = 0;

    binauralEffectParams.direction.x = binauralEffect->direction.x;
    binauralEffectParams.direction.y = binauralEffect->direction.y;
    binauralEffectParams.direction.z = binauralEffect->direction.z;
    binauralEffectParams.interpolation = IPL_HRTFINTERPOLATION_NEAREST;
    binauralEffectParams.spatialBlend = 1.0f;
    binauralEffectParams.hrtf = binauralEffect->HRTF;
    binauralEffectParams.peakDelays = nullptr;

    inputBufferDesc.numChannels = (IPLint32) ma_node_get_input_channels(pNode, 0);

    //use a loop in case the deinterleaved buffers are too small
    outputBufferDesc.numSamples = binauralEffect->audioSettings.frameSize;
    outputBufferDesc.numChannels = 2;
    outputBufferDesc.data = binauralEffect->outBuffer;

    while (totalFramesProcessed < totalFramesToProcess) {

        ma_uint32  framesToProcessThisIteration = totalFramesToProcess - totalFramesProcessed;
        if  (framesToProcessThisIteration > (ma_uint32)binauralEffect->audioSettings.frameSize) {
            framesToProcessThisIteration = (ma_uint32)binauralEffect->audioSettings.frameSize;
        }

        if (inputBufferDesc.numChannels == 1) {
            // no need for deinterleaving since its a mono stream.
            binauralEffect->inBuffer[0] = (float*) ma_offset_pcm_frames_const_ptr_f32(ppFramesIn[0], totalFramesProcessed, 1);
        } else {
            // womp womp we need to deinterleave the input stream.
            ma_deinterleave_pcm_frames(ma_format_f32, inputBufferDesc.numChannels, framesToProcessThisIteration,
                                       ma_offset_pcm_frames_const_ptr_f32(ppFramesIn[0], totalFramesProcessed, inputBufferDesc.numChannels),
                                       (void **) binauralEffect->inBuffer); // note the example doenst cast to (void **)
        }

        inputBufferDesc.data = binauralEffect->inBuffer;
        inputBufferDesc.numSamples = (IPLint32) framesToProcessThisIteration;
        // spannend
        iplBinauralEffectApply(binauralEffect->iplEffect, &binauralEffectParams, &inputBufferDesc, &outputBufferDesc);

        ma_interleave_pcm_frames(
                ma_format_f32,
                2,
                framesToProcessThisIteration,
                (const void **) binauralEffect->outBuffer, // note the example also doenst cast anything here.
                ma_offset_pcm_frames_ptr_f32(ppFramesOut[0], totalFramesProcessed, 2)
        );


        totalFramesProcessed += framesToProcessThisIteration;
    }

    (void)pFrameCountIn; // unused?

}

Quack::BinauralEffectConfig Quack::AudioEngine::initBinauralEffectConfig(ma_uint32 channelsIn, IPLAudioSettings audioSettings, IPLContext context, IPLHRTF hrtf)  {
    BinauralEffectConfig config;

    MA_ZERO_OBJECT(&config);
    config.nodeConfig = ma_node_config_init();
    config.channelsIn = channelsIn;
    config.audioSettings = audioSettings;
    config.context = context;
    config.HRTF = hrtf;

    return config;
}

ma_result Quack::AudioEngine::initBinauralEffect(ma_node_graph* nodeGraph, const BinauralEffectConfig* config, const ma_allocation_callbacks* allocationCallbacks, BinauralEffect& binauralEffect) {
    ma_result result;
    ma_node_config baseConfig;
    ma_uint32 channelsIn;
    ma_uint32 channelsOut;
    IPLBinauralEffectSettings iplBinauralEffectSettings;
    size_t heapSizeInBytes;

    MA_ZERO_OBJECT(&binauralEffect);

    if (config == NULL || !(config->channelsIn == 2 || config->channelsIn == 1)) {
        return MA_INVALID_ARGS;
    }

    channelsIn = config->channelsIn;
    channelsOut = 2; // must be stereo

    baseConfig = ma_node_config_init();
    baseConfig.vtable = &this->binauralNodeVTable;
    baseConfig.pInputChannels = &channelsIn;
    baseConfig.pOutputChannels = &channelsOut;


    binauralEffect.audioSettings = config->audioSettings;
    binauralEffect.context       = config->context;
    binauralEffect.HRTF          = config->HRTF;

    MA_ZERO_OBJECT(&iplBinauralEffectSettings);
    iplBinauralEffectSettings.hrtf = binauralEffect.HRTF;

    result = ma_node_init(nodeGraph, &baseConfig, allocationCallbacks, &binauralEffect.baseNode);
    if (result != MA_SUCCESS) {
        return result;
    }



    auto success = iplBinauralEffectCreate(binauralEffect.context, &binauralEffect.audioSettings, &iplBinauralEffectSettings, &binauralEffect.iplEffect);
    if (success != IPL_STATUS_SUCCESS) {
        return MA_INVALID_DATA;
    }

    heapSizeInBytes = 0;

    heapSizeInBytes += sizeof(float) * channelsOut * binauralEffect.audioSettings.frameSize; // out buffer
    heapSizeInBytes += sizeof(float) * channelsIn  * binauralEffect.audioSettings.frameSize; // in buffer

    binauralEffect._heap = ma_malloc(heapSizeInBytes, allocationCallbacks);
    if (binauralEffect._heap == NULL) {
        iplBinauralEffectRelease(&binauralEffect.iplEffect);
        ma_node_uninit(&binauralEffect.baseNode, allocationCallbacks);
        return MA_OUT_OF_MEMORY;
    }

    binauralEffect.outBuffer[0] = (float*)binauralEffect._heap;
    binauralEffect.outBuffer[1] = (float*)ma_offset_ptr(binauralEffect._heap, sizeof(float) * binauralEffect.audioSettings.frameSize);

    ma_uint32 iChannelIn;
    for (iChannelIn = 0; iChannelIn < channelsIn; iChannelIn += 1) {
        binauralEffect.inBuffer[iChannelIn] = (float*)ma_offset_ptr(binauralEffect._heap, sizeof(float) * binauralEffect.audioSettings.frameSize * (channelsOut + iChannelIn));
    }

    return MA_SUCCESS;
}

void Quack::AudioEngine::destroyBinauralEffect(Quack::BinauralEffect *binauralEffect,
                                               const ma_allocation_callbacks *allocationCallbacks) {
    if (binauralEffect == NULL) {
        spdlog::error("CPP IS TWEAKING IS SWEAR TO GOD");
        return;
    }

    ma_node_uninit(&binauralEffect->baseNode, allocationCallbacks);

    iplBinauralEffectRelease(&binauralEffect->iplEffect);

    ma_free(binauralEffect->_heap, allocationCallbacks);

}

void Quack::AudioEngine::Init() {
    initializeMiniAudioObjects();
    initializeSteamAudioObjects();
}

void Quack::AudioEngine::CleanUp() {
    destroySteamAudioObjects();
    destroyMiniAudioObjects();
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

    ma_sound_uninit(&g_sound);
    destroyBinauralEffect(&g_binauralEffect, nullptr);
    ma_engine_uninit(&engine);

}

Quack::SoundID Quack::AudioEngine::registerSound(SoundCreationInfo info) {
    ma_sound_config soundConfig;

    soundConfig = ma_sound_config_init();
    soundConfig.pFilePath = info.filePath;
    soundConfig.flags = MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT;
    result = ma_sound_init_ex(&engine, &soundConfig, &g_sound);
    if (result != MA_SUCCESS) {
        return -108;
    }

    // No need for miniaudio to do this, since steam audio does this already
    ma_sound_set_directional_attenuation_factor(&g_sound, 0);

    ma_sound_set_looping(&g_sound, info.shouldLoop);




    // add binauralEffect
    BinauralEffectConfig binauralEffectConfig;

    binauralEffectConfig = initBinauralEffectConfig(CHANNELS, iplAudioSettings, iplContext, iplHRTF);

    result = initBinauralEffect(ma_engine_get_node_graph(&engine), &binauralEffectConfig, NULL, g_binauralEffect);

    if (result != MA_SUCCESS) {
        spdlog::error("Could not initialize binaural Effect: {}", result);
        return -1;
    }

    ma_node_attach_output_bus(&g_binauralEffect, 0, ma_engine_get_endpoint(&engine), 0);

    ma_node_attach_output_bus(&g_sound, 0, &g_binauralEffect, 0);

}

void Quack::AudioEngine::playSound(Quack::SoundID id) {
    ma_sound_start(&g_sound);
}

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

        g_binauralEffect.direction.x = direction.x;
        g_binauralEffect.direction.y = direction.y;
        g_binauralEffect.direction.z = direction.z;
        angle += stepAngle;

        spdlog::info("D {} {} {}", direction.x, direction.y, direction.z);

        ma_sleep(1);
        if (i > 1000) {
            break;
        }
        i++;
    };


}
