///////////////////////////////////////////////////////////////
//
//  CodeMachine.cpp
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

#include "CodeMachine.h"

const byte _SIGON  = 1;
const byte _SIGOFF = 0;

// Constructor
CodeMachine::CodeMachine ()
{
}

// Initialize the CodeMachine machine
void CodeMachine::begin (byte playPin, byte readPin)
{

    _readPin = readPin;
    _playPin = playPin;
    _setup();

}

// Initializes code machine with write pin. Read disabled
void CodeMachine::begin (byte playPin)
{
    _playPin = playPin;
    _setup();

}

void CodeMachine::_setup() {

    _light = Adafruit_NeoPixel(1, _playPin, NEO_GRB + NEO_KHZ800);
    _light.begin();

    _ONCOLOR  = _light.Color (0, 150, 250);
    _OFFCOLOR = _light.Color (0, 0, 0);

    wpm (6);

}

// Read a CodeMachine code
int CodeMachine::read ()
{
    // on();
    // delay(3000);
    // // byte b = EEPROM.read(0);
    // // return b;
    return 0;
}

// Play a CodeMachine code
void CodeMachine::play (char code[])
{

	_code = code;
	_codeIndex = 0;
	_nextElement();
	_start = millis();

}

// Update the code player
void CodeMachine::update ()
{

	// If timer hasn't expired, do nothing
	if ((millis() - _start) < _interval) return;

	// If the signal is on, then turn it off and start an element pause
	if (_signalState == _SIGON) {
		_signalOFF();
		_interval = _elementPauseLength;

	// If signal is off, play the next code element
	} else {
		_nextElement();

	}

	// Reset timer
	_start = millis();

}

// Set up timing parameters in milliseconds
void CodeMachine::wpm (byte w) {

	_dotLength  = 1200 / w;
	_dashLength = 3 * _dotLength;
	_elementPauseLength = _dotLength;
	_charPauseLength = (_dashLength) - _elementPauseLength;
	_wordPauseLength = (10 * _dotLength) - _elementPauseLength;
	_wpm = w;

}

void CodeMachine::on () {
	_signalON();
}

void CodeMachine::off () {
	_signalOFF();
}

// Sets color
void CodeMachine::color (byte r, byte g, byte b) {
	_ONCOLOR = _light.Color(r, g, b);
	if (_signalState == _SIGON) {
		_signalON();
	}

}

void CodeMachine::_signalON () {
	_light.setPixelColor(0, _ONCOLOR);
	_light.show();
	_signalState = _SIGON;
}

void CodeMachine::_signalOFF () {
	_light.setPixelColor(0, _OFFCOLOR);
	_light.show();
	_signalState = _SIGOFF;
}

// Play an Element (LED is ON for 1 or 3 units)
void CodeMachine::_nextElement ()
{
	
	// Get the next code element, wrap to 0
	char c = _code[_codeIndex];
	if (byte(c) == 0) {
		_codeIndex = 0;
		c = _code[_codeIndex];
	}
	_codeIndex++;

	// Set state/timer for next element
	switch (c) {

		// DOT
		case '.':
			_signalON();
			_interval = _dotLength;
			break;

		// DASH
		case '-':
			_signalON();
			_interval = _dashLength;
			break;

		// Character Break
		case '/':
			_signalOFF();
			_interval = _charPauseLength;
			break;

		// Word Break
		case ' ':
			_signalOFF();
			_interval = _wordPauseLength;
			break;

		default:
		break;

	}
	
}
