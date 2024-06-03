//
// Created by allos on 6/3/2024.
//

#ifndef GAME_HASHMAP_H
#define GAME_HASHMAP_H

#ifndef NULL
#define NULL 0
#endif

namespace Quack {


    template<typename KEY, typename VAL>
    class Pair {

    public:
        Pair(const KEY& key, const VAL& value) :
        key(key), value(value), next(NULL) {

        }


    private:
        KEY key;
        VAL value;
        Pair* next;
    };

    class Hashmap {

    };
}



#endif //GAME_HASHMAP_H
