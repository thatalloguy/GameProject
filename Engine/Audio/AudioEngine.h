//
// Created by allos on 6/26/2024.
//
#pragma once


#include <miniaudio.h>


#include <phonon.h>
#include "Math/Vecs.h"

#define FORMAT ma_format_32
#define CHANNELS 2
#define SAMPLE_RATE 48000

namespace Quack {

    typedef unsigned int SoundID;

    struct SoundEffectConfig {
        ma_node_config nodeConfig;
        ma_uint32 channelsIn;
        IPLAudioSettings  audioSettings;
        IPLContext context;
        IPLHRTF HRTF;
    };

    struct SoundEffect {
        ma_node_base baseNode;
        IPLAudioSettings  audioSettings;
        IPLContext context;
        IPLHRTF HRTF;
        IPLBinauralEffect binauralEffect;
        IPLDirectEffect  directEffect;
        Quack::Math::Vector3 direction;
        Quack::Math::Vector3 soundPosition;
        float* inBuffer[2];
        float* outBuffer[2];
        void* _heap;
    };

    struct SoundCreationInfo {
        const char* filePath = nullptr;
        bool shouldLoop = false;
        //todo add  steamaudio options here?
    };


    class AudioEngine {


    public:

        void Init();
        void CleanUp();

        SoundID registerSound(SoundCreationInfo info);

        void playSound(SoundID id);

        void doSillyDirectionTest();

    private:

        // lifetime functions
        void initializeSteamAudioObjects();
        void initializeMiniAudioObjects();

        void destroySteamAudioObjects();
        void destroyMiniAudioObjects();



        // helper functions
        static void soundEffectProccessPCMFrames(ma_node* pNode, const float** ppFramesIn, ma_uint32* pFrameCountIn, float** ppFramesOut, ma_uint32* pFrameCountOut);

        SoundEffectConfig initSoundEffectConfig(ma_uint32 channelsIn, IPLAudioSettings audioSettings, IPLContext context, IPLHRTF hrtf);
        ma_result initSoundEffect(ma_node_graph* nodeGraph, const SoundEffectConfig* config, const ma_allocation_callbacks* allocationCallbacks, SoundEffect& soundEffect);

        void destroySoundEffect(SoundEffect* binauralEffect, const ma_allocation_callbacks * allocationCallbacks);

        ma_node_vtable soundNodeVTable {
            AudioEngine::soundEffectProccessPCMFrames,
            NULL,
            1,
            1,
            0
        };


        // Dummy
        ma_sound g_sound;
        SoundEffect g_soundEffect;

        //MiniAudio Objects
        ma_result result;
        ma_engine engine;
        ma_engine_config    engineConfig;

        // SteamAudio objects
        IPLAudioSettings    iplAudioSettings;
        IPLContextSettings  iplContextSettings{};
        IPLContext          iplContext = nullptr;
        IPLHRTFSettings     iplhrtfSettings;
        IPLHRTF             iplHRTF;



    };



}// namespace quack
