#ifndef Led_h
#define Led_h

// library interface description
class Led
{
  // user-accessible "public" interface
  public:
    Led(int, char*);
    void blinking(int blink_count, unsigned long interval);
    void blinkWithPWM(int blink_count, unsigned long interval, int brightness);
    void breathingLight(unsigned long interval);
    char* getLedName(void);

  // library-accessible "private" interface
  private:
    // LED info
    char *name;  // LED name
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
};

#endif