#include "main/GlobalMacros.h"
#if MELODY_LIB == 1
/*
  Copyright (c) 2016 Inria, Stan Borkowski

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

/* melody */
/* Melody playing class */

#include <math.h>
#include "application.h"
#include "MelodySpeaker.h"

MelodySpeaker::MelodySpeaker(uint8_t speakerpin, int maxNotes, bool mode) {
    duration.reserve(maxNotes);
    frequency.reserve(maxNotes);
    blocking = mode;
    pin = speakerpin;
    len = 0;
    itemEnd = 0;
    pause = 1.3;
    onMelodyEnd = NULL;
    onToneStart = NULL;
    onToneEnd = NULL;
    setTempo(120);
}

void MelodySpeaker::begin(void) {
    pinMode(pin, OUTPUT);
}

void MelodySpeaker::setCallbacks(std::function<void()> onMelodyEnd, std::function<void(int)> onToneStart, std::function<void(int)> onToneEnd) {
    this->onMelodyEnd = onMelodyEnd;
    this->onToneStart = onToneStart;
    this->onToneEnd = onToneEnd;
}

void MelodySpeaker::setInterNotePause(float noteRatio) {
    pause = 1.0+noteRatio;
}

void MelodySpeaker::setTempo(uint16_t beatsPerMinute) {
    wholeNote = 240000/beatsPerMinute;
}

void MelodySpeaker::setMelody(const char* melodycode) {
    int melodyCodeLen = strlen(melodycode);
    if(melodyCodeLen < 3) {
        return;
    }
    len = (1+melodyCodeLen)/4; // +1 is for the missing coma at the end of melody string
    duration.clear();
    frequency.clear();
    cursor = 0;
    itemEnd = 0;

    for (int i = 0; i < len; ++i) {
        duration.push_back(codeToDuration(melodycode[i*4]));
        // to calculate the note duration, take one second and divide it by note type
        // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        frequency.push_back(codeToFrequency(melodycode[i*4+1], melodycode[i*4+2]));
    }
}

void MelodySpeaker::processMelody(void) {
    if(len == 0) {
        return;
    }
    if (blocking) {
        for(int i=0; i<len; ++i) {
            if(onToneStart) {
                onToneStart(pin);
            }
            isTonePlaying = true;
            tone(pin, frequency[i], duration[i]);
            delay(duration[i]*pause);
            if(onToneEnd) {
                onToneEnd(pin);
            }
            isTonePlaying = false;
        }
        duration.clear();
        frequency.clear();
        len = 0;
        if(onMelodyEnd) {
            onMelodyEnd();
        }
        return;
    }


    if(itemEnd == 0) {
        if(onToneStart) {
            onToneStart(pin);
        }
        isTonePlaying = true;
        tone(pin, frequency[cursor], duration[cursor]);
        itemEnd = millis()+duration[cursor]*pause;
        return;
    }
    if(millis() < itemEnd) { // do nothing while playing a tone
        return;
    } else { // jump to next tone
        ++cursor;
        itemEnd = 0;
        if(onToneEnd) {
            onToneEnd(pin);
        }
        isTonePlaying = false;
    }
    if(cursor >= len) {
        duration.clear();
        frequency.clear();
        len = 0;
        if(onMelodyEnd) {
            onMelodyEnd();
        }
    }
}

void MelodySpeaker::stop()
{
    duration.clear();
    frequency.clear();
    if(isTonePlaying)
    {
        if(onToneEnd)
        {
            onToneEnd(pin);
        }
        isTonePlaying = false;
    }
    itemEnd = 0;
    cursor = 0;
    len = 0;
}

uint16_t MelodySpeaker::codeToFrequency(char tone, char octave) {
    float f=1.0;
    float freq = 0.0;
    switch(octave) {
        case '7': f=8.0; break;
        case '6': f=4.0; break;
        case '5': f=2.0; break;
        case '3': f=0.5; break;
        case '2': f=0.25; break;
        case '1': f=0.125;
    } // alt: f=2^((scale-4))
    switch(tone) {
        case 'C': freq = 261.626; break; // C4
        case 'd': freq = 277.183; break; 
        case 'D': freq = 293.665; break;
        case 'e': freq = 311.127; break;
        case 'E': freq = 329.628; break;
        case 'F': freq = 349.228; break;
        case 'g': freq = 369.994; break;
        case 'G': freq = 391.995; break;
        case 'a': freq = 415.305; break;
        case 'A': freq = 440.0;   break;
        case 'b': freq = 466.164; break;
        case 'B': freq = 493.883;
    }
    return (uint16_t)roundf(freq*f);
}

uint16_t MelodySpeaker::codeToDuration(char length) {
    uint16_t fraction=1;
    switch(length) {
        case '1': fraction=1;  break;
        case '2': fraction=2;  break;
        case '4': fraction=4;  break;
        case '8': fraction=8;  break;
        case 'f': fraction=16; break;
        case 'g': fraction=32; break;
        case 'h': fraction=64; break;
        case 'i': fraction=128; break;
        case 'j': fraction=256;
    }
    return wholeNote/fraction;
}

#endif
