#include <SPI.h>
#include "RF24.h"
#include <MPU6050_tockn.h>
#include <Wire.h>


// RF24 init
#define CE 7
#define CS 8
byte transmitterAddress[] = "transmitter00";  // change this
byte recieverAddress[] = "reciever00";  // change this

RF24 nRF(CS, CE);

// mpu initialization
MPU6050 mpu(Wire);

// buzzer
#define BUZZER_PIN 3
const int ALERT_ANGLE_LOW = 40;
const int ALERT_ANGLE_MID = 50;
const int ALERT_ANGLE_HI = 60;

const int INTERVAL_LOW = 500;
const int INTERVAL_MID = 250;
const int INTERVAL_HI = 100;

unsigned long previousMillis = 0;
bool alertState = false;

void playAlert(bool play = 1, unsigned int interval = INTERVAL_LOW) {
  if (play) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      if (alertState == false) {
        alertState = true;
        Serial.println("on");
      }
      else {
        alertState = false;
        Serial.println("off");
      }

      if (alertState == true) {
        tone(BUZZER_PIN, 2500, 100);
      }
      else {
        noTone(BUZZER_PIN);
      }
    }
  }
  else {
    noTone(BUZZER_PIN);
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  Serial.println(F("Initializing I2C devices..."));
  mpu.begin();
  //mpu.calcGyroOffsets(true);  // uncomment this for calibration
  mpu.setGyroOffsets(-1.5, 0.4, 0.07);

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
  float values[4];
  values[0] = mpu.getAngleX();
  values[1] =  mpu.getAngleY();
  values[2] =  mpu.getAngleZ();
  values[3] = mpu.getTemp();

  //Serial.println("Sending data");
  nRF.stopListening();
  if (!nRF.write(&values, sizeof(values))) {
    //Serial.println("Data not sent!");
  }
  else {
    //Serial.println("Data sent!");
  }

  int maxAngle = max(abs(values[0]), abs(values[1]));
  Serial.println(maxAngle);

  if (ALERT_ANGLE_LOW < maxAngle && maxAngle < ALERT_ANGLE_MID) {
    playAlert(1, INTERVAL_LOW);
  }
  if (ALERT_ANGLE_MID < maxAngle && maxAngle < ALERT_ANGLE_HI) {
    playAlert(1, INTERVAL_MID);
  }
  if (maxAngle > ALERT_ANGLE_HI) {
    playAlert(1, INTERVAL_HI);
  }
  if (ALERT_ANGLE_LOW > maxAngle) {
    playAlert(0);
  }
}
