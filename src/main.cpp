
#include <Arduino.h>

// LED pin definitions
const int redLED = 25;
const int yellowLED = 26;
const int greenLED = 27;

// Pedestrian button
const int buttonPin = 14;

// State machine definition
enum TrafficState
{
    STATE_GREEN,
    STATE_YELLOW,
    STATE_RED,
    STATE_PEDESTRIAN
};

// Current system state
TrafficState currentState = STATE_GREEN;

// Timing variable
unsigned long previousTime = 0;

// Pedestrian request flag
bool pedestrianRequest = false;

void setup()
{
    pinMode(redLED, OUTPUT);
    pinMode(yellowLED, OUTPUT);
    pinMode(greenLED, OUTPUT);

    // Enable internal pull-down resistor
    pinMode(buttonPin, INPUT_PULLDOWN);

    Serial.begin(115200);

    Serial.println("ESP32 Traffic Light Controller Started");
}

void loop()
{
    // Check pedestrian button
    if (digitalRead(buttonPin) == HIGH)
    {
        pedestrianRequest = true;
    }

    // GREEN STATE
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

    // YELLOW STATE
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

    // RED STATE
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

    // PEDESTRIAN STATE
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
