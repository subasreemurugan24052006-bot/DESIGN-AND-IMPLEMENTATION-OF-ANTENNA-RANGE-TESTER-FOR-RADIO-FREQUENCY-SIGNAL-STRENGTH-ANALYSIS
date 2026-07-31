RX Code:

#include <SPI.h> 
#include <LoRa.h> 
#include <Wire.h> 
#include <U8g2lib.h>
 #include <math.h>
// OLED U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0);
// LoRa Pins #define SS 10 #define RST 9 #define DIO0 2

float rxLat = 11.000000; float rxLon = 77.000000;
String data = ""; unsigned long packetCount = 0;

// Distance Calculation float calculateDistance(float lat1, float lon1, float lat2, float lon2) { float R = 6371000;
float dLat = (lat2 - lat1) * PI / 180.0; float dLon = (lon2 - lon1) * PI / 180.0;
float a = sin(dLat/2) * sin(dLat/2) + cos(lat1 * PI / 180.0) * cos(lat2 * PI / 180.0) * sin(dLon/2) * sin(dLon/2);
float c = 2 * atan2(sqrt(a), sqrt(1-a)); return R * c; }
void setup() { Serial.begin(9600); u8g2.begin();

// 🟢 STARTUP SCREEN u8g2.firstPage();
 do { u8g2.setFont(u8g2_font_ncenB10_tr); u8g2.setCursor(10,15); u8g2.print("RF Test Kit");
u8g2.setFont(u8g2_font_6x10_tr);
u8g2.setCursor(5,35);
u8g2.print("LoRa 433-525 MHz");

u8g2.setCursor(0,55);
u8g2.print("Antenna Range Finder");
 
} while (u8g2.nextPage());
delay(1500);
// 🟡 STATUS SCREEN u8g2.firstPage(); do { 

u8g2.setFont(u8g2_font_ncenB08_tr); u8g2.setCursor(10,25); u8g2.print("Waiting for GPS");
u8g2.setCursor(10,45);
u8g2.print("Analysing Data...");
 
} while (u8g2.nextPage());
delay(1500);

// LoRa Init LoRa.setPins(SS, RST, DIO0);
if (!LoRa.begin(433E6)) { Serial.println("LoRa Failed!"); while (1); }
Serial.println("RX Ready"); }
void loop() { int packetSize = LoRa.parsePacket();
if (packetSize) {
packetCount++;

data = "";

while (LoRa.available()) {
  data += (char)LoRa.read();
}

int rssi = LoRa.packetRssi();

int commaIndex = data.indexOf(',');
float txLat = data.substring(0, commaIndex).toFloat();
float txLon = data.substring(commaIndex + 1).toFloat();

float distance = calculateDistance(rxLat, rxLon, txLat, txLon);

String quality;
if (rssi > -60) quality = "STRONG";
else if (rssi > -90) quality = "GOOD";
else quality = "WEAK";

// Serial Output
Serial.print("Dist:");
Serial.print(distance, 2);
Serial.print("m | RSSI:");
Serial.print(rssi);
Serial.print(" | Pkt:");
Serial.print(packetCount);
Serial.print(" | ");
Serial.println(quality);

// 🔵 LIVE DATA SCREEN
u8g2.firstPage();
do {
  u8g2.setFont(u8g2_font_ncenB08_tr);

  u8g2.setCursor(0,12);
  u8g2.print("Dist:");
  u8g2.print(distance, 2);
  u8g2.print("m");

  u8g2.setCursor(0,26);
  u8g2.print("RSSI:");
  u8g2.print(rssi);

  u8g2.setCursor(0,40);
  u8g2.print("Pkt:");
  u8g2.print(packetCount);

  u8g2.setCursor(0,60);
  u8g2.print("Sig:");
  u8g2.print(quality);

} while (u8g2.nextPage());
 
} }
