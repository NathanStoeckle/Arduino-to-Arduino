// Libraries
#include <Wire.h>

// Fields
int receivedValue = 0;
int buttonPin = 12;
int isSelected = false;
int btnState = 0;
 
void setup() 
{
  Wire.begin();
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
}
 
void loop() 
{
  // Request a byte from Arduino at address 9 (DigitalPin 8)
  Wire.requestFrom(9, 1);

  while (Wire.available()){
    receivedValue = Wire.read();
    Serial.println(receivedValue);
  }

  btnState = digitalRead(buttonPin);

  if (btnState == HIGH) {
    isSelected = !isSelected;
    delay(500);
  }

  Wire.beginTransmission(9);
  Wire.write(isSelected);
  Wire.endTransmission();
  delay(100);
}
