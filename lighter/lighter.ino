class Led {
    // LED info
    String name;  // LED name
    int pin_num;  // LED pin number
  

    //LED status
    int current_count = 0;  // Current counter
    bool current_state = false; // LED current state
    long currentMillis; // Current time

    //For blinking
    int blink_count;  // Number of blinking count
    
    //Breathing Light status
    int current_brightness = 0;
    // true: ascending, false: descending
    bool current_iteration = true;

  public:
    Led(int pin_num, String name) {  //Constructor
      this->pin_num = pin_num;
      this->name = name;

      pinMode(pin_num, OUTPUT);
    }

    void blinking(int blink_count, long interval) {
      this->blink_count = blink_count;
      if (current_count == 0) currentMillis = 0;
      if ((millis() - currentMillis) > interval &&
          current_count < blink_count) {
        // update the current state
        currentMillis = millis();
        current_count++;
        current_state = !current_state;
        digitalWrite(pin_num, current_state);
      }
    }

    /**
       Blinking with a specific brightness
    */
    void blinkWithPWM(int blink_count, long interval, int brightness) {
      this->blink_count = blink_count;
      if (current_count == 0) currentMillis = 0;
      if ((millis() - currentMillis) > interval &&
          current_count < blink_count) {
        // update the current state
        currentMillis = millis();
        current_count++;
        current_state = !current_state;
        if (current_state) {
          digitalWrite(pin_num, LOW);
        } else {
          analogWrite(pin_num, brightness);
        }
      }
    }

    /**
       Breathing Light effect
    */
    void breathingLight(int interval) {
      if (current_brightness == 0) currentMillis = 0;
      if ((millis() - currentMillis) > interval) {
        currentMillis = millis();
        if(current_brightness == 255) {
          current_iteration = false;
        }
        if(current_brightness == 0) {
          current_iteration = true;
        }
        if(current_iteration) {
          current_brightness++;
        } else {
          current_brightness--;
        }
        Serial.println(current_brightness);
        analogWrite(pin_num, current_brightness);
      }
  }
};

Led led1(13, "led 1");
Led led2(12, "led 2");
Led led3(11, "led 3");


void setup() {
  Serial.begin(9600);
}

void loop() {
  led1.blinking(1000, 500);
  led2.blinking(1000, 1000);
  led3.breathingLight(50);
}
