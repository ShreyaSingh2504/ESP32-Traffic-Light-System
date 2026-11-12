
#include <Arduino.h>

// LED pin definitions
const int redLED = 25;
const int yellowLED = 26;
const int greenLED = 27;

// Pedestrian button pin
const int buttonPin = 14;

// Traffic light states
enum TrafficState
{
    STATE_GREEN,
    STATE_YELLOW,
    STATE_RED,
    STATE_PEDESTRIAN
};

// Current state
TrafficState currentState = STATE_GREEN;

// Timing variables
unsigned long previousTime = 0;
unsigned long lastDebounceTime = 0;

const unsigned long debounceDelay = 50;

// Pedestrian request
bool pedestrianRequest = false;

// Button debouncing variables
bool lastButtonReading = LOW;
bool stableButtonState = LOW;

void setup()
{
    pinMode(redLED, OUTPUT);
    pinMode(yellowLED, OUTPUT);
    pinMode(greenLED, OUTPUT);

    // Internal pull-down resistor
    pinMode(buttonPin, INPUT_PULLDOWN);

    Serial.begin(115200);

    // Initial LED condition
    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);

    previousTime = millis();

    Serial.println("ESP32 Traffic Light Controller Started");
    Serial.println("Current State: GREEN");
}

void loop()
{
    // --------------------------------
    // PEDESTRIAN BUTTON DEBOUNCING
    // --------------------------------

    bool buttonReading = digitalRead(buttonPin);

    if (buttonReading != lastButtonReading)
    {
        lastDebounceTime = millis();
    }

    if (millis() - lastDebounceTime >= debounceDelay)
    {
        if (buttonReading != stableButtonState)
        {
            stableButtonState = buttonReading;

            // Register only a new button press
            if (stableButtonState == HIGH)
            {
                pedestrianRequest = true;
                Serial.println("Pedestrian Request Received");
            }
        }
    }

    lastButtonReading = buttonReading;

    // --------------------------------
    // GREEN STATE
    // --------------------------------

    if (currentState == STATE_GREEN)
    {
        digitalWrite(greenLED, HIGH);
        digitalWrite(yellowLED, LOW);
        digitalWrite(redLED, LOW);

        if (millis() - previousTime >= 10000)
        {
            currentState = STATE_YELLOW;
            previousTime = millis();

            Serial.println("GREEN -> YELLOW");
        }
    }

    // --------------------------------
    // YELLOW STATE
    // --------------------------------

    else if (currentState == STATE_YELLOW)
    {
        digitalWrite(greenLED, LOW);
        digitalWrite(yellowLED, HIGH);
        digitalWrite(redLED, LOW);

        if (millis() - previousTime >= 3000)
        {
            currentState = STATE_RED;
            previousTime = millis();

            Serial.println("YELLOW -> RED");
        }
    }

    // --------------------------------
    // RED STATE
    // --------------------------------

    else if (currentState == STATE_RED)
    {
        digitalWrite(greenLED, LOW);
        digitalWrite(yellowLED, LOW);
        digitalWrite(redLED, HIGH);

        if (millis() - previousTime >= 7000)
        {
            if (pedestrianRequest == true)
            {
                currentState = STATE_PEDESTRIAN;
                pedestrianRequest = false;

                Serial.println("RED -> PEDESTRIAN");
            }
            else
            {
                currentState = STATE_GREEN;

                Serial.println("RED -> GREEN");
            }

            previousTime = millis();
        }
    }

    // --------------------------------
    // PEDESTRIAN STATE
    // --------------------------------

    else if (currentState == STATE_PEDESTRIAN)
    {
        digitalWrite(greenLED, LOW);
        digitalWrite(yellowLED, LOW);
        digitalWrite(redLED, HIGH);

        if (millis() - previousTime >= 5000)
        {
            currentState = STATE_GREEN;
            previousTime = millis();

            Serial.println("PEDESTRIAN -> GREEN");
        }
    }
}
