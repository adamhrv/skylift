/*
  PARALLEL ARTIFACTS
  https://ahprojects.com/parallel-artifacts
  Adam Harvey 2022
*/

// {{TEMPLATE:HEADER
// TEMPLATE:HEADER}}

#include "display.h"

byte bssid[6] = {};    // store next bssid
uint8_t ncounter = 0;  // network counter
uint8_t ssid_len;      // holder for ssid length
uint8_t channel_pre = 0; 

// beacon interval is 100.24 milliseconds (0x64)
unsigned long beacon_interval_us = 102400;
unsigned long last_beacon_us = 0;
char* hidden_ssid = "\x00";  // 0-length SSID (Network name unavailable)
#define NUM_TS_PLACES 8
byte time_packet[NUM_TS_PLACES];  // placeholder for timestamp data#define NUM_TS_PLACES 8
char cc[2];                       // placeholer for timestamp data


// ---------------------------------------------------------
// START template data
// ---------------------------------------------------------
// {{TEMPLATE:ESP
#define ESP32 1
// TEMPLATE:ESP}}

#ifdef ESP32
#include "WiFi.h"
extern "C" {
#include "esp_wifi.h"
  esp_err_t esp_wifi_set_channel(uint8_t primary, wifi_second_chan_t second);
  esp_err_t esp_wifi_80211_tx(wifi_interface_t ifx, const void* buffer, int len, bool en_sys_seq);
}
#else
#include <ESP8266WiFi.h>
extern "C" {
#include "user_interface.h"
}
#endif

// {{TEMPLATE:HIDDEN_SSIDS
// #define USE_HIDDEN_SSID 1 // uncomment to use hidden SSIDs
// TEMPLATE:HIDDEN_SSIDS}}

// {{TEMPLATE:NN
#define NN 3  // number of networks
// TEMPLATE:NN}}

// {{TEMPLATE:SSIDS
char* ssids[NN] = {
  "Network1",
  "Network2",
  "Network3",
};
// TEMPLATE:SSIDS}}

// {{TEMPLATE:BSSIDS
byte bssids[NN][6] = {
  { 0x94, 0x57, 0xA5, 0x0E, 0x57, 0x01 },
  { 0x8A, 0x15, 0x04, 0xB8, 0x8A, 0x02 },
  { 0x00, 0x18, 0x0A, 0x80, 0x26, 0x03 },
};
// TEMPLATE:BSSIDS}}

// {{TEMPLATE:CHANNELS
byte channels[NN] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
// TEMPLATE:CHANNELS}}

// {{TEMPLATE:DBM_LEVELS
// TODO
// TEMPLATE:DBM_LEVELS}}

// ---------------------------------------------------------
// END template data
// ---------------------------------------------------------


// Variables referencing templated data
uint8_t channel = channels[0];  // Transmit on this channel
unsigned long ap_epochs[NN];

// Beacon frame packet structure
uint8_t packet[128] = {
  /*0*/ 0x80, 0x00,                                       // Frame Control
  /*2*/ 0x00, 0x00,                                       // Duration
  /*4*/ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,               // Destination address
  /*10*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,              // Source address - overwritten later
  /*16*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,              // BSSID - overwritten to the same as the source address
  /*22*/ 0xc0, 0x6c,                                      // Seq-ctl
  /*24*/ 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,  // Timestamp - the number of microseconds the AP has been active
  /*32*/ 0x64, 0x00,                                      // Beacon interval = 0.102400 seconds
                                                          // /*34*/ 0x01, 0x04,                                    // Capability info
  /*34*/ 0x31, 0x14,                                       // Capability inf
  /*36*/ 0x00
  /* SSID is added after here */
};

uint8_t packet_tail[13] = {
  0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c,  // supported rate
  0x03, 0x01, 0x04                                             // DSSS (Current Channel)
};


// ---------------------------------------------------------------
// Convert milliseconds to timestamp for beacon frame
// ---------------------------------------------------------------
void get_timestamp(char* times) {

  long t = micros();
  unsigned int tlen = 8;
  String time_hex_forward = String(t, HEX);

  while (time_hex_forward.length() < tlen * 2) time_hex_forward = "0" + time_hex_forward;
  String time_hex_rev = time_hex_forward;  // reversed
  for (int i = tlen; i > 0; i--) {
    time_hex_rev[(tlen * 2) - (i * 2)] = time_hex_forward.charAt((i * 2) - 2);      // 1
    time_hex_rev[(tlen * 2) - (i * 2) + 1] = time_hex_forward.charAt((i * 2) - 1);  // 2
  }
  // convert to 8-length byte array
  for (int i = 0; i < tlen; i++) {

    cc[0] = time_hex_rev.charAt(i * 2);
    cc[1] = time_hex_rev.charAt((i * 2) + 1);
    long hex_val = strtol(cc, NULL, 16);
    times[i] = hex_val;
  }
}

void set_timestamp(byte* p, byte n) {
  // return time in reverse order as byte array pointer
  long t = millis() + ap_epochs[n];  // random start time
  p[0] = (t & 0x00000000000000FF);
  p[1] = (t & 0x000000000000FF00) >> 8;
  p[2] = (t & 0x0000000000FF0000) >> 16;
  p[3] = (t & 0x00000000FF000000) >> 24;
  p[4] = (t & 0x000000FF00000000) >> 32;
  p[5] = (t & 0x0000FF0000000000) >> 40;
  p[6] = (t & 0x00FF000000000000) >> 48;
  p[7] = (t & 0x00FF000000000000) >> 56;
}


long strtol(const char* __nptr, char** __endptr, int __base);  // additional function for string to long conversion


// ---------------------------------------------------------------
// Send the beacon frame
// ---------------------------------------------------------------
void send_beacon_frame() {

  // set SSID
#ifdef USE_HIDDEN_SSID
  char* ssid = hidden_ssid;
#else
  char* ssid = ssids[ncounter];
#endif

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
  // get_timestamp(time_packet);
  set_timestamp(time_packet, ncounter);
  for (int i = 0; i < NUM_TS_PLACES; i++) {
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

if (channels[ncounter] != channel_pre) {
#ifdef ESP32
    esp_wifi_set_channel(channels[ncounter], WIFI_SECOND_CHAN_NONE);  // esp32
#else
    wifi_set_channel(channels[ncounter]);           // esp8266
#endif
    packet[50 + ssid_len] = channels[ncounter];
    channel_pre = channels[ncounter];
  }

// Send the packet
#ifdef ESP32
  esp_wifi_80211_tx(WIFI_IF_STA, packet, packet_length, 0);  // esp32
#else
  wifi_send_pkt_freedom(packet, packet_length, 0);  // esp8266
#endif
}


// ---------------------------------------------------------------
// Initialize the ESP8266
// Set max power and put into promiscious mode
// ---------------------------------------------------------------
void setup() {
  setup_display();
#ifdef ESP32
  WiFi.mode(WIFI_MODE_STA);
  WiFi.setTxPower(WIFI_POWER_19dBm);
  esp_wifi_set_channel(channels[0], WIFI_SECOND_CHAN_NONE);
#else
  WiFi.setOutputPower(20.5);
  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(1);
#endif
}

// ---------------------------------------------------------------
// Transmit Beacon Frame
// ---------------------------------------------------------------
void loop() {
  // Limit to every 100ms
  if (micros() - last_beacon_us < beacon_interval_us) {
    return;
  }

  last_beacon_us = micros();
  ncounter = (ncounter + 1) % NN;

  // Send the packet
  send_beacon_frame();

  // display
  run_display();
}