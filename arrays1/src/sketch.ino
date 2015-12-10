int timer = 250;
int ledPins[] = {2, 7, 4, 6, 5, 3, 8, 11, 10, 9, 13, 12};
int pinCount = sizeof(ledPins) / sizeof(int);

void setup() {
  for (int thisPin = 0; thisPin < pinCount; thisPin++)  {
    pinMode(ledPins[thisPin], OUTPUT);      
  }
}

void loop() {
  for (int thisPin = 0; thisPin < pinCount; thisPin++) { 
    digitalWrite(ledPins[thisPin], HIGH);   
    delay(timer);                  
    digitalWrite(ledPins[thisPin], LOW);    

  }

  for (int thisPin = pinCount - 1; thisPin >= 0; thisPin--) { 
    digitalWrite(ledPins[thisPin], HIGH);
    delay(timer);
    digitalWrite(ledPins[thisPin], LOW);
  }
}
