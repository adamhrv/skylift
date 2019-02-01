/*

  S K Y L I F T (0.3): Research for Solitude Edition
  https://ahprojects.com/projects/skylift
  Adam Harvey 2017

  Display WiFi animation on OLED screen
  (No WiFi functionality in this script)

  This example shows how to scroll text horizontally and display BSSIDs
  Compiled with Arduino 1.6.12 on ES8266 NodeMCU 0.9 12

  OLED Wiring:
  - D2 -> SDA
  - D3 -> SCL

*/

// Use D0 for wide-PCB NodeMCU tor D4 for slim (normal) NodeMCU boards
#define PIN_LED_BLINK D4

// OLED includes
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define nn 6 // number of networks
uint8_t ncounter = 0; // network counter

char* ssids_str[nn] = {
  "AA:AA:AA:AA:AA:AA",
  "BB:BB:BB:BB:BB:BB",
  "CC:CC:CC:CC:CC:CC",
  "DD:DD:DD:DD:DD:DD",
  "EE:EE:EE:EE:EE:EE",
  "FF:FF:FF:FF:FF:FF"
};


// OLED Display
#define OLED_RESET 1 // nicht genutzt bei diesem Display
Adafruit_SSD1306 display(OLED_RESET);
boolean refresh_display = false;


// Blink LED
uint8_t blink_interval_a = 1500; // ms
uint8_t blink_interval_b = 50; // ms
unsigned long last_blink_ms = 0;
boolean blink_val = 0;
uint8_t blink_count = 0;

// Scrolling text
char welcome_to[] = "Scrolling Text Demo 123 456";
unsigned int xoffset = 336; // about 12px per char, adjust as needed
unsigned int x2pos = 13; // position of offscreen scrolling text, adjust as needed
unsigned int cpos = 0; // current position of text on screen
unsigned int xspeed = 2; // horizontal animation speed
unsigned int scroll_interval_ms = 90; // move xpseed every every interval
unsigned long last_scroll_ms  = 0;
unsigned int yoffset = 14; // for wifi icon and BSSIDs

// Run BSSID
unsigned int mac_anim_interval = 449;
unsigned long last_mac_anim_ms = 0;
unsigned int ssid_counter = 0;
unsigned int pre_ssid_counter = 0;

// Run WiFi
unsigned int wifi_anim_interval = 261; // ms
unsigned long last_wifi_ms = 0;
unsigned int wifi_state = 0;
unsigned int wifi_icon_wh = 16;


// WiFi icon graphics for animation (sm, md, lg)
static const unsigned char PROGMEM wifi_icon_lg[] =
{
  0x1f, 0xe0, 0xe0, 0x1c, 0x0, 0x0, 0x7, 0x80, 0x18, 0x60, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};
static const unsigned char PROGMEM wifi_icon_md[] =
{
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0x80, 0x18, 0x60, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};
static const unsigned char PROGMEM wifi_icon_sm[] =
{
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};


void setup() {
  // Initialize OLED I2C display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setRotation(2); // flip screen upside down
  display.clearDisplay();
  // Display intro message
  display.setTextColor(WHITE);
  display.setTextWrap(false);
  display.setTextSize(1);
  display.setCursor(23, 14);
  display.println("S K Y L I F T");
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  run_blink();
  run_screen();
}

// ---------------------------------------------------------------
// Update display screen
// ---------------------------------------------------------------
void run_screen() {
  run_scroll_welcome();
  run_wifi_icon();
  run_mac_addr();
  if (refresh_display == true) {
    display.display(); // keep display() calls to a minimum
    refresh_display = false;
  }
}

// ---------------------------------------------------------------
// Amimate mac addresses
// ---------------------------------------------------------------
void run_mac_addr() {
  if (millis() - last_mac_anim_ms < mac_anim_interval) {
    return;
  }
  // Use the pixels from previous text to fill in black
  // this reduces number of operations to clear display
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(24, 0);
  display.println(ssids_str[pre_ssid_counter]);
  display.setCursor(24, 0);
  display.setTextColor(WHITE);
  display.println(ssids_str[ssid_counter]);
  refresh_display = true;
  last_mac_anim_ms = millis();
  pre_ssid_counter = ssid_counter;
  ssid_counter = (ssid_counter + 1) % nn;
  mac_anim_interval = random(800, 1400); // randomize to appear more realistic

}


// ---------------------------------------------------------------
// Animate WiFi icon
// ---------------------------------------------------------------
void run_wifi_icon() {
  // Animate between 3 graphics for WiFi icon
  if ( millis() - last_wifi_ms < wifi_anim_interval) {
    return;
  }
  wifi_state++;
  if (wifi_state > 3) {
    wifi_state = 0;
  }
  // Clear background
  display.fillRect(0, 0, 14, 7, BLACK);
  switch (wifi_state) {
    case 0:
      // Use the pixels from bitmap shape to fill in black
      // this reduces number of operations to clear display
      display.drawBitmap(0, 0,  wifi_icon_sm, wifi_icon_wh, wifi_icon_wh, 0);
      display.drawBitmap(0, 0,  wifi_icon_md, wifi_icon_wh, wifi_icon_wh, 0);
      display.drawBitmap(0, 0,  wifi_icon_lg, wifi_icon_wh, wifi_icon_wh, 0);
      break;
    case 1:
      display.drawBitmap(0, 0,  wifi_icon_sm, wifi_icon_wh, wifi_icon_wh, 1);
      break;
    case 2:
      display.drawBitmap(0, 0,  wifi_icon_md, wifi_icon_wh, wifi_icon_wh, 1);
      break;
    case 3:
      display.drawBitmap(0, 0,  wifi_icon_lg, wifi_icon_wh, wifi_icon_wh, 1);
      break;
  }
  refresh_display = true;
  last_wifi_ms = millis();

}


// ---------------------------------------------------------------
// Scrolling Welcome Text
// ---------------------------------------------------------------
void run_scroll_welcome() {

  // Use the pixels from previous text to fill in black
  // this reduces number of operations to clear display
  if (millis() - last_scroll_ms < scroll_interval_ms) {
    return;
  }
  display.setTextSize(2);
  cpos -= xspeed;
  // reset animation after xoffset mark
  if (cpos < -xoffset) {
    cpos = 0;
  }
  last_scroll_ms = millis();
  // Add second text instance
  if (cpos < -x2pos) {
    // pixels off
    display.setTextColor(BLACK);
    display.setCursor(cpos + xoffset + xspeed, yoffset);
    display.println(welcome_to);
    // pixels on
    display.setTextColor(WHITE);
    display.setCursor(cpos + xoffset, yoffset);
    display.println(welcome_to);
  }
  // First, leftmost, text instance
  display.setTextColor(BLACK);
  display.setCursor(cpos + xspeed, yoffset);
  display.println(welcome_to);
  display.setTextColor(WHITE);
  display.setCursor(cpos, yoffset);
  display.println(welcome_to);
  refresh_display = true;

}

// ---------------------------------------------------------------
// Blink on-board LED
// ---------------------------------------------------------------
void run_blink() {
  long t = blink_count == 0 ? blink_interval_a : blink_interval_b;
  if (millis() - last_blink_ms < t) {
    return;
  }
  digitalWrite(PIN_LED_BLINK, blink_count);
  last_blink_ms = millis();
  blink_count++;
  blink_count %= 2;

}


