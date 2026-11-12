#include <Arduino.h>

//LED pin definitions
const int redLED =25;// the value will not chnage during the program
const int yellowLED =26;
const int greenLED =27;

//pedestrian button
const int buttonPin = 14;

// state machine definition
enum TrafficState
{
  STATE_GREEN,//ASSIGNING NAME TO NO.(0)
  STATE_YELLOW,//(1)
  STATE_RED,//(2)
  STATE_PEDESTRIAN//(3)
};

// current sys state
TrafficState currentState = STATE_GREEN;

//timing variable
unsigned long previousTime = 0;

//pedestrian request flag
bool pedestrianRequest = false;// false --> no request true --> someone pressed button

void setup()
{
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  pinMode(buttonPin, INPUT);

  Serial.begin(115200);
}
void loop()
{
  // check pedestrian button first
  if(digitalRead(buttonPin) == HIGH)
  {
    pedestrianRequest = true;
  }

  //green state
  if(currentState == STATE_GREEN)
  {
    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);
    if(millis() - previousTime >= 10000)
    {
      currentState = STATE_YELLOW;
      previousTime = millis();
    }
  }
  
  //yellow state
  else if(currentState == STATE_YELLOW)
  {
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, LOW);

    if(millis() - previousTime >= 3000)
    {
      currentState = STATE_RED;
      previousTime = millis();
    }
  }

  //red state
  else if(currentState == STATE_RED)
  {
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, HIGH);

    if(millis() - previousTime >= 7000)
    {
      if(pedestrianRequest == true)
      {
        currentState = STATE_PEDESTRIAN;
        pedestrianRequest = false;// the request has been handled 
      }
      else{
        currentState = STATE_GREEN;
      }
    }
  }
    
    //pedestrian state
    else if(currentState == STATE_PEDESTRIAN)
    {
      digitalWrite(greenLED, LOW);
      digitalWrite(yellowLED, LOW);
      digitalWrite(redLED, HIGH);

      if(millis() - previousTime >= 5000)
      {
        currentState = STATE_GREEN;
        previousTime = millis();
      }
    }
  
}
