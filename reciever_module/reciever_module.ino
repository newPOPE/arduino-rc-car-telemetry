#include <SPI.h>
#include "RF24.h"
#include "I2Cdev.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// RF24 init
#define CE 7
#define CS 8
byte transmitterAddress[] = "transmitter00";
byte recieverAddress[] = "reciever00";

RF24 nRF(CE, CS);

// Display init
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {

  Serial.begin(9600);

  nRF.begin();
  nRF.setPALevel(RF24_PA_LOW);
  nRF.openWritingPipe(recieverAddress);
  nRF.openReadingPipe(1, transmitterAddress);
  nRF.startListening();

  // Display
  lcd.init();
  lcd.begin(16, 2); //Defining 16 columns and 2 rows of lcd display
  lcd.setCursor(0, 0);
  lcd.backlight();
  lcd.print("     RC car   ");
  lcd.setCursor(0, 1);
  lcd.print("   Telemetry  ");
  delay(2000);
  lcd.clear();

  Serial.println("Setup done");

  lcd.setCursor(0,0);
  lcd.print("|: ");
  lcd.setCursor(0,1);
  lcd.print("-: ");
  lcd.setCursor(8,1);
  lcd.print("t: ");
}

void loop() {
  float values[4];

  if ( nRF.available()) {
    while (nRF.available()) {
      nRF.read( &values, sizeof(values) );
    }

    Serial.print("Recieved values: \t");
    Serial.print(values[0]);
    Serial.print("\t");
    Serial.println(values[1]);
    Serial.print("\t");
    Serial.println(values[2]);
    Serial.print("\t");
    Serial.println(values[3]);

    // print to LCD
    lcd.setCursor(3, 0);
    lcd.print("   ");
    lcd.setCursor(3, 0);
    lcd.print((int) ceil(abs(values[1])));
    lcd.setCursor(3, 1);
    lcd.print("   ");
    lcd.setCursor(3, 1);
    lcd.print((int) ceil(abs(values[0])));

    // temp
    lcd.setCursor(11, 1);
    lcd.print("     ");
    lcd.setCursor(11, 1);
    lcd.print((int) values[3]);

    nRF.stopListening();
    nRF.startListening();    
  }
}
