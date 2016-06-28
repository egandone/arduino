const byte interruptPin = 2;
volatile int isrVariable = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting setup() ...");

  //
  // Turn on the pull up resistor on the interrupt pin
  // so we use level low as the interrup trigger.
  pinMode(interruptPin, INPUT_PULLUP);

  //
  // To get the behaviour of a single button push increments the counter
  // we use FALLING mode - ie. edge trigger.  If we use LOW the the interrupt
  // continuously fires while the button is pressed and the counter get incremented
  // something like 20,000 for a quick push!
  attachInterrupt(digitalPinToInterrupt(interruptPin), iterruptServiceRoutine, FALLING);

 Serial.println(".. setup() finished");
}

void iterruptServiceRoutine() {
  isrVariable++;  
}

void loop() {
  Serial.print("isrVariable = ");
  Serial.println(isrVariable);
  delay(1000); 
}
