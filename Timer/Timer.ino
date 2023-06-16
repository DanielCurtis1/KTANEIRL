#include <CAN.h>



const int SIMON = 0x00012;
const int BUTTON = 0x00013;

long currentTime = millis();
int currentStrikes = 0;

void setup() {
  CAN.begin(500E3);
}

void loop() {
  sendTime(SIMON);
  sendTime(BUTTON);
  sendStrikes(SIMON);
}

void sendTime(int moduleName) {
  CAN.beginPacket(moduleName);
  CAN.write(currentTime);
  CAN.endPacket();

}

void sendStrikes(int moduleName) {
  CAN.beginPacket(moduleName);
  CAN.write(currentStrikes);
  CAN.endPacket();

}
