/**
   Touch Sensor Module
   Add Method to detect double tap and long tap
*/

class TouchSensor {
  private:
    int pinNum; // Digital Pin for Touch sensor

    // For polling
    long currentMillis = 0;
    long previousMillis = 0;

    // Double Touch Status
    boolean firstTouched = false;
    long firstTouchedTime = 0;
    boolean firstLeave = false;
    long firstLeaveTime = 0;
    boolean secondTouched = false;
    long secondTouchedTime = 0;

    // Long Touch Status
    boolean startLongTouched = false;
    long startLongTouchedTime = 0;
    boolean longTouched = false;

    // Double Tap Callback
    typedef void (*TapCallback)();

    // Check Status
    boolean checkDouble = false;
    boolean checkSingle = false;
    boolean checkLong = false;

  public:
    TouchSensor(int pinNum) {
      this->pinNum = pinNum;
      pinMode(pinNum, INPUT);
    }

    void checkStatus() {
      this->checkSingleTap();
    }

    void checkSingleTap() {

    }

    void checkLongTap(TapCallback callback) {
      int isTouched = digitalRead(pinNum);
      if (isTouched) {
        if (!startLongTouched) {
          startLongTouched = true;
          startLongTouchedTime = millis();
        } else {
          if ((millis() - startLongTouchedTime) > 3000) {
            if (!longTouched) {
              // Long Tap action
              Serial.println("-------------Long Tap-----------");
              longTouched = true;
              callback();
            }
          }
        }
      } else {
        startLongTouched = false;
        longTouched = false;
      }
    }

    void checkDoubleTap(TapCallback callback) {
      currentMillis = millis();
      if (previousMillis == 0) previousMillis = millis();
      if ( (currentMillis - previousMillis) > 10) {
        int isTouched = digitalRead(pinNum);
        if (isTouched) {
          if (!firstTouched) { //Not yet touched the first time
            firstTouched = true;
            firstLeave = false;
            firstTouchedTime = millis();
            Serial.println("-----------First Touch--------------");
          } else {
            // already touch the first time
            if (firstLeave) {
              if ((millis() - firstLeaveTime) < 1000) {
                if (!secondTouched) {
                  // Second Touch (release and touch it again)
                  secondTouched = true;
                  secondTouchedTime = millis();
                  callback();
                  Serial.println("-----------Double Tap--------------");
                }
              } else {
                // second touch timeout, treat it as first touch
                firstTouched = true;
                firstTouchedTime = millis();
                firstLeave = false;
                Serial.println("-----------Second Touch Timeout--------------");
              }
            } else {
              // first touch and not yet released
            }
          }
        }
        if (!isTouched) {
          if (firstTouched && !firstLeave) {
            if ((millis() - firstTouchedTime) < 1000) { // already touch the first time and leave within 1 min
              firstLeave = true;
              firstLeaveTime = millis();
              Serial.println("-----------First Leave Success--------------");
            } else {  // first touch timeout
              firstTouched = false;
              firstLeave = false;
              Serial.println("-----------First Leave Timeout--------------");
            }
          }

          if (firstTouched && firstLeave) {
            // touch the first time and leave on time
            // Nothing happened
            // Wait for second touch
            if (millis() - firstLeaveTime > 2000) {
              firstTouched = false;
              firstLeave = false;
              Serial.println("-----------No second touch--------------");
            }
          }

          if (secondTouched) {
            // leave the second time
            // reset the first touch and first leave
            firstTouched = false;
            firstLeave = false;
            secondTouched = false;
            Serial.println("-----------Second Leave--------------");
          }
        }
        previousMillis = currentMillis;
      }
    }
};
const int TOUCH_SENSOR = 3;
const int LED = 4;
int ledStatus = LOW;

// Define Touch Sensor using class
TouchSensor ts(TOUCH_SENSOR);

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, ledStatus);
}

void loop() {
  ts.checkDoubleTap(changeLedStatus);
  //  ts.checkLongTap(changeLedStatus);
}

void changeLedStatus() {
  ledStatus = !ledStatus;
  digitalWrite(LED, ledStatus);
}
