#include "Core/App.h"

#include <Audio/AudioEngine.h>

#include <SDL3_mixer/SDL_mixer.h>



struct Structure {

    int id = 0;

};

static bool isFirst = true;

static void callback(int chan, void *stream, int len, void *udata) {
    Quack::EffectUserData* info = static_cast<Quack::EffectUserData*>(udata);

    if (isFirst) {
        Quack::SoundCreateInfo createInfo = {len, 2, 44100};
        info->audioEngine->registerSound(createInfo);

        isFirst = false;
    }

    auto buf =  Quack::AudioBuffer{2, stream, len};
    info->audioEngine->processEffect(info->soundId, buf);
    stream = buf.data;

    bool s = (buf.data == stream);

}

/*
 * Cornes / notes:
 * - What would happen if you convert an deinterleaved buffer to a deinterleaved buffer?
 * - How do i check if an audio file is deinterleaved.
 * - Read somewhere that SDL_Mixer is interleaved.
 */

int main() {


/*
    App application{};


    application.init();

    application.run();

    application.cleanup();

*/
    auto r = SDL_Init(SDL_INIT_AUDIO);

    if (r < 0) {
        printf("%s \n", SDL_GetError());
    }

    Mix_Init(MIX_INIT_WAVPACK);



    SDL_AudioSpec spec;

    spec.freq = MIX_DEFAULT_FREQUENCY;
    spec.format = MIX_DEFAULT_FORMAT;
    spec.channels = MIX_DEFAULT_CHANNELS;

    Mix_OpenAudio(0, &spec);

    Quack::AudioEngine audioEngine;

    audioEngine.initialize();

    Quack::EffectUserData soundEffectInfo = {
            &audioEngine, 1
    };


    auto music = Mix_LoadWAV("../Assets/Audio/bluebonnet_in_b_major_looped.mp3");


    if (!music) {
        printf("COuldnt load: %s \n ",  Mix_GetError());
    }




    Mix_RegisterEffect(0, &callback, nullptr, &soundEffectInfo);

    Mix_PlayChannel(0, music, 1);


    printf("Press enter to exit\n");
    getchar();

    Mix_FreeChunk(music);


    audioEngine.destroy();
    Mix_Quit();
    SDL_Quit();
    return 0;
}

