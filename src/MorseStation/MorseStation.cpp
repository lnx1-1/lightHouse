//
// Created by Linus on 07.06.2022.
//

#include "MorseStation.h"

void MorseStation::addMessage(string msg) {
    for (char ch: msg) {

        convertChar(_morseList, ch);
        printf("[%c]: list: [%s]\n", ch, listToString(_morseList).c_str());
    }
}

string MorseStation::listToString(list<char> inList) {
    string outStr;
    for (char ch: inList) {
        outStr.append(1, ch);
    }
    return outStr;
}

void MorseStation::convertChar(list<char> &morseout, char ch) {
    string codeSigns = MorseEncoder::morseEncode(ch);
    if (*codeSigns.c_str() == *MorseEncoder::pauseSign) {
        addDITPauses(&morseout, WORD_PAUSE-LETTER_PAUSE); // Durch den letzten Buchstaben hinzugefügte pausen brauchen wir nicht
        cout << "detected Pause Sign" << endl;
    } else {
        for (char impuls: codeSigns) {
            morseout.push_back(impuls);
            addDITPauses(&morseout, DIT_PAUSE);
        }
        addDITPauses(&_morseList, LETTER_PAUSE);
    }
}

void MorseStation::addDITPauses(list<char> *morseList, int count) {
    for (int i = 0; i < count; i++) {
        morseList->push_back(DIT_PAUSE_SIGN);
    }
}

bool MorseStation::isDataReady() {
    return !_morseList.empty();
}

char MorseStation::getNextImpuls() {
    char element = DIT_PAUSE_SIGN;
    if (!_morseList.empty()) {
        element = _morseList.front();
        _morseList.pop_front();
    }
    return element;
}