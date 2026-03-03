#define LED1 LED_BUILTIN
#define LED2 16
void setup() {
  // put your setup code here, to run once:
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED1, LOW);
  delay(200);
  digitalWrite(LED1, HIGH);
  delay(200);
  digitalWrite(LED2, LOW);
  delay(200);
  digitalWrite(LED2, HIGH);
  delay(200);
}