#include <EEPROM.h>

// this constant won't change:
const int inPin = 2;   // choose the input pin (for a pushbutton)

// Variables will change:
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
int pin13state = 1;          // kdyby = 0 (LOW) nebo neurceno, tak ledka sviti hned po nahrani programu
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonPushCounter1 = 0;
int led_builtin;
int ledState;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);    // declare LED as input
  pinMode(inPin, INPUT_PULLUP);    // declare pushbutton as input....vyuzit internal pull up rezistor

  checkLedState();
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

  buttonState = digitalRead(inPin);                                           // read the pushbutton input pin: na zacatku HIGH, jelikoz mam INPUT_PULLUP

  if (buttonState != lastButtonState && buttonPushCounter1 != 0) {            // kvuli EEPROM pridana druha podminka; blok se vykona aspon po jednom zmacknuti)
    if (buttonState == HIGH) {                                                // kdyby = 0 (LOW), tak ledka sviti hned po nahrani programu; drzim button -> buttonState = 1, kdyz pustim -> buttonState = 0
      if (pin13state == 0) {                                                  // kdyz se zmeni ButtonState, prehod state pinu13 z 0 na 1 nebo obracene
        pin13state = 1;
      }
      else {
        pin13state = 0;
      }
      buttonPushCounter++;
      Serial.print(buttonPushCounter);
      Serial.println(" macknuti");
      delay(250);                                   // Delay a little bit to avoid bouncing; pokud moc maly, button neprepina poradne
    }
    lastButtonState = buttonState;                  // save the current state as the last state; kdyz tlacitko drzim (lastButtonState = 0), kdyz pustim (= 1) - je to kvuli INPUT_PULLUP
    digitalWrite(LED_BUILTIN, pin13state);
    EEPROM.update(10, pin13state);
  }
  
  if (buttonState == LOW) {
    buttonPushCounter1++;
  }
}

void checkLedState() {
  ledState = EEPROM.read(10);
  delay(250);
  if (ledState == 1) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (ledState == 0) {
    digitalWrite(LED_BUILTIN, LOW);
  }

}

// Furt neni uplne OK, pri prvnim zmacknuti nezmeni svuj stav, az pri druhym!!! (Oprava: pokud na zacatku sviti, pri macknuti vypne; ale pokud na zacatku nesviti, pro zapnuti musis macknout dvakrat)

/*
  if (buttonState != lastButtonState) {...tenhle blok je provede pouze pokud se zmeni stav tlacitka}
  if (buttonState == HIGH) {u zmeny stavu "spinace" detekuju HIGH, tedy kdyz nedrzim spinac, protoze mam INPUT_PULLUP; proto se mi taky rovnou ten blok provede po zpusteni a ja to obesel ze vypisuju "0 macknuti"}
  if (pin13state == 0) {}...zajistuje prepinani DIODY, detekuju v podstate kazde puštění tlacitka (spinace), tomi zajistuje ta podminka nad "if (buttonState == HIGH)" .....pokr dalsi radka
  else {}                    .. jelikoz nechci aby mi to svitilo rovnou po prvnim pusteni a mam nastavenou globalni promenou pin13state = 1, tak se provede cast "else"...
*/

