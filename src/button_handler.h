// button_handler.h
// Header file for handling button presses with multiple click types

#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>

// Define the button pin
#define BUTTON_PIN 15  // GPIO pin for the button. This pin is used to detect button presses.

// Debounce delay to filter out noise from button presses
#define DEBOUNCE_DELAY 50  // Debounce delay in milliseconds

// Enum for different button states
enum ButtonState {
    NONE,
    SINGLE_CLICK,
    DOUBLE_CLICK,
    LONG_PRESS,
    ULTRA_LONG_PRESS,
    SHORT_PRESS
};

// Declare global variables
extern unsigned long buttonPressStartTime;
extern unsigned long lastButtonPressTime;
extern bool buttonPressed;
extern int clickCount;
extern bool actionTaken;

// Function prototypes
void setupButton();
void loopButtonHandler();
void handleButtonState(ButtonState state);

#endif  // BUTTON_HANDLER_H
