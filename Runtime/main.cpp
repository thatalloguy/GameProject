#include "Core/App.h"

#include <Audio/AudioEngine.h>





int main() {
/*
    App application{};


    application.init();

    application.run();

    application.cleanup();

*/


    Quack::AudioEngine audioEngine;

    audioEngine.Init();

    audioEngine.registerSound({"../Assets/Audio/bluebonnet_in_b_major_looped.wav", true});

    audioEngine.playSound(1);

    audioEngine.doSillyDirectionTest();


    audioEngine.CleanUp();

    return 0;
}

