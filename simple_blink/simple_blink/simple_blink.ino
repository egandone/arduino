int timer = 100;
int pins[] = {0,1};
int pinCount = sizeof(pins) / sizeof(int);

void setup() {
  for (int thisPin = 0; thisPin < pinCount; thisPin++)  {
    pinMode(pins[thisPin], OUTPUT);      
  }
}

void loop() {
  int pinIndex = random(0, pinCount);

  if ((pinIndex >= 0) && (pinIndex < pinCount)) {
        digitalWrite(pins[pinIndex], HIGH);   
    delay(timer);                  
        digitalWrite(pins[pinIndex], LOW);    
  }
}
