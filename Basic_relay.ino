/*
Basic relay 
*/
int relayPin = 2; // The input to the relay pin

void setup() {
  pinMode(relayPin, OUTPUT); // Initialize pin as OUTPUT
}

void loop() {
  //Sätter igång reläet
  digitalWrite(relayPin, HIGH);
  //Väntar 1 sekund
  delay(1000);
  //Stänger av reläet
  digitalWrite(relayPin, LOW);
  //Väntar 1 sekund
  delay(1000)
  //Går tillbaka till det förta i void loop funktionen
}
