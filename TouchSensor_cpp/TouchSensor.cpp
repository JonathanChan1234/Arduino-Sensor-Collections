#include "TouchSensor.h"
#include "Arduino.h"

TouchSensor::TouchSensor(int pinNum)
{
    //Constructor
    this->pinNum = pinNum;
    pinMode(pinNum, INPUT);
}

void TouchSensor::subscribeSingleTap(TapCallback callback)
{
    singleTapCallback = callback;
    checkSingleTap = true;
    checkTapStatus();
}

void TouchSensor::subscribeDoubleTap(TapCallback callback)
{
    checkDoubleTap = true;
    checkTapStatus();
    doubleTapCallback = callback;
}

void TouchSensor::subscribeLongTap(TapCallback callback)
{
    checkLongTap = true;
    checkTapStatus();
    longTapCallback = callback;
}

void TouchSensor::unsubscribeDoubleTap()
{
    checkDoubleTap = false;
}

void TouchSensor::unsubscribeLongTap()
{
    checkLongTap = false;
}

void TouchSensor::checkTapStatus()
{
    currentMillis = millis();
    if (previousMillis == 0)
        previousMillis = millis();

    if ((currentMillis - previousMillis) > 10)
    {
        int isTouched = digitalRead(pinNum);
        if (isTouched)
        {
            if (checkSingleTap)
                singleTapCallback();

            if (checkDoubleTap)
            {
                // Double Tap Checking
                if (!firstTouched)
                { //Not yet touched the first time
                    // Double Tap
                    firstTouched = true;
                    firstLeave = false;
                    firstTouchedTime = millis();
                    Serial.println("-----------First Touch--------------");
                }
                else
                {
                    // Double Tap
                    if (firstLeave)
                    {
                        if ((millis() - firstLeaveTime) < 1000)
                        {
                            if (!secondTouched)
                            {
                                // Second Touch (release and touch it again)
                                secondTouched = true;
                                secondTouchedTime = millis();
                                doubleTapCallback();
                                //                    Serial.println("-----------Double Tap--------------");
                            }
                        }
                        else
                        {
                            // second touch timeout, treat it as first touch
                            firstTouched = true;
                            firstTouchedTime = millis();
                            firstLeave = false;
                            Serial.println("-----------Second Touch Timeout--------------");
                        }
                    }
                    else
                    {
                        // first touch and not yet released
                    }
                }
            }

            if (checkLongTap)
            {
                //  Long Tap Checking
                if (!startLongTouched)
                {
                    startLongTouched = true;
                    startLongTouchedTime = millis();
                }
                else
                {
                    if ((millis() - startLongTouchedTime) > 3000)
                    {
                        if (!longTouched)
                        {
                            // Long Tap action
                            //                  Serial.println("-------------Long Tap-----------");
                            longTouched = true;
                            longTapCallback();
                        }
                    }
                }
            }
        }

        if (!isTouched)
        {
            if (checkDoubleTap)
            {
                // Check Double Tap
                if (firstTouched && !firstLeave)
                {
                    if ((millis() - firstTouchedTime) < 1000)
                    { // already touch the first time and leave within 1 min
                        firstLeave = true;
                        firstLeaveTime = millis();
                        Serial.println("-----------First Leave Success--------------");
                    }
                    else
                    { // first touch timeout
                        firstTouched = false;
                        firstLeave = false;
                        Serial.println("-----------First Leave Timeout--------------");
                    }
                }

                if (firstTouched && firstLeave)
                {
                    // touch the first time and leave on time
                    // Nothing happened
                    // Wait for second touch
                    if (millis() - firstLeaveTime > 2000)
                    {
                        firstTouched = false;
                        firstLeave = false;
                        Serial.println("-----------No second touch--------------");
                    }
                }

                if (secondTouched)
                {
                    // leave the second time
                    // reset the first touch and first leave
                    firstTouched = false;
                    firstLeave = false;
                    secondTouched = false;
                    Serial.println("-----------Second Leave--------------");
                }
            }

            if (checkLongTap)
            {
                // Check Long Tap
                startLongTouched = false;
                longTouched = false;
            }
        }
        previousMillis = currentMillis;
    }
}
