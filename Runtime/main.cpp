#include "Core/App.h"

#include <Audio/AudioEngine.h>





int main() {

    App application{};


    application.init();

    application.run();

    application.cleanup();


    return 0;
}

