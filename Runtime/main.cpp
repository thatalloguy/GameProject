#include "Core/App.h"

#include <phonon.h>

int main() {

/*
    App application{};


    application.init();

    application.run();

    application.cleanup();
*/

    IPLContextSettings  contextSettings{};
    contextSettings.version = STEAMAUDIO_VERSION;

    IPLContext context{};
    iplContextCreate(&contextSettings, &context);

    iplContextRelease(&context);




    return 0;
}
