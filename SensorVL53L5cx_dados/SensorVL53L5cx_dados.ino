#include <Wire.h>

#include <SparkFun_VL53L5CX_Library.h>  //http://librarymanager/All#SparkFun_VL53L5CX

SparkFun_VL53L5CX myImager;
VL53L5CX_ResultsData measurementData;  // Result data class structure, 1356 byes of RAM

int imageResolution = 0;  //Used to pretty print output
int imageWidth = 0;       //Used to pretty print output

void setup() {
  Serial.begin(9600);
  delay(100);
  Serial.println("SparkFun VL53L5CX Imager Example");

  Wire.begin();            //This resets I2C bus to 100kHz
  Wire.setClock(1000000);  //Sensor has max I2C freq of 1MHz

  //myImager.setWireMaxPacketSize(128); //Increase default from 32 bytes to 128 - not supported on all platforms

  Serial.println("Initializing sensor board. This can take up to 10s. Please wait.");

  //Time how long it takes to transfer firmware to sensor
  long startTime = millis();
  bool startup = myImager.begin();
  long stopTime = millis();

  if (startup == false) {
    Serial.println(F("Sensor not found - check your wiring. Freezing"));
    while (1)
      ;
  }

  Serial.print("Firmware transfer time: ");
  float timeTaken = (stopTime - startTime) / 1000.0;
  Serial.print(timeTaken, 3);
  Serial.println("s");

  myImager.setResolution(8 * 8);  //Enable all 64 pads

  imageResolution = myImager.getResolution();  //Query sensor for current resolution - either 4x4 or 8x8
  imageWidth = sqrt(imageResolution);          //Calculate printing width

  myImager.startRanging();
}

void loop() {
  //Poll sensor for new data
  if (myImager.isDataReady() == true) {
    if (myImager.getRangingData(&measurementData))  //Read distance data into array
    {
      //The ST library returns the data transposed from zone mapping shown in datasheet
      //Pretty-print data with increasing y, decreasing x to reflect reality
      Serial.print(millis());
      Serial.print(" - ");
      Serial.print("[");

      for (int i = 0; i <= (imageResolution - 1); i++) {
        Serial.print(measurementData.distance_mm[i]);
        if (i != (imageResolution - 1)) Serial.print(";");
      }
      Serial.println("]");
    }
  }

  delay(100);  //Small delay between polling
}
