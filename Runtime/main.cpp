#include <iostream>
#include "Core/App.h"

int main() {

    App application;

    application.init();

    application.run();

    application.cleanup();

    return 0;
}
