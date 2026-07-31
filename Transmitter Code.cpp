TX Code:

#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
 
// LoRa Pins
#define SS 10
#define RST 9
#define DIO0 2
 
// GPS Setup
TinyGPSPlus gps;
SoftwareSerial gpsSerial(4, 3); // RX, TX
 
void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
 
  // LoRa Init
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Failed!");
    while (1);
  }
  Serial.println("TX Ready");
}
void loop() {
 
  // Read GPS data
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }
 
  // Check if GPS valid
  if (gps.location.isValid()) {
 
    float lat = gps.location.lat();
    float lon = gps.location.lng();
 
    // Send via LoRa
    LoRa.beginPacket();
    LoRa.print(lat, 6);  // 6 decimal precision
    LoRa.print(",");
    LoRa.print(lon, 6);
    LoRa.endPacket();
 
    // Serial Output
    Serial.print("Sent: ");
    Serial.print(lat, 6);
    Serial.print(", ");
    Serial.println(lon, 6);
 
  } else {
    Serial.println("Waiting for GPS...");
  }
 
  delay(1000); // send every 1 sec
}
