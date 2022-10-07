/*
   Setup and run the OLED display and blinking on-board LED
*/

// libs
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ------------------------------------------------------
// globals
// ------------------------------------------------------
extern String place_names[NPLACES];
extern String place_cities[NPLACES];
extern unsigned int place_idx_cur;
extern boolean wifi_tx_status;


// ------------------------------------------------------
// Local
// ------------------------------------------------------
// Blink LED
const byte PIN_LED_BLINK = D4; // Usually D4 on NodeMCU 12
unsigned int blink_interval_a = 1000; // ms
unsigned int blink_interval_b = 25; // ms
unsigned long last_blink_ms = 0;
unsigned char blink_state = 1;

// OLED Display
#define OLED_RESET 1 // nicht genutzt bei diesem Display
Adafruit_SSD1306 display(OLED_RESET);

// WiFi icon graphics for animation (sm, md, lg)
unsigned int wifi_icon_wh = 16;
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



// ------------------------------------------
// Update screen display
// ------------------------------------------

void update_display() {
  /*
   * NB: the screen is only updated when wifi toggle switch is pressed 
   * running the screen uses MCU cycles which will degrade wifi beacon transmit interval accuracy
  */
  display.clearDisplay();

  // Pool A/B
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Location ");
  display.print((place_idx_cur + 1));
  display.print("/");
  display.println(NPLACES);

  // Name and location
  display.setCursor(0, 13);
  display.println(place_names[place_idx_cur]);
  display.setCursor(0, 23);
  display.println(place_cities[place_idx_cur]);

  // Wifi icon
  if (wifi_tx_status == 1) {
    display.drawBitmap(112, 0,  wifi_icon_sm, wifi_icon_wh, wifi_icon_wh, 1);
    display.drawBitmap(112, 0,  wifi_icon_md, wifi_icon_wh, wifi_icon_wh, 1);
    display.drawBitmap(112, 0,  wifi_icon_lg, wifi_icon_wh, wifi_icon_wh, 1);
  } else {
    display.setCursor(110, 0);
    display.print("OFF");
  }
  display.display();

  if (wifi_tx_status == 0) {
    digitalWrite(PIN_LED_BLINK, 1);
    last_blink_ms = millis();
  }

}


// ------------------------------------------
// Setup
// ------------------------------------------

void setup_display() {

  // Setup LED
  pinMode(PIN_LED_BLINK, OUTPUT);
  digitalWrite(PIN_LED_BLINK, 0);

  // Initialize OLED I2C display (128x64)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setRotation(0); // flip screen upside down
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextWrap(false);

  // Title only
  display.setCursor(28, 13);
  display.setTextSize(1);
  display.println("S K Y L I F T");

  display.display();

  delay(1000);

  // Skylift
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(23, 10);
  display.println("S K Y L I F T");
  display.setCursor(45, 21);
  display.println(DEV_VERSION);
  display.display();
  delay(1000);

  // AH
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(30, 10);
  display.println("ADAM HARVEY");
  display.setCursor(42, 21);
  display.println("2 0 1 8");
  display.display();
  delay(500);

  
  // Sketch stats
  display.clearDisplay();
  display.setCursor(4, 2);
  display.setTextSize(1);
  display.print("Version: ");
  display.setCursor(70, 2);
  display.print(DEV_VERSION);

  display.setCursor(4, 12);
  display.setTextSize(1);
  display.print("Locations: ");
  display.setCursor(70, 12);
  display.print(NPLACES);

  display.setCursor(4, 22);
  display.setTextSize(1);
  display.print("Networks: ");
  display.setCursor(70, 22);
  display.print(NNETS);
  
  // Print location to screen
  update_display();

}


// ------------------------------------------
// Blink LED
// ------------------------------------------

void run_blink() {
  if (!wifi_tx_status) {
    // LED is OFF when WiFi is OFF
    return;
  }
  long t = blink_state == 0 ? blink_interval_a : blink_interval_b;
  if (millis() - last_blink_ms > t) {
    digitalWrite(PIN_LED_BLINK, blink_state);
    last_blink_ms = millis();
    blink_state++;
    blink_state %= 2;
  }
}

void run_display() {
  // run the LED (screen is updated only when button pressed)
  run_blink();
}
