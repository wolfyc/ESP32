// button_handler.cpp
// Implementation of button handling functions

#include "Button_Handler.h"

// Declare global variables
unsigned long buttonPressStartTime = 0;
unsigned long lastButtonPressTime = 0;
bool buttonPressed = false;
int clickCount = 0;
bool actionTaken = false;

// Setup function to initialize the button pin
void setupButton() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);  // Set the button pin as input with pull-up resistor enabled
}

// Main loop function to handle button events
void loopButtonHandler() {
    static unsigned long lastDebounceTime = 0;
    static bool buttonState = HIGH;  // Initial state is HIGH (unpressed)
    static bool lastButtonState = HIGH;

    // Read the current state of the button
    bool reading = digitalRead(BUTTON_PIN);

    // Debounce logic to filter out noise
    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        // Only proceed if debounce delay has passed
        if (reading != buttonState) {
            buttonState = reading;
            if (buttonState == LOW) {
                buttonPressStartTime = millis();
                buttonPressed = true;
                clickCount++;
                actionTaken = false;  // Reset action flag when button is pressed
            } else {
                unsigned long pressDuration = millis() - buttonPressStartTime;

                if (pressDuration > 2000) {
                    handleButtonState(ULTRA_LONG_PRESS);
                    actionTaken = true;
                } else if (pressDuration > 1000) {
                    handleButtonState(LONG_PRESS);
                    actionTaken = true;
                } else if (pressDuration > 300) {
                    handleButtonState(SHORT_PRESS);
                    actionTaken = true;
                } else {
                    // Store time for double-click detection
                    lastButtonPressTime = millis();
                }
            }
        }
    }

    // Detect double-click or single-click
    if (buttonPressed && (millis() - lastButtonPressTime > 500) && buttonState == HIGH) {
        if (!actionTaken) {  // Only proceed if no long/short press was detected
            if (clickCount == 1) {
                handleButtonState(SINGLE_CLICK);
            } else if (clickCount == 2) {
                handleButtonState(DOUBLE_CLICK);
            }
        }
        clickCount = 0;
        buttonPressed = false;
    }

    // Update last button state
    lastButtonState = reading;
}

// Function to handle different button press types
void handleButtonState(ButtonState state) {
    switch (state) {
        case SINGLE_CLICK:
            Serial.println("Single Click Detected");
            break;
        case DOUBLE_CLICK:
            Serial.println("Double Click Detected");
            break;
        case LONG_PRESS:
            Serial.println("Long Press Detected");
            break;
        case ULTRA_LONG_PRESS:
            Serial.println("Ultra Long Press Detected");
            break;
        case SHORT_PRESS:
            Serial.println("Short Press Detected");
            break;
        default:
            break;
    }
}
