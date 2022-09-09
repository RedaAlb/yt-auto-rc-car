#include "RF24.h"

// Radio nRF24L01 module.
const int CSN_PIN = 21;
const int CE_PIN = 22;
const byte ADDRESS[6] = "RCCAR";


// Controls pins for the motor drivers.
const int MOTORS_FORWARD_PIN = 26;    // Motors forward.
const int MOTORS_BACKWARD_PIN = 25;   // Motors backwards.


// PWM channels to control the motors, channel 0 for forward, 1 for backwards.
const int PWM_CHANNELS[] = { 0, 1 };
const int PWM_FREQ = 500;
const int PWM_RES = 8;


const int JOYSTICK_RESTING_VALUE = 127;


// This array holds all the data that is received from the NRF module.
// The order and length of this array has to match with the array in the transmitter.
// Order: joystickLX, joystickLY.
const int NUM_OF_DATA_ITEMS = 2;
byte dataArray[NUM_OF_DATA_ITEMS];


RF24 radio(CE_PIN, CSN_PIN);


void setup() {
  Serial.begin(115200);

  initRadio();
  initPwm();
}


void loop() {
  if (!radio.available()) {
    return;
  }

  radio.read(&dataArray, sizeof(dataArray));

  handleVerticalMovement();

  printArrayValues(dataArray, (sizeof(dataArray) / sizeof(dataArray[0])), " ");
}


void initRadio() {
  if (!radio.begin()) {
    while (1) {}
  }

  radio.openReadingPipe(1, ADDRESS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);

  radio.startListening();
}


void initPwm() {
  int numOfChannels = sizeof(PWM_CHANNELS) / sizeof(PWM_CHANNELS[0]);

  for (int i = 0; i < numOfChannels; i++) {
    int channel = PWM_CHANNELS[i];
    ledcSetup(channel, PWM_FREQ, PWM_RES);
  }

  ledcAttachPin(MOTORS_FORWARD_PIN, PWM_CHANNELS[0]);
  ledcAttachPin(MOTORS_BACKWARD_PIN, PWM_CHANNELS[1]);
}


// Based on the position of the left joystick, move forwards or backwards.
void handleVerticalMovement() {
  int joystickLeftY = dataArray[1];

  if (joystickLeftY > JOYSTICK_RESTING_VALUE) {
    int speed = map(joystickLeftY, JOYSTICK_RESTING_VALUE + 1, 255, 0, 255);
    forward(speed);
  }
  else if (joystickLeftY < JOYSTICK_RESTING_VALUE) {
    int speed = map(joystickLeftY, JOYSTICK_RESTING_VALUE - 1, 0, 0, 255);
    backward(speed);
  }
  else {
    stopCar();
  }
}


void printArrayValues(byte array[], int arrayLength, String spacer) {
  for (int i = 0; i < arrayLength; i++) {
    int value = array[i];

    Serial.print(value);
    Serial.print(spacer);
  }

  Serial.println("");
}


void forward(int speed) {
  ledcWrite(PWM_CHANNELS[0], speed);
  ledcWrite(PWM_CHANNELS[1], 0);
}


void backward(int speed) {
  ledcWrite(PWM_CHANNELS[0], 0);
  ledcWrite(PWM_CHANNELS[1], speed);
}


void stopCar() {
  ledcWrite(PWM_CHANNELS[0], 0);
  ledcWrite(PWM_CHANNELS[1], 0);
}
