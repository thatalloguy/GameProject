#include "Core/App.h"

#include <Audio/AudioEngine.h>
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

int main() {


/*
    App application{};


    application.init();

    application.run();

    application.cleanup();

*/


    ma_result result;
    ma_engine engine;

    result = ma_engine_init(NULL, &engine);

    if (result != MA_SUCCESS) {
        return -105;
    }

    ma_engine_play_sound(&engine, "../Assets/Audio/bluebonnet_in_b_major_looped.wav", NULL);

    printf("Enter to quit");

    getchar();

    ma_engine_uninit(&engine);

    return 0;
}

