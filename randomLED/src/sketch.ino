
int timer = 100;
int pins[] = {2,3,4,5,6,7,8,9,10,11,12,13};
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
