#include <SoftwareSerial.h>
#include "RF24.h"


const int RX = 7;
const int TX = 6;

const int CSN_PIN = 15;
const int CE_PIN = 5;
const byte ADDRESS[6] = "RCCAR";


const int JOYSTICK_L_X_PIN = A0;
const int JOYSTICK_L_Y_PIN = A1;

const int JOYSTICK_RESTING_DEV = 16;  // Deviation from joystick resting value to prevent micro-scale movements.


// This array holds all the data that is going to be sent through the NRF module.
// The order and length of this array has to match with the array in the receiver.
// Order: joystickLX, joystickLY.
const int NUM_OF_DATA_ITEMS = 2;
byte dataArray[NUM_OF_DATA_ITEMS];


SoftwareSerial Serial1(RX, TX);
RF24 radio(CE_PIN, CSN_PIN);


void setup() {
  // Serial1.begin(19200);

  initRadio();
}


void loop() {
  // Storing previous data array, to be used to only send data if data has changed.
  int prevDataArray[NUM_OF_DATA_ITEMS];
  memcpy(prevDataArray, dataArray, sizeof(dataArray));


  // Left joystick
  dataArray[0] = getJoystickValue(JOYSTICK_L_X_PIN);
  dataArray[1] = getJoystickValue(JOYSTICK_L_Y_PIN);


  // If the data array is not the same (data has changed), then send data.
  int comp = memcmp(prevDataArray, dataArray, sizeof(dataArray));

  if (comp != 0) {
    radio.write(&dataArray, sizeof(dataArray));
  }

  delay(33);
}


void initRadio() {
  if (!radio.begin()) {
    while (1) {}
  }

  radio.openWritingPipe(ADDRESS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);

  radio.stopListening();
}


int getJoystickValue(int pin) {
  int maxAdcValue = 1023;  // 1023 is ADC pin max reading for the Attiny88.
  int halfMaxAdcValue = maxAdcValue / 2;

  int potValue = analogRead(pin);

  // Only return read value if outside joystick resting position.
  if ((potValue > (halfMaxAdcValue + JOYSTICK_RESTING_DEV)) ||
    (potValue < (halfMaxAdcValue - JOYSTICK_RESTING_DEV))) {

    int potValueMapped = map(potValue, 0, maxAdcValue, 0, 255);

    return potValueMapped;
  }
  else {
    // Return a constant resting value.
    int restingValue = map(halfMaxAdcValue, 0, maxAdcValue, 0, 255);
    return restingValue;
  }
}
