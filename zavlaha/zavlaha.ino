void setup() {
  pinMode(12, OUTPUT);
  pinMode(14, OUTPUT);
  //pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(12, HIGH);
  digitalWrite(14, LOW);
  //digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(12, LOW);
  digitalWrite(14, HIGH);
  //digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
