const int buttonPin = 4; // GPIO 16 (D0 on NodeMCU)
int buttonState = 0;

void setup() {
  Serial.begin(115200);
  // Initialize the pushbutton pin as an input with internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // Read the state of the pushbutton value
  buttonState = digitalRead(buttonPin);

  // Check if the button is pressed. 
  // (In pull-up configuration, pressed = LOW)
  if (buttonState == LOW) {
    Serial.println("Button is pressed!");
  } else {
    Serial.println("Button is released.");
  }
  
  delay(2000); // Simple delay for debouncing
}