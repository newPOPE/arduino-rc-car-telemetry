#include <SPI.h>
#include "RF24.h"


// RF24 init
#define CE 7
#define CS 8
byte transmitterAddress[] = "transmitter00";
byte recieverAddress[] = "reciever00";

RF24 nRF(CE, CS);

void setup() {
  Serial.begin(9600);

  nRF.begin();
  nRF.setPALevel(RF24_PA_LOW);
  nRF.openWritingPipe(recieverAddress);
  nRF.openReadingPipe(1, transmitterAddress);
  nRF.startListening();
}

void loop() {
  int values[2];

  if ( nRF.available()) {
    while (nRF.available()) {
      nRF.read( &values, sizeof(values) );
    }

    Serial.print("Recieved values: \t");
    Serial.print(values[0]);
    Serial.print("\t");
    Serial.println(values[1]);

    nRF.stopListening();
    nRF.startListening();
  }
}
