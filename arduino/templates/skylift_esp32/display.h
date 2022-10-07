/*
   Setup and run the display
  - built in LED
*/


// ------------------------------------------------------
// Local
// ------------------------------------------------------

// Blink LED
const byte PIN_LED_BLINK = 2;
unsigned int blink_interval_a = 25; // ms
unsigned int blink_interval_b = 1000; // ms
unsigned int blink_interval_ms = 1000;
unsigned long last_blink_ms = 0;
unsigned char blink_state = 1;


// ------------------------------------------
// Setup
// ------------------------------------------

void setup_display() {
  // Setup LED
  pinMode(PIN_LED_BLINK, OUTPUT);
  digitalWrite(PIN_LED_BLINK, LOW);
}


// ------------------------------------------
// Blink LED
// ------------------------------------------

void run_blink() {
  blink_interval_ms = blink_state == 0 ? blink_interval_a : blink_interval_b;
  if (millis() - last_blink_ms > blink_interval_ms) {
    digitalWrite(PIN_LED_BLINK, blink_state);
    last_blink_ms = millis();
    blink_state++;
    blink_state %= 2;
  }
}

void run_display() {
  run_blink();
}
