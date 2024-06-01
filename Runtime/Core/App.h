//
// Created by allos on 6/1/2024.
//

#ifndef GAME_APP_H
#define GAME_APP_H

#include <Video/Window.h>

class App {

public:
    void init();
    void run();
    void cleanup();

private:
    Quack::Window* window;

};


#endif //GAME_APP_H
