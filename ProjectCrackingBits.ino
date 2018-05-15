

// this constant won't change:
const int inPin = 2;   // choose the input pin (for a pushbutton)


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);    // declare LED as input
}


void loop() {
  if (Serial.available()) {
    //delay(50);                               // wait a bit for the entire message to arrive; it is neccessary? commented 15 5 2018
    while (Serial.available() > 0) {           // read all the available characters
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

}




