/*
  Copyright (c) 2016, Inria, Stan Borkowski
 
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

#include "application.h"

#ifndef melody_speaker__h
#define melody_speaker__h


typedef void (*CallbackType)(void);

class MelodySpeaker {

    public:

        MelodySpeaker(uint8_t pin, bool blocking = true);
        ~MelodySpeaker();

        void 
            begin(),
            setInterNotePause(float noteRatio),
            setTempo(uint16_t beatsPerMinute), // tempo applies only to the next setMelody call. Mind call order.
            setMelody(char* melody),
            processMelody(),
            onEnd(CallbackType callback);

    private:

        bool
            blocking;
        uint8_t
            pin,
            cursor,
            len;
        uint16_t
            wholeNote; // related to tempo
        uint16_t
            *duration,
            *frequency;
        unsigned long
            itemEnd;
        float
            pause;
        CallbackType callback;

        uint16_t codeToFrequency(char, char);
        uint16_t codeToDuration(char);
};

#endif
