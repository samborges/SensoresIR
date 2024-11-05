
#include <Wire.h>
#include <Adafruit_AMG88xx.h>

Adafruit_AMG88xx amg;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];

void setup() {
  Serial.begin(9600);
  Serial.println(F("AMG88xx pixels"));

  bool status;

  // default settings
  status = amg.begin();
  if (!status) {
    Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
    while (1)
      ;
  }

  Serial.println("-- Pixels Test --");

  Serial.println();

  delay(100);  // let sensor boot up
}


void loop() {
  //read all the pixels
  amg.readPixels(pixels);
  Serial.print(millis());
  Serial.print(" - ");
  Serial.print("[");
  for (int i = 1; i <= AMG88xx_PIXEL_ARRAY_SIZE; i++) {
    Serial.print(pixels[i - 1]);
    if (i != AMG88xx_PIXEL_ARRAY_SIZE) Serial.print("; ");
  }
  Serial.println("]");

  //delay a second
  delay(100);
}
