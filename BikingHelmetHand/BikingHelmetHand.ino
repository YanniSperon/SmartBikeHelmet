#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

//https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor/arduino-code

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  // /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("ERROR\n");
    while(1)
    {
      Serial.print("ERROR\n");
    }
  }
  
  delay(1000);
  
  bno.setExtCrystalUse(true);
}

void loop() {
  /* Get a new sensor event */ 
  sensors_event_t event; 
  bno.getEvent(&event);
  
  /* Display the floating point data */
  //Serial.print(String(event.orientation.x) + "," + String(event.orientation.y) + "," + String(event.orientation.z) + "\n");
  
  delay(15);
}
