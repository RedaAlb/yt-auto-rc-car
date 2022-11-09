#include "RF24.h"
#include <SoftwareSerial.h>


const int RX = 7;
const int TX = 6;

const int CSN_PIN = 15;
const int CE_PIN = 5;
const byte ADDRESS[6] = "RCCAR";

const int BTN_LEFT_PIN = 14;
const int BTN_RIGHT_PIN = 8;

const int JOYSTICK_L_X_PIN = A0;
const int JOYSTICK_L_Y_PIN = A1;
const int JOYSTICK_R_X_PIN = A2;
const int JOYSTICK_R_Y_PIN = A3;

const int JOYSTICK_RESTING_DEV = 20;  // Deviation from joystick resting value to prevent micro-scale movements.


// This array holds all the data that is going to be sent through the NRF module.
// The order and length of this array has to match with the array in the receiver.
// Order: joystickLX, joystickLY, joystickRX, joystickRY, btnLeft, btnRight.
const int NUM_OF_DATA_ITEMS = 6;
byte dataArray[NUM_OF_DATA_ITEMS];


SoftwareSerial Serial1(RX, TX);
RF24 radio(CE_PIN, CSN_PIN);


void setup() {
    // Serial1.begin(19200);

    initRadio();

    pinMode(BTN_LEFT_PIN, INPUT_PULLUP);
    pinMode(BTN_RIGHT_PIN, INPUT_PULLUP);
}


void loop() {
    // Storing previous data array, to be used to only send data if data has changed.
    int prevDataArray[NUM_OF_DATA_ITEMS];
    memcpy(prevDataArray, dataArray, sizeof(dataArray));


    // Left joystick
    dataArray[0] = getJoystickValue(JOYSTICK_L_X_PIN);
    dataArray[1] = getJoystickValue(JOYSTICK_L_Y_PIN);

    // Right joystick
    dataArray[2] = getJoystickValue(JOYSTICK_R_X_PIN);
    dataArray[3] = getJoystickValue(JOYSTICK_R_Y_PIN);

    // Buttons
    dataArray[4] = digitalRead(BTN_LEFT_PIN);
    dataArray[5] = digitalRead(BTN_RIGHT_PIN);


    // If the data array is not the same (data has changed), then send data.
    int comp = memcmp(prevDataArray, dataArray, sizeof(dataArray));

    if (comp != 0) {
        radio.write(&dataArray, sizeof(dataArray));
    }
}


void initRadio() {
    if (!radio.begin()) {
        while (1) {}
    }

    radio.openWritingPipe(ADDRESS);
    radio.setPALevel(RF24_PA_HIGH);
    radio.setDataRate(RF24_2MBPS);

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
