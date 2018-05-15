

// this constant won't change:
const int inPin = 2;   // choose the input pin (for a pushbutton)

// Variables will change:
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
int pin13state = 1;          // kdyby = 0 (LOW), tak ledka sviti hned po nahrani programu
int buttonPushCounter = 0;   // counter for the number of button presses

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);    // declare LED as input
  pinMode(inPin, INPUT_PULLUP);    // declare pushbutton as input....vyuzit internal pull up rezistor

}

void loop() {
  if (Serial.available()) {
    //delay(50);                               // wait a bit for the entire message to arrive; it is neccessary? commented 15 5 2018
    while (Serial.available() > 0) {          // read all the available characters
      String ch;
      ch = Serial.readString();
      ch.trim();
      if (ch == "zapni" || ch == "ZAPNI") {
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("Mas to zaply");
      }
      else if (ch == "vypni" || ch == "VYPNI") {
        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("Mas to vyply");
      }
      else {
        Serial.println("Neplatny vyraz");
      }
    }
  }

  buttonState = digitalRead(inPin);                // read the pushbutton input pin: na zacatku HIGH, jelikoz mam INPUT_PULLUP

  if (buttonState != lastButtonState) {            // compare the buttonState to its previous state
    if (buttonState == HIGH) {                     // kdyby = 0 (LOW), tak ledka sviti hned po nahrani programu
      if (pin13state == 0) {                       // kdyz se zmeni ButtonState, prehod state pinu13 z 0 na 1 nebo obracene
        pin13state = 1;
        buttonPushCounter++;
      }
      else {
        pin13state = 0;
        buttonPushCounter++;
      }
      Serial.println(buttonPushCounter);
      delay(250);                                   // Delay a little bit to avoid bouncing; pokud moc maly, button neprepina poradne
    }
    lastButtonState = buttonState;                  // save the current state as the last state, for next time through the loop
    digitalWrite(LED_BUILTIN, pin13state);
  }
}



