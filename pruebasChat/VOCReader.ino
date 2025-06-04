#include <SoftwareSerial.h>

// SoftwareSerial setup: RX on pin 3, TX on pin 4
SoftwareSerial vocSerial(3, 4);

// Command to ensure the sensor is in initiative upload mode
const byte INIT_CMD[] = {0xFF, 0x01, 0x78, 0x40, 0x00, 0x00, 0x00, 0x00, 0x47};

uint8_t packet[9];

uint8_t calculateChecksum(const uint8_t *buf) {
  uint8_t sum = 0;
  for (int i = 1; i < 8; ++i) {
    sum += buf[i];
  }
  return (~sum) + 1;
}

bool readPacket() {
  static uint8_t idx = 0;
  while (vocSerial.available()) {
    uint8_t b = vocSerial.read();
    if (idx == 0 && b != 0xFF) {
      // waiting for start byte
      continue;
    }
    packet[idx++] = b;
    if (idx == 9) {
      idx = 0;
      return calculateChecksum(packet) == packet[8];
    }
  }
  return false;
}

void setup() {
  Serial.begin(9600);      // Serial monitor
  vocSerial.begin(9600);   // Sensor serial
  delay(100);
  vocSerial.write(INIT_CMD, sizeof(INIT_CMD));
}

void loop() {
  if (readPacket()) {
    uint16_t concentration = ((uint16_t)packet[4] << 8) | packet[5];
    Serial.print("TVOC: ");
    Serial.print(concentration);
    Serial.println(" ug/m3");
  }
}

