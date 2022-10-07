/*
 * Test the buttons using serial
 */
const byte PIN_LEFT = D5;
const byte PIN_RIGHT = D6;
const byte PIN_ENTER = D7;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LEFT, INPUT_PULLUP);
  pinMode(PIN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_ENTER, INPUT_PULLUP);
}


void loop() {
  int val_left = digitalRead(PIN_LEFT);
  int val_right = digitalRead(PIN_RIGHT);
  int val_center = digitalRead(PIN_ENTER);
  Serial.print(val_left);
  Serial.print("\t");
  Serial.print(val_right);
  Serial.print("\t");
  Serial.println(val_center);
  delay(100);
}
