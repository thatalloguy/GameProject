//
// Created by allos on 6/26/2024.
//

#ifndef GAME_AUDIOENGINE_H
#define GAME_AUDIOENGINE_H

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include <phonon.h>

#define FORMAT ma_format_32
#define CHANNELS 2
#define SAMPLE_RATE 48000

namespace Quack {

    typedef unsigned int SoundID;

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


    class AudioEngine {


    public:

        void Init();
        void CleanUp();

        SoundID registerSound(const char* filePath);

    private:

        static void binauralEffectProccessPCMFrames(ma_node* pNode, const float** ppFramesIn, ma_uint32* pFrameCountIn, float** ppFramesOut, ma_uint32* pFrameCountOut);

        BinauralEffectConfig initBinauralEffectConfig(ma_uint32 channelsIn, IPLAudioSettings audioSettings, IPLContext context, IPLHRTF hrtf);
        ma_result initBinauralEffect(ma_node_graph* nodeGraph, const BinauralEffectConfig* config, const ma_allocation_callbacks* allocationCallbacks, BinauralEffect& binauralEffect);

        void destroyBinauralEffect(BinauralEffect* binauralEffect, const ma_allocation_callbacks * allocationCallbacks);

        ma_node_vtable binauralNodeVTable {
            AudioEngine::binauralEffectProccessPCMFrames,
            NULL,
            1,
            1,
            0
        };





        // Dummy
        ma_sound g_sound;

    };



}// namespace quack


#endif //GAME_AUDIOENGINE_H
