#include <iostream>
#include "Core/App.h"

int main() {

    App application;

    application.init();

    application.run();

    application.cleanup();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
