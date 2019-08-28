#include <Led.h>

char *name = "led 1";
Led led = Led(13, name);

void setup() {

}

void loop() {
    led.blinking(1000, 500);
}