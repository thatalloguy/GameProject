#include "Core/App.h"

#include <Audio/AudioEngine.h>
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#define FORMAT ma_format_32
#define CHANNELS 2
#define SAMPLE_RATE 48000


struct BinauralEffectConfig {
    ma_node_config nodeConfig;
    ma_uint32 channelsIn;
    IPLAudioSettings  audioSettings;
    IPLContext context;
    IPLHRTF HRTF;
};

struct BinauralEffect {
    ma_node_base baseNode;
    IPLAudioSettings  audioSettings;
    IPLContext context;
    IPLHRTF HRTF;
    IPLBinauralEffect  iplEffect;
    Quack::Math::Vector3 direction;
    float* inBuffer[2];
    float* outBuffer[2];
    void* _heap;
};

static void binauralEffectProccessPCMFrames(ma_node* pNode, const float** ppFramesIn, ma_uint32* pFrameCountIn, float** ppFramesOut, ma_uint32* pFrameCountOut) {
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

static ma_node_vtable binauralNodeVtable {
    binauralEffectProccessPCMFrames,
    NULL,
    1,
    1,
    0
};

BinauralEffectConfig initBinauralEffectConfig(ma_uint32 channelsIn, IPLAudioSettings audioSettings, IPLContext context, IPLHRTF hrtf) {

    BinauralEffectConfig config;

    MA_ZERO_OBJECT(&config);
    config.nodeConfig = ma_node_config_init();
    config.channelsIn = channelsIn;
    config.audioSettings = audioSettings;
    config.context = context;
    config.HRTF = hrtf;

    return config;
}

ma_result initBinauralEffect(ma_node_graph* nodeGraph, const BinauralEffectConfig* config, const ma_allocation_callbacks* allocationCallbacks, BinauralEffect* binauralEffect) {

    ma_result result;
    ma_node_config baseConfig;
    ma_uint32 channelsIn;
    ma_uint32 channelsOut;
    IPLBinauralEffectSettings iplBinauralEffectSettings;
    size_t heapSizeInBytes;

    MA_ZERO_OBJECT(&binauralEffect);

    if (binauralEffect == NULL || config == NULL ||config->channelsIn < 1 || config->channelsIn > 2) {
        return MA_INVALID_ARGS;
    }

    channelsIn = config->channelsIn;
    channelsOut = 2; // must be stereo

    baseConfig = ma_node_config_init();
    baseConfig.vtable = &binauralNodeVtable;
    baseConfig.pInputChannels = &channelsIn;
    baseConfig.pOutputChannels = &channelsOut;

    result = ma_node_init(nodeGraph, &baseConfig, allocationCallbacks, &binauralEffect->baseNode);
    if (result != MA_SUCCESS) {
        return result;
    }

    binauralEffect->audioSettings = config->audioSettings;
    binauralEffect->context       = config->context;
    binauralEffect->HRTF          = config->HRTF;

    MA_ZERO_OBJECT(&iplBinauralEffectSettings);
    iplBinauralEffectSettings.hrtf = binauralEffect->HRTF;

    auto success = iplBinauralEffectCreate(binauralEffect->context, &binauralEffect->audioSettings, &iplBinauralEffectSettings, &binauralEffect->iplEffect);
    if (success != IPL_STATUS_SUCCESS) {
        return MA_INVALID_DATA;
    }

    heapSizeInBytes = 0;

    heapSizeInBytes += sizeof(float) * channelsOut * binauralEffect->audioSettings.frameSize; // out buffer
    heapSizeInBytes += sizeof(float) * channelsIn  * binauralEffect->audioSettings.frameSize; // in buffer

    binauralEffect->_heap = ma_malloc(heapSizeInBytes, allocationCallbacks);
    if (binauralEffect->_heap == NULL) {
        iplBinauralEffectRelease(&binauralEffect->iplEffect);
        ma_node_uninit(&binauralEffect->baseNode, allocationCallbacks);
        return MA_OUT_OF_MEMORY;
    }

    binauralEffect->outBuffer[0] = (float*)binauralEffect->_heap;
    binauralEffect->outBuffer[1] = (float*)ma_offset_ptr(binauralEffect->_heap, sizeof(float) * binauralEffect->audioSettings.frameSize);

    ma_uint32 iChannelIn;
    for (iChannelIn = 0; iChannelIn < channelsIn; iChannelIn += 1) {
        binauralEffect->inBuffer[iChannelIn] = (float*)ma_offset_ptr(binauralEffect->_heap, sizeof(float) * binauralEffect->audioSettings.frameSize * (channelsOut + iChannelIn));
    }

    return MA_SUCCESS;
}

int main() {
/*
    App application{};


    application.init();

    application.run();

    application.cleanup();

*/

    ma_sound g_sound;
    BinauralEffect g_binauralEffect;

    ma_result result;
    ma_engine engine;
    ma_engine_config    engineConfig;
    IPLAudioSettings    iplAudioSettings;
    IPLContextSettings  iplContextSettings;
    IPLContext          iplContext;
    IPLHRTFSettings     iplhrtfSettings;
    IPLHRTF             iplHRTF;



    // load engine config
    engineConfig = ma_engine_config_init();
    engineConfig.channels = CHANNELS;
    engineConfig.sampleRate = SAMPLE_RATE;
    engineConfig.periodSizeInFrames = 256;

    result = ma_engine_init(&engineConfig, &engine);
    if (result != MA_SUCCESS) {
        return -105;
    }

    // Steam audio initialization.
    MA_ZERO_OBJECT(&iplAudioSettings);
    iplAudioSettings.samplingRate = ma_engine_get_sample_rate(&engine);

    iplAudioSettings.frameSize = engineConfig.periodSizeInFrames;

    MA_ZERO_OBJECT(&iplContext);

    iplContextSettings.version = STEAMAUDIO_VERSION;
    auto success = iplContextCreate(&iplContextSettings, &iplContext);
    if (success != IPL_STATUS_SUCCESS) {
        return -106;
    }


    //HRTF
    MA_ZERO_OBJECT(&iplhrtfSettings);
    iplhrtfSettings.type = IPL_HRTFTYPE_DEFAULT;

    success = iplHRTFCreate(iplContext, &iplAudioSettings, &iplhrtfSettings, &iplHRTF);

    if (success != IPL_STATUS_SUCCESS) {
        return -107;
    }

    // Sound Effect Config
    ma_sound_config soundConfig;

    soundConfig = ma_sound_config_init();
    soundConfig.pFilePath = "../Assets/Audio/bluebonnet_in_b_major_looped.wav";
    soundConfig.flags = MA_SOUND_FLAG_NO_DEFAULT_ATTACHMENT;
    result = ma_sound_init_ex(&engine, &soundConfig, &g_sound);
    if (result != MA_SUCCESS) {
        return -108;
    }

    // No need for miniaudio to do this, since steam audio does this already
    ma_sound_set_directional_attenuation_factor(&g_sound, 0);

    ma_sound_set_looping(&g_sound, MA_TRUE);


    // steam audio time! :D
    BinauralEffectConfig binauralEffectConfig;

    binauralEffectConfig = initBinauralEffectConfig(CHANNELS, iplAudioSettings, iplContext, iplHRTF);

    result = initBinauralEffect(ma_engine_get_node_graph(&engine), &binauralEffectConfig, NULL, &g_binauralEffect);

    if (result != MA_SUCCESS) {
        spdlog::error("Could not initialize binaural Effect");
        return -1;
    }

    ma_node_attach_output_bus(&g_binauralEffect, 0, ma_engine_get_endpoint(&engine), 0);

    ma_node_attach_output_bus(&g_sound, 0, &g_binauralEffect, 0);
    ma_sound_start(&g_sound);

    printf("Press Enter to quit..\n");
    getchar();


    ma_sound_uninit(&g_sound);

    ma_engine_uninit(&engine);



    return 0;
}

