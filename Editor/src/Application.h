//
// Created by allos on 7/2/2024.
//

#ifndef GAME_APPLICATION_H
#define GAME_APPLICATION_H


#include <Video/Window.h>


namespace Lake {


    class Application {
    public:

        void Init();

        void Run();

        void Destroy();

    private:
        Quack::Window* _window;

    };

}// Lake
#endif //GAME_APPLICATION_H
