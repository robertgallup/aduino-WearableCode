///////////////////////////////////////////////////////////////
//
//  CodeMachine.h
//
//  Class for CodeMachine Morse code library. Read's and plays code
//
//  The MIT License (MIT)
//  
//  Copyright (c) 2015 Robert Gallup
//  
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//
///////////////////////////////////////////////////////////////

#ifndef __codemachine_h
#define __codemachine_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// CLASS
class CodeMachine
{
  public:
  
    CodeMachine ();                     // Constructor
    void begin (byte, byte);            // Initializes code machine with play pin/read pin
    void begin (byte);                  // Initializes code machine with play pin. Read disabled
    int read ();                        // Initiates read into code array
    void play (char[]);                 // Initiates play from beginning of code array
    
    void update ();                     // Called continually to update the current operation's status
    void on();                          // Turns LED on
    void off();                         // Turns LED off
        
    void wpm (byte);                    // Sets blink speed (words per minute)
    void color (byte, byte, byte);      // Sets pixel to RGB color
    
  private:  
    
    uint32_t _ONCOLOR;                  // Current color of LED
    uint32_t _OFFCOLOR;                 // Black color
    
    byte _readPin;                      // Analog pin for reading code
    byte _playPin;                      // Digital pin for playing code
    
    char *_code;                        // Pointer to code string
    unsigned int _codeIndex;            // Index to current code element

    unsigned long _start;               // Single timer used for all elements
    unsigned long _interval;            // Current interval being timed
    byte _signalState;                  // State: ON/OFF of signal element (LED)

    byte         _wpm;                  // Current playback speed in words per minute
    unsigned int _dotLength;            // Length of a dot in millis
    unsigned int _dashLength;           // Dash length in millis
    unsigned int _elementPauseLength;   // Pause between elements in millis
    unsigned int _charPauseLength;      // Pause between characters in millis
    unsigned int _wordPauseLength;      // Pause between words in millis

    Adafruit_NeoPixel _light;           // The NeoPixel used to display the code

 	void _nextElement ();               // Play the element pointed to by the codePtr
    void _setup();                      // Initializes colors, variables, etc.

    void _signalON ();                  // Turns code signal off
    void _signalOFF ();                 // Turns code signal on
    
};

#endif
