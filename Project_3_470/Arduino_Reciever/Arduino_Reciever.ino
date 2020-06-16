// Libraries
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>

// Declare the BNO055 Object!
Adafruit_BNO055 bno = Adafruit_BNO055(55);

// Fields
int LED_PIN = 2;
#define LED_COUNT 60 // How many NeoPixels are attached? - it's really 30 but can be adjusted
bool valueReceived;
bool btnValue;
int isOn; // Off = 0, On = 1
// This will allow me to manipulate the LED Strip (to change colors and pixel postioning)
float x, y, z;
int btnCheck;
int btnClick = 0;
int rng;

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  // BNO055 Code
  Serial.begin(115200);

  Serial.println("Orientation Sensor Test"); Serial.println("");

  // Initialise the sensor
  if (!bno.begin())
  {
    // There was a problem detecting the BNO055 ... check your connections
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }

  (1000);

  bno.setExtCrystalUse(true);
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  /* WIRES */
  // Start the I2C Bus as Slave on address 9 (DigitalPin #8)
  Wire.begin(9);

  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);

  //Attach a function to trigger when something is requested
  Wire.onRequest(requestEvent);

  // Button for changing colors
  pinMode(4, INPUT_PULLUP);
}

void receiveEvent(bool bytes) {
  //Read one value from the I2C
  valueReceived = Wire.read();

  //Display the value received
  Serial.println(valueReceived);

  //If the value received was true turn the led on, otherwise turn it off
  if (valueReceived) {
    isOn = 1;
  }
  else {
    isOn = 0;
  }
}

void requestEvent() {
  //Tell the master whether the led is on or not
  Wire.write(isOn);
}


// loop() function -- runs repeatedly as long as board is on ---------------
void loop() {
  // Get a new sensor event
  sensors_event_t event;
  bno.getEvent(&event);

  // Display the floating point data
  x = event.orientation.x;
  y = event.orientation.y;
  z = event.orientation.z;
  Serial.print("X: ");
  Serial.print(x, 4);
  Serial.print("\tY: ");
  Serial.print(y, 4);
  Serial.print("\tZ: ");
  Serial.print(z, 4);
  Serial.println("");
  delay(100);

  int btnValue = digitalRead(4);
  Serial.println(btnValue);

  if (btnValue == 0) {
    btnClick = btnClick + 1;
    delay(250);
  }

  if (btnClick == 6) {
    btnClick = 0;
  }

  if (isOn) {
    if (btnClick == 0) {
      colorWipe(strip.Color(x, 0, z), 0);
    }

    if (btnClick == 1) {
      colorWipe(strip.Color(0, x, z), 0);
    }

    if (btnClick == 2) {
      colorWipe(strip.Color(z, x, 0), 0);
    }

    if (btnClick == 3) {
      colorWipe(strip.Color(z, 0, x), 0);
    }

    if (btnClick == 4) {
      colorWipe(strip.Color(0, z, x), 0);
    }

    if (btnClick == 5) {
      colorWipe(strip.Color(x, z, 0), 0);
    }
  }

  else {
    colorWipe(strip.Color(0, 0, 0), 0);
  }

  // This was used as a guide to help me put the code in the correct order
  //colorWipe(strip.Color(x,   0,   z), 0);
  //colorWipe(strip.Color(  0, x,   z), 0);
  //colorWipe(strip.Color(  z,   x, 0), 0);
  //colorWipe(strip.Color(z,   0,   x), 0);
  //colorWipe(strip.Color(  0, z,   x), 0);
  //colorWipe(strip.Color(  x,   z, 0), 0);
}


// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {

  // Reset the y value if we're at an "edge"
  for (uint16_t i = y; i < 360; i++) {
    uint16_t clearColor = (0, 0, 0);
    strip.setPixelColor(i, clearColor);
  }

  // Fill the pixels based on the y value
  for (uint16_t i = 0; i < y; i++) {
    strip.setPixelColor(i, color);
    delay(0);
  }

  strip.show();
}
