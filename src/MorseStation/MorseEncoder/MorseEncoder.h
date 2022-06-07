//
// Created by Linus on 07.06.2022.
//

#ifndef LIGHTHOUSE_MORSEENCODER_H
#define LIGHTHOUSE_MORSEENCODER_H


#include <string>
#include <string>
#include <iostream>
#define PAUSE_SIGN "/"
using namespace std;

class MorseEncoder {
public:
    static constexpr const char* pauseSign = PAUSE_SIGN;
    static string morseEncode(char x);
};


#endif //LIGHTHOUSE_MORSEENCODER_H
