/*
  - Switches use AceButtons library
  - on release callbacks are fired for updates in screen and wifi
*/


// ----------------------------------------
// Local
// ----------------------------------------

#include <AceButton.h>
using namespace ace_button;

const byte PIN_ENTER = D5; // Left
const byte PIN_UP = D6; // Right-top
const byte PIN_DOWN = D7; // Right-bottom
AceButton button_up(PIN_UP);
AceButton button_down(PIN_DOWN);
AceButton button_enter(PIN_ENTER);
int switch_ms_threshold = 20;
long switch_ms_last = 0;

// ----------------------------------------
// Globals
// ----------------------------------------

extern unsigned int place_idx_cur;
extern boolean wifi_tx_status;
extern const unsigned int NPLACES;

// callbacks for switches
extern void update_display();
extern void update_wifi();

// eeprom
extern unsigned long last_eeprom_place_update_ms;
extern unsigned long last_eeprom_tx_update_ms;
extern boolean eeprom_values_to_write_place;
extern boolean eeprom_values_to_write_tx;


// ------------------------------------------
// Button Callbacks
// ------------------------------------------

void on_switch_up() {
  place_idx_cur = (place_idx_cur + 1 ) % NPLACES;
  eeprom_values_to_write_place = true;
  last_eeprom_place_update_ms = millis();
  update_display();
  update_wifi();
}
void on_switch_down() {
  if (place_idx_cur <= 0) {
    place_idx_cur = NPLACES - 1;
  } else {
    place_idx_cur--;
  }
  eeprom_values_to_write_place = true;
  last_eeprom_place_update_ms = millis();
  update_display();
  update_wifi();
}

void on_switch_enter() {
  // toggle wifi
  wifi_tx_status = !wifi_tx_status;
  eeprom_values_to_write_tx = true;
  last_eeprom_tx_update_ms = millis();
  update_display();
  update_wifi();
}


void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  switch (eventType) {
    case AceButton::kEventPressed:
      break;
    case AceButton::kEventReleased:
      uint8_t pin = button->getPin();
      switch (pin) {
        case PIN_UP:
          on_switch_up();
          break;
        case PIN_DOWN:
          on_switch_down();
          break;
        case PIN_ENTER:
          on_switch_enter();
          break;
      }
      break;
  }
}


void setup_switches() {

  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_ENTER, INPUT_PULLUP);

  // Right Up Button
  ButtonConfig* buttonConfigUp = button_up.getButtonConfig();
  buttonConfigUp->setEventHandler(handleEvent);
  buttonConfigUp->setFeature(ButtonConfig::kFeatureClick);
  buttonConfigUp->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfigUp->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfigUp->setFeature(ButtonConfig::kFeatureRepeatPress);

  // Right Down Button
  ButtonConfig* buttonConfigDown = button_down.getButtonConfig();
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


void run_switches() {
  if (millis() - switch_ms_last > switch_ms_threshold) {
    // threshold to reduce mcu load
    switch_ms_last = millis();
    // update AceButtons
    button_up.check();
    button_down.check();
    button_enter.check();
  }
}

