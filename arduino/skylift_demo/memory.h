/*
   Setup and run the EEPROM memory management
  - address 0: place index
  - address 1: wifi on/off
*/
#include <EEPROM.h>

// Global
extern const unsigned int NPLACES;
extern unsigned int place_idx_cur;
extern boolean wifi_tx_status;
unsigned long last_eeprom_place_update_ms = 0;
unsigned long last_eeprom_tx_update_ms  = 0;
boolean eeprom_values_to_write_place = false;
boolean eeprom_values_to_write_tx = false;

// Local
unsigned int eeprom_write_delay_place_ms = 2000; // ms until EEPROM writes current network
unsigned int eeprom_write_delay_tx_ms = 5000; // ms until EEPROM writes current network


// ------------------------------------------
// EEPROM
// ------------------------------------------

void update_eeprom_place() {
  eeprom_values_to_write_place = false; // reset flag
  EEPROM.write(0, place_idx_cur);
  EEPROM.commit();
}
void update_eeprom_wifi_tx() {
  // Write the WiFi transmit (on/off)
  eeprom_values_to_write_tx = false; // reset flag
  EEPROM.write(1, wifi_tx_status);
  EEPROM.commit();
}

void run_memory() {
  // Timer function for writing current VIP index to eeprom
  if (millis() - last_eeprom_place_update_ms > eeprom_write_delay_place_ms ) {
    last_eeprom_place_update_ms = millis(); // reset timer
    if (eeprom_values_to_write_place) {
      update_eeprom_place();
    }
  }

  // Timer function for writing current wifi status to eeprom
  if (millis() - last_eeprom_tx_update_ms > eeprom_write_delay_tx_ms) {
    last_eeprom_tx_update_ms = millis(); // reset timer
    if (eeprom_values_to_write_tx) {
      update_eeprom_wifi_tx();
    }
  }
}

void setup_memory() {
  EEPROM.begin(512);
  // Get VIP and WiFi status values from memory
  place_idx_cur = EEPROM.read(0);
  // threshold in case anything weird happens to EEPROM
  if (place_idx_cur > NPLACES - 1) {
    place_idx_cur = NPLACES - 1;
  } else if (place_idx_cur < 0) {
    place_idx_cur = 0;
  }
  
  // WiFi on/off
  wifi_tx_status = EEPROM.read(1);
  if (wifi_tx_status > 0) {
    wifi_tx_status = 1;
  }

}
