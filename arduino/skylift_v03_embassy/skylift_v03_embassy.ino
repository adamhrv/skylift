/*

  S K Y L I F T (0.3): Schloss Solitude Edition
  https://ahprojects.com/projects/skylift
  Adam Harvey 2017

  Send WiFi Beacon Frames and display WiFi animation
  Compiled with Arduino 1.6.12 on ES8266 NodeMCU 0.9 12

  SkyLift V0.3 is a continuation of the first prototype, SkyLift V0.1, a collaboration
  with Surya Mattu and !Mediengruppe Bitnik that used a Raspberry Pi,  mdk3, and collected BSSIDs
  to virtually relocate smartphones to Julian Assange's residence at the Ecuadorean Embassy in London.

  OLED Wiring:
  - D2 -> SDA
  - D3 -> SCL

*/

// ESP8266
#include <ESP8266WiFi.h>
extern "C" {
#include "user_interface.h"
}

// OLED includes
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


//#define USE_HIDDEN_SSID // uncomment to use hidden SSIDs
//#define USE_SCREEN 1 // uncomment when using screen (for display device)
#define PIN_LED_BLINK D4 // Or D0 depending on board

// ------------------------------------------------------------------
// ------------------------------------------------------------------
// START Editable Location data
// ------------------------------------------------------------------
// ------------------------------------------------------------------

#define nn 17

char* ssids_str[nn] = {
  "A0:56:B2:91:67:6D",
  "80:71:7A:9D:6F:70",
  "C0:3E:0F:C3:11:15",
  "7C:4C:A5:69:70:C1",
  "84:18:3A:24:60:88",
  "26:6A:9B:EE:E1:A0",
  "12:8E:38:21:83:E6",
  "AC:84:C9:AB:F9:74",
  "C0:3E:0F:87:E2:45",
  "02:8A:AE:91:7F:AC",
  "32:CD:A7:AF:FB:10",
  "1C:49:7B:18:D3:54",
  "A0:8D:16:8A:DC:D8",
  "00:62:2C:09:6C:34",
  "9C:80:DF:E2:9F:26",
  "EC:8E:B5:F5:0A:7E",
  "1C:49:7B:17:78:37"
};

byte bssids[nn][6] = {
  {0xA0, 0x56, 0xB2, 0x91, 0x67, 0x6D},
  {0x80, 0x71, 0x7A, 0x9D, 0x6F, 0x70},
  {0xC0, 0x3E, 0x0F, 0xC3, 0x11, 0x15},
  {0x7C, 0x4C, 0xA5, 0x69, 0x70, 0xC1},
  {0x84, 0x18, 0x3A, 0x24, 0x60, 0x88},
  {0x26, 0x6A, 0x9B, 0xEE, 0xE1, 0xA0},
  {0x12, 0x8E, 0x38, 0x21, 0x83, 0xE6},
  {0xAC, 0x84, 0xC9, 0xAB, 0xF9, 0x74},
  {0xC0, 0x3E, 0x0F, 0x87, 0xE2, 0x45},
  {0x02, 0x8A, 0xAE, 0x91, 0x7F, 0xAC},
  {0x32, 0xCD, 0xA7, 0xAF, 0xFB, 0x10},
  {0x1C, 0x49, 0x7B, 0x18, 0xD3, 0x54},
  {0xA0, 0x8D, 0x16, 0x8A, 0xDC, 0xD8},
  {0x00, 0x62, 0x2C, 0x09, 0x6C, 0x34},
  {0x9C, 0x80, 0xDF, 0xE2, 0x9F, 0x26},
  {0xEC, 0x8E, 0xB5, 0xF5, 0x0A, 0x7E},
  {0x1C, 0x49, 0x7B, 0x17, 0x78, 0x37}
};

char* ssids[nn] = {
  "MB WLAN 59210",
  "TALKTALK9D6F67",
  "SKYDC5C1",
  "SKYF3AE2",
  "Clear 2.4g",
  "Guest Network",
  "Parrot_00:26:7E:16:93:B5",
  "Happy",
  "SKY7CB62",
  "BTWi-fi",
  "DIRECT-ICM2020 Series",
  "Relish_Home_18D356",
  "TALKTALK8ADCD1",
  "BTHub5-C9NP",
  "BTHub5-5ZJ7",
  "DIRECT-7D-HP ENVY 4520 series",
  "Relish_Home_177839"
};

#ifdef USE_SCREEN
byte channels[nn] = {1, 1, 1, 1, 11, 6, 6, 6, 6, 6, 6, 6, 11, 11, 11, 11, 11};
#else
byte channels[nn] = {3, 4, 7, 0, 5, 1, 6, 10, 6, 4, 5, 8, 3, 10, 10, 2, 12};
#endif


// ------------------------------------------------------------------
// ------------------------------------------------------------------
// END Editable Location data
// ------------------------------------------------------------------
// ------------------------------------------------------------------

// OLED Display
#define OLED_RESET 1 // nicht genutzt bei diesem Display
Adafruit_SSD1306 display(OLED_RESET);
boolean refresh_display = false;


// Blink LED
unsigned int blink_interval_a = 1500; // ms
unsigned int blink_interval_b = 50; // ms
unsigned long last_blink_ms = 0;
boolean blink_val = 0;
unsigned char  blink_count = 0;

// Scrolling text
char welcome_to[] = "Welcome to !Ecuador";
unsigned int xoffset = 239; // about 12px per char, adjust as needed
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

// Beacon frame packet structure
char* hidden_ssid = "\x00"; // appears as 0-length SSID (Network name unavailable)
byte bssid[6] = {};
unsigned char channel;
unsigned long last_beacon_us;
unsigned long beacon_interval_us = 102400 / nn; // Typical interval of beacon frames is 100.24 milliseconds (0x64)
char time_packet[8]; // placeholder for timestamp data
char cc[2]; // placeholer for timestamp data
uint8_t ncounter = 0; // network counter

uint8_t packet[128] = {
  /*0*/   0x80, 0x00, // Frame Control
  /*2*/   0x00, 0x00, // Duration
  /*4*/   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // Destination address
  /*10*/  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Source address - overwritten later
  /*16*/  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // BSSID - overwritten to the same as the source address
  /*22*/  0xc0, 0x6c, // Seq-ctl
  /*24*/  0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, // Timestamp - the number of microseconds the AP has been active
  /*32*/  0x64, 0x00, // Beacon interval = 0.102400 seconds
  /*34*/  0x01, 0x04, // Capability info
  /*36*/  0x00
  /* SSID is added after here */
  /* Followed by packet_tail */
};

uint8_t packet_tail[13] = {
  0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, // supported data rates
  0x03, 0x01, 0x04 // DSSS (Current Channel)
};

// WiFi icon graphics for animation (1 bar, 2 bars, 3 bars)
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

  // Init ESP8266
  WiFi.setOutputPower(20.5); // 0 (min) - 20.5 (max)
  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(1);

#ifdef USE_SCREEN
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
#endif

  // Init I/O pins
  pinMode(PIN_LED_BLINK, OUTPUT);
  digitalWrite(PIN_LED_BLINK, 0);
}


void loop() {
  run_blink(); // Blink onboard LED
  run_wifi(); // Transmit beacon frames
#ifdef USE_SCREEN // Update display
  run_screen();
#endif
}



// **************************************************************
// ESP8266 Functions
// **************************************************************

// ---------------------------------------------------------------
// Transmit Beacon packets
// ---------------------------------------------------------------
void run_wifi() {

  if (micros() - last_beacon_us < beacon_interval_us) {
    return;
  }
  last_beacon_us = micros();

  ncounter = (ncounter + 1) % nn;

  // Send the packet
#ifdef USE_HIDDEN_SSID
  char* ssid = hidden_ssid;
#else
  char* ssid = ssids[ncounter];
#endif
  send_beacon_frame(ssid , channels[ncounter]);
}


// ---------------------------------------------------------------
// Send the beacon frame
// ---------------------------------------------------------------
void send_beacon_frame(char* ssid, byte channel) {

  uint8_t ssid_len = strlen(ssid);
  uint8_t packet_length = 51 + ssid_len;

  // Set BSSID
  for (int i = 0; i < 6; i++) {
    bssid[i] = bssids[ncounter][i];
  }
  for (int i = 0; i < 6; i++) {
    packet[10 + i] = packet[16 + i] = bssid[i];
  }

  // Update the timestamp
  get_timestamp(time_packet);
  for (int i = 0; i < 8; i++) {
    packet[24 + i] = time_packet[i];
  }

  // set the SSID length byte
  packet[37] = ssid_len;
  for (int i = 0; i < ssid_len; i++) {
    packet[38 + i] = ssid[i];
  }

  // Add the ending packet bytes
  for (int i = 0; i < 12; i++) {
    packet[38 + ssid_len + i] = packet_tail[i];
  }

  // Set transmit frequency and packet channel
  wifi_set_channel(channel);
  packet[50 + ssid_len] = channel;

  // Send the packet
  wifi_send_pkt_freedom(packet, packet_length, 0);
}

// ---------------------------------------------------------------
// Convert milliseconds to timestamp for beacon frame
// ---------------------------------------------------------------
void get_timestamp(char *times) {

  long t = micros();
  unsigned int tlen = 8;
  String time_hex_forward = String(t, HEX);

  while (time_hex_forward.length () < tlen * 2) time_hex_forward = "0" + time_hex_forward;
  String time_hex_rev = time_hex_forward; // reversed
  for (int i = tlen; i > 0; i--) {
    time_hex_rev[(tlen * 2) - (i * 2) ] = time_hex_forward.charAt((i * 2) - 2); // 1
    time_hex_rev[(tlen * 2) - (i * 2) + 1] = time_hex_forward.charAt((i * 2) - 1); // 2
  }
  // convert to 8-length byte array
  for (int i = 0; i < tlen; i++) {

    cc[0] = time_hex_rev.charAt(i * 2);
    cc[1] = time_hex_rev.charAt((i * 2) + 1);
    long hex_val = strtol(cc, NULL, 16);
    times[i] = hex_val;
  }
}

long strtol (const char *__nptr, char **__endptr, int __base); // additional function for string to long conversion




// **************************************************************
// Screen functions
// **************************************************************

// ---------------------------------------------------------------
// Update display screen
// ---------------------------------------------------------------
#ifdef USE_SCREEN
void run_screen() {
  run_scroll_welcome();
  run_wifi_icon();
  run_mac_addr();
  if (refresh_display == true) {
    display.display(); // keep display() calls to a minimum
    refresh_display = false;
  }
}
#endif

// ---------------------------------------------------------------
// Amimate mac addresses
// ---------------------------------------------------------------
#ifdef USE_SCREEN
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
#endif


// ---------------------------------------------------------------
// Animate WiFi icon
// ---------------------------------------------------------------
#ifdef USE_SCREEN
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
#endif


// ---------------------------------------------------------------
// Scrolling Welcome Text
// ---------------------------------------------------------------
#ifdef USE_SCREEN
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
#endif

// ---------------------------------------------------------------
// Blink on-board LED
// ---------------------------------------------------------------
void run_blink() {
  long t = blink_count == 0 ? blink_interval_a : blink_interval_b;
  if (millis() - last_blink_ms > t) {
    digitalWrite(PIN_LED_BLINK, blink_count);
    last_blink_ms = millis();
    blink_count++;
    blink_count %= 2;
  }
}


