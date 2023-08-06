/* 
 * This file is part of the Arduino Steering Wheel Ctrl (https://github.com/keyboard-slayer/arduino-steering-wheel-ctrl)
 * Copyright (c) 2023 Jordan D. & Martin M.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <DigiPotX9Cxxx.h>

#define UINT_MAX 4294967295

int raw = 0;
int Vin = 5;
uint8_t poto = 0;               // uint8_t is an unsigned 8-bit integer
float Vout = 0.0;
float R1 = 1000.0;
float R2 = 0.0;
float buffer = 0.0; 

const int analogPin = 0;        //  Analog input pin A0 
const int inc_outPin = 10;      //  Pin D10 
const int ud_outPin =  9;       //  Pin D9
const int cs_outPin =  8;       //  Pin D8

DigiPot pot(inc_outPin,ud_outPin,cs_outPin);

typedef struct
{
    unsigned int max;
    unsigned int min;
    uint8_t pot;
} ResistorRange;

ResistorRange resistorRanges[7] = {
    // Note: 1 unit = steps of 3.2kohm on the digital potentiometer

    {UINT_MAX, 2500, 30}, // -> 96K
    {2500, 1050, 3},      // -> 11k
    {1050, 550, 2},       // -> 8k
    {550, 350, 0},        // -> 1K
    {350, 200, 7},        // -> 24k
    {200, 100, 5},        // -> 16k
    {100, 10, 1}          // -> 4k
};

void setup()
{
}

void loop()
{
    raw = analogRead(analogPin);

    if(raw!=0)
    {
        buffer = raw * Vin;
        Vout = (buffer) / 1024.0;
        buffer = (Vin / Vout) - 1;
        R2 = R1 * buffer;
    }
    else
    {
        R2 = 2600;
    }

    // 7 is the number of ranges in the resistorRanges array
    for (int i = 0; i < 7; i++)
    {
        if (resistorRanges[i].min < R2 && R2 <= resistorRanges[i].max)
        {
            pot.set(resistorRanges[i].pot);
        }
    }
}
