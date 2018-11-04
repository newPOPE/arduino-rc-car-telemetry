#include <SPI.h>
#include "RF24.h"
#include <MPU6050_tockn.h>
#include <Wire.h>


// RF24 init
#define CE 7
#define CS 8
byte transmitterAddress[] = "transmitter00";
byte recieverAddress[] = "reciever00";

RF24 nRF(CE, CS);

// mpu initialization
MPU6050 mpu(Wire);

void setup() {
  Serial.begin(9600);
  Wire.begin();

  Serial.println(F("Initializing I2C devices..."));
  mpu.begin();
  //mpu.calcGyroOffsets(true);
  mpu.setGyroOffsets(-1.09, 1.19, 0.17);

  Serial.println("Transmitter setup.");
  nRF.begin();
  nRF.setPALevel(RF24_PA_LOW);
  nRF.openWritingPipe(transmitterAddress);
  nRF.openReadingPipe(1, recieverAddress);
  nRF.startListening();
  Serial.println("Transmitter setup done!");
}

void loop() {
  // measure angles
  mpu.update();

  // sending data
  int values[2];
  values[0] = mpu.getAngleX();
  values[1] =  mpu.getAngleY();

  Serial.println("Sending data");
  nRF.stopListening();
  if (!nRF.write(&values, sizeof(values))) {
    Serial.println("Data not sent!");
  }
  else {
    Serial.println("Data sent!");
  }

  delay(50);

}
