#include "Led.h"
#include "Arduino.h"

Led::Led(int pin_num, char *name)
{
  //Constructor
  this->pin_num = pin_num;
  this->name = name;
  pinMode(pin_num, OUTPUT);
}

char* Led::getLedName() {
  return name;
}

void Led::blinking(int blink_count, unsigned long interval)
{
  this->blink_count = blink_count;
  if (current_count == 0)
    currentMillis = 0;
  if ((millis() - currentMillis) > interval &&
      current_count < blink_count)
  {
    // update the current state
    currentMillis = millis();
    current_count++;
    current_state = !current_state;
    digitalWrite(pin_num, current_state);
  }
}

void Led::blinkWithPWM(int blink_count, unsigned long interval, int brightness)
{
  this->blink_count = blink_count;
  if (current_count == 0)
    currentMillis = 0;
  if ((millis() - currentMillis) > interval &&
      current_count < blink_count)
  {
    // update the current state
    currentMillis = millis();
    current_count++;
    current_state = !current_state;
    if (current_state)
    {
      digitalWrite(pin_num, LOW);
    }
    else
    {
      analogWrite(pin_num, brightness);
    }
  }
}

void Led::breathingLight(unsigned long interval)
{
  if (current_brightness == 0)
    currentMillis = 0;
  if ((millis() - currentMillis) > interval)
  {
    currentMillis = millis();
    if (current_brightness == 255)
    {
      current_iteration = false;
    }
    if (current_brightness == 0)
    {
      current_iteration = true;
    }
    if (current_iteration)
    {
      current_brightness++;
    }
    else
    {
      current_brightness--;
    }
    Serial.println(current_brightness);
    analogWrite(pin_num, current_brightness);
  }
}
