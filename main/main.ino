
/***************************************************
  This is an example for the Adafruit Triple-Axis Gyro sensor

  Designed specifically to work with the Adafruit L3GD20 Breakout
  ----> https://www.adafruit.com/products/1032

  These sensors use I2C or SPI to communicate, 2 pins (I2C)
  or 4 pins (SPI) are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Kevin "KTOWN" Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_L3GD20.h>

// Comment this next line to use SPI
#define USE_I2C

#ifdef USE_I2C
// The default constructor uses I2C
Adafruit_L3GD20 gyro;
#else
// To use SPI, you have to define the pins
#define GYRO_CS 4 // labeled CS
#define GYRO_DO 5 // labeled SA0
#define GYRO_DI 6  // labeled SDA
#define GYRO_CLK 7 // labeled SCL
Adafruit_L3GD20 gyro(GYRO_CS, GYRO_DO, GYRO_DI, GYRO_CLK);
#endif

int button = 13;

int volumePlus = 2;
int volumeMinus = 3;
int playPause = 4;

void setup()
{
  Serial.begin(9600);
  pinMode(button, INPUT);

  // Try to initialise and warn if we couldn't detect the chip
  if (!gyro.begin(gyro.L3DS20_RANGE_250DPS))
    //if (!gyro.begin(gyro.L3DS20_RANGE_500DPS))
    //if (!gyro.begin(gyro.L3DS20_RANGE_2000DPS))
  {
    Serial.println("Oops ... unable to initialize the L3GD20. Check your wiring!");
    while (1);
  }
}
int absoluteX = 0;
int absoluteY = 0;
int absoluteZ = 0;
int offset = 500;
bool flagX = true;
bool flagXPlus = true;
bool flagZ = true;
bool flagZPlus = true;

void loop()
{
  if (digitalRead(button) == HIGH) {
    Serial.println("button pressed");
    absoluteX = 0;
    absoluteZ = 0;
  }

  gyro.read();
  absoluteX = absoluteX + (int)gyro.data.x;
  absoluteZ = absoluteZ + (int)gyro.data.z;
  //Serial.print("X: "); Serial.print((int)absoluteX);   Serial.println(" ");
  //Serial.print("Z: "); Serial.print((int)absoluteZ);   Serial.println(" ");
  

  if (absoluteX > offset && flagXPlus ) {
    Serial.println("X +");
    digitalWrite(volumePlus, HIGH);
    delay(50);
    digitalWrite(volumePlus, LOW);
    flagXPlus = false;
  } else if (absoluteX < offset ) {
    flagXPlus = true;
  }

  if (absoluteX < -offset && flagX ) {
    Serial.println("X -");
    digitalWrite(volumeMinus, HIGH);
    delay(50);
    digitalWrite(volumeMinus, LOW);
    flagX = false;
  } else if (absoluteX > -offset ) {
    flagX = true;
  }

  if (absoluteZ > offset && flagZPlus ) {
    Serial.println("Z +");
    digitalWrite(playPause, HIGH);
    delay(50);
    digitalWrite(playPause, LOW);
    flagZPlus = false;
  } else if (absoluteZ < offset ) {
    flagZPlus = true;
  }

  if (absoluteZ < -offset && flagZ ) {
    Serial.println("Z - ");
    digitalWrite(volumePlus, HIGH);
    digitalWrite(volumeMinus, HIGH);
    delay(50);
    digitalWrite(volumePlus, LOW);
    digitalWrite(volumeMinus, LOW);
    flagZ = false;
  } else if (absoluteZ > -offset ) {
    flagZ = true;
  }

  delay(50);
}
