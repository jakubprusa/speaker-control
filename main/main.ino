
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

Adafruit_L3GD20 gyro;

/*
 * Data structures
 */

struct ButtonsPins {
  short reset;
  short volumeUp;
  short volumeDown;
  short playPause;

  ButtonsPins(){
    reset = 13;
    volumeUp = 2;
    volumeDown = 3;
    playPause = 4;

    pinMode(reset, INPUT);
    pinMode(volumeUp, OUTPUT);
    pinMode(volumeDown, OUTPUT);
    pinMode(playPause, OUTPUT);
  }
};

struct Axis
{
  int absV;
  int treshold;
  bool negative;
  bool positive;

  Axis(): absV(0), treshold(500), negative(false), positive(false){}
};

struct Values
{
  Axis x;
  Axis y;
  Axis z;

  Values(){
    x = Axis();
    y = Axis();
    z = Axis();
  }
};

/*
 * Global variables
 */
ButtonsPins pins;
Values val;

/*
 * Code
 */
void resetAbsoluteValues(){
  val.x.absV = 0;
  val.y.absV = 0;
  val.z.absV = 0;
}

void readAndWriteValues(){
   gyro.read();
   val.x.absV += (int)gyro.data.x; 
   val.y.absV += (int)gyro.data.y; 
   val.z.absV += (int)gyro.data.z; 
}

void checkButtonsInteruptions(){
   if (digitalRead(pins.reset) == HIGH) {
    Serial.println("Button pressed");
    resetAbsoluteValues();
  }
}

void triggerButtons(short btnA, short btnB, int ms){
  digitalWrite(btnA, HIGH);
  digitalWrite(btnB, HIGH);
  delay(ms);
  digitalWrite(btnB, LOW);
  digitalWrite(btnA, LOW);
}

void triggerButton(short btn, int ms){
  digitalWrite(btn, HIGH);
  delay(ms);
  digitalWrite(btn, LOW);
}

void setup()
{
  Serial.begin(9600);
  pins = ButtonsPins();
  val = Values();
  
  // Try to initialise and warn if we couldn't detect the chip
  if (!gyro.begin(gyro.L3DS20_RANGE_250DPS))
    //if (!gyro.begin(gyro.L3DS20_RANGE_500DPS))
    //if (!gyro.begin(gyro.L3DS20_RANGE_2000DPS))
  {
    Serial.println("Oops ... unable to initialize the L3GD20. Check your wiring!");
    while (1);
  }
}


void loop()
{
  checkButtonsInteruptions();
  
  readAndWriteValues();  

  if (val.x.absV > val.x.treshold && val.x.positive ) {
    Serial.println("X +");
    triggerButton(pins.volumeUp,50);
    val.x.positive = false;
  } else if (val.x.absV < val.x.treshold ) {
    val.x.positive = true;
  }

  if (val.x.absV < -val.x.treshold && val.x.negative ) {
    Serial.println("X -");
    triggerButton(pins.volumeDown,50);
    val.x.negative = false;
  } else if (val.x.absV > -val.x.treshold ) {
    val.x.negative = true;
  }

  if (val.z.absV > val.z.treshold && val.z.positive ) {
    Serial.println("Z +");
    triggerButton(pins.playPause,50);
    val.z.positive = false;
  } else if (val.z.absV < val.z.treshold ) {
    val.z.positive = true;
  }

  if (val.z.absV < -val.z.treshold && val.z.negative ) {
    Serial.println("Z - ");
    triggerButtons(pins.volumeUp,pins.volumeDown, 50);
    val.z.negative = false;
  } else if (val.z.absV > -val.z.treshold ) {
    val.z.negative = true;
  }

  delay(50);
}
