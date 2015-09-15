///////////////////////////////////////////////////////////////
//
//  WearableCode
//
//  Software for WearableCode: Wearable Morse Code Blinky
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

#include <Adafruit_NeoPixel.h>
#include <BobaBlox.h>

// Include the local CodeMachine library and create the CodeMachine variable, enigma
#include "CodeMachine.h"
CodeMachine enigma;

// Messages
const byte NUM_MESSAGES = 3;
byte theCurrentMessage;

// CQ
char message0[] = "-.-./--.- ";

// Hello
char message1[] = "...././.-../.-../--- ";

// Make It
char message2[] = "--/../-.-/. ../- ";

// Color Hue Timers
const unsigned long hueInterval = 150;
const byte hueIncrement = 1;
unsigned long hueStart;
boolean cycleColor;
byte hue;

// NeoPixel and button pins
const int PIN_NEOPIXEL = 0;
const int PIN_BUTTON = 1;

Button pushButton = Button(PIN_BUTTON);

// Long Press
const unsigned long longPressInterval = 3000;
unsigned long pressStart;
boolean longPress, buttonWasUp;
const int DEBOUNCE_INTERVAL = 18;

void setup() {

	// Initialize the code machine and set speed
	enigma.begin (PIN_NEOPIXEL, PIN_BUTTON);
	enigma.wpm (10);

	// Set up the first message
	theCurrentMessage = 0;
	playMessage (theCurrentMessage);

	// Initialize the pushbutton (analog write makes sure the button voltage is maximum)
	analogWrite(PIN_BUTTON, 255);
	pushButton.wasPressed();
  longPress = false;
	buttonWasUp = true;

	// Start the hue cycle
	cycleColor = true;
	hue = 0;
	setColor(hue);

}

void loop() {

	// Update the enigma CodeMachine as often as possible
	enigma.update();

	// Detect and act on a long button press (with simple debounce)
	if (pushButton.isDown() && !longPress) {
		delay(DEBOUNCE_INTERVAL);
		if (pushButton.isDown()) {
			if (buttonWasUp) {
				buttonWasUp = false;
				pressStart = millis();

			} else {
				if ((millis()-pressStart) > longPressInterval) {
					longPress=true;
					flicker();
					cycleColor = !cycleColor;

				}
			}
		}
	}

	// When the button is released
	if (pushButton.wasReleased()) {
	
		// If not a long press, then cycle the messages
		if (!longPress) {
			theCurrentMessage = (++theCurrentMessage % NUM_MESSAGES);
			playMessage(theCurrentMessage);
		}
		buttonWasUp = true;
		longPress = false;
	}

	// If cycling colors, go to the next color (unless button is being pressed)
	if (cycleColor && pushButton.isUp()) {

		if ((millis()-hueStart) > hueInterval) {
			hueStart = millis();
			hue = (hue + hueIncrement) % 256;
			setColor(hue);
		}

	}

}

///////////////////////////////////////////////////////////////
//
// Utility Functions

// Flicker pixel several times
void flicker() {

	for (int i=0; i<4; i++) {
		enigma.on();
		delay (40);
		enigma.off();
		delay (40);
	}
}

// Start playing the specified message
void playMessage (byte m) {
	switch (m) {
		case 0: enigma.play(message0); break;
		case 1: enigma.play(message1); break;
		case 2: enigma.play(message2); break;
		default: break;
	}
}

// Set color. Hue rotates R > G > B on input 0-255
void setColor (byte p) {

  p = 255 - p;
  
  if(p < 85) {
    enigma.color(255 - (p * 3), 0, (p * 3));
  } else if (p < 170) {
    p -= 85;
    enigma.color(0, (p * 3), 255 - (p * 3));
  } else {
    p -= 170;
    enigma.color((p * 3), 255 - (p * 3), 0);
  }

}
