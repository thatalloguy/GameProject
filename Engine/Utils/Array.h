//
// Created by allos on 8/14/2024.
//

#pragma once



namespace STL {

    template<class T, auto S>
    struct Array {

        T _data[S];

        int getSize() {
            return S;
        }

    };

}
