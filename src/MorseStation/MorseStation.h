//
// Created by Linus on 07.06.2022.
//

#ifndef LIGHTHOUSE_MORSESTATION_H
#define LIGHTHOUSE_MORSESTATION_H

#include "MorseEncoder/MorseEncoder.h"
#include <iostream>
#include <list>
#include <string>

using namespace std;

// DIT ist Kurz
// DAH ist lang (3x DIT)
// Platzhalter zwischen DIT & DAH : 1x DIT Pause
// Platzhalter zwischen Buchstaben: 3x DIT Pause
// Platzhalter zwischen Wörtern: 7x DIT pause

#define WORD_PAUSE 7
#define LETTER_PAUSE 3
#define DIT_PAUSE 1
#define DIT_LEN (1*200)
#define DAH_LEN (3*DIT_LEN)
#define DIT_PAUSE_SIGN '_'

class MorseStation {

private:
    list<char> _morseList;

    static void addDITPauses(list<char> *morseList, int count);

    void convertChar(list<char> &morseout, char ch);

    static string listToString(list<char> inList);

public:

    void addMessage(string);

    char getNextImpuls();

    bool isDataReady();
};

#endif // LIGHTHOUSE_MORSESTATION_H
