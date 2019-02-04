/*
   S K Y L I F T
   Test Switches using Ace Buttons library
   (no Wi-Fi functionality in this script)
*/


// ----------------------------------------
// Ace Buttons
// ----------------------------------------

#include <AceButton.h>
using namespace ace_button;

const byte PIN_ENTER = D5; // Left
const byte PIN_RIGHT_TOP = D6; // Right-top
const byte PIN_RIGHT_BOTTOM = D7; // Right-bottom
AceButton button_right_top(PIN_RIGHT_TOP);
AceButton button_right_bottom(PIN_RIGHT_BOTTOM);
AceButton button_enter(PIN_ENTER);
int switch_ms_threshold = 20;
long switch_ms_last = 0;


void setup() {
  Serial.begin(9600);

  pinMode(PIN_RIGHT_TOP, INPUT_PULLUP);
  pinMode(PIN_RIGHT_BOTTOM, INPUT_PULLUP);
  pinMode(PIN_ENTER, INPUT_PULLUP);

  // Right top Button
  ButtonConfig* buttonConfigUp = button_right_top.getButtonConfig();
  buttonConfigUp->setEventHandler(handleEvent);
  buttonConfigUp->setFeature(ButtonConfig::kFeatureClick);
  buttonConfigUp->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfigUp->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfigUp->setFeature(ButtonConfig::kFeatureRepeatPress);

  // Right bottom Button
  ButtonConfig* buttonConfigDown = button_right_bottom.getButtonConfig();
  buttonConfigDown->setEventHandler(handleEvent);
  buttonConfigDown->setFeature(ButtonConfig::kFeatureClick);
  buttonConfigDown->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfigDown->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfigDown->setFeature(ButtonConfig::kFeatureRepeatPress);

  // Left Enter Button
  ButtonConfig* buttonConfigEnter = button_enter.getButtonConfig();
  buttonConfigEnter->setEventHandler(handleEvent);
  buttonConfigEnter->setFeature(ButtonConfig::kFeatureClick);
  buttonConfigEnter->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfigEnter->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfigEnter->setFeature(ButtonConfig::kFeatureRepeatPress);
}


void loop() {
  if (millis() - switch_ms_last > switch_ms_threshold) {
    // threshold to reduce mcu load
    switch_ms_last = millis();
    // update AceButtons
    button_right_top.check();
    button_right_bottom.check();
    button_enter.check();
  }
}


// ------------------------------------------
// Button Callbacks
// ------------------------------------------

void on_button_right_top() {
  Serial.println("Right top pressed");
}
void on_button_right_bottom() {
  Serial.println("Right bottom pressed");
}
void on_button_enter() {
  Serial.println("Left button pressed");
}


void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  switch (eventType) {
    case AceButton::kEventPressed:
      break;
    case AceButton::kEventReleased:
      uint8_t pin = button->getPin();
      switch (pin) {
        case PIN_RIGHT_TOP:
          on_button_right_top();
          break;
        case PIN_RIGHT_BOTTOM:
          on_button_right_bottom();
          break;
        case PIN_ENTER:
          on_button_enter();
          break;
      }
      break;
  }
}
