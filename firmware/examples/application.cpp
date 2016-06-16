/*
 ******************************************************************************
  Copyright (c) 2016, Inria, Stan Borkowski

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
 */

#include "application.h"
#include "MelodySpeaker/MelodySpeaker.h"

PRODUCT_ID(PLATFORM_ID);
PRODUCT_VERSION(3);


SYSTEM_MODE(AUTOMATIC);
//SYSTEM_MODE(SEMI_AUTOMATIC);
//SYSTEM_MODE(MANUAL);


MelodySpeaker speaker = MelodySpeaker(D1, false);

void setup()
{
    pinMode(D7, OUTPUT);
    speaker.begin();

    speaker.setTempo(240);
    speaker.setMelody("4C7,8G6,8G6,4A6,4G6,4  ,4B6,4C7");
}


void loop()
{
    speaker.processMelody();

    // uncomment the code below to see how delay influences melody  playback
    //digitalWrite(D7, HIGH);
    //delay(500);
    //digitalWrite(D7, LOW);
    //delay(500);

}
