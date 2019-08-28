const int SOUND_SENSOR = 5;

void setup() {
  Serial.begin(9600);
  pinMode(SOUND_SENSOR, INPUT);
}

void loop() {
  int hasSound = digitalRead(SOUND_SENSOR);
  if(hasSound) {
    Serial.println("------------------Sound detected---------------");
  } 
}
