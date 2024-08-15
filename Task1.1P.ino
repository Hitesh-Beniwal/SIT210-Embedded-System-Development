const int ledPin = LED_BUILTIN; // LED pin
const int buttonPin = 2; // Button pin
const int dotDuration = 250; // Duration of a dot
const int dashDuration = 1000; // Duration of a dash
const int letterPause = 1000; // Pause between letters
const int wordPause = 1500; // Pause between words
bool buttonPressed = false;  // State of the button press

// Function to blink a dot
void blinkDot() {
  digitalWrite(ledPin, HIGH);
  delay(dotDuration);
  digitalWrite(ledPin, LOW);
  delay(dotDuration);
  Serial.print("."); // Print dot to Serial Monitor
}

//Similarly the function to blink a dash
void blinkDash() {
  digitalWrite(ledPin, HIGH);
  delay(dashDuration);
  digitalWrite(ledPin, LOW);
  delay(dotDuration);
  Serial.print("-"); // Print dash to Serial Monitor
}

// Function to blink a character in Morse code
void blinkCharacter(char c) {
  switch (c) {
    case 'H':
      blinkDot(); blinkDot(); blinkDot(); blinkDot(); // H: ....
      break;
    case 'I':
      blinkDot(); blinkDot(); // I: ..
      break;
    case 'T':
      blinkDash(); // T: -
      break;
    case 'E':
      blinkDot(); // E: .
      break;
    case 'S':
      blinkDot(); blinkDot(); blinkDot(); // S: ...
      break;
  }
  delay(letterPause); // Pause between letters
  Serial.print(" ");  // Print a space for clarity between characters
}

// Function to blink the name "HITESH" in Morse code
void blinkName() {
  char name[] = {'H', 'I', 'T', 'E', 'S', 'H'};
  int length = sizeof(name) / sizeof(name[0]);
  
  Serial.print("Morse code of Hitesh is "); // Print the initial message
  
  for (int i = 0; i < length; i++) {
    blinkCharacter(name[i]);
  }
  delay(wordPause); // Pause between words
  Serial.println(); // Move to the next line in Serial Monitor
}

// Function to check if the button is pressed
bool isButtonPressed() {
  if (digitalRead(buttonPin) == LOW) {
    delay(50); // Debounce delay
    if (digitalRead(buttonPin) == LOW) {
      return true;
    }
  }
  return false;
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Enable internal pull-up resistor
  Serial.begin(9600); // Start serial communication
}

void loop() {
  if (isButtonPressed()) {
    blinkName();  // Blink the Morse code for HITESH
  }
}